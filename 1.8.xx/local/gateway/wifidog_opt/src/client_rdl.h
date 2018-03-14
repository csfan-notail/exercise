#ifndef __CLIENT_RDL_H__
#define __CLIENT_RDL_H__

extern int g_ext_dbg_level;
#define STA_RDL_LOG(__level__, __format__, __arg__...) \
		do{ \
			if((LOG_DEBUG == __level__) && (g_ext_dbg_level != 0))\
				syslog(LOG_INFO, "wdpriv-rdl: "__format__, ##__arg__);\
			else if(LOG_DEBUG == __level__)\
				break;\
			else\
				syslog(__level__, "wdpriv-rdl: "__format__, ##__arg__);\
		}while(0)

typedef struct
{
	unsigned char mac[6];          		/* 用户MAC */
	struct timeval time; 	  			/* 用户上次连接的时间*/
	struct hlist_node mac_Hlist;       	/* user mac hash chain */
} WD_USER_T;
            
typedef struct
{
	unsigned char mac[6];          		/* 用户MAC */
	int m_times;                        /* 浏览器类型访问访问次数 */
	int n_times;                        /* network类型访问次数 */
	unsigned int bitmap;               	/* 访问记录bitmap*/
    int check;                         	/*用户是否已通过Mozilla访问portal*/
	struct timeval time; 	  			/* 用户上次连接的时间*/
	struct timeval init_checktime; 	  	/*接第三方ap时network切INIT状态的校验时间*/    
	unsigned long duration;				/* network回复success持续时间*/
	struct hlist_node mac_Hlist;       	/* user mac hash chain */
} WD_USER_IOS_ACCESS_T;
/*主要用作判断iOS用户关联wifi时3G变wifi*/
#define CHECK_INIT      0 //初始化
#define CHECK_MOZILLA   1 //表示mozilla已经开始访问
#define CHECK_PORTAL   2 //表示已经弹出portal

/*主要用作判断是否需要新增用户*/
#define CHECK_USER_INIT      0 //查不到用户时是否需要新增用户
#define CHECK_USER_GET       1 //仅查找用户，不做其他操作

/* 主要用作ios用户关联wifi自动弹portal，还能够wifi连接上。 */
#define USER_IOS_ACCESS_HASH_SIZE 1024

#define USER_IOS_ACCESS_LIST_LOCK(id)	pthread_mutex_lock(&user_ios_access_list_mutex[id])
#define USER_IOS_ACCESS_LIST_UNLOCK(id)	pthread_mutex_unlock(&user_ios_access_list_mutex[id])

extern pthread_mutex_t user_ios_access_list_mutex[FIRST_PAGE_TASK_NUM];
extern struct hlist_head g_user_ios_access_mac_HList[FIRST_PAGE_TASK_NUM][USER_IOS_ACCESS_HASH_SIZE];

extern unsigned long g_ios_access_timeout_time;
extern unsigned long g_ios_access_timeout_time_m;
extern unsigned long g_ios_network_access_timeout_time;
extern WD_USER_IOS_ACCESS_T *user_ios_access_get_by_mac(int loop,unsigned char *mac,int flag);
extern int user_timeout_check(struct timeval *pstOldtime,unsigned long ulTimeout);
extern void firstnetwork_init_ioshlist(WD_USER_IOS_ACCESS_T *pstuser);

enum user_acl_type
{
	USER_ACL_WHITE = 0,
	USER_ACL_BLACK
};


typedef struct
{
	unsigned char mac[6];          		/* 用户MAC */
	enum user_acl_type type;			/* 名单类型: 0 -白名单1-黑名单*/
	struct timeval time; 	  			/* 名单创建的时间*/
	unsigned long duration;				/* 名单超时时间,0表示永久*/
	struct hlist_node mac_Hlist;       	/* user mac hash chain */
} WD_USER_ACL_T;


#define USER_ACL_LIST_HASH_SIZE 1024

#define USER_ACL_LIST_LOCK()	pthread_mutex_lock(&user_acl_list_mutex);
#define USER_ACL_LIST_UNLOCK()	pthread_mutex_unlock(&user_acl_list_mutex);

extern struct hlist_head g_user_acl_HList[USER_ACL_LIST_HASH_SIZE];
extern pthread_mutex_t user_acl_list_mutex;


enum user_http_record_stage
{
	USER_HTTP_RECORD_INIT = 0,
	USER_HTTP_RECORD_IN_WHITE,		/*处于临时白名单状态*/
	USER_HTTP_RECORD_OUT_WHITE,		/*临时白名单阶段结束*/
	USER_HTTP_RECORD_IN_BLACK,		/*处于临时黑名单状态*/
	USER_HTTP_RECORD_OUT_BLACK		/*临时黑名单状态结束*/
};



typedef struct
{
	unsigned char mac[6];          		/* 用户MAC */
	unsigned int cnt;					/* http访问计数*/
	int hit;							/* 是否有访问1-是0-否*/
	enum user_http_record_stage stage;	/* 状态 : 0-初始1-临时白名单阶段2-临时黑名单阶段*/
	struct hlist_node mac_Hlist;       	/* user mac hash chain */
} WD_USER_HTTP_RECORD_T;


#define USER_HTTP_RECORD_HASH_SIZE 1024

#define USER_HTTP_RECORD_LOCK(_ID_)	pthread_mutex_lock(&user_http_record_mutex[_ID_]);
#define USER_HTTP_RECORD_UNLOCK(_ID_)	pthread_mutex_unlock(&user_http_record_mutex[_ID_]);

extern struct hlist_head g_user_http_record_HList[FIRST_PAGE_TASK_NUM][USER_HTTP_RECORD_HASH_SIZE];
extern pthread_mutex_t user_http_record_mutex[FIRST_PAGE_TASK_NUM];

extern int user_http_record_add_by_mac(unsigned char *mac, int id);

#endif//__CLIENT_RDL_H__

