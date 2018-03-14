#include "pal_common.h"
#include "pal_wifi.h"
#include <linux/wireless.h>
#include "config_common.h"

//#include "pal_wifi_util.h"



static inline int
iw_set_ext(const char *ifname,		/* Device name */
	            int	request,	        /* WE ID */
	            struct iwreq *pwrq)		/* Fixed part of the request */
{
	int skfd;/* socket */
	int iRet;

	/* 入参检测 */
	if(NULL == ifname || NULL == pwrq)
	{
	    gw_log(LOG_INFO,"[%s][%d]Null Point\n",__func__,__LINE__);
	    return WIFI_API_E_NULL_POINTER;
	}

	/* 创建socket */
	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (skfd < 0)
	{
	    gw_log(LOG_INFO,"[%s][%d]create socket fail\n",__func__,__LINE__);
	    return WIFI_API_E_SOCKET;
	}


	/* Set device name */
	strncpy(pwrq->ifr_name, ifname, IFNAMSIZ);
	pwrq->ifr_name[IFNAMSIZ - 1] = '\0';


	/* Do the request */
	iRet = ioctl(skfd, request, pwrq);

	/* 关闭socket */
	if(close(skfd) != 0)
	{
	    gw_log(LOG_INFO,"[%s][%d] close() failed! errno[%d] \n",__func__,__LINE__,errno);
	}

	return iRet;
}

int pal_wifi_get_bssid_by_name(char *name,unsigned char *bssid)
{
	char acCmd[256] = {0};
	FILE *pfData = NULL;
	char acTempBssid[64] = {0};
	
	snprintf(acCmd, 256, "ifconfig %s |grep HWaddr |awk '{print $5}'", name);

	gw_log(LOG_INFO,"acCmd %s   ",acCmd);
	
	pfData = popen(acCmd, "r");
	if(NULL == pfData){
		gw_log(LOG_INFO,"popen failed. ");
	}
	else{
		fread(acTempBssid, sizeof(acTempBssid)-1, 1, pfData);		
		pclose(pfData);
	}
      strmac_to_mac(acTempBssid,bssid);
      gw_log(LOG_INFO,"bssid:"MAC_FMT"",MAC_ARG(bssid));
	return 1;
}


int pal_wifi_get_vap_name(unsigned int uiIndex,char *acVapName)
{
	int i;
	char nvram_name[128];
	char nvram_value[128];
	
	int vap_index_24=0,vap_index_5=0;

	for(i=0;i<=uiIndex;i++){
		/*wifi_id*/
		memset(nvram_value,0,sizeof(nvram_value));
		sprintf(nvram_name,"wireless.@wifi-iface[%d].device",i);
		if(!uci_config_get(nvram_value,nvram_name)){
			if(!memcmp(nvram_value,"wifi0",strlen("wifi0"))){
				
				if(vap_index_24 == 0)
					sprintf(acVapName,"ath0");
				else
					sprintf(acVapName,"ath0%d",vap_index_24);

				gw_log(LOG_INFO,"i %d uiIndex %d vap_index_24 %d  acVapName %s ",i,uiIndex,vap_index_24,acVapName);

				if(i == uiIndex)
					return 0;
				vap_index_24++;
			}
			else if(!memcmp(nvram_value,"wifi1",strlen("wifi1"))){
				if(vap_index_5 == 0)
					sprintf(acVapName,"ath1");
				else
					sprintf(acVapName,"ath1%d",vap_index_5);
				//gw_log(LOG_INFO,"i %d uiIndex %d vap_index_24 %d  acVapName %s ",i,uiIndex,vap_index_24,acVapName);

				if(i == uiIndex)
					return 0;
				vap_index_5++;
			}
		}
	}

}


int pal_wifi_get_vap_network(unsigned int uiIndex,char *acNetwork)
{
	char nvram_name[128];
	char nvram_value[128];
	
	sprintf(nvram_name,"wireless.@wifi-iface[%d].network",uiIndex);
	if(!uci_config_get(nvram_value,nvram_name)){
		strncpy(acNetwork,nvram_value,strlen(nvram_value));
	}
	
}

int pal_wifi_get_vap_basic_info(AP_VAP_BASIC_T *pstVapBasic,unsigned int uiIndex)
{
	char uci_name[128];
	char uci_value[128];
   
      memset(pstVapBasic,0,sizeof(AP_VAP_BASIC_T));
	memset(uci_value,0,sizeof(uci_value));
	sprintf(uci_name,"wireless.@wifi-iface[%d]",uiIndex);
	if(!uci_config_get(uci_value,uci_name)){
		/*wifi_id*/
		memset(uci_value,0,sizeof(uci_value));
		sprintf(uci_name,"wireless.@wifi-iface[%d].device",uiIndex);
		if(!uci_config_get(uci_value,uci_name)){
			if(!memcmp(uci_value,"wifi0",strlen("wifi0"))){
				pstVapBasic->uiWifiId = 0;
			}
			else if(!memcmp(uci_value,"wifi1",strlen("wifi1"))){
				pstVapBasic->uiWifiId = 1;
			}
		}
		/*ssid name */

		memset(uci_value,0,sizeof(uci_value));
		sprintf(uci_name,"wireless.@wifi-iface[%d].ssid",uiIndex);
		if(!uci_config_get(uci_value,uci_name)){
			memcpy(pstVapBasic->acSsid,uci_value,strlen(uci_value));
		}
		
		/*ssid enable*/
		sprintf(uci_name,"wireless.@wifi-iface[%d].disabled",uiIndex);
		memset(uci_value,0,sizeof(uci_value));
		if(!uci_config_get(uci_value,uci_name)){
			pstVapBasic->uiSsidEnable = atoi(uci_value);
		}
		/*ssid hide*/
		sprintf(uci_name,"wireless.@wifi-iface[%d].hidden",uiIndex);
		memset(uci_value,0,sizeof(uci_value));
		if(!uci_config_get(uci_value,uci_name)){
			pstVapBasic->uiSsidHide = atoi(uci_value);
		}

            pal_wifi_get_vap_name(uiIndex,pstVapBasic->acVapName);
            pal_wifi_get_bssid_by_name(pstVapBasic->acVapName,pstVapBasic->aucBssid);


	}

	return 0;
}

int pal_wifi_get_vap_encrypt_info(AP_VAP_ENCRYPT_T *pstEncrypt,unsigned int uiIndex)
{
	char nvram_name[128];
	char nvram_value[128];

	int wep_key_format;

	/*encrypt*/

	sprintf(nvram_name,"wireless.@wifi-iface[%d].encryption",uiIndex);
	memset(nvram_value,0,sizeof(nvram_value));
	if(!uci_config_get(nvram_value,nvram_name)){
		if(!memcmp(nvram_value,"none",strlen("none")))
		{
	             pstEncrypt->ulSecurityMode = OPEN;
			pstEncrypt->ulAuthenticationType = DISABLED;
		}
		else if(!memcmp(nvram_value,"wep-open",strlen("wep-open")))	
		{
			pstEncrypt->ulSecurityMode = OPEN;
			pstEncrypt->ulAuthenticationType=WEP;

			sprintf(nvram_name,"wireless.@wifi-iface[%d].key",uiIndex);
			memset(nvram_value,0,sizeof(nvram_value));
			if(!uci_config_get(nvram_value,nvram_name)){

				wep_key_format = atoi(nvram_value);
				sprintf(nvram_name,"wireless.@wifi-iface[%d].key%d",wep_key_format);
				memset(nvram_value,0,sizeof(nvram_value));
				if(!uci_config_get(nvram_value,nvram_name)){
					memcpy(pstEncrypt->acWepKey,nvram_value, strlen(nvram_value));
				}
			}

			
		}
		else if(!memcmp(nvram_value,"wep-shared",strlen("wep-shared")))
		{
			pstEncrypt->ulSecurityMode = SHARED;
			pstEncrypt->ulAuthenticationType=WEP;
			sprintf(nvram_name,"wireless.@wifi-iface[%d].key",uiIndex);
			memset(nvram_value,0,sizeof(nvram_value));
			if(!uci_config_get(nvram_value,nvram_name)){

				wep_key_format = atoi(nvram_value);
				sprintf(nvram_name,"wireless.@wifi-iface[%d].key%d",wep_key_format);
				memset(nvram_value,0,sizeof(nvram_value));
				if(!uci_config_get(nvram_value,nvram_name)){
					memcpy(pstEncrypt->acWepKey,nvram_value, strlen(nvram_value));
				}
			}
		}
		else if(!memcmp(nvram_value,"psk",strlen("psk")))
		{
			pstEncrypt->ulSecurityMode = WPA_PSK;
			pstEncrypt->ulAuthenticationType=DISABLED;
			/*AES or TKIP*/
			if(!memcmp(nvram_value,"psk+ccmp",strlen("psk+ccmp")))
				pstEncrypt->ulEncryptType = 1;
			else if (!memcmp(nvram_value,"psk+tkip",strlen("psk+tkip")))
				pstEncrypt->ulEncryptType = 0;
			sprintf(nvram_name,"wireless.@wifi-iface[%d].key",uiIndex);
			memset(nvram_value,0,sizeof(nvram_value));
			if(!uci_config_get(nvram_value,nvram_name))
			{
				memcpy(pstEncrypt->acWpaKey,nvram_value, strlen(nvram_value));
			}
			
		}
		else if(!memcmp(nvram_value,"psk2",strlen("psk2")))
		{
			pstEncrypt->ulSecurityMode = WPA2_PSK;
			pstEncrypt->ulAuthenticationType=DISABLED;
			/*AES*/
			if(!memcmp(nvram_value,"psk2+ccmp",strlen("psk2+ccmp")))
				pstEncrypt->ulEncryptType = 1;
			else if (!memcmp(nvram_value,"psk2+tkip",strlen("psk2+tkip")))
				pstEncrypt->ulEncryptType = 0;
			sprintf(nvram_name,"wireless.@wifi-iface[%d].key",uiIndex);
			memset(nvram_value,0,sizeof(nvram_value));
			if(!uci_config_get(nvram_value,nvram_name))
			{
				memcpy(pstEncrypt->acWpaKey,nvram_value, strlen(nvram_value));
			}
		}
		else if(!memcmp(nvram_value,"psk-mixed",strlen("psk-mixed")))
		{
			pstEncrypt->ulSecurityMode = WPA_PSK_WPA2_PSK;
			pstEncrypt->ulAuthenticationType=DISABLED;
			/*AES*/
			if(!memcmp(nvram_value,"psk-mixed+ccmp",strlen("psk-mixed+ccmp")))
				pstEncrypt->ulEncryptType = 1;
			else if (!memcmp(nvram_value,"psk-mixed+tkip",strlen("psk-mixed+tkip")))
				pstEncrypt->ulEncryptType = 0;
			sprintf(nvram_name,"wireless.@wifi-iface[%d].key",uiIndex);
			memset(nvram_value,0,sizeof(nvram_value));
			if(!uci_config_get(nvram_value,nvram_name))
			{
				memcpy(pstEncrypt->acWpaKey,nvram_value, strlen(nvram_value));
			}
		}
	}
	return 0;
}

int pal_wifi_get_vap_info(AP_WIFI_T *pstWifi)
{
	int i;
	char nvram_name[128];
	char nvram_value[128];
	char ssid_name[32];
	int  radio_flag_24=0,radio_flag_5=0;
	AP_RADIO_T *pstRadio;
	AP_VAP_ENCRYPT_T *pstEncrypt;
	pstRadio = pstWifi->astRadio;

	for(i=0;i<32;i++){
		memset(ssid_name,0,sizeof(ssid_name));
		sprintf(nvram_name,"wireless.@wifi-iface[%d].ssid",i);
		if(!uci_config_get(ssid_name,nvram_name)){
			sprintf(nvram_name,"wireless.@wifi-iface[%d].device",i);
			memset(nvram_value,0,sizeof(nvram_value));
			if(!uci_config_get(nvram_value,nvram_name)){
				if(!memcmp(nvram_value,"wifi0",strlen("wifi0"))){
					pstRadio[0].uiRadioId = 0;
					pstRadio[0].uiRadioType= ((0 == pstRadio[0].uiRadioId) ? 0 : 1);
					if(pstRadio[0].uiVapNum == 0)
						sprintf(pstRadio[0].astVap[pstRadio[0].uiVapNum].acVapName,"ath0");
					else
						sprintf(pstRadio[0].astVap[pstRadio[0].uiVapNum].acVapName,"ath0%d",pstRadio[0].uiVapNum);
					
					pstRadio[0].astVap[pstRadio[0].uiVapNum].uiIndex = i;
						
					/*ssid enable*/
					sprintf(nvram_name,"wireless.@wifi-iface[%d].disabled",i);
					memset(nvram_value,0,sizeof(nvram_value));
					if(!uci_config_get(nvram_value,nvram_name)){
						pstRadio[0].astVap[pstRadio[0].uiVapNum].uiSsidEnable = atoi(nvram_value);
					}
					/*ssid hide*/
					sprintf(nvram_name,"wireless.@wifi-iface[%d].hidden",i);
					memset(nvram_value,0,sizeof(nvram_value));
					if(!uci_config_get(nvram_value,nvram_name)){
						pstRadio[0].astVap[pstRadio[0].uiVapNum].uiSsidHide = atoi(nvram_value);
					}

					pstEncrypt = &pstRadio[0].astVap[pstRadio[0].uiVapNum].stVapEncrypt;
					pal_wifi_get_vap_encrypt_info(pstEncrypt,i);
		
				
				
	                
					memcpy(pstRadio[0].astVap[pstRadio[0].uiVapNum].acSsid,ssid_name,32);
					pal_wifi_get_bssid_by_name(pstRadio[0].astVap[pstRadio[0].uiVapNum].acVapName,pstRadio[0].astVap[pstRadio[0].uiVapNum].aucBssid);
					gw_log(LOG_INFO," pstRadio[0].uiVapNum %d  ssid %s  bssid "MAC_FMT" uiSsidEnable:%u ",\
                                    pstRadio[0].uiVapNum,pstRadio[0].astVap[pstRadio[0].uiVapNum].acSsid,MAC_ARG(pstRadio[0].astVap[pstRadio[0].uiVapNum].aucBssid),\
                                    pstRadio[0].astVap[pstRadio[0].uiVapNum].uiSsidEnable);
					pstRadio[0].uiVapNum++;
					radio_flag_24 =1;

				}
				else if(!memcmp(nvram_value,"wifi1",strlen("wifi1"))){
					pstRadio[1].uiRadioId = 1;
					pstRadio[1].uiRadioType= ((0 == pstRadio[1].uiRadioId) ? 0 : 1);
					if(pstRadio[1].uiVapNum == 0)
						sprintf(pstRadio[1].astVap[pstRadio[1].uiVapNum].acVapName,"ath1");
					else
						sprintf(pstRadio[1].astVap[pstRadio[1].uiVapNum].acVapName,"ath1%d",pstRadio[1].uiVapNum);
					
					memcpy(pstRadio[1].astVap[pstRadio[1].uiVapNum].acSsid,ssid_name,32);

					pstRadio[1].astVap[pstRadio[1].uiVapNum].uiIndex = i;
					


					/*ssid enable*/
					sprintf(nvram_name,"wireless.@wifi-iface[%d].disabled",i);
					memset(nvram_value,0,sizeof(nvram_value));
					if(!uci_config_get(nvram_value,nvram_name)){
						pstRadio[1].astVap[pstRadio[1].uiVapNum].uiSsidEnable = atoi(nvram_value);
					}
					/*ssid hide*/
					sprintf(nvram_name,"wireless.@wifi-iface[%d].hidden",i);
					memset(nvram_value,0,sizeof(nvram_value));
					if(!uci_config_get(nvram_value,nvram_name)){
						pstRadio[1].astVap[pstRadio[1].uiVapNum].uiSsidHide = atoi(nvram_value);
					}
					
					/*encypt*/
					pstEncrypt = &pstRadio[1].astVap[pstRadio[1].uiVapNum].stVapEncrypt;
					pal_wifi_get_vap_encrypt_info(pstEncrypt,i);
					
					pal_wifi_get_bssid_by_name(pstRadio[1].astVap[pstRadio[1].uiVapNum].acVapName,pstRadio[1].astVap[pstRadio[1].uiVapNum].aucBssid);
					
					gw_log(LOG_INFO," pstRadio[1].uiVapNum %d  ssid %s  bssid "MAC_FMT"  uiSsidEnable:%u  ",\
                                    pstRadio[1].uiVapNum,pstRadio[1].astVap[pstRadio[1].uiVapNum].acSsid,MAC_ARG(pstRadio[1].astVap[pstRadio[1].uiVapNum].aucBssid),\
                                    pstRadio[1].astVap[pstRadio[1].uiVapNum].uiSsidEnable);
					pstRadio[1].uiVapNum++;
					radio_flag_5 =1;
				}
				}
		}

		
	}
	if(radio_flag_24 && radio_flag_5 )
		pstWifi->uiRadioNum  = 2;
	else if(radio_flag_24 ^ radio_flag_5)
		pstWifi->uiRadioNum = 1;
	else
		pstWifi->uiRadioNum = 0;
	return OK;
}


int pal_wifi_get_vap_num(void)
{
	int i;
	char nvram_name[128];
	char nvram_value[128];
	int vap_num=0;
	for(i=0;i<VAP_NUMBER;i++){
		memset(nvram_value,0,sizeof(nvram_value));
		sprintf(nvram_name,"wireless.@wifi-iface[%d]",i);
		if(!uci_config_get(nvram_value,nvram_name)){
			vap_num++;
			}
	}
	return vap_num;
}

int pal_wifi_get_vap_index(void)
{
	int i;
	char nvram_name[128];
	char nvram_value[128];
	int vap_num=0;
	for(i=0;i<VAP_NUMBER;i++){
		memset(nvram_value,0,sizeof(nvram_value));
		sprintf(nvram_name,"wireless.@wifi-iface[%d]",i);
		if(!uci_config_get(nvram_value,nvram_name)){
			vap_num++;
			}
	}
	return vap_num;
}



int pal_wifi_add_vap(unsigned char *vap_name,unsigned int wifi_id)
{
	int ret=0;
	char cmd[128]={0};
	sprintf(cmd,"wlanconfig %s create wlandev wifi%d wlanmode ap",vap_name,wifi_id);
	ret = system(cmd);
	if(ret)
		return -1;
	return 0;
}
int pal_wifi_delete_vap(unsigned char *vap_name)
{
	int ret=0;
	char cmd[128]={0};
	sprintf(cmd,"wlanconfig %s destroy",vap_name);
	ret = system(cmd);
	if(ret)
		return -1;
	return 0;
}

int pal_wifi_update_vap(unsigned char *vap_name)
{
	
}



int pal_wifi_set_vap_uci(unsigned int uiIndex,unsigned char *name,unsigned char *value)
{
	char uci_name[128];
	char uci_value[128];

	memset(uci_value,0,sizeof(uci_value));
	sprintf(uci_name,"wireless.@wifi-iface[%d]",uiIndex);
	if(!uci_config_get(uci_value,uci_name)){
		sprintf(uci_name,"wireless.@wifi-iface[%d].%s",uiIndex,name);
		uci_config_set(uci_name,value);
	}
	uci_config_commit();
	return 0;
}



int pal_wifi_add_vap_uci(AP_VAP_BASIC_T *pstVapBasic,unsigned int uiIndex)
{
	char uci_name[128];
	char uci_value[128];
	gw_log(LOG_INFO,"uiIndex %d",uiIndex);
	memset(uci_value,0,sizeof(uci_value));
	sprintf(uci_name,"wireless.@wifi-iface[%d]",uiIndex);
	if(uci_config_get(uci_value,uci_name)){
		
		sprintf(uci_name,"wireless");
		uci_config_add(uci_name,"wifi-iface");

		gw_log(LOG_INFO,"uiIndex %d",uiIndex);

		memset(uci_value,0,sizeof(uci_value));
		sprintf(uci_value,"wifi%d",pstVapBasic->uiWifiId);
		sprintf(uci_name,"wireless.@wifi-iface[%d].device",uiIndex);
		uci_config_set(uci_name,uci_value);

		memset(uci_value,0,sizeof(uci_value));
		sprintf(uci_name,"wireless.@wifi-iface[%d].mode",uiIndex);
		if(!pstVapBasic->uiSsidMode)
			strcpy(uci_value,"ap");
		else
			strcpy(uci_value,"sta");
		uci_config_set(uci_name,uci_value);

		memset(uci_value,0,sizeof(uci_value));
		strncpy(uci_value,pstVapBasic->network,strlen(pstVapBasic->network));
		sprintf(uci_name,"wireless.@wifi-iface[%d].network",uiIndex);
		uci_config_set(uci_name,uci_value);

		memset(uci_value,0,sizeof(uci_value));
		sprintf(uci_value,"lan");
		sprintf(uci_name,"wireless.@wifi-iface[%d].encryption",uiIndex);
		uci_config_set(uci_name,uci_value);

		memset(uci_value,0,sizeof(uci_value));
		strncpy(uci_value,pstVapBasic->acSsid,strlen(pstVapBasic->acSsid));
		sprintf(uci_name,"wireless.@wifi-iface[%d].ssid",uiIndex);
		uci_config_set(uci_name,uci_value);

		uci_config_commit();
		
	}
	
	return 0;
}



int pal_wifi_delete_vap_uci(unsigned int uiIndex)
{
	char uci_name[128];
	memset(uci_name,0,sizeof(uci_name));
	sprintf(uci_name,"wireless.@wifi-iface[%d]",uiIndex);
	uci_config_delete(uci_name);
	uci_config_commit();
	return 0;
}



int pal_wifi_set_ssid(unsigned char *vap_name,unsigned char *ssid_name)
{
	int ret=0;
	char cmd[128]={0};
	sprintf(cmd,"iwconfig %s essid %s",vap_name,ssid_name);
	ret = system(cmd);
	if(ret)
		return -1;
	return 0;
}




pal_wifi_get_wifi_info(WIFI_INFO_T *pstWifiInfo,unsigned int wifi_id)
{
	char uci_name[128];
	char uci_value[128];
	
	
}

int pal_wifi_vap_sta_analysis(char *buf,WIFI_API_STA_STATS_T *pstStaStats)
{
	int i=0,line=0,row=0;
	char str[20][32]={0};
	int len=0;

	len=strlen(buf);
	/*format:*/
	/*0                      1     2       3         4         5      6      7         8         9              10          11       12         13                   14            15           16      17      18*/
	/*ADDR               AID CHAN TXRATE RXRATE RSSI IDLE  TXSEQ  RXSEQ  CAPS        ACAPS     ERP    STATE MAXRATE(DOT11) HTCAPS ASSOCTIME    IEs   MODE PSMODE
	24:77:03:4b:94:34    1    4   0M        97M      45    15     49       992       ESs            0           f          0        PGM             00:00:25 WME IEEE80211_MODE_11NG_HT20  0 */

	gw_log(LOG_DEBUG,"index:%d buf  %s len:%d",pstStaStats->usStaIndex,buf,len);

	while('\0' !=buf[i] )
	{
		if(' ' != buf[i])
		{
			str[line][row] = buf[i];
			++row;
		}
		else
		{	
			if(i+1<=len){
				if(' ' !=buf[i+1]){
				str[line][row]='\0';

				gw_log(LOG_DEBUG,"str: %s",str[line]);
				++line;
				row=0;	
				}
			}
		}
		
		i++;  
	}

	if(line < 18){
		
		gw_log(LOG_ERR,"analysis fail! i%d",i);
		return -1;
	}
	/*ADDR*/
	strmac_to_mac(str[0],pstStaStats->aucStaMac);

	/*信道*/
	pstStaStats->ucChannel = atoi(str[2]);


	str[3][strlen(str[3])-1] = '\0'; 
	pstStaStats->ulTxLastRate = atoi(str[3]);

	str[4][strlen(str[4])-1] = '\0'; 
	pstStaStats->ulRxLastRate = atoi(str[4]);

	pstStaStats->cStaRssi = atoi(str[5]);

	/*关联时间转换成　10ms*/
	time_convert_1(str[14],pstStaStats->ulStaLiveTime);

	gw_log(LOG_DEBUG,"MAC:"MAC_FMT"  pstStaStats->ucChannel:%d  pstStaStats->ulTxLastRate: %d  pstStaStats->ulRxLastRate:%d pstStaStats->cStaRssi :%d",\
		MAC_ARG(pstStaStats->aucStaMac),pstStaStats->ucChannel,pstStaStats->ulTxLastRate,pstStaStats->ulRxLastRate,pstStaStats->cStaRssi );
	return 1;
}
	

/*获取已关联终端信息*/
int pal_wifi_vap_get_enduseromc(const char *szVapName,WIFI_API_ENDUSER_STATS_T *pstStats)
{
	/*解析wlanconfig athx list for tmp*/
	char cmd[32];
	FILE *fp;
	char pbuf[256]={0};
	int len;
	int staIndex=0;
	sprintf(cmd,"wlanconfig %s list",szVapName);

	gw_log(LOG_DEBUG, "cmd %s ", cmd);

	fp=popen(cmd,"r");
	if(!fp)
		return -1;

	fseek(fp, 0, SEEK_END);
	len = ftell(fp);			/* 获取文件字节数*/
	fseek(fp, 0, SEEK_SET);


	
	while(!feof(fp))
	{
		fgets(pbuf,256,fp);
		gw_log(LOG_DEBUG, "pbuf :%s ", pbuf);
		if(feof(fp))
			goto finish;
		
		if(strncmp(pbuf,"ADDR",strlen("ADDR")))
		{
			pstStats->pstStaStats[staIndex].usStaIndex = staIndex;
			pal_wifi_vap_sta_analysis(pbuf,&pstStats->pstStaStats[staIndex]);
			staIndex++;
			pstStats->usTotalStaNumber++;
			gw_log(LOG_DEBUG, "pstStats->usTotalStaNumber %d ", pstStats->usTotalStaNumber);
		}
	}
	

finish:	
	pclose(fp);
	return 1;
}

int wifi_api_vap_get_channel(const char *szVapName, unsigned int *pulChannel)
{

}

int get_vap_name_for_wifi_id(char *vap_name, int wifi_id)
{

}
int wlan_set_txpower(char *vap_name, int current)
{
}


int wlan_set_rssi_static_threshold_all(int wifi_id, int rssi)
{
}

int wlan_set_date_ht_rates(char *vap_name, char *rate_list)
{
}

int wlan_set_date_basic_rates(char *vap_name, char *rate_list)
{
}

int pal_wifi_set_ssid_enabled(char *vap_name, int enabled)
{

}
int pal_wifi_set_ssid_hide(char *vap_name, int hide)
{

}


int wifi_api_vap_get_ssid(const char *szVapName, char *pcSsid, const int bufsize)
{
	int iRet = 0;
	struct iwreq iwr;

	/*TBD*/ 
	if(!szVapName || !pcSsid)
	return -1;

	memset(&iwr, 0, sizeof(iwr));
	memcpy(iwr.ifr_name, szVapName, strlen(szVapName)>IFNAMSIZ?IFNAMSIZ:strlen(szVapName));
	iwr.u.essid.pointer = pcSsid;
	iwr.u.essid.length = (bufsize > IEEE80211_SSID_LEN) ? IEEE80211_SSID_LEN : bufsize;

	iRet = iw_set_ext(szVapName, SIOCGIWESSID, &iwr);
	return iRet;
}



#if 0
static void pal_wifi_vap_sta_list(const char *ifname)
{
#define LIST_STATION_ALLOC_SIZE 24*1024

	uint8_t *buf;
	struct iwreq iwr;
	uint8_t *cp;
	int s, len;
    u_int32_t txrate, rxrate = 0, maxrate = 0;

	buf = malloc(LIST_STATION_ALLOC_SIZE);
	if(!buf) {
	 gw_log(ERROR, "Unable to allocate memory for station list\n");
	  return;
	} 

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0){
		free(buf);
		err(1, "socket(SOCK_DRAGM)");
	}

	(void) memset(&iwr, 0, sizeof(iwr));
	(void) strncpy(iwr.ifr_name, ifname, sizeof(iwr.ifr_name));
	iwr.u.data.pointer = (void *) buf;
	iwr.u.data.length = LIST_STATION_ALLOC_SIZE;
	if (ioctl(s, IEEE80211_IOCTL_STA_INFO, &iwr) < 0){
		free(buf);
		errx(1, "unable to get station information");
	}
	len = iwr.u.data.length;
	if (len < sizeof(struct ieee80211req_sta_info)){
		free(buf);
		return;
    }
	printf("%-17.17s %4s %4s %4s %4s %4s %4s %6s %6s %5s %12s %7s %8s %14s %6s\n"
		, "ADDR"
		, "AID"
		, "CHAN"
		, "TXRATE"
		, "RXRATE"
		, "RSSI"
		, "IDLE"
		, "TXSEQ"
		, "RXSEQ"
		, "CAPS"
	    , "ACAPS"
		, "ERP"
		, "STATE"
        , "MAXRATE(DOT11)"
	    , "HTCAPS"
	);
	cp = buf;
	do {
		struct ieee80211req_sta_info *si;
		uint8_t *vp;

		si = (struct ieee80211req_sta_info *) cp;
		vp = (u_int8_t *)(si+1);
        if(si->isi_txratekbps == 0)
           txrate = (si->isi_rates[si->isi_txrate] & IEEE80211_RATE_VAL)/2;
        else
            txrate = si->isi_txratekbps / 1000;
        if(si->isi_rxratekbps >= 0) {
            rxrate = si->isi_rxratekbps / 1000;
		}

        maxrate = si->isi_maxrate_per_client;

        if (maxrate & 0x80) maxrate &= 0x7f;

		gw_log(LOG_INFO,"%s %4u %4d %3dM %6dM %4d %4d %6d %7d %5.4s %-5.5s %3x %10x %14d %14.6s"
			, ieee80211_ntoa(si->isi_macaddr)
			, IEEE80211_AID(si->isi_associd)
			, ieee80211_mhz2ieee(si->isi_freq)
			, txrate 
			, rxrate 
			, si->isi_rssi
			, si->isi_inact
			, si->isi_txseqs[0]
			, si->isi_rxseqs[0]
		    , getcaps(si->isi_capinfo)
		    , getathcaps(si->isi_athflags)
			, si->isi_erp
			, si->isi_state
            , maxrate 
		    , gethtcaps(si->isi_htcap)
		);
		printies(vp, si->isi_ie_len, 24);
		printf("\n");
		cp += si->isi_len, len -= si->isi_len;
	} while (len >= sizeof(struct ieee80211req_sta_info));
	
	free(buf);
}


#endif
