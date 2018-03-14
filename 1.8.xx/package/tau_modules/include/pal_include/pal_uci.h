#ifndef __PAL_UCI_H__
#define __PAL_UCI_H__



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
