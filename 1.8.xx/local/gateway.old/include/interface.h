/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : interface.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/5/27
  Last Modified :
  Description   : 接口头文件定义
  Function List :
  History       :
  1.Date        : 2013/5/27
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "app_public.h"

#define BACKUP_NAME         "/tmp/backup.dat"

/* 加密配置脚本路径 */
#define ENCRYPT_PATH        ("/tmp/%s.hostpad.conf")

#define CONFIG_VLAN         "/tmp/vlan_config.sh"

#define CONFIG_DHCP         "/tmp/dhcp_config.sh"

/*begin: add@2014.9.12*/
/*ROUTE*/
#define OM_ROUTE_STATIC_MAX 32

typedef enum 
{
    ROUTE_STATIC_DST_IP        =0,
    ROUTE_STATIC_DST_NETMASK      =1,   
    ROUTE_STATIC_NEXT_STOP      =2,   
    ROUTE_STATIC_PRI      =3,   
    ROUTE_STATIC_IF_NAME      =4,   
}ROUTE_STATIC_INFO_E;

/*end: add@2014.9.12*/

#define OM_PORTMAP_MAX 32

typedef enum 
{
    PORTMAP_OUTPORT        =0,
    PORTMAP_INPORT      =1,   
    PORTMAP_IP      =2,   
    PORTMAP_PROT      =3, 
}PORTMAP_STATIC_INFO_E;



/*----------------------- app_interface.c -----------------------*/
extern int device_info_get(char *dev_info, char *nvram_name);
extern int device_uptime_get(char *uptime, int language);
extern int device_time_get(char *time);
extern int check_user_login(user_login *u);
extern int set_user(users *pst_user);
extern int set_backup(int flag);
extern int get_sta_system_info(stas_info *pstUserInfo);
extern int sta_flow_write(vap_qos *pst_qos);
extern int sta_flow_read(vap_qos *pst_qos);
extern int sta_flow_config(char *name, char *mac, int flag);
extern int recover_config(char *name);
extern int snmp_configfile_set(char *read, char *write);
extern int check_ap_mac(char *wifi0_mac, char *wifi1_mac, char *eth0_mac, char *msg);
extern int get_user_level(char *username);
extern int del_vlan_to_vap(int vlan_id);
extern int flow_synchro_vap(void);
extern int flow_synchro_ap(void);
extern int config_ap_flow(int type, int ap_upload, int ap_download);
extern int ap_sta_flow_write(int type, int upload, int download);
extern int config_vlan_mapping(vlan_info *vlan);
extern int config_single_apmode();
extern int config_single_vlan_mapping(vlan_info *vlan);
extern int device_cpu_get(int *cpu);
extern int device_mem_get(int *mem);
extern int set_wifidog_conf(char *hotspot_name, char *hotspot_url, char *hotspot_path,
	char *ios_auto_portal, char *ccs_server, char *snmp_server);
extern int set_free_auth(char *port_name, int auth_state);
extern int export_log(void);
extern int config_vap_vlan_mapping(vlan_map *maps);
extern int vlan_clear_interface(char *vlanList, vlan_map *maps, int num);
extern int vlan_config_interface(char *vlanList, vlan_map *maps, int num, int flag);
extern int vlan_map_clear(char *vlanList, vlan_map *maps, int num);
extern int vlan_map_config(char *vlanList, vlan_map *maps, int num, int flag);
extern int vlan_map_clear_gw(char *vlanList, vlan_map *maps, int num);
extern int vlan_map_config_gw(char *vlanList, vlan_map *maps, int num, int flag);
extern int wifidog_status(void);

/*----------------------- network_interface.c -----------------------*/
extern int link_encap_ip_get(unsigned char *ip, char *port);
extern int link_encap_ip_set(unsigned char *ip, char *port);
extern int link_encap_mac_get(unsigned char *mac, char *port);
extern int link_encap_mask_set(unsigned char *mask, char *port);
extern int link_encap_ipinfo_get(char *addr, char *mask, char *gw, char *port);
extern int network_advanced_config(net_advanced *pst_adv, net_advanced *pst_adv_old, int flag);
extern int network_ethernet_rate_config(wan_rate *pst_wan, wan_rate *pst_wan_old, int flag);
extern int network_ipv4_config(ipv4 *pst_ipv4, ipv4 *pst_ipv4_old, int flag);
extern int ifconfig_device_down(char *name);
extern int ifconfig_device_up(char *name);
extern int bridge_set_config(int id, int flag);
extern int vlan_set_config(vlan_info *pst_vlan, int flag);
extern int vlan_create(char *port, int tag, int flag);
extern int vlan_mapping_config(vlan_map *pst_map, vlan_map *pst_mapOld, int flag);
extern int ssid_flow_config(char *name, int flag, int download, int upload);
extern int link_encap_port_status(char *port);
extern int arp_tables_get(arp_tables *arp);
extern int sta_info_all(sta_info *sta);
extern int link_encap_ethspeed(char *speed);
extern int get_ap_single(int * single);
extern int network_dhcp_config(dhcp_server *pst_dhcp, dhcp_server *pst_dhcp_old, int flag, int change_flag);
extern int fireware_set(int mode, int flag);
extern int dhcp_info_del(dhcp_server *pst_dhcp, vlan_map *vlan, int port_num, FILE *pf);
#if (defined(GW_X86) || defined(GW_ARM) || defined(GW_AC))
extern int dhcpd_conf_file_write(dhcp_server_param *dhcp_para, int flag, char *lan_ip, char *opt43, char *opt15);
/* BEGIN: Added by cj, 2014/6/20   PN:添加vrrp主备功能 */
extern int set_vrrp_conf(char *vrrp_mode, char *vrrp_interface, char *vrrp_priority,dhcp_server *pstDhcp);
/* END:   Added by cj, 2014/6/20   PN:添加vrrp主备功能 */
#else
extern int dnsmasq_conf_file_write(dhcp_server_param *dhcp_para, int flag, char *lan_ip, char *opt43, char *opt15);
#endif
extern int dhcp_info_create(dhcp_server *pst_dhcp, vlan_map *vlan, int port_num, FILE *pf);
extern char *getDevMac(int iFormat);

/*----------------------- wireless_interface.c -----------------------*/
extern int vap_info_config(vap *pst_vap, int flag);
extern int vap_advanced_config(vap_advanced *pst_vap, vap_advanced *pst_vap_old, char *vap_name, int flag);
extern int vap_basic_config(vap_basic *pst_vap, vap_basic *pst_vap_old, char *vap_name, int flag);
extern int vap_encrypt_config(encrypt *en, encrypt *en_old, char *vap_name, char *ssid_name, int flag);
extern int wifi_advanced_config(wifi_advanced *pst_ad, wifi_advanced *pst_ad_old, char *vap_name, int wifi_id, int flag);
extern int wifi_mode_config(wifi_80211n *pst_mode, wifi_80211n *pst_mode_old, char *wifi_name, char *vap_name, int wifi_id, int flag);
extern int wifi_txpower_config(wifi_txpower *pst_power, wifi_txpower *pst_power_old, char *vap_name, int flag);
extern int wifi_monitor_config(wifi_envmoni * pst_moni, wifi_envmoni * pst_moni_old, int wifiId,int iflag);
extern int wlan_set_ampdu(char *wifi_name, int ampdu);
extern int wlan_set_amsdu(char *wifi_name, int amsdu);
extern int wlan_set_band(char *vap_name, int band, int mode);
extern int wlan_set_beacon_interval(char *vap_name, int beacon_interval);
extern int wlan_set_channel(char *vap_name, int channel);
extern int wlan_set_encrypt_clear(char *vap_name);
extern int wlan_set_encrypt_shared(char *vap_name, shared *pst_shared);
extern int wlan_set_encrypt_wep(char *vap_name, wep *pst_wep);
extern int wlan_set_encrypt_wpa_enterprise(char *vap_name, char *ssid_name, int wpa, wpa_enterprise *pst_wpa);
extern int wlan_set_encrypt_wpa_personal(char *vap_name, char *ssid_name, int wpa, wpa_personal *pst_wpa);
extern int wlan_set_inact(char *vap_name, int inact);
extern int wlan_set_mode(int mode, int band, char *vap_name);
extern int wlan_set_protmode(char *vap_name, int cts);
extern int wlan_set_pureg(char *vap_name, int param);
extern int wlan_set_puren(char *vap_name, int param);
extern int wlan_set_rfnum(char *wifi_name, int channel);
extern int wlan_set_shortgi(char *vap_name, int shortgi);
extern int wlan_set_shpreamble(char *vap_name, int shpreamble);
extern int wlan_set_ssid_dtim(char *vap_name, int dtim);
extern int wlan_set_ssid_enabled(char *vap_name, int enabled);
extern int wlan_set_ssid_frag(char *vap_name, int frag);
extern int wlan_set_ssid_hide(char *vap_name, int hide);
extern int wlan_set_ssid_isolate(char *vap_name, int isolate);
extern int wlan_set_ssid_max_users(char *vap_name, int users);
extern int wlan_set_ssid_name(char *vap_name, char *ssid_name);
extern int wlan_set_ssid_rts(char *vap_name, int rts);
extern int wlan_set_wds(char *vap_name, int wds);
extern int wlan_set_txpower(char *vap_name, int current);
extern int wlan_set_vap_create(vap *pst_vap, char *vap_name, char *wifi_name);
extern int wlan_set_vap_destroy(char *vap_name);
extern int wlan_set_countrycode(int countrycode);
extern int wlan_set_11n(char *vap_name, int param, int delaytime, int statimeout);
extern int wlan_set_5g(char *wifi_name, int param, int delaytime, int statimeout);
extern int wlan_vap_up_all(int wifi_id, char *name);
extern int wlan_vap_down_all(int wifi_id);
extern int wlan_sta_get(char *vap_name, sta_info *pst_stas, int *total);
extern int get_wifi_sys_info(sys_wifi_info * stwifi_info, char vap_id, int wifi_id);
extern int replace_wifi_info(char * pcWifi24List, char * pcWifi5List);
extern int get_wifi_stat_info(sys_wifi_stat * pstwifi_stat, int vap_id);
extern int replace_wifi_stat(char * pcWifi24List, char * pcWifi5List);
extern int wifi_rate_config(wifi_rate *pst_rate, wifi_rate *pst_rateOld, char *wifi_name, int wifi_id, int flag);
extern int wlan_set_mode_all(int wifi_id, int mode, int band);
extern int wlan_set_channel_all(int wifi_id, int channel);
extern int wlan_set_rssi_mode(char *vap_name, int rssi_mode);
extern int wlan_set_rssi_static_threshold(char *vap_name, int rssi);
extern int wlan_set_rssi_static_threshold_all(int wifi_id, int rssi);
extern int wlan_set_acl_flush(char *vap_name);
extern int wlan_set_acl_mac(char *vap_name, char *mac);
extern int wlan_set_acl_maccmd(char *vap_name, int acl, int rule);
extern int wlan_set_date_ht_rates(char *vap_name, char *rate_list);
extern int wlan_set_date_basic_rates(char *vap_name, char *rate_list);
extern int wifi_get_channel(char *aucDevName, int *channNo);

/*begin: add@2014.9.12*/
//extern int  om_net_route_show(ROUTE_ITERM_T *pstRouteIterm);
extern int  om_net_route_get(OM_NET_ROUTE_STATIC_CFG_T *pstRouteInfoCfg);
extern int  om_net_route_del(OM_NET_ROUTE_STATIC_CFG_T *pstRouteInfoCfgNew);
extern int  om_net_route_add(OM_NET_ROUTE_STATIC_CFG_T *pstRouteInfoCfg);
extern int om_net_get_route_list(OM_NET_ROUTE_STATIC_CFG_T *pstRouteInfoCfg, int *piNum);
extern int om_net_add_new_route(OM_NET_ROUTE_STATIC_CFG_T *pstRouteInfoCfg);
extern int om_net_del_one_route(OM_NET_ROUTE_STATIC_CFG_T *pstRoute);
extern int  om_net_route_init(void);
/*end: add@2014.9.12*/

extern int  om_net_portmap_get(OM_NET_PORTMAP_STATIC_CFG_T *pstPortmapInfoCfg);
extern int  om_net_portmap_del(OM_NET_PORTMAP_STATIC_CFG_T *pstPortmapInfoCfgNew);
extern int  om_net_portmap_add(OM_NET_PORTMAP_STATIC_CFG_T *pstPortmapInfoCfg);
extern int  om_net_get_portmap_list(OM_NET_PORTMAP_STATIC_CFG_T *pstPortmapInfoCfg, int *piNum);
extern int  om_net_add_new_portmap(OM_NET_PORTMAP_STATIC_CFG_T *pstPortmapInfoCfg);
extern int  om_net_del_one_portmap(OM_NET_PORTMAP_STATIC_CFG_T *pstPortmapInfoCfg);
extern int  om_net_portmap_init(void);

#endif

