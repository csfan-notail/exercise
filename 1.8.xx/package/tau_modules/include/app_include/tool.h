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
#define EXIST  -1


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
extern int addr_string_to_ip_string(const char *addr, char *ip, int len);
extern int is_ip_valid(const char *ip);
extern int get_if_ip(const char *ifname, char *ifip, int len);
extern int get_if_mac(const char *ifname, char *ifmac, int len);
extern int get_wan_if(char *wan_if, int len);
extern int sys_log_configfile(char *config_file, char *log_upload_type, 
	char *log_upload_addr, char *log_upload_port);
extern void sys_log_upload(const char *ident, int option, int facility, 
	int priority, const char *format, ...);


extern int array_string_count_char_num(char *pcStr , char cChar);
#endif /* __TOOL_H__ */
