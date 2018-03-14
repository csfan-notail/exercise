#ifndef __LTE_CLIENT_H__
#define __LTE_CLIENT_H__

#ifndef LTE_LIBEVENT_PRIORITY_FD_LEVEL
               
#define LTE_LIBEVENT_PRIORITY_FD_LEVEL   1	
               
#endif /* LTE_LIBEVENT_PRIORITY_FD_LEVEL */
     
#ifndef LTE_LIBEVENT_PRIORITY_TIMER_LEVEL
               
#define LTE_LIBEVENT_PRIORITY_TIMER_LEVEL   2
               
#endif /* LTE_LIBEVENT_PRIORITY_TIMER_LEVEL */
     
#ifndef LTE_OPER_INFO_TIMER_INTERVAL
          
#define LTE_OPER_INFO_TIMER_INTERVAL   5	/* 单位：秒 */
          
#endif /* LTE_OPER_INFO_TIMER_INTERVAL */    
#ifndef LTE_SIG_TIMER_INTERVAL
          
#define LTE_SIG_TIMER_INTERVAL   5	/* 单位：秒 */
          
#endif /* LTE_SIG_TIMER_INTERVAL */
#ifndef LTE_LINK_TIMER_INTERVAL
          
#define LTE_LINK_TIMER_INTERVAL   5	/* 单位：秒 */
          
#endif /* LTE_LINK_TIMER_INTERVAL */    
#ifndef LTE_CELLID_TIMER_INTERVAL
          
#define LTE_CELLID_TIMER_INTERVAL   30	/* 单位：秒 */
          
#endif /* LTE_CELLID_TIMER_INTERVAL */ 

#ifndef LTE_SIMSTATE_TIMER_INTERVAL
          
#define LTE_SIMSTATE_TIMER_INTERVAL   5	/* 单位：秒 */
          
#endif /* LTE_SIMSTATE_TIMER_INTERVAL */          
#ifndef LTE_ACCESSNET_TIMER_INTERVAL
          
#define LTE_ACCESSNET_TIMER_INTERVAL   10	/* 单位：秒 */
          
#endif /* LTE_ACCESSNET_TIMER_INTERVAL */            
#ifndef COLLECT_TIMER_INTERVAL
          
#define COLLECT_TIMER_INTERVAL 10  	/* 单位：秒 */
          
#endif /* COLLECT_TIMER_INTERVAL */
          
          
          
#ifndef ALARM_TIMER_INTERVAL
#define ALARM_TIMER_INTERVAL          5  /* 单位：秒 */
#endif /* ALARM_TIMER_INTERVAL */
          
#ifndef EVENT_TIMER_INTERVAL
#define EVENT_TIMER_INTERVAL          5  /* 单位：秒 */
#endif /* ALARM_TIMER_INTERVAL */
          
#ifndef STATUS_TIMER_INTERVAL
#define STATUS_TIMER_INTERVAL          20 /* 单位：秒 */
#endif /* STATUS_TIMER_INTERVAL */
          
#ifndef GPS_TIMER_INTERVAL
#define GPS_TIMER_INTERVAL          5  /* 单位：秒 */
#endif /* GPS_TIMER_INTERVAL */


typedef int EVENT_BASE_ID;
typedef int EVENT_ID;

typedef struct __lte_timer_t
{
    struct timeval  stTimeVal;
    
    EVENT_ID eiTimer;
    
}LTE_EVENT_TIMER_T;


typedef struct __lte_event_t
{
    LTE_EVENT_TIMER_T   stSigTimer;
    LTE_EVENT_TIMER_T   stSimStateTimer;
    LTE_EVENT_TIMER_T   stCellidTimer;
    LTE_EVENT_TIMER_T   stLteInfoTimer;
    EVENT_BASE_ID       eEventBaseID;
    
}LTE_LIB_EVENT_T;


#endif /* __LTE_CLIENT_H__ */

