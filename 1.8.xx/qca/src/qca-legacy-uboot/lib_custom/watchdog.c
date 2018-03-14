
#include "custom_internal.h"


#define USEC_PER_SEC (1000000L)

#define CUSTOM_DEFAULT_WD_TMO	(60ul * USEC_PER_SEC)

#define CUSTOM_WD_ACT_RESET     3u /* Full Chip Reset */ 
#define CUSTOM_WD_ACT_MASK      3u
#define CUSTOM_WD_ACT_NONE      0u /* No Action */


static inline void
custom_set_wd_timer(uint32_t usec /* micro seconds */)
{
    uint32_t vTmpusec = 0;
    uint32_t vCnt = 0;
    
	if (RST_BOOTSTRAP_REF_CLK_GET(ath_reg_rd(RST_BOOTSTRAP_ADDRESS))) {
		//40Mhz
		usec = usec * (40000000 / USEC_PER_SEC);
	} else {
		//25Mhz
		usec = usec * (25000000 / USEC_PER_SEC);
	}
    
    //EV[131847], wait WDT timer write in( wait limit time )        
    vTmpusec = ath_reg_rd(RST_WATCHDOG_TIMER);
    ath_reg_wr(RST_WATCHDOG_TIMER, usec);        
    while(ath_reg_rd(RST_WATCHDOG_TIMER) <= vTmpusec)        
    {            
        vCnt++;            
        if(vCnt >= 100)            
        {                 
            printf("%s: Write WDT Timer fail !\n",__func__);                 
            break;            
        }        
    }

    return;
}

static inline int
custom_set_wd_timer_action(uint32_t val)
{
	if (val & ~CUSTOM_WD_ACT_MASK) {
		return -22;
	}
	/*
	 * bits  : 31 30 - 2 0-1
	 * access: RO  rsvd  Action
	 *
	 * Since bit 31 is read only and rest of the bits
	 * are zero, don't have to do a read-modify-write
	 */
	ath_reg_wr(RST_WATCHDOG_TIMER_CONTROL, val);
	return 0;
}

void custom_watchdog_enable(void)
{
	custom_set_wd_timer(CUSTOM_DEFAULT_WD_TMO);
    custom_set_wd_timer_action(CUSTOM_WD_ACT_RESET);
    return;
}

void custom_watchdog_disable(void)
{

	custom_set_wd_timer_action(CUSTOM_WD_ACT_NONE);
    return;
}


