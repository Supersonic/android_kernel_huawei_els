

#ifndef __HMAC_RESET_H__
#define __HMAC_RESET_H__

/* 1 ����ͷ�ļ����� */
#include "mac_vap.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_RESET_H

#ifdef __cplusplus // windows ut���벻����������������
#if __cplusplus
extern "C" {
#endif
#endif

/* 2 �궨�� */
/* 3 ö�ٶ��� */
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
uint32_t hmac_reset_sys_event(mac_vap_stru *pst_mac_vap, uint8_t uc_len, uint8_t *puc_param);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_reset.h */