/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : syslib.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/6/18
  Last Modified :
  Description   : 
  Function List :
  History       :
  1.Date        : 2013/6/18
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#ifndef __SYSLIB_H__
#define __SYSLIB_H__

#define OEM_INFO_PATH       "/tmp/oem_info.txt"

#define NVRAM_MESSAGES      "/tmp/nvram.tmp"
#define PRODUCT_INFO_PATH   "/etc/.product_info"
#define BOA_CONFIG_FILE_PATH "/tmp/boa/boa.conf"

#define CONFIG_X86  "/factory/.factory"

typedef struct netbak{
	char net_forward_mode[16];
	char  ipv4_ip_method[16];
	char ipv4_ip_gw[16];
	char ipv4_0_ip_dns[16];
	char ipv4_0_ip_address[16];
	char ipv4_0_ip_mask[16];
	char ipv4_1_ip_dns[16];
	char ipv4_pppoe_name[16];
	char ipv4_pppoe_pwd[16];
	char ipv4_lte_access[16];
}netbak_t;

extern product_info g_product;

extern char *argv_init(int total, char *argv []);
extern void argv_free(char *list);
extern int analysis(char *src, char *mapping, char *result);
extern int config_vap(vap_config *pst_vap, int vap_num);
extern int config_wifi(vap_config *pst_vap, int vap_num, wifi_config *pst_wifi, int wifi_num);
/* BEGIN: Added by WeiHonggang, 2014/10/15   PN:TRAC#772 自动信道选择优化 */
extern int config_wifi_special(vap_config *pst_vap, int vap_num, wifi_config *pst_wifi, int wifi_num);
/* END:   Added by WeiHonggang, 2014/10/15   PN:TRAC#772 自动信道选择优化 */
extern int create_vap(vap_config *pst_vap, int vap_num, wifi_config *pst_wifi, int wifi_num);
extern int factory_restore_init(int *flag);
extern int get_board_info(wifi_info *pst_wifi, int device_type);
extern int get_oem_info(oem_info *pst_oem, int device_type);
extern int oma_advanced(void);
extern int synchronizer_config(void);
extern int get_device_id(void);
extern int get_customer_prosn(char *sn);
extern int web_init(void);
extern int ap_flow_init(void);
extern int users_config(void);
extern int wifidog_config(void);
extern int wifi_device_init(void);
extern int wifi_init_destory(wifi_config *pst_wifi, vap_config *pst_vap);
extern int wifi_init_start(wifi_config **pst_wifi, int *wifi_num, vap_config **pst_vap, int *vap_num);
extern int wan_rate_init(void);
extern int net_advanced_init(void);
extern int dnsmasq_init(void);
extern int ipv4_init(void);
extern int route_init(void);
extern void bridge_init();
extern int vlan_mapping_init(void);
extern int vlan_flowctrl_init();
extern int vlan_add_init(void);
extern int snmp_services(void);
extern int wifidog_services(void);
#ifdef HAVE_NETSNIFF
extern int netsniff_services(void);
#endif
extern int tr69c_services(void);
extern int cdn_services(void);

extern int dnsmasq_services(void);
int multicast_services(void);
extern int dev_advanced_init(void);
extern int set_machine_code(void);
extern void wan_mac_init(void);
extern int eth_mac_init(void);
#ifdef LTE_SUPPORT
extern int lte_mac_init(void);
#endif
extern int get_oem_company(char *company);
extern int get_dev_type();
extern void get_device_type(char *model);
extern void getDevSoftwearVer(char *Dst);
/* BEGIN: Added by WeiHonggang, 2014/3/6   PN:TASK#0LTE-CPE拨号函数添加 */
#if defined(LTE_OAM)
extern  int lte_operation( void );
#endif
/* END:   Added by WeiHonggang, 2014/3/6   PN:TASK#0LTE-CPE拨号函数添加 */
/* BEGIN: Added by WeiHonggang, 2014/4/11   PN:TASK#0 定位功能相关 */
#if (!defined(GW_X86) && !defined(GW_ARM) && !defined(GW_AC))
extern int locator_services(void);
extern int stadct_services(void);
#endif
/* END:   Added by WeiHonggang, 2014/4/11   PN:TASK#0 定位功能相关 */
/* BEGIN: Added by cj, 2014/6/20   PN:添加vrrp主备功能 */
#if (defined(GW_X86) || defined(GW_ARM) || defined(GW_AC))
int vrrp_config(void);
int vrrp_services(void);
#endif
/* END:   Added by cj, 2014/6/20   PN:添加vrrp主备功能 */
#endif

#ifdef HAVE_WXFLOW
extern int wxflow_services(void);
#endif

extern int audit_services(void);

#ifdef LTE_SUPPORT
int wan_is_3g_4g( void );
#endif

#ifdef AD_SUPPORT
extern int advertisement_init();
#endif

