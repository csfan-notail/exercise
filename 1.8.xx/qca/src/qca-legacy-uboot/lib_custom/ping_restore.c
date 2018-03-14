
#include "custom_internal.h"
#include <net.h>

#define NV_PING_RESTORE "pingrestore"
#define PING_RESTORE_ECHO_LENGTH 1074
#define PING_RESTORE_ACT_COUNT 3
#define PING_RESTORE_TIMEOUT 2
#define PING_RESTORE_INIT_RETRY 3

DECLARE_GLOBAL_DATA_PTR;
int pingrestore_len = PING_RESTORE_ECHO_LENGTH;
int pingrestore_warting = 0;
int eth_init_retry_cnt = 0;
int netloop_restart = 0;
int echoreq_count = 0;


static void
ping_restore_timeout(void)
{
    struct eth_device *dev = eth_get_dev();
    if ((dev->state != ETH_STATE_ACTIVE) && (eth_init_retry_cnt < 3)) {
        netloop_restart = 1;
        eth_init_retry_cnt++;
        printf("eth not active, ping restore loop retry %d\n", eth_init_retry_cnt);
    }
    else {
        printf("ping restore loop timeout\n");
    }
    
	eth_halt();
    NetState = NETLOOP_FAIL;
    
    return;
}

static void 
ping_restore_loop(IPaddr_t ip) 
{
    echoreq_count = 0;
    eth_init_retry_cnt = 0;
    
    NetCopyIP(&NetOurIP, &ip);
    
    pingrestore_warting = 1;
    /*no need to register a protocal,'cuz nothing else to be done in NetLoop*/ 
    do {
        netloop_restart = 0;
        //del by xiaowei 2016-3-14
    	//NetSetTimeout(PING_RESTORE_TIMEOUT * CFG_HZ, ping_restore_timeout);
        //printf("ping restore enter netloop --->\n");
        //NetLoop(-1);
    } while (netloop_restart);
    pingrestore_warting = 0;
    
    return;
}

void 
ping_restore_recv_echoreq(int len)
{   
    printf("recv ICMP request, len %d\n", len);
    if (!pingrestore_warting) {
        /*we are not warting for ECHO Request*/
        return;
    }
    
    if (pingrestore_len == (len - IP_HDR_SIZE)) {
        echoreq_count++;        

        /*restart timer to wait for next ECHO Req*/
        NetSetTimeout(PING_RESTORE_TIMEOUT * CFG_HZ, ping_restore_timeout);     
        if (echoreq_count >= PING_RESTORE_ACT_COUNT) {
            custom_reset_default();
            puts ("### Factory reset by ping ###\n");
            NetState = NETLOOP_SUCCESS;
            return;
        }
    }
    return;
}

void ping_restore_monitor(void)
{
    bd_t *bd = gd->bd;
    char *nvrpingrestore = getnvram(NV_PING_RESTORE);

    if (nvrpingrestore) {
        pingrestore_len = simple_strtoul(nvrpingrestore, NULL, 10);
    }
    
    if (pingrestore_len) {
        ping_restore_loop(bd->bi_ip_addr);
    }
    
    return;
}


