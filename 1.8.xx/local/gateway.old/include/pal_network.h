#ifndef __PAL_NETWORK_H__
#define __PAL_NETWORK_H__
enum{
	IP_MODE_STATIC,
	IP_MODE_DHCP,
	IP_MODE_PPPOE
};

typedef struct
{
	UINT8  aucInterfaceName[32];
	UINT32  ulIpMode;           /* IP模式:static(0), dhcp(1) */
	UINT8   aucIpAddress[4];    /* IP 地址 */
	UINT8   aucIpNetMask[4];    /* 子网掩码 */
	UINT8   aucGwAddress[4];    /* 网关地址 */
	UINT8   aucDnsMaster[4];    /* 主用DNS服务器 */
	UINT8   aucDnsSlave[4];     /* 备用DNS服务器 */
}SYS_IP_ENTRY_T;

#define DHCP_SERVER_NUM 16

typedef struct
{
	UINT16 dhcp_index;
	UINT16 dhcp_enable;
	UINT8 dhcp_interface[32];
	UINT8 dhcp_start_ip[32];
	UINT8 dhcp_stop_ip[32];
	UINT8 dhcp_lease_time[32];
}DHCP_SERVER_INFO;


typedef struct
{
	UINT16 dhcp_server_num;    
	DHCP_SERVER_INFO dhcp_server_info[DHCP_SERVER_NUM];
}DHCP_SERVER_T;




#define DHCP_LEASE_FILE "/tmp/dhcp.leases"

typedef struct
{
	UINT8 dhcp_client_ip[4];
	UINT8 dhcp_client_mac[6];
	UINT8 dhcp_client_devinfo[32];
}DHCP_CLIENT_INFO;

int pal_dhcp_server_info_get(DHCP_SERVER_T *pstDhcpServer);

#endif
