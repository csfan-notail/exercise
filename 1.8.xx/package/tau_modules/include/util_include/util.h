/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : util.h
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2013��3��14��������
  ����޸�   :
  ��������   : util.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��3��14��������
    ��    ��   : ����
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

#ifndef __UTIL_H__
#define __UTIL_H__

#include <fcntl.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define MAC_FMT     "%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC_ARG(x) ((UINT8*)(x))[0],((UINT8*)(x))[1],((UINT8*)(x))[2],((UINT8*)(x))[3],((UINT8*)(x))[4],((UINT8*)(x))[5]

#define IP_FMT	    "%u.%u.%u.%u"
#define IP_ARG(x) ((UINT8*)(x))[0], ((UINT8*)(x))[1], ((UINT8*)(x))[2], ((UINT8*)(x))[3]


 /*----------------------------------------------*
 * �����                                       *
 *----------------------------------------------*/
#ifndef UINT8
#define UINT8  unsigned char
#endif


 
 /*�ַ����Ƚ�*/
#define STR_MATCH(szStr1, szStr2) (0 == strcmp(szStr1, szStr2))
/*�޶����ȵ��ַ����Ƚ�*/
#define STR_NMATCH(szStr1, szStr2, uiSize) (0 == strncmp(szStr1, szStr2, uiSize))
/*�ڴ��Ƚ�*/
#define MEM_MATCH(uiAddr1, uiAddr2, uiSize) (0 == memcmp(uiAddr1, uiAddr2, uiSize))



extern int util_find_pid_by_name(char *pcPidName, int *piPid);
extern char *util_itoa(int value, char *str, int radix);
extern int util_lock_file(struct flock *stLock,int iComfd);
extern int util_system_cmd( char *szCmd );
extern int util_unlock_file(struct flock *stLock,int iComfd);
extern int util_str_to_number( char *pcScrStr, int *piDesInt ,char cFlag);
extern int util_ip_to_str( unsigned char *pucIp, char *pcIpAddr );
extern int util_str_to_ip( char *pcIpAddr, unsigned char *pucIp );
extern int util_str_to_uint8( char *pcScrStr, UINT8 *piDesInt ,char cFlag);
extern unsigned char atou8(char *string);
extern int util_look_up_ipaddr(char *pcStr, char *pcDesIP);
extern unsigned char atou8x( char *string );
extern int util_get_file_lines( FILE *fp );
extern int util_find_str_from_file( char *szFileName,char *szString  );


unsigned char util_macstr_to_uint8( char *pcScrStr, UINT8 *piDesInt ,char cFlag );
char *util_mac_to_str(unsigned char * pucMac, char * pcMacStr);
int  util_array_string_to_int( char *src,char *dst,unsigned int *total );
int  util_array_del_someone( char *src,int id );
extern int send_signal(pid_t pid, int sig) ;
extern char** util_my_split(char * str, char sym);
extern void util_bubble_sort(int a[], int n);
extern int util_del_file_lines(char *file, int iDelHeadL, int iDelTailL);
extern char *util_shmem_create(unsigned int iShMemID, unsigned int iSize);
extern int util_shmem_detach(char *acSharedMemory);
extern int util_shmem_destroy(unsigned int iShMemID);
extern int util_get_mac_by_dev(char * dev, unsigned char * pucMac);
extern int util_set_mac(char * pcDev, unsigned char * pucMac);




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __UTIL_H__ */




