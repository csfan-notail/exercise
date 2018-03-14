/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : lib_pa.h
  �� �� ��   : ����
  ��    ��   : WeiHonggang
  ��������   : 2013��11��28��
  ����޸�   :
  ��������   : lib_pa.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��11��28��
    ��    ��   : WeiHonggang
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/

#ifndef __LIB_PA_H__
#define __LIB_PA_H__
typedef struct
{
    unsigned char aucMac[6];
    unsigned int uiIp;
    unsigned int uiSec;
    unsigned int ui4GByteUp;
    unsigned int uiByteUp;
    unsigned int ui4GByteDown;
    unsigned int uiByteDown;
}PORTAL_STA_INFO_T;

extern  int portal_get_auth_state(unsigned char *pucMac,unsigned int *puiState);
extern  int portal_set_auth_state(unsigned char *pucMac,unsigned int uiState);
extern int portal_set_host_ip(unsigned int uiIp);
extern int portal_set_auth_server_ip(unsigned int uiIp);
extern int portal_set_snmp_server_ip(unsigned int uiIp);
extern int portal_set_set_free_auth_sw(unsigned int uiSW);
extern int portal_set_lan_if_name(unsigned char *pucIfName);
int portal_get_sta_list
    (unsigned int *puiStartNum,
    unsigned int *puiStaNum,
    void *ptr);
/* BEGIN:fengjing add 2014-09-10 */
int portal_get_all_sta_list
    (unsigned int *puiStartNum,
    unsigned int *puiStaNum,
    void *ptr);
/* END:fengjing add 2014-09-10 */
int portal_set_ccs_server_ip(unsigned int uiIp);
int portal_add_mac_black_list(unsigned char *pucMac);
int portal_del_mac_black_list(unsigned char *pucMac);
int portal_flush_mac_black_list(void);
int portal_add_mac_white_list(unsigned char *pucMac);
int portal_del_mac_white_list(unsigned char *pucMac);
int portal_flush_mac_white_list(void);
int portal_add_mac_free_auth(unsigned char *pucMac);
int portal_del_mac_free_auth(unsigned char *pucMac);
int portal_flush_mac_free_auth(void);
int portal_add_ip_free_auth(unsigned int uiIp);
int portal_del_ip_free_auth(unsigned int uiIp);
int portal_flush_ip_free_auth(void);
int portal_add_dest_ip_black_list(unsigned int uiIp,unsigned short usPortNum);
int portal_del_dest_ip_black_list(unsigned int uiIp,unsigned short usPortNum);
int portal_flush_dest_ip_black_list(void);
int portal_add_dest_ip_white_list(unsigned int uiIp,unsigned short usPortNum);
int portal_del_dest_ip_white_list(unsigned int uiIp,unsigned short usPortNum);
int portal_flush_dest_ip_white_list(void);
int portal_add_dest_net_black_list(unsigned int uiIp,unsigned int uiMask);
int portal_del_dest_net_black_list(unsigned int uiIp,unsigned int uiMask);
int portal_flush_dest_net_black_list(void);
int portal_add_url_black_list(unsigned char *pucUrl);
int portal_del_url_black_list(unsigned char *pucUrl);
int portal_flush_url_black_list(void);
/*begin: add@2014.11.03 ǿ���ض���url����*/
int portal_add_url_must_redirect_list(unsigned char *pucUrl);
int portal_del_url_must_redirect_list(unsigned char *pucUrl);
int portal_flush_url_must_redirect_list(void);
int portal_add_dest_ip_must_redirect_list(unsigned int uiIp,unsigned short usPortNum);
int portal_del_dest_ip_must_redirect_list(unsigned int uiIp,unsigned short usPortNum);
int portal_flush_dest_ip_must_redirect_list(void);
/*end: add@2014.11.03 ǿ���ض���url����*/
int portal_add_url_white_list(unsigned char *pucUrl);
int portal_del_url_white_list(unsigned char *pucUrl);
int portal_flush_url_white_list(void);
int portal_add_url_free_auth_list(unsigned char *pucUrl);
int portal_del_url_free_auth_list(unsigned char *pucUrl);
int portal_flush_url_free_auth_list(void);
int portal_add_https_url_free_auth_list(unsigned char *pucUrl);
int portal_del_https_url_free_auth_list(unsigned char *pucUrl);
int portal_flush_https_url_free_auth_list(void);


/*begin: add@2015.01.31 for wechat connect wifi*/
int portal_add_wxl_serverip(unsigned int uiIp, unsigned int uiMask);
int portal_del_wxl_serverip(unsigned int uiIp, unsigned int uiMask);
int portal_flush_wxl_serverip(void);
/*end: add@2015.01.31 for wechat connect wifi*/

#endif /* __LIB_PA_H__ */
