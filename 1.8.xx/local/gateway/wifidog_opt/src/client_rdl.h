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
	unsigned char mac[6];          		/* �û�MAC */
	struct timeval time; 	  			/* �û��ϴ����ӵ�ʱ��*/
	struct hlist_node mac_Hlist;       	/* user mac hash chain */
} WD_USER_T;
            
typedef struct
{
	unsigned char mac[6];          		/* �û�MAC */
	int m_times;                        /* ��������ͷ��ʷ��ʴ��� */
	int n_times;                        /* network���ͷ��ʴ��� */
	unsigned int bitmap;               	/* ���ʼ�¼bitmap*/
    int check;                         	/*�û��Ƿ���ͨ��Mozilla����portal*/
	struct timeval time; 	  			/* �û��ϴ����ӵ�ʱ��*/
	struct timeval init_checktime; 	  	/*�ӵ�����apʱnetwork��INIT״̬��У��ʱ��*/    
	unsigned long duration;				/* network�ظ�success����ʱ��*/
	struct hlist_node mac_Hlist;       	/* user mac hash chain */
} WD_USER_IOS_ACCESS_T;
/*��Ҫ�����ж�iOS�û�����wifiʱ3G��wifi*/
#define CHECK_INIT      0 //��ʼ��
#define CHECK_MOZILLA   1 //��ʾmozilla�Ѿ���ʼ����
#define CHECK_PORTAL   2 //��ʾ�Ѿ�����portal

/*��Ҫ�����ж��Ƿ���Ҫ�����û�*/
#define CHECK_USER_INIT      0 //�鲻���û�ʱ�Ƿ���Ҫ�����û�
#define CHECK_USER_GET       1 //�������û���������������

/* ��Ҫ����ios�û�����wifi�Զ���portal�����ܹ�wifi�����ϡ� */
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
	unsigned char mac[6];          		/* �û�MAC */
	enum user_acl_type type;			/* ��������: 0 -������1-������*/
	struct timeval time; 	  			/* ����������ʱ��*/
	unsigned long duration;				/* ������ʱʱ��,0��ʾ����*/
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
	USER_HTTP_RECORD_IN_WHITE,		/*������ʱ������״̬*/
	USER_HTTP_RECORD_OUT_WHITE,		/*��ʱ�������׶ν���*/
	USER_HTTP_RECORD_IN_BLACK,		/*������ʱ������״̬*/
	USER_HTTP_RECORD_OUT_BLACK		/*��ʱ������״̬����*/
};



typedef struct
{
	unsigned char mac[6];          		/* �û�MAC */
	unsigned int cnt;					/* http���ʼ���*/
	int hit;							/* �Ƿ��з���1-��0-��*/
	enum user_http_record_stage stage;	/* ״̬ : 0-��ʼ1-��ʱ�������׶�2-��ʱ�������׶�*/
	struct hlist_node mac_Hlist;       	/* user mac hash chain */
} WD_USER_HTTP_RECORD_T;


#define USER_HTTP_RECORD_HASH_SIZE 1024

#define USER_HTTP_RECORD_LOCK(_ID_)	pthread_mutex_lock(&user_http_record_mutex[_ID_]);
#define USER_HTTP_RECORD_UNLOCK(_ID_)	pthread_mutex_unlock(&user_http_record_mutex[_ID_]);

extern struct hlist_head g_user_http_record_HList[FIRST_PAGE_TASK_NUM][USER_HTTP_RECORD_HASH_SIZE];
extern pthread_mutex_t user_http_record_mutex[FIRST_PAGE_TASK_NUM];

extern int user_http_record_add_by_mac(unsigned char *mac, int id);

#endif//__CLIENT_RDL_H__

