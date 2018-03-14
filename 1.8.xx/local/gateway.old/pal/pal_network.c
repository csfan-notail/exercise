#include "pal_common.h"
#include "pal_uci.h"
#include "pal_network.h"



/*wan ip */
int pal_network_wan_ip_get(u8 *wan_ip)
{
    struct ifreq ifr;
    struct sockaddr_in *wanaddr = NULL, *wanmask = NULL;
    int sockfd;
    int ret=0;
    char wanport[UCI_VALUE_LEN] = {0};
    char uci_value[UCI_VALUE_LEN] = {0};
    unsigned char ip_addr[IP_LEN];
    uci_config_get(uci_value,UCI_NETWORK_WAN_PROTO);
    if(!strncmp(uci_value,"pppoe",strlen("pppoe")))
        strncpy(wanport,"pppoe-wan",sizeof("pppoe-wan"));
    else
    {
    if(uci_config_get(uci_value,UCI_NETWORK_WAN_IFNAME) == 0)
	    strncpy(wanport,uci_value,strlen(uci_value));
    else
	    strncpy(wanport,"eth0",strlen("eth0"));
    }
    while(1)
    {
    	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    		return -1;
    	strncpy(ifr.ifr_name, wanport, IFNAMSIZ-1);
    	if((ret = ioctl(sockfd, SIOCGIFADDR, &ifr)) == 0){
    		wanaddr = (struct sockaddr_in *)&(ifr.ifr_addr);
    		if(ntohl(wanaddr->sin_addr.s_addr) != 0xC0A80101){ /*不是默认地址，说明已经拿到地址*/
    			memcpy(ip_addr,(u8 *)&wanaddr->sin_addr.s_addr,IP_LEN);
                          sprintf(wan_ip,IP_FMT,IP_ARG(ip_addr));
                          gw_log(LOG_ERR, "wan ip :(%s) ", wan_ip);
    			close(sockfd);
    			break;
    		}
    	}
    	close(sockfd);
    	sleep(5);
    }

    return OK;
}



int pal_network_mac_get(unsigned char *mac, char *port)
{
     int skdf;
    struct ifreq ifr;
    skdf = -1;
    skdf = creat_socket();
    if(-1 == skdf)
    {
        gw_log(APP_LOG_ERR, "creat socket failed");
        return ERROR;
    }

    memset(&ifr, 0, sizeof(struct ifreq));
    strncpy(ifr.ifr_name, port, IFNAMSIZ);
    ifr.ifr_name[IFNAMSIZ -1] = '\0';
    // define SIOCGIFHWADDR 0x8927
    if(ioctl(skdf, SIOCGIFHWADDR, &ifr))
    {
        perror("perror");
        gw_log(APP_LOG_ERR, "ioctl's SIOCGIFHWADDR(%x) get %s mac faile, error: %s", 
            SIOCGIFHWADDR, port, strerror(errno));
        close_socket(skdf);
        return ERROR;
    }
    close_socket(skdf);
    memcpy(mac, (unsigned char *)ifr.ifr_hwaddr.sa_data, 6);
    return OK;
}






int pal_network_ip_info_get(SYS_IP_ENTRY_T *pstSysIpInfo)
{
    char uci_value[UCI_VALUE_LEN] = {0};
    int ip_addr[4]={0};
    
    if(!STRCMP(pstSysIpInfo->aucInterfaceName,"wan")){
        uci_config_get(uci_value,UCI_NETWORK_WAN_PROTO);
       
        if(!STRCMP(uci_value,"static")){
            pstSysIpInfo->ulIpMode = IP_MODE_STATIC;
            uci_config_get(uci_value,UCI_NETWORK_WAN_IP);
            string_ip_tools(uci_value,pstSysIpInfo->aucIpAddress);
            uci_config_get(uci_value,UCI_NETWORK_WAN_IP_NETMASK);
            string_ip_tools(uci_value,pstSysIpInfo->aucIpNetMask);
            uci_config_get(uci_value,UCI_NETWORK_WAN_DNS);
            string_ip_tools(uci_value,pstSysIpInfo->aucDnsMaster);
            uci_config_get(uci_value,UCI_NETWORK_WAN_GATEWAY);
            string_ip_tools(uci_value,pstSysIpInfo->aucGwAddress);
            
        }
        else if (!STRCMP(uci_value,"dhcp")){
            pstSysIpInfo->ulIpMode = IP_MODE_DHCP;
        }
        else if (!STRCMP(uci_value,"pppoe")){
            pstSysIpInfo->ulIpMode = IP_MODE_PPPOE;
        }
        else {
            /*not handle ex: l2tp */
        }
    }
    return OK;
}


int pal_net_bridge_add_if(unsigned char *brname,unsigned char *ifname)
{
	char cmd[128]={0};
	sprintf(cmd,"brctl addif br-%s %s",brname,ifname);
	gw_log(LOG_INFO,"cmd %s",cmd);
	system(cmd);
	return 0;
}


int pal_net_bridge_del_if(unsigned char *brname,unsigned char *ifname)
{
	char cmd[128]={0};
	sprintf(cmd,"brctl delif br-%s %s",brname,ifname);
	gw_log(LOG_INFO,"cmd %s",cmd);
	system(cmd);
	return 0;
}





int pal_dhcp_server_info_get(DHCP_SERVER_T *pstDhcpServer)
{
    int i;
    char uci_name[UCI_NAME_LEN];
    char uci_value[UCI_VALUE_LEN];

    memset(pstDhcpServer,0,sizeof(DHCP_SERVER_T));
    
    for(i=0;i<16;i++)
    {
        sprintf(uci_name,"dhcp.@dhcp[%d]",i);
        if(!uci_config_get(uci_value,uci_name)){
            pstDhcpServer->dhcp_server_num++;
            memset(uci_name,0,UCI_NAME_LEN);
            sprintf(uci_name,"dhcp.@dhcp[%d].ignore",i);
            if(uci_config_get(uci_value,uci_name))
            {
                pstDhcpServer->dhcp_server_info[i].dhcp_enable=1;
                pstDhcpServer->dhcp_server_info[i].dhcp_index=i;

                memset(uci_name,0,UCI_NAME_LEN);
                sprintf(uci_name,"dhcp.@dhcp[%d].interface",i);
                if(!uci_config_get(uci_value,uci_name)){
                    strncpy(pstDhcpServer->dhcp_server_info[i].dhcp_interface,uci_value,strlen(uci_value));
                }

                memset(uci_name,0,UCI_NAME_LEN);
                sprintf(uci_name,"dhcp.@dhcp[%d].start",i);
                if(!uci_config_get(uci_value,uci_name)){
                    strncpy(pstDhcpServer->dhcp_server_info[i].dhcp_start_ip,uci_value,strlen(uci_value));
                }

                memset(uci_name,0,UCI_NAME_LEN);
                sprintf(uci_name,"dhcp.@dhcp[%d].limit",i);
                if(!uci_config_get(uci_value,uci_name)){
                    strncpy(pstDhcpServer->dhcp_server_info[i].dhcp_stop_ip,uci_value,strlen(uci_value));
                }

                memset(uci_name,0,UCI_NAME_LEN);
                sprintf(uci_name,"dhcp.@dhcp[%d].leasetime",i);
                if(!uci_config_get(uci_value,uci_name))
                    strncpy(pstDhcpServer->dhcp_server_info[i].dhcp_lease_time,uci_value,strlen(uci_value));
                }

        }
    }
}



int pal_dhcp_get_sta_info(DHCP_CLIENT_INFO *pstClient)
{
	FILE *fp;
	char buf[128]={0};
	char mac[32];
	char **str=NULL;
	int str_num=0;
	int len;
	/*format:*/
      /*     0                  1                        2                   3                               4*/
	/*1437072047 68:3e:34:09:7e:83 192.168.1.173 android-33cf54dcf43cf945 *
	   1437053735 64:09:80:f3:e4:84 192.168.1.180 MI3C-xiaomishouji */

	sprintf(mac,MAC_FMT,MAC_ARG(pstClient->dhcp_client_mac));
	gw_log(LOG_DEBUG,"mac: %s",mac);
	
	fp=fopen(DHCP_LEASE_FILE,"r");
	if(!fp){
		gw_log(LOG_ERR,"open lease file fail!"DHCP_LEASE_FILE);
		return -1;
	}
	
	while(!feof(fp))
	{
		fgets(buf,128,fp);
		gw_log(LOG_DEBUG,"buf: %s!len %d",buf,len);
		if(strstr(buf,mac))
		{
			/*mac match*/
			str=split_str(buf,' ');
			if(str)
			{		
				for(str_num;str[str_num];str_num++);
				
				gw_log(LOG_DEBUG,"str_num: %d!  str[2]:%s str[3]:%s ",str_num, str[2], str[3]);
				/*ip*/
				string_ip_tools(str[2],pstClient->dhcp_client_ip);
				/*devinfo*/
				strncpy(pstClient->dhcp_client_devinfo,str[3],32);
			}
			if(str)
				free(str);
		}
	}
	fclose(fp);
	return 0;
}

