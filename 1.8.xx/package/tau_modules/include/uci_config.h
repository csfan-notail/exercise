#ifndef __UCI_CONFIG_H__
#define __UCI_CONFIG_H__



#define UCI_NAME_LEN 64

#define UCI_VALUE_LEN 64

#define UCI_CONFIG "config.@config[0]."

#define UCI_NETWORK "network."
#define UCI_GATEWAY "gateway.@gateway[0]."
#define UCI_SYSTEM "system.@system[0]."


#define UCI_NETWORK_WAN  UCI_NETWORK"wan."
#define UCI_NETWORK_LAN   UCI_NETWORK"lan."


#define UCI_NETWORK_WAN_IFNAME           UCI_NETWORK_WAN"ifname"
#define UCI_NETWORK_WAN_PROTO           UCI_NETWORK_WAN"proto"
#define UCI_NETWORK_WAN_IP                  UCI_NETWORK_WAN"ipaddr"
#define UCI_NETWORK_WAN_IP_NETMASK UCI_NETWORK_WAN"netmask"
#define UCI_NETWORK_WAN_DNS             UCI_NETWORK_WAN"dns"
#define UCI_NETWORK_WAN_GATEWAY     UCI_NETWORK_WAN"gateway"




#define UCI_NETWORK_LAN_IFNAME           UCI_NETWORK_LAN"ifname"
#define UCI_NETWORK_LAN_PROTO           UCI_NETWORK_LAN"proto"
#define UCI_NETWORK_LAN_IP                  UCI_NETWORK_LAN"ipaddr"
#define UCI_NETWORK_LAN_IP_NETMASK UCI_NETWORK_LAN"netmask"
#define UCI_NETWORK_LAN_DNS             UCI_NETWORK_LAN"dns"
#define UCI_NETWORK_LAN_GATEWAY     UCI_NETWORK_LAN"gateway"




#define UCI_LAN_IP_CONFIG			UCI_NETWORK"lan.ipaddr"

#define UCI_WAN_PROTO				UCI_NETWORK"wan.proto"

#define UCI_WAN_IFNAME				UCI_NETWORK"wan.ifname"


// Add: by xiaowei, 2016/03/23 02:05 PN: add tau uci defaulte
#define UCI_CONFIG_SNMP_TRAP_IP				"config.mini_snmpd.trap_ip"
#define UCI_CONFIG_SNMP_SWITCH				"config.mini_snmpd.switch"
#define UCI_CONFIG_SNMP_STATUS				"config.mini_snmpd.status"

#define UCI_LTEINFO_SIM_STATUS                     "lteinfo.info.sim_status"
#define UCI_LTEINFO_SIM_INFO_IMSI                  "lteinfo.info.sim_info_imsi"
#define UCI_LTEINFO_SIM_INFO_ICCID                 "lteinfo.info.sim_info_iccid"
#define UCI_LTEINFO_PIN_STATUS                     "lteinfo.info.pin_status"
#define UCI_LTEINFO_DIAL_STATUS                    "lteinfo.info.dial_status"
#define UCI_LTEINFO_CURRENT_ACCESS_NETWORK         "lteinfo.info.current_access_network"
#define UCI_LTEINFO_OPERATOR                       "lteinfo.info.operator"
#define UCI_LTEINFO_IPV4_ADDR                      "lteinfo.info.ipv4_addr"
#define UCI_LTEINFO_COMMUNITY_ID                   "lteinfo.info.community_id"               //逻辑小区
#define UCI_LTEINFO_PHYSICAL_COMMUNITY_ID          "lteinfo.info.physical_community_id"      //物理小区
#define UCI_LTEINFO_ENODEB			               "lteinfo.info.enodeb"
#define UCI_LTEINFO_TAC                            "lteinfo.info.tac"
#define UCI_LTEINFO_ACCESS_FREQUENCY               "lteinfo.info.access_frequency"           //频点 
#define UCI_LTEINFO_ACCESS_SPECTRUM                "lteinfo.info.access_spectrum"            //频段
#define UCI_LTEINFO_LOCK_SPECTRUM                  "lteinfo.info.lock_spectrum"              //锁频段
#define UCI_LTEINFO_INFO_RSSI                      "lteinfo.info.info_rssi"
#define UCI_LTEINFO_INFO_RSRP                      "lteinfo.info.info_rsrp"
#define UCI_LTEINFO_INFO_RSRQ                      "lteinfo.info.info_rsrq"
#define UCI_LTEINFO_INFO_SINR                      "lteinfo.info.info_sinr"
#define UCI_LTEINFO_LTE_VER                        "lteinfo.info.lte_ver"
#define UCI_LTEINFO_LTE_QOS_CID                        "lteinfo.info.lte_qos_cid"
#define UCI_LTEINFO_LTE_QOS_QCI                        "lteinfo.info.lte_qos_qci"
#define UCI_LTEINFO_LTE_QOS_DLGBR                        "lteinfo.info.lte_qos_dlgbr"
#define UCI_LTEINFO_LTE_QOS_ULGBR                        "lteinfo.info.lte_qos_ulgbr"
#define UCI_LTEINFO_LTE_QOS_DLMBR                      "lteinfo.info.lte_qos_dlmbr"
#define UCI_LTEINFO_LTE_QOS_ULMBR                      "lteinfo.info.lte_qos_ulmbr"


/************************************************/




//int nvram_set(char *name ,char *value);
//int nvram_unset(char *name);
//int nvram_get(char *value,char *name, int len);
//int nvram_commit(void);
//int nvram_set_and_commit(char *name ,char *value);
int uci_set(char *name, char *value);
int uci_delete(char *name);
int uci_get(char *name,char *value);
int uci_commit(void);
int uci_set_and_commit(char *name ,char *value);

int factory_env_get(char *value, char *name);

int uci_config_set(char *name, char *value);
int uci_config_delete(char *name);
int uci_config_get(char *name,char *value);
int uci_config_commit(void);
int uci_config_set_and_commit(char *name ,char *value);

int uci_get_test(void);

#endif
