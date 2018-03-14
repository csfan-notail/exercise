#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdarg.h>
#include <pthread.h>

#include "includes.h"
#include "httpd.h"
#include "conf.h"
#include "commandline.h"
#include "client_list.h"
#include "auth_server.h"
#include "firewall.h"
#include "util.h"
#include "log.h"
#include "config.h"
#include "common.h"
#include "http.h"


#if 0
void packet_print(const u_char*ptr, unsigned int ulLen)
{
	unsigned int 	ulLoop;
	char aucBuf[128];
	unsigned int 	ulDataLen = 128;
	unsigned int 	ulLine;
	const u_char *p;
	
	if (ulLen < ulDataLen )
		ulDataLen = ulLen;
	
	ulLine = ulDataLen/16;
    
	p = ptr;
    	
    	for(ulLoop=0; ulLoop < ulLine; ulLoop++)
    	{
        	sys_log_set(WIFIDOG_PCAP_LOG, "%02x %02x %02x %02x %02x %02x %02x %02x-%02x %02x %02x %02x %02x %02x %02x %02x\n", p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],p[8],p[9],p[10],p[11],p[12],p[13],p[14],p[15]);
        	p = p+16;
    	} 
    
    	aucBuf[0] = 0;
    	for (ulLoop=0; ulLoop < ulLen%16; ulLoop++)
    	{
        	sprintf(aucBuf + strlen(aucBuf), "%02x ", p[ulLoop]);    
    	}
    	sys_log_set(WIFIDOG_PCAP_LOG, aucBuf);
    
    	sys_log_set(WIFIDOG_PCAP_LOG,"\n");
}
#else
void packet_print(const u_char*ptr, unsigned int ulLen)
{
    return;
}
#endif

#define __init __attribute__ ((constructor(101)))
void __init init()
{
    sys_log_set(WIFIDOG_PCAP_LOG, "start...\n");
}

int qq_pcap(char *mac,char *ip,char *qq_id)
{
    t_client *ptr=NULL;
    char *qq_id_list=NULL;

    sys_log_set(WIFIDOG_PCAP_LOG, "mac:%s  ip:%s  qq_id:%s\n",mac,ip,qq_id);
    
    ptr = client_list_find_by_ip(ip);
    if(!ptr){
        sys_log_set(WIFIDOG_PCAP_LOG, "not such client...\n");
        return -1;
    }

    sys_log_set(WIFIDOG_PCAP_LOG, "match client !\n");
    if(!ptr->qq_id_list)
    {
        /*no qq exist*/
        ptr->qq_id_list = safe_strdup(qq_id);
        sys_log_set(WIFIDOG_PCAP_LOG, "first qq num %s !n",qq_id);
    }
    else
    {
        /*search qq_id */
        if(!strstr(ptr->qq_id_list,qq_id)){
            /*insert new qq_id*/
            if(strlen(ptr->qq_id_list) >896)
                memset(ptr->qq_id_list,0,strlen(ptr->qq_id_list));
                
            sys_log_set(WIFIDOG_PCAP_LOG, "insert new  qq num :%s !\n",qq_id);
            qq_id_list = malloc(strlen(ptr->qq_id_list)+strlen(qq_id)+1);
            if(!qq_id_list)
                return -1;  
            memset(qq_id_list,0,strlen(ptr->qq_id_list)+strlen(qq_id)+1);
            memcpy(qq_id_list,ptr->qq_id_list,strlen(ptr->qq_id_list));
            if(strlen(ptr->qq_id_list))
                qq_id_list[strlen(ptr->qq_id_list)]=',';
            qq_id_list[strlen(qq_id_list)]='\0';
            strcat(qq_id_list,qq_id);
            sys_log_set(WIFIDOG_PCAP_LOG, "qq_id_list:%s\n",qq_id_list);
            free(ptr->qq_id_list);
            ptr->qq_id_list = qq_id_list;
        }
    }
    return 0;
}
void callback(u_char *userless, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{
    const u_char* tmp = packet;
    int len = pkthdr->caplen;
    char mac[32]={0};
    char ip[16]={0};
    char qq_id[32]={0};
    unsigned long qq_num=0;
    if(len >1514)
        return;
  
    if((len >= 55) && (tmp[23] == 0x11) && (tmp[42] == 0x02) && (tmp[len-1] == 0x03) && (tmp[45] == 0)&&(tmp[46] != 0) &&(tmp[53] == 0 ||tmp[53] == 2 ) &&(tmp[54] == 0) &&(tmp[55] == 0)) /*qq flag must be 0x02*/
    {
        sys_log_set(WIFIDOG_PCAP_LOG,"udp mac:%x:%x:%x:%x:%x:%x\n",tmp[6],tmp[7],tmp[8],tmp[9],tmp[10],tmp[11]);/*user mac*/
        sys_log_set(WIFIDOG_PCAP_LOG,"udp qq:%x%x%x%x\n",tmp[42+7], tmp[42+8], tmp[42+9], tmp[42+10]);  /*qq number*/
        qq_num = ntohl(*(unsigned long *)(&tmp[42+7]));
        sprintf(qq_id,"%lu",qq_num);
    }
    else if((len >= 78)&&(tmp[23] == 0x06) && (tmp[68] == 0x02) && (tmp[len-1] == 0x03) && (tmp[71] == 0x00) && (tmp[72] != 0x00)) /*71 :commad[0]:must be 0*/
    {
        sys_log_set(WIFIDOG_PCAP_LOG,"tcp mac:%x:%x:%x:%x:%x:%x\n",tmp[6],tmp[7],tmp[8],tmp[9],tmp[10],tmp[11]);/* user mac*/
        sys_log_set(WIFIDOG_PCAP_LOG,"tcp qq:%x%x%x%x\n",tmp[68+7], tmp[68+8], tmp[68+9], tmp[68+10]);  /*qq number*/
        qq_num = ntohl(*(unsigned long *)(&tmp[68+7]));
	  sprintf(qq_id,"%lu",qq_num);
    }
    else if((len >= 72)&&(tmp[54]==0)&&(tmp[55]==0)&&(tmp[58]==0)&&(tmp[59]==0)&&(tmp[60]==0)&&(tmp[61]==0x09 || tmp[61]==0x0b)&&(tmp[62]==0x01)&&(tmp[63]==0))
    {
        int qq_len = tmp[71] - 4;
        sys_log_set(WIFIDOG_PCAP_LOG,"android qq:%.*s\n", qq_len, tmp+72);
        strncpy(qq_id,(char *)(tmp+72),qq_len);
    }
    else if((len >= 136)&&(tmp[54]==0)&&(tmp[55]==0)&&(tmp[58]==0)&&(tmp[59]==0)&&(tmp[60]==0)&&(tmp[61]==0x08)&&(tmp[62]==0x01)&&(tmp[63]==0))
    {
        int qq_len = tmp[135] - 4;
        sys_log_set(WIFIDOG_PCAP_LOG,"ios qq:%.*s\n", qq_len, tmp+136);
        strncpy(qq_id,(char *)(tmp+136),qq_len);
    }
        
    else
    {
        return ;
    }
    
    sprintf(mac,MAC_FMT,tmp[6],tmp[7],tmp[8],tmp[9],tmp[10],tmp[11]);    
    sprintf(ip,IP_FMT,tmp[26],tmp[27],tmp[28],tmp[29]);

    char dip[16]={0};
    sprintf(dip,IP_FMT,tmp[30],tmp[31],tmp[32],tmp[33]);
    	
    sys_log_set(WIFIDOG_PCAP_LOG, "pkt len %d mac :%s  ip :%s  dip: %s  qq: %s \n",len,mac,ip,dip,qq_id);

    packet_print(tmp,80);
    
    qq_pcap(mac,ip,qq_id);
    return ;
}

void thread_pcap(void)
{
    pcap_t *handle;         /* Session handle */
    char *dev;          /* The device to sniff on */
    char errbuf[PCAP_ERRBUF_SIZE];  /* Error string */
    struct bpf_program fp;      /* The compiled filter */
    char filter_exp[] = "udp dst port 8000 or tcp dst port 443 or tcp dst port 80 or tcp dst port 8080";  /* The filter expression */
    bpf_u_int32 mask;       /* Our netmask */
    bpf_u_int32 net;        /* Our IP */

 
    /* Define the device */
    dev = pcap_lookupdev(errbuf);
    if (dev == NULL) {
        sys_log_set(WIFIDOG_PCAP_LOG, "Couldn't find default device: %s\n", errbuf);
        return;
    }
    /* Find the properties for the device */
    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
        sys_log_set(WIFIDOG_PCAP_LOG, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
        net = 0;
        mask = 0;
    }
    /* Open the session in promiscuous mode */
    handle = pcap_open_live("br0", BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        sys_log_set(WIFIDOG_PCAP_LOG,  "Couldn't open device %s: %s\n", dev, errbuf);
        return;
    }
    /* Compile and apply the filter */
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
        sys_log_set(WIFIDOG_PCAP_LOG, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return;
    }
    if (pcap_setfilter(handle, &fp) == -1) {
        sys_log_set(WIFIDOG_PCAP_LOG,  "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return;
    }
    sys_log_set(WIFIDOG_PCAP_LOG,  "Start success!\n");
    /* Grab a packet */
    //packet = pcap_next(handle, &header);
    pcap_loop(handle, 0, callback, NULL);
    /* Print its length */
    //printf("Jacked a packet with length of [%d]\n", header.len);
    //printf("%s",packet);
    /* And close the session */
    pcap_close(handle);
    return;
}
