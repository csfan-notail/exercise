/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : firewall.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/11/26
  Last Modified :
  Description   : firewall.h header file
  Function List :
  History       :
  1.Date        : 2013/11/26
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#ifndef __FIREWALL_H__
#define __FIREWALL_H__

/** Used by iptables_fw_access to select if the client should be granted of denied access */
typedef enum fw_access_t_ {
    FW_ACCESS_ALLOW,
    FW_ACCESS_DENY,
    FW_ACCESS_DEL
} fw_access_t;

/** Used by fw_iptables.c */
typedef enum _t_fw_marks {
    FW_MARK_PROBATION = 1, /**< @brief The client is in probation period and must be authenticated 
			    @todo: VERIFY THAT THIS IS ACCURATE*/
    FW_MARK_KNOWN = 2,  /**< @brief The client is known to the firewall */
    FW_MARK_AUTH_ONGOING = 3,
    FW_MARK_LOCKED = 254 /**< @brief The client has been locked out */
} t_fw_marks;

extern int fw_destroy(void);
extern int fw_init(s_config *config);
extern int nf_fw_access(fw_access_t type, const char *ip, const char *mac);
extern char *arp_get(char *req_ip);

extern int fw_set_free_auth(void);
extern int fw_set_auth(void);

extern int fw_qos_init(void);
extern int fw_qos_reinit(void);
extern int fw_qos_add(t_client *client);
extern int fw_qos_del(const char *mac);
/* BEGIN: Added by WeiHonggang, 2014/4/9   PN:BUG #5289  云平台关闭流控，终端重新认证，仍被限速 */
extern int fw_qos_flush(void);
/* END:   Added by WeiHonggang, 2014/4/9   PN:BUG #5289  云平台关闭流控，终端重新认证，仍被限速 */

#endif /* __FIREWALL_H__ */
