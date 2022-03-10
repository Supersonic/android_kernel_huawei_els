/*
 * Based on arch/arm/mm/mmu.c
 *
 * Copyright (C) 1995-2005 Russell King
 * Copyright (C) 2012 ARM Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/cache.h>
#include <linux/export.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/kexec.h>
#include <linux/libfdt.h>
#include <linux/mman.h>
#include <linux/nodemask.h>
#include <linux/memblock.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#ifdef CONFIG_HKIP_PRMEM
#include <linux/hisi/prmem.h>
#endif
#include <linux/hisi/hkip.h>

#include <asm/barrier.h>
#include <asm/cputype.h>
#include <asm/fixmap.h>
#include <asm/kasan.h>
#include <asm/kernel-pgtable.h>
#include <asm/sections.h>
#include <asm/setup.h>
#include <asm/sizes.h>
#include <asm/tlb.h>
#include <asm/memblock.h>
#include <asm/mmu_context.h>
#include <asm/ptdump.h>

#define NO_BLOCK_MAPPINGS	BIT(0)
#define NO_CONT_MAPPINGS	BIT(1)

u64 idmap_t0sz = TCR_T0SZ(VA_BITS);

u64 kimage_voffset __ro_after_init;
EXPORT_SYMBOL(kimage_voffset);

/*
 * Empty_zero_page is a special page that is used for zero-initialized data
 * and COW.
 */
unsigned long empty_zero_page[PAGE_SIZE / sizeof(unsigned long)] __page_aligned_bss;
EXPORT_SYMBOL(empty_zero_page);

static pte_t bm_pte[PTRS_PER_PTE] __page_aligned_bss;
static pmd_t bm_pmd[PTRS_PER_PMD] __page_aligned_bss __maybe_unused;
static pud_t bm_pud[PTRS_PER_PUD] __page_aligned_bss __maybe_unused;

pgprot_t phys_mem_access_prot(struct file *file, unsigned long pfn,
			      unsigned long size, pgprot_t vma_prot)
{
	if (!pfn_valid(pfn))
		return pgprot_noncached(vma_prot);
	else if (file->f_flags & O_SYNC)
		return pgprot_writecombine(vma_prot);
	return vma_prot;
}
EXPORT_SYMBOL(phys_mem_access_prot);

static phys_addr_t __init early_pgtable_alloc(void)
{
	phys_addr_t phys;
	void *ptr;

	phys = memblock_alloc(PAGE_SIZE, PAGE_SIZE);

	/*
	 * The FIX_{PGD,PUD,PMD} slots may be in active use, but the FIX_PTE
	 * slot will be free, so we can (ab)use the FIX_PTE slot to initialise
	 * any level of table.
	 */
	ptr = pte_set_fixmap(phys);

	memset(ptr, 0, PAGE_SIZE);

	/*
	 * Implicit barriers also ensure the zeroed page is visible to the page
	 * table walker
	 */
	pte_clear_fixmap();

	return phys;
}

static bool pgattr_change_is_safe(u64 old, u64 new)
{
	/*
	 * The following mapping attributes may be updated in live
	 * kernel mappings without the need for break-before-make.
	 */
	static const pteval_t mask = PTE_PXN | PTE_RDONLY | PTE_WRITE | PTE_NG;

	/* creating or taking down mappings is always safe */
	if (old == 0 || new == 0)
		return true;

	/* live contiguous mappings may not be manipulated at all */
	if ((old | new) & PTE_CONT)
		return false;

	/* Transitioning from Non-Global to Global is unsafe */
	if (old & ~new & PTE_NG)
		return false;

	return ((old ^ new) & ~mask) == 0;
}

static void init_pte(pmd_t *pmd, unsigned long addr, unsigned long end,
		     phys_addr_t phys, pgprot_t prot)
{
	pte_t *pte;

	pte = pte_set_fixmap_offset(pmd, addr);
	do {
		pte_t old_pte = *pte;

		set_pte(pte, pfn_pte(__phys_to_pfn(phys), prot));

		/*
		 * After the PTE entry has been populated once, we
		 * only allow updates to the permission attributes.
		 */
		BUG_ON(!pgattr_change_is_safe(pte_val(old_pte), pte_val(*pte)));

		phys += PAGE_SIZE;
	} while (pte++, addr += PAGE_SIZE, addr != end);

	pte_clear_fixmap();
}

static void alloc_init_cont_pte(pmd_t *pmd, unsigned long addr,
				unsigned long end, phys_addr_t phys,
				pgprot_t prot,
				phys_addr_t (*pgtable_alloc)(void),
				int flags)
{
	unsigned long next;

	BUG_ON(pmd_sect(*pmd));
	if (pmd_none(*pmd)) {
		phys_addr_t pte_phys;
		BUG_ON(!pgtable_alloc);
		pte_phys = pgtable_alloc();
		__pmd_populate(pmd, pte_phys, PMD_TYPE_TABLE);
	}
	BUG_ON(pmd_bad(*pmd));

	do {
		pgprot_t __prot = prot;

		next = pte_cont_addr_end(addr, end);

		/* use a contiguous mapping if the range is suitably aligned */
		if ((((addr | next | phys) & ~CONT_PTE_MASK) == 0) &&
		    (flags & NO_CONT_MAPPINGS) == 0)
			__prot = __pgprot(pgprot_val(prot) | PTE_CONT);

		init_pte(pmd, addr, next, phys, __prot);

		phys += next - addr;
	} while (addr = next, addr != end);
}

static void init_pmd(pud_t *pud, unsigned long addr, unsigned long end,
		     phys_addr_t phys, pgprot_t prot,
		     phys_addr_t (*pgtable_alloc)(void), int flags)
{
	unsigned long next;
	pmd_t *pmd;

	pmd = pmd_set_fixmap_offset(pud, addr);
	do {
		pmd_t old_pmd = *pmd;

		next = pmd_addr_end(addr, end);

		/* try section mapping first */
		if (((addr | next | phys) & ~SECTION_MASK) == 0 &&
		    (flags & NO_BLOCK_MAPPINGS) == 0) {
			pmd_set_huge(pmd, phys, prot);

			/*
			 * After the PMD entry has been populated once, we
			 * only allow updates to the permission attributes.
			 */
			BUG_ON(!pgattr_change_is_safe(pmd_val(old_pmd),
						      pmd_val(*pmd)));
		} else {
			alloc_init_cont_pte(pmd, addr, next, phys, prot,
					    pgtable_alloc, flags);

			BUG_ON(pmd_val(old_pmd) != 0 &&
			       pmd_val(old_pmd) != pmd_val(*pmd));
		}
		phys += next - addr;
	} while (pmd++, addr = next, addr != end);

	pmd_clear_fixmap();
}

static void alloc_init_cont_pmd(pud_t *pud, unsigned long addr,
				unsigned long end, phys_addr_t phys,
				pgprot_t prot,
				phys_addr_t (*pgtable_alloc)(void), int flags)
{
	unsigned long next;

	/*
	 * Check for initial section mappings in the pgd/pud.
	 */
	BUG_ON(pud_sect(*pud));
	if (pud_none(*pud)) {
		phys_addr_t pmd_phys;
		BUG_ON(!pgtable_alloc);
		pmd_phys = pgtable_alloc();
		__pud_populate(pud, pmd_phys, PUD_TYPE_TABLE);
	}
	BUG_ON(pud_bad(*pud));

	do {
		pgprot_t __prot = prot;

		next = pmd_cont_addr_end(addr, end);

		/* use a contiguous mapping if the range is suitably aligned */
		if ((((addr | next | phys) & ~CONT_PMD_MASK) == 0) &&
		    (flags & NO_CONT_MAPPINGS) == 0)
			__prot = __pgprot(pgprot_val(prot) | PTE_CONT);

		init_pmd(pud, addr, next, phys, __prot, pgtable_alloc, flags);

		phys += next - addr;
	} while (addr = next, addr != end);
}

static inline bool use_1G_block(unsigned long addr, unsigned long next,
			unsigned long phys)
{
	if (PAGE_SHIFT != 12)
		return false;

	if (((addr | next | phys) & ~PUD_MASK) != 0)
		return false;

	return true;
}

static void alloc_init_pud(pgd_t *pgd, unsigned long addr, unsigned long end,
				  phys_addr_t phys, pgprot_t prot,
				  phys_addr_t (*pgtable_alloc)(void),
				  int flags)
{
	pud_t *pud;
	unsigned long next;

	if (pgd_none(*pgd)) {
		phys_addr_t pud_phys;
		BUG_ON(!pgtable_alloc);
		pud_phys = pgtable_alloc();
		__pgd_populate(pgd, pud_phys, PUD_TYPE_TABLE);
	}
	BUG_ON(pgd_bad(*pgd));

	pud = pud_set_fixmap_offset(pgd, addr);
	do {
		pud_t old_pud = *pud;

		next = pud_addr_end(addr, end);

		/*
		 * For 4K granule only, attempt to put down a 1GB block
		 */
		if (use_1G_block(addr, next, phys) &&
		    (flags & NO_BLOCK_MAPPINGS) == 0) {
			pud_set_huge(pud, phys, prot);

			/*
			 * After the PUD entry has been populated once, we
			 * only allow updates to the permission attributes.
			 */
			BUG_ON(!pgattr_change_is_safe(pud_val(old_pud),
						      pud_val(*pud)));
		} else {
			alloc_init_cont_pmd(pud, addr, next, phys, prot,
					    pgtable_alloc, flags);

			BUG_ON(pud_val(old_pud) != 0 &&
			       pud_val(old_pud) != pud_val(*pud));
		}
		phys += next - addr;
	} while (pud++, addr = next, addr != end);

	pud_clear_fixmap();
}

static void __create_pgd_mapping(pgd_t *pgdir, phys_addr_t phys,
				 unsigned long virt, phys_addr_t size,
				 pgprot_t prot,
				 phys_addr_t (*pgtable_alloc)(void),
				 int flags)
{
	unsigned long addr, length, end, next;
	pgd_t *pgd = pgd_offset_raw(pgdir, virt);

	/*
	 * If the virtual and physical address don't have the same offset
	 * within a page, we cannot map the region as the caller expects.
	 */
	if (WARN_ON((phys ^ virt) & ~PAGE_MASK))
		return;

	phys &= PAGE_MASK;
	addr = virt & PAGE_MASK;
	length = PAGE_ALIGN(size + (virt & ~PAGE_MASK));

	end = addr + length;
	do {
		next = pgd_addr_end(addr, end);
		alloc_init_pud(pgd, addr, next, phys, prot, pgtable_alloc,
			       flags);
		phys += next - addr;
	} while (pgd++, addr = next, addr != end);
}

static phys_addr_t pgd_pgtable_alloc(void)
{
	void *ptr = (void *)__get_free_page(PGALLOC_GFP);
	if (!ptr || !pgtable_page_ctor(virt_to_page(ptr)))
		BUG();

	/* Ensure the zeroed page is visible to the page table walker */
	dsb(ishst);
	return __pa(ptr);
}

/*
 * This function can only be used to modify existing table entries,
 * without allocating new levels of table. Note that this permits the
 * creation of new section or page entries.
 */
static void __init create_mapping_noalloc(phys_addr_t phys, unsigned long virt,
				  phys_addr_t size, pgprot_t prot)
{
	if (virt < VMALLOC_START) {
		pr_warn("BUG: not creating mapping for %pa at 0x%016lx - outside kernel range\n",
			&phys, virt);
		return;
	}
	__create_pgd_mapping(init_mm.pgd, phys, virt, size, prot, NULL,
			     NO_CONT_MAPPINGS);
}

void __init create_pgd_mapping(struct mm_struct *mm, phys_addr_t phys,
			       unsigned long virt, phys_addr_t size,
			       pgprot_t prot, bool page_mappings_only)
{
	int flags = 0;

	BUG_ON(mm == &init_mm);

	if (page_mappings_only)
		flags = NO_BLOCK_MAPPINGS | NO_CONT_MAPPINGS;

	__create_pgd_mapping(mm->pgd, phys, virt, size, prot,
			     pgd_pgtable_alloc, flags);
}

static void update_mapping_prot(phys_addr_t phys, unsigned long virt,
				phys_addr_t size, pgprot_t prot)
{
	if (virt < VMALLOC_START) {
		pr_warn("BUG: not updating mapping for %pa at 0x%016lx - outside kernel range\n",
			&phys, virt);
		return;
	}

	__create_pgd_mapping(init_mm.pgd, phys, virt, size, prot, NULL,
			     NO_CONT_MAPPINGS);

	/* flush the TLBs after updating live kernel mappings */
	flush_tlb_kernel_range(virt, virt + size);
}

static void __init __map_memblock(pgd_t *pgd, phys_addr_t start,
				  phys_addr_t end, pgprot_t prot, int flags)
{
	__create_pgd_mapping(pgd, start, __phys_to_virt(start), end - start,
			     prot, early_pgtable_alloc, flags);
}

void __init mark_linear_text_alias_ro(void)
{
	/*
	 * Remove the write permissions from the linear alias of .text/.rodata
	 */
	update_mapping_prot(__pa_symbol(_text), (unsigned long)lm_alias(_text),
			    (unsigned long)__init_begin - (unsigned long)_text,
			    PAGE_KERNEL_RO);
}

#ifdef CONFIG_HISI_LB_L3_EXTENSION
static void lb_memblk_map_clear(pgd_t *pgd, int flags)
{
	int i;

	for (i = 0; i < MAX_LB_MEMBLK_SP; i++) {
		struct lb_memory_block *lbmem = &lb_memblk_sp[i];
		if (lbmem->size) {
			/*
			 * Use section-level mappings here
			 * so that we can mark the region
			 * in section granularity and
			 * up the mem perfermance for cam_mem.
			 */
			__map_memblock(pgd, lbmem->base,
					lbmem->base + lbmem->size,
					PAGE_KERNEL, flags);/*lint !e446*/
			memblock_clear_nomap(lbmem->base,
					     lbmem->size);
		}
	}
}

static void lb_memblk_mark_nomap(void)
{
	int i;

	for (i = 0; i < MAX_LB_MEMBLK_SP; i++) {
		struct lb_memory_block *lbmem = &lb_memblk_sp[i];;
		if (lbmem->size)
			memblock_mark_nomap(lbmem->base,
					    lbmem->size);
	}
}

#endif

static void __init map_mem(pgd_t *pgd)
{
	phys_addr_t kernel_start = __pa_symbol(_text);
	phys_addr_t kernel_end = __pa_symbol(__init_begin);
	struct memblock_region *reg;
	int flags = 0;

	if (debug_pagealloc_enabled())
		flags = NO_BLOCK_MAPPINGS | NO_CONT_MAPPINGS;

	/*
	 * Take care not to create a writable alias for the
	 * read-only text and rodata sections of the kernel image.
	 * So temporarily mark them as NOMAP to skip mappings in
	 * the following for-loop
	 */
	memblock_mark_nomap(kernel_start, kernel_end - kernel_start);
#ifdef CONFIG_KEXEC_CORE
	if (crashk_res.end)
		memblock_mark_nomap(crashk_res.start,
				    resource_size(&crashk_res));
#endif

#ifdef CONFIG_HISI_LB_L3_EXTENSION
	lb_memblk_mark_nomap();
#endif

	/* map all the memory banks */
	for_each_memblock(memory, reg) {
		phys_addr_t start = reg->base;
		phys_addr_t end = start + reg->size;

		if (start >= end)
			break;
		if (memblock_is_nomap(reg))
			continue;

#ifdef CONFIG_HISI_LB_L3_EXTENSION
		__map_memblock(pgd, start, end, PAGE_KERNEL,
				NO_BLOCK_MAPPINGS | NO_CONT_MAPPINGS);
#else
		__map_memblock(pgd, start, end, PAGE_KERNEL, flags);
#endif
	}

	/*
	 * Map the linear alias of the [_text, __init_begin) interval
	 * as non-executable now, and remove the write permission in
	 * mark_linear_text_alias_ro() below (which will be called after
	 * alternative patching has completed). This makes the contents
	 * of the region accessible to subsystems such as hibernate,
	 * but protects it from inadvertent modification or execution.
	 * Note that contiguous mappings cannot be remapped in this way,
	 * so we should avoid them here.
	 */
	__map_memblock(pgd, kernel_start, kernel_end,
		       PAGE_KERNEL, NO_CONT_MAPPINGS);
	memblock_clear_nomap(kernel_start, kernel_end - kernel_start);

#ifdef CONFIG_KEXEC_CORE
	/*
	 * Use page-level mappings here so that we can shrink the region
	 * in page granularity and put back unused memory to buddy system
	 * through /sys/kernel/kexec_crash_size interface.
	 */
	if (crashk_res.end) {
		__map_memblock(pgd, crashk_res.start, crashk_res.end + 1,
			       PAGE_KERNEL,
			       NO_BLOCK_MAPPINGS | NO_CONT_MAPPINGS);
		memblock_clear_nomap(crashk_res.start,
				     resource_size(&crashk_res));
	}
#endif

#ifdef CONFIG_HISI_LB_L3_EXTENSION
	lb_memblk_map_clear(pgd, flags);
#endif
}

#ifdef CONFIG_HKIP_PRMEM
void mark_wr_data_wr(void)
{
	unsigned long section_size;

	if (!prmem_active())
		return;

	section_size = (unsigned long)(uintptr_t)__end_data_wr -
			(unsigned long)(uintptr_t)__start_data_wr;
	update_mapping_prot(__pa_symbol(__start_data_wr),
			    (unsigned long)(uintptr_t)__start_data_wr,
			    section_size, PAGE_KERNEL_RO);
	hkip_register_rowm((void *)__start_data_wr,
			   ALIGN(section_size, PAGE_SIZE));
}

void mark_wr_after_init_data_wr(void)
{
	unsigned long section_size;

	if (!prmem_active())
		return;

	section_size = (unsigned long)(uintptr_t)__end_data_wr_after_init-
			(unsigned long)(uintptr_t)__start_data_wr_after_init;
	if (unlikely(!section_size)) /* Skip empty segment */
		return;
	update_mapping_prot(__pa_symbol(__start_data_wr_after_init),
			    (unsigned long)(uintptr_t)__start_data_wr_after_init,
			    section_size, PAGE_KERNEL_RO);
	hkip_register_rowm((void *)__start_data_wr_after_init,
			   ALIGN(section_size, PAGE_SIZE));
}
#else /* CONFIG_HKIP_PRMEM */
void mark_wr_data_wr(void)
{
}

void mark_wr_after_init_data_wr(void)
{
}
#endif /* CONFIG_HKIP_PRMEM */

void mark_rodata_ro(void)
{
	unsigned long section_size;
	unsigned long text_size;

	section_size = (unsigned long)__end_data_ro_after_init -
			(unsigned long)__start_data_ro_after_init;
	update_mapping_prot(__pa_symbol(__start_data_ro_after_init), (unsigned long)__start_data_ro_after_init,
			    section_size, PAGE_KERNEL_RO);
	hkip_register_ro((void *)__start_data_ro_after_init,
			 ALIGN(section_size, PAGE_SIZE));
	text_size = (unsigned long)_etext - (unsigned long)_text;
	hkip_register_rox_text((void *)_text, ALIGN(text_size, PAGE_SIZE));

	mark_wr_after_init_data_wr();
}

#ifdef CONFIG_DEBUG_HKIP_EARLY_RODATA_PROTECTION
void test_early_rodata_protection(void);
#endif
void mark_constdata_ro(void)
{
#ifdef CONFIG_DEBUG_HKIP_EARLY_RODATA_PROTECTION
	test_early_rodata_protection();
#else
	unsigned long start = (unsigned long)__start_rodata;
	unsigned long end = (unsigned long)__init_begin;
	unsigned long section_size = (unsigned long)end - (unsigned long)start;

	/*
	 * mark .rodata as read only. Use __init_begin rather than __end_rodata
	 * to cover NOTES and EXCEPTION_TABLE.
	 */
	update_mapping_prot(__pa_symbol(__start_rodata), start,
			    section_size, PAGE_KERNEL_RO);
	hkip_register_ro((void *)start, ALIGN(section_size, PAGE_SIZE));
#endif
	debug_checkwx();
}

static void __init map_kernel_segment(pgd_t *pgd, void *va_start, void *va_end,
				      pgprot_t prot, struct vm_struct *vma,
				      int flags, unsigned long vm_flags)
{
	phys_addr_t pa_start = __pa_symbol(va_start);
	unsigned long size = va_end - va_start;

#ifdef CONFIG_HKIP_PRMEM
	if (unlikely(!size)) /* Mapping an empty segment will BUG() */
		return;
#endif

	BUG_ON(!PAGE_ALIGNED(pa_start));
	BUG_ON(!PAGE_ALIGNED(size));

	__create_pgd_mapping(pgd, pa_start, (unsigned long)va_start, size, prot,
			     early_pgtable_alloc, flags);

	if (!(vm_flags & VM_NO_GUARD))
		size += PAGE_SIZE;

	vma->addr	= va_start;
	vma->phys_addr	= pa_start;
	vma->size	= size;
	vma->flags	= VM_MAP | vm_flags;
	vma->caller	= __builtin_return_address(0);

	vm_area_add_early(vma);
}

static int __init parse_rodata(char *arg)
{
	return strtobool(arg, &rodata_enabled);
}
early_param("rodata", parse_rodata);

#ifdef CONFIG_UNMAP_KERNEL_AT_EL0
static int __init map_entry_trampoline(void)
{
	extern char __entry_tramp_text_start[];

	/* change to the code which exist in kernel/git/arm64/linux.git */
	pgprot_t prot = rodata_enabled ? PAGE_KERNEL_ROX : PAGE_KERNEL_EXEC;
	phys_addr_t pa_start = __pa_symbol(__entry_tramp_text_start);

	/* The trampoline is always mapped and can therefore be global */
	pgprot_val(prot) &= ~PTE_NG;

	/* Map only the text into the trampoline page table */
	memset(tramp_pg_dir, 0, PGD_SIZE);
	__create_pgd_mapping(tramp_pg_dir, pa_start, TRAMP_VALIAS, PAGE_SIZE,
			     prot, pgd_pgtable_alloc, 0);

	/* Map both the text and data into the kernel page table */
	__set_fixmap(FIX_ENTRY_TRAMP_TEXT, pa_start, prot);
	if (IS_ENABLED(CONFIG_RANDOMIZE_BASE)) {
		extern char __entry_tramp_data_start[];

		__set_fixmap(FIX_ENTRY_TRAMP_DATA,
			     __pa_symbol(__entry_tramp_data_start),
			     PAGE_KERNEL_RO);
	}

	return 0;
}
core_initcall(map_entry_trampoline);
#endif

/*
 * Create fine-grained mappings for the kernel.
 */
static void __init map_kernel(pgd_t *pgd)
{
	static struct vm_struct vmlinux_text, vmlinux_rodata, vmlinux_inittext,
				vmlinux_initdata, vmlinux_data, vmlinux_ro_after_init_data;
#ifdef CONFIG_HKIP_PRMEM
	static struct vm_struct vmlinux_wr_data, vmlinux_wr_after_init_data,
				vmlinux_rw;
#endif
	/*
	 * External debuggers may need to write directly to the text
	 * mapping to install SW breakpoints. Allow this (only) when
	 * explicitly requested with rodata=off.
	 */
	pgprot_t text_prot = rodata_enabled ? PAGE_KERNEL_ROX : PAGE_KERNEL_EXEC;

	/*
	 * Only rodata will be remapped with different permissions later on,
	 * all other segments are allowed to use contiguous mappings.
	 */
	map_kernel_segment(pgd, _text, _etext, text_prot, &vmlinux_text, 0,
			   VM_NO_GUARD);
	map_kernel_segment(pgd, __start_data_ro_after_init, __end_data_ro_after_init, PAGE_KERNEL,
			   &vmlinux_ro_after_init_data, NO_CONT_MAPPINGS, VM_NO_GUARD);
	map_kernel_segment(pgd, __start_rodata, __inittext_begin, PAGE_KERNEL,
			   &vmlinux_rodata, NO_CONT_MAPPINGS, VM_NO_GUARD);
	map_kernel_segment(pgd, __inittext_begin, __inittext_end, text_prot,
			   &vmlinux_inittext, 0, VM_NO_GUARD);
	map_kernel_segment(pgd, __initdata_begin, __initdata_end, PAGE_KERNEL,
			   &vmlinux_initdata, 0, VM_NO_GUARD);
#ifdef CONFIG_HKIP_PRMEM
	map_kernel_segment(pgd, __start_data_wr, __end_data_wr, PAGE_KERNEL,
			   &vmlinux_wr_data, NO_CONT_MAPPINGS, VM_NO_GUARD);
	map_kernel_segment(pgd, __start_data_wr_after_init, __end_data_wr_after_init, PAGE_KERNEL,
			   &vmlinux_wr_after_init_data, NO_CONT_MAPPINGS, VM_NO_GUARD);
	map_kernel_segment(pgd, __start_data_rw, __end_data_rw, PAGE_KERNEL,
			   &vmlinux_rw, NO_CONT_MAPPINGS, VM_NO_GUARD);
#endif /* CONFIG_HKIP_PRMEM */
	map_kernel_segment(pgd, _data, _end, PAGE_KERNEL, &vmlinux_data, 0, 0);

	if (!pgd_val(*pgd_offset_raw(pgd, FIXADDR_START))) {
		/*
		 * The fixmap falls in a separate pgd to the kernel, and doesn't
		 * live in the carveout for the swapper_pg_dir. We can simply
		 * re-use the existing dir for the fixmap.
		 */
		set_pgd(pgd_offset_raw(pgd, FIXADDR_START),
			*pgd_offset_k(FIXADDR_START));
	} else if (CONFIG_PGTABLE_LEVELS > 3) {
		/*
		 * The fixmap shares its top level pgd entry with the kernel
		 * mapping. This can really only occur when we are running
		 * with 16k/4 levels, so we can simply reuse the pud level
		 * entry instead.
		 */
		BUG_ON(!IS_ENABLED(CONFIG_ARM64_16K_PAGES));
		pud_populate(&init_mm, pud_set_fixmap_offset(pgd, FIXADDR_START),
			     lm_alias(bm_pmd));
		pud_clear_fixmap();
	} else {
		BUG();
	}

	kasan_copy_shadow(pgd);
}

/*
 * paging_init() sets up the page tables, initialises the zone memory
 * maps and sets up the zero page.
 */
void __init paging_init(void)
{
	phys_addr_t pgd_phys = early_pgtable_alloc();
	pgd_t *pgd = pgd_set_fixmap(pgd_phys);

	map_kernel(pgd);
	map_mem(pgd);

	/*
	 * We want to reuse the original swapper_pg_dir so we don't have to
	 * communicate the new address to non-coherent secondaries in
	 * secondary_entry, and so cpu_switch_mm can generate the address with
	 * adrp+add rather than a load from some global variable.
	 *
	 * To do this we need to go via a temporary pgd.
	 */
	cpu_replace_ttbr1(__va(pgd_phys));
	memcpy(swapper_pg_dir, pgd, PGD_SIZE);
	cpu_replace_ttbr1(lm_alias(swapper_pg_dir));

	pgd_clear_fixmap();
	memblock_free(pgd_phys, PAGE_SIZE);

	/* Ensure the zero page is visible to the page table walker */
	dsb(ishst);

	/*
	 * We only reuse the PGD from the swapper_pg_dir, not the pud + pmd
	 * allocated with it.
	 */
#ifndef CONFIG_HISI_LB_L3_EXTENSION
	memblock_free(__pa_symbol(swapper_pg_dir) + PAGE_SIZE,
		      SWAPPER_DIR_SIZE - PAGE_SIZE);
#endif
}

#ifdef CONFIG_MEMORY_HOTPLUG
/*
 * hotplug_paging() is used by memory hotplug to build new page tables
 * for hot added memory.
 */
void hotplug_paging(phys_addr_t start, phys_addr_t size)
{
	int flags;

	flags = debug_pagealloc_enabled() ? NO_BLOCK_MAPPINGS : 0;
	__create_pgd_mapping(swapper_pg_dir, start, __phys_to_virt(start), size,
		PAGE_KERNEL, pgd_pgtable_alloc, flags);
}

#ifdef CONFIG_MEMORY_HOTREMOVE
#define PAGE_INUSE 0xFD

static void  free_pagetable(struct page *page, int order, bool direct)
{
	unsigned long magic;
	unsigned int nr_pages = 1 << order;

	/* bootmem page has reserved flag */
	if (PageReserved(page)) {
		__ClearPageReserved(page);

		magic = (unsigned long)page->lru.next;
		if (magic == SECTION_INFO || magic == MIX_SECTION_INFO) {
			while (nr_pages--)
				put_page_bootmem(page++);
		} else {
			while (nr_pages--)
				free_reserved_page(page++);
		}
	} else {
		/*
		 * Only direct pagetable allocation (those allocated via
		 * hotplug) call the pgtable_page_ctor; vmemmap pgtable
		 * allocations don't.
		 */
		if (direct)
			pgtable_page_dtor(page);

		free_pages((unsigned long)page_address(page), order);
	}
}

static void free_pte_table(pmd_t *pmd, bool direct)
{
	pte_t *pte_start = NULL;
	pte_t *pte = NULL;
	struct page *page = NULL;
	int i;

	pte_start =  (pte_t *) pmd_page_vaddr(*pmd);
	/* Check if there is no valid entry in the PMD */
	for (i = 0; i < PTRS_PER_PTE; i++) {
		pte = pte_start + i;
		if (!pte_none(*pte))
			return;
	}

	page = pmd_page(*pmd);

	free_pagetable(page, 0, direct);

	/*
	 * This spin lock could be only taken in _pte_aloc_kernel
	 * in mm/memory.c and nowhere else (for arm64). Not sure if
	 * the function above can be called concurrently. In doubt,
	 * I am living it here for now, but it probably can be removed
	 */
	spin_lock(&init_mm.page_table_lock);
	pmd_clear(pmd);
	spin_unlock(&init_mm.page_table_lock);
}

static void free_pmd_table(pud_t *pud, bool direct)
{
	pmd_t *pmd_start = NULL;
	pmd_t *pmd = NULL;
	struct page *page = NULL;
	int i;

	pmd_start = (pmd_t *) pud_page_vaddr(*pud);
	/* Check if there is no valid entry in the PMD */
	for (i = 0; i < PTRS_PER_PMD; i++) {
		pmd = pmd_start + i;
		if (!pmd_none(*pmd))
			return;
	}

	page = pud_page(*pud);

	free_pagetable(page, 0, direct);

	/*
	 * This spin lock could be only taken in _pte_aloc_kernel
	 * in mm/memory.c and nowhere else (for arm64). Not sure if
	 * the function above can be called concurrently. In doubt,
	 * I am living it here for now, but it probably can be removed
	 */
	spin_lock(&init_mm.page_table_lock);
	pud_clear(pud);
	spin_unlock(&init_mm.page_table_lock);
}

/*
 * When the PUD is folded on the PGD (three levels of paging),
 * there's no need to free PUDs
 */
#if CONFIG_PGTABLE_LEVELS > 3
static void free_pud_table(pgd_t *pgd, bool direct)
{
	pud_t *pud_start = NULL;
	pud_t *pud = NULL;
	struct page *page = NULL;
	int i;

	pud_start = (pud_t *) pgd_page_vaddr(*pgd);
	/* Check if there is no valid entry in the PUD */
	for (i = 0; i < PTRS_PER_PUD; i++) {
		pud = pud_start + i;
		if (!pud_none(*pud))
			return;
	}

	page = pgd_page(*pgd);

	free_pagetable(page, 0, direct);

	/*
	 * This spin lock could be only
	 * taken in _pte_aloc_kernel in
	 * mm/memory.c and nowhere else
	 * (for arm64). Not sure if the
	 * function above can be called
	 * concurrently. In doubt,
	 * I am living it here for now,
	 * but it probably can be removed.
	 */
	spin_lock(&init_mm.page_table_lock);
	pgd_clear(pgd);
	spin_unlock(&init_mm.page_table_lock);
}
#endif

static void remove_pte_table(pte_t *pte, unsigned long addr,
	unsigned long end, bool direct)
{
	unsigned long next;
	void *page_addr = NULL;

	for (; addr < end; addr = next, pte++) {
		next = (addr + PAGE_SIZE) & PAGE_MASK;
		if (next > end)
			next = end;

		if (!pte_present(*pte))
			continue;

		if (PAGE_ALIGNED(addr) && PAGE_ALIGNED(next)) {
			/*
			 * Do not free direct mapping pages since they were
			 * freed when offlining, or simplely not in use.
			 */
			if (!direct)
				free_pagetable(pte_page(*pte), 0, direct);

			/*
			 * This spin lock could be only
			 * taken in _pte_aloc_kernel in
			 * mm/memory.c and nowhere else
			 * (for arm64). Not sure if the
			 * function above can be called
			 * concurrently. In doubt,
			 * I am living it here for now,
			 * but it probably can be removed.
			 */
			spin_lock(&init_mm.page_table_lock);
			pte_clear(&init_mm, addr, pte);
			spin_unlock(&init_mm.page_table_lock);
		} else {
			/*
			 * If we are here, we are freeing vmemmap pages since
			 * direct mapped memory ranges to be freed are aligned.
			 *
			 * If we are not removing the whole page, it means
			 * other page structs in this page are being used and
			 * we canot remove them. So fill the unused page_structs
			 * with 0xFD, and remove the page when it is wholly
			 * filled with 0xFD.
			 */
			memset((void *)addr, PAGE_INUSE, next - addr);

			page_addr = page_address(pte_page(*pte));
			if (!memchr_inv(page_addr, PAGE_INUSE, PAGE_SIZE)) {
				free_pagetable(pte_page(*pte), 0, direct);

				/*
				 * This spin lock could be only
				 * taken in _pte_aloc_kernel in
				 * mm/memory.c and nowhere else
				 * (for arm64). Not sure if the
				 * function above can be called
				 * concurrently. In doubt,
				 * I am living it here for now,
				 * but it probably can be removed.
				 */
				spin_lock(&init_mm.page_table_lock);
				pte_clear(&init_mm, addr, pte);
				spin_unlock(&init_mm.page_table_lock);
			}
		}
	}

	/*
	 * I am adding this flush here in simmetry to the x86 code.
	 * Why do I need to call it here and not in remove_p[mu]d
	 */
	flush_tlb_all();
}

static void remove_pmd_table(pmd_t *pmd, unsigned long addr,
	unsigned long end, bool direct)
{
	unsigned long next;
	void *page_addr = NULL;
	pte_t *pte = NULL;

	for (; addr < end; addr = next, pmd++) {
		next = pmd_addr_end(addr, end);

		if (!pmd_present(*pmd))
			continue;

		/* check if we are using 2MB section mappings */
		if (pmd_sect(*pmd)) {
			if (PAGE_ALIGNED(addr) && PAGE_ALIGNED(next)) {
				if (!direct)
					free_pagetable(pmd_page(*pmd),
						get_order(PMD_SIZE), direct);

				/*
				 * This spin lock could be only
				 * taken in _pte_aloc_kernel in
				 * mm/memory.c and nowhere else
				 * (for arm64). Not sure if the
				 * function above can be called
				 * concurrently. In doubt,
				 * I am living it here for now,
				 * but it probably can be removed.
				 */
				spin_lock(&init_mm.page_table_lock);
				pmd_clear(pmd);
				spin_unlock(&init_mm.page_table_lock);
			} else {
				/* If here, we are freeing vmemmap pages. */
				memset((void *)addr, PAGE_INUSE, next - addr);

				page_addr = page_address(pmd_page(*pmd));
				if (!memchr_inv(page_addr, PAGE_INUSE,
						PMD_SIZE)) {
					free_pagetable(pmd_page(*pmd),
						get_order(PMD_SIZE), direct);

					/*
					 * This spin lock could be only
					 * taken in _pte_aloc_kernel in
					 * mm/memory.c and nowhere else
					 * (for arm64). Not sure if the
					 * function above can be called
					 * concurrently. In doubt,
					 * I am living it here for now,
					 * but it probably can be removed.
					 */
					spin_lock(&init_mm.page_table_lock);
					pmd_clear(pmd);
					spin_unlock(&init_mm.page_table_lock);
				}
			}
			continue;
		}

		BUG_ON(!pmd_table(*pmd));

		pte = pte_offset_map(pmd, addr);
		remove_pte_table(pte, addr, next, direct);
		free_pte_table(pmd, direct);
	}
}

static void remove_pud_table(pud_t *pud, unsigned long addr,
	unsigned long end, bool direct)
{
	unsigned long next;
	pmd_t *pmd = NULL;
	void *page_addr = NULL;

	for (; addr < end; addr = next, pud++) {
		next = pud_addr_end(addr, end);
		if (!pud_present(*pud))
			continue;
		/*
		 * If we are using 4K granules, check if we are using
		 * 1GB section mapping.
		 */
		if (pud_sect(*pud)) {
			if (PAGE_ALIGNED(addr) && PAGE_ALIGNED(next)) {
				if (!direct)
					free_pagetable(pud_page(*pud),
						get_order(PUD_SIZE), direct);

				/*
				 * This spin lock could be only
				 * taken in _pte_aloc_kernel in
				 * mm/memory.c and nowhere else
				 * (for arm64). Not sure if the
				 * function above can be called
				 * concurrently. In doubt,
				 * I am living it here for now,
				 * but it probably can be removed.
				 */
				spin_lock(&init_mm.page_table_lock);
				pud_clear(pud);
				spin_unlock(&init_mm.page_table_lock);
			} else {
				/* If here, we are freeing vmemmap pages. */
				memset((void *)addr, PAGE_INUSE, next - addr);

				page_addr = page_address(pud_page(*pud));
				if (!memchr_inv(page_addr, PAGE_INUSE,
						PUD_SIZE)) {

					free_pagetable(pud_page(*pud),
						get_order(PUD_SIZE), direct);

					/*
					 * This spin lock could be only
					 * taken in _pte_aloc_kernel in
					 * mm/memory.c and nowhere else
					 * (for arm64). Not sure if the
					 * function above can be called
					 * concurrently. In doubt,
					 * I am living it here for now,
					 * but it probably can be removed.
					 */
					spin_lock(&init_mm.page_table_lock);
					pud_clear(pud);
					spin_unlock(&init_mm.page_table_lock);
				}
			}
			continue;
		}

		BUG_ON(!pud_table(*pud));

		pmd = pmd_offset(pud, addr);
		remove_pmd_table(pmd, addr, next, direct);
		free_pmd_table(pud, direct);
	}
}

void remove_pagetable(unsigned long start, unsigned long end, bool direct)
{
	unsigned long next;
	unsigned long addr;
	pgd_t *pgd = NULL;
	pud_t *pud = NULL;

	for (addr = start; addr < end; addr = next) {
		next = pgd_addr_end(addr, end);

		pgd = pgd_offset_k(addr);
		if (pgd_none(*pgd))
			continue;

		pud = pud_offset(pgd, addr);
		remove_pud_table(pud, addr, next, direct);
		/*
		 * When the PUD is folded on the PGD (three levels of paging),
		 * I did already clear the PMD page in free_pmd_table,
		 * and reset the corresponding PGD==PUD entry.
		 */
#if CONFIG_PGTABLE_LEVELS > 3
		free_pud_table(pgd, direct);
#endif
	}

	flush_tlb_all();
}


#endif /* CONFIG_MEMORY_HOTREMOVE */
#endif /* CONFIG_MEMORY_HOTPLUG */

/*
 * Check whether a kernel address is valid (derived from arch/x86/).
 */
int kern_addr_valid(unsigned long addr)
{
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;

	if ((((long)addr) >> VA_BITS) != -1UL)
		return 0;

	pgd = pgd_offset_k(addr);
	if (pgd_none(*pgd))
		return 0;

	pud = pud_offset(pgd, addr);
	if (pud_none(*pud))
		return 0;

	if (pud_sect(*pud))
		return pfn_valid(pud_pfn(*pud));

	pmd = pmd_offset(pud, addr);
	if (pmd_none(*pmd))
		return 0;

	if (pmd_sect(*pmd))
		return pfn_valid(pmd_pfn(*pmd));

	pte = pte_offset_kernel(pmd, addr);
	if (pte_none(*pte))
		return 0;

	return pfn_valid(pte_pfn(*pte));
}
#ifdef CONFIG_SPARSEMEM_VMEMMAP
#if !ARM64_SWAPPER_USES_SECTION_MAPS
int __meminit vmemmap_populate(unsigned long start, unsigned long end, int node)
{
	return vmemmap_populate_basepages(start, end, node);
}
#else	/* !ARM64_SWAPPER_USES_SECTION_MAPS */
int __meminit vmemmap_populate(unsigned long start, unsigned long end, int node)
{
	unsigned long addr = start;
	unsigned long next;
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;

	do {
		next = pmd_addr_end(addr, end);

		pgd = vmemmap_pgd_populate(addr, node);
		if (!pgd)
			return -ENOMEM;

		pud = vmemmap_pud_populate(pgd, addr, node);
		if (!pud)
			return -ENOMEM;

		pmd = pmd_offset(pud, addr);
		if (pmd_none(*pmd)) {
			void *p = NULL;

			p = vmemmap_alloc_block_buf(PMD_SIZE, node);
			if (!p)
				return -ENOMEM;

			pmd_set_huge(pmd, __pa(p), __pgprot(PROT_SECT_NORMAL));
		} else
			vmemmap_verify((pte_t *)pmd, node, addr, next);
	} while (addr = next, addr != end);

	return 0;
}
#endif	/* CONFIG_ARM64_64K_PAGES */
void vmemmap_free(unsigned long start, unsigned long end)
{
#ifdef CONFIG_MEMORY_HOTREMOVE
	remove_pagetable(start, end, false);
#endif
}
#endif	/* CONFIG_SPARSEMEM_VMEMMAP */

static inline pud_t * fixmap_pud(unsigned long addr)
{
	pgd_t *pgd = pgd_offset_k(addr);

	BUG_ON(pgd_none(*pgd) || pgd_bad(*pgd));

	return pud_offset_kimg(pgd, addr);
}

static inline pmd_t * fixmap_pmd(unsigned long addr)
{
	pud_t *pud = fixmap_pud(addr);

	BUG_ON(pud_none(*pud) || pud_bad(*pud));

	return pmd_offset_kimg(pud, addr);
}

static inline pte_t * fixmap_pte(unsigned long addr)
{
	return &bm_pte[pte_index(addr)];
}

/*
 * The p*d_populate functions call virt_to_phys implicitly so they can't be used
 * directly on kernel symbols (bm_p*d). This function is called too early to use
 * lm_alias so __p*d_populate functions must be used to populate with the
 * physical address from __pa_symbol.
 */
void __init early_fixmap_init(void)
{
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	unsigned long addr = FIXADDR_START;

	pgd = pgd_offset_k(addr);
	if (CONFIG_PGTABLE_LEVELS > 3 &&
	    !(pgd_none(*pgd) || pgd_page_paddr(*pgd) == __pa_symbol(bm_pud))) {
		/*
		 * We only end up here if the kernel mapping and the fixmap
		 * share the top level pgd entry, which should only happen on
		 * 16k/4 levels configurations.
		 */
		BUG_ON(!IS_ENABLED(CONFIG_ARM64_16K_PAGES));
		pud = pud_offset_kimg(pgd, addr);
	} else {
		if (pgd_none(*pgd))
			__pgd_populate(pgd, __pa_symbol(bm_pud), PUD_TYPE_TABLE);
		pud = fixmap_pud(addr);
	}
	if (pud_none(*pud))
		__pud_populate(pud, __pa_symbol(bm_pmd), PMD_TYPE_TABLE);
	pmd = fixmap_pmd(addr);
	__pmd_populate(pmd, __pa_symbol(bm_pte), PMD_TYPE_TABLE);

	/*
	 * The boot-ioremap range spans multiple pmds, for which
	 * we are not prepared:
	 */
	BUILD_BUG_ON((__fix_to_virt(FIX_BTMAP_BEGIN) >> PMD_SHIFT)
		     != (__fix_to_virt(FIX_BTMAP_END) >> PMD_SHIFT));

	if ((pmd != fixmap_pmd(fix_to_virt(FIX_BTMAP_BEGIN)))
	     || pmd != fixmap_pmd(fix_to_virt(FIX_BTMAP_END))) {
		WARN_ON(1);
		pr_warn("pmd %p != %p, %p\n",
			pmd, fixmap_pmd(fix_to_virt(FIX_BTMAP_BEGIN)),
			fixmap_pmd(fix_to_virt(FIX_BTMAP_END)));
		pr_warn("fix_to_virt(FIX_BTMAP_BEGIN): %08lx\n",
			fix_to_virt(FIX_BTMAP_BEGIN));
		pr_warn("fix_to_virt(FIX_BTMAP_END):   %08lx\n",
			fix_to_virt(FIX_BTMAP_END));

		pr_warn("FIX_BTMAP_END:       %d\n", FIX_BTMAP_END);
		pr_warn("FIX_BTMAP_BEGIN:     %d\n", FIX_BTMAP_BEGIN);
	}
}

void __set_fixmap(enum fixed_addresses idx,
			       phys_addr_t phys, pgprot_t flags)
{
	unsigned long addr = __fix_to_virt(idx);
	pte_t *pte;

	BUG_ON(idx <= FIX_HOLE || idx >= __end_of_fixed_addresses);

	pte = fixmap_pte(addr);

	if (pgprot_val(flags)) {
		set_pte(pte, pfn_pte(phys >> PAGE_SHIFT, flags));
	} else {
		pte_clear(&init_mm, addr, pte);
		flush_tlb_kernel_range(addr, addr+PAGE_SIZE);
	}
}

void *__init __fixmap_remap_fdt(phys_addr_t dt_phys, int *size, pgprot_t prot)
{
	const u64 dt_virt_base = __fix_to_virt(FIX_FDT);
	int offset;
	void *dt_virt;

	/*
	 * Check whether the physical FDT address is set and meets the minimum
	 * alignment requirement. Since we are relying on MIN_FDT_ALIGN to be
	 * at least 8 bytes so that we can always access the magic and size
	 * fields of the FDT header after mapping the first chunk, double check
	 * here if that is indeed the case.
	 */
	BUILD_BUG_ON(MIN_FDT_ALIGN < 8);
	if (!dt_phys || dt_phys % MIN_FDT_ALIGN)
		return NULL;

	/*
	 * Make sure that the FDT region can be mapped without the need to
	 * allocate additional translation table pages, so that it is safe
	 * to call create_mapping_noalloc() this early.
	 *
	 * On 64k pages, the FDT will be mapped using PTEs, so we need to
	 * be in the same PMD as the rest of the fixmap.
	 * On 4k pages, we'll use section mappings for the FDT so we only
	 * have to be in the same PUD.
	 */
	BUILD_BUG_ON(dt_virt_base % SZ_2M);

	BUILD_BUG_ON(__fix_to_virt(FIX_FDT_END) >> SWAPPER_TABLE_SHIFT !=
		     __fix_to_virt(FIX_BTMAP_BEGIN) >> SWAPPER_TABLE_SHIFT);

	offset = dt_phys % SWAPPER_BLOCK_SIZE;
	dt_virt = (void *)dt_virt_base + offset;

	/* map the first chunk so we can read the size from the header */
	create_mapping_noalloc(round_down(dt_phys, SWAPPER_BLOCK_SIZE),
			dt_virt_base, SWAPPER_BLOCK_SIZE, prot);

	if (fdt_magic(dt_virt) != FDT_MAGIC)
		return NULL;

	*size = fdt_totalsize(dt_virt);
	if (*size > MAX_FDT_SIZE)
		return NULL;

	if (offset + *size > SWAPPER_BLOCK_SIZE)
		create_mapping_noalloc(round_down(dt_phys, SWAPPER_BLOCK_SIZE), dt_virt_base,
			       round_up(offset + *size, SWAPPER_BLOCK_SIZE), prot);

	return dt_virt;
}

void *__init fixmap_remap_fdt(phys_addr_t dt_phys)
{
	void *dt_virt;
	int size;

	dt_virt = __fixmap_remap_fdt(dt_phys, &size, PAGE_KERNEL_RO);
	if (!dt_virt)
		return NULL;

#ifdef CONFIG_HISI_LB_L3_EXTENSION
	memblock_reserve(dt_phys, ALIGN(size, SZ_2M));
#else
	memblock_reserve(dt_phys, size);
#endif

	return dt_virt;
}

int __init arch_ioremap_pud_supported(void)
{
	/*
	 * Only 4k granule supports level 1 block mappings.
	 * SW table walks can't handle removal of intermediate entries.
	 */
	return IS_ENABLED(CONFIG_ARM64_4K_PAGES) &&
	       !IS_ENABLED(CONFIG_ARM64_PTDUMP_DEBUGFS);
}

int __init arch_ioremap_pmd_supported(void)
{
	/* See arch_ioremap_pud_supported() */
	return !IS_ENABLED(CONFIG_ARM64_PTDUMP_DEBUGFS);
}

int pud_set_huge(pud_t *pudp, phys_addr_t phys, pgprot_t prot)
{
	pgprot_t sect_prot = __pgprot(PUD_TYPE_SECT |
					pgprot_val(mk_sect_prot(prot)));
	BUG_ON(phys & ~PUD_MASK);
	set_pud(pudp, pfn_pud(__phys_to_pfn(phys), sect_prot));
	return 1;
}

int pmd_set_huge(pmd_t *pmdp, phys_addr_t phys, pgprot_t prot)
{
	pgprot_t sect_prot = __pgprot(PMD_TYPE_SECT |
					pgprot_val(mk_sect_prot(prot)));
	BUG_ON(phys & ~PMD_MASK);
	set_pmd(pmdp, pfn_pmd(__phys_to_pfn(phys), sect_prot));
	return 1;
}

int pud_clear_huge(pud_t *pud)
{
	if (!pud_sect(*pud))
		return 0;
	pud_clear(pud);
	return 1;
}

int pmd_clear_huge(pmd_t *pmd)
{
	if (!pmd_sect(*pmd))
		return 0;
	pmd_clear(pmd);
	return 1;
}

int pud_free_pmd_page(pud_t *pud, unsigned long addr)
{
	return pud_none(*pud);
}

int pmd_free_pte_page(pmd_t *pmd, unsigned long addr)
{
	return pmd_none(*pmd);
}
