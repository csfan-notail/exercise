/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : tool.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/5/29
  Last Modified :
  Description   : tool.c header file
  Function List :
  History       :
  1.Date        : 2013/5/29
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#ifndef __TOOL_H__
#define __TOOL_H__

#define OK      0
#define ERROR   1

#define MAC_FMT         "%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC_AGR(x)      ((unsigned char *)(x))[0], ((unsigned char *)(x))[1], ((unsigned char *)(x))[2], \
                        ((unsigned char *)(x))[3], ((unsigned char *)(x))[4], ((unsigned char *)(x))[5]

#define IP_FMT          "%u.%u.%u.%u"
#define IP_AGR(x)       ((unsigned char*)(x))[0], ((unsigned char*)(x))[1], ((unsigned char*)(x))[2], ((unsigned char*)(x))[3]

extern int string_mac_tools(char *srcmac, char *dstmac);
extern int string_ip_tools(char *srcip, char *dstip);
extern int array_string_to_int(char *src, char *dst, int *total);
extern int array_string_to_integer(char *src, int *dst, int *total);
extern int array_string_add_someone(char *src, int id);
extern int array_string_del_someone(char *src, int id);
extern int pwd_decryption(char *str, int key_format, int len);
extern int pwd_encryption(char *str, int key_format, int len);
extern char *ReplaceToString(char * source, char *S_str, char * D_str);
extern void string_foramt_rates(char *rate_list, int length);
extern char *cmd_return_str(char *cmd);

#endif /* __TOOL_H__ */
