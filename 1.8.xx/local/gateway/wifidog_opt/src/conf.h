/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : conf.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/11/25
  Last Modified :
  Description   : conf.h header file
  Function List :
  History       :
  1.Date        : 2013/11/25
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#ifndef __CONF_H__
#define __CONF_H__

#include "list.h"

/**
 * Defaults configuration values
 */
#if (defined(GW_X86) || defined(GW_ARM) || defined(GW_AC))    //兼容X86平台的config文件默认地址
#define DEFAULT_CONFIGFILE  "/appfs/etc/wifidog.conf"
#define DEFAULT_HTMLMSGFILE "/appfs/etc/wifidog-msg.html"
#elif defined OPENWRT
#define DEFAULT_CONFIGFILE  "/etc/wifidog/wifidog.conf"
#define CURRENT_CONFIGFILE  "/tmp/wifidog.conf"
#define DEFAULT_HTMLMSGFILE "/etc/wifidog/wifidog-msg.html"
#else
#define DEFAULT_CONFIGFILE  "/etc/wifidog.conf"
#define DEFAULT_HTMLMSGFILE "/etc/wifidog-msg.html"
#endif

#define DEFAULT_MAXUSERFILE     "/portal/maxuser.html"
#define DEFAULT_OFFLINEFILE     "/portal/offline.html"
/* portal 下沉增强版首页路径 add by zhz 2014-6-12 要修改 */
#ifdef GW_X86
#define PORTAL_FIRST_PAGE       "/portal/first_page/"
#else
#define PORTAL_FIRST_PAGE       "/portal/first_page"
#endif
#if (defined(GW_X86) || defined(GW_ARM) || defined(GW_AC))   //by cj
#ifndef LIGHTTPD_ROOT_DOC
#define LIGHTTPD_ROOT_DOC   "/appfs/web/webpages/"     /* lighttpd存放网页的根目录 */
#endif
#else
#ifndef LIGHTTPD_ROOT_DOC
#define LIGHTTPD_ROOT_DOC   "/tmp/app/home/cloud/webpages/"     /* lighttpd存放网页的根目录 */
#endif
#endif

#ifdef GW_X86
#define FIRST_PAGE_TASK_NUM		4
#else
#define FIRST_PAGE_TASK_NUM		1
#endif


#define NOISE_FLOOR (-95)

#define AUTH_OK         "/portal/auth_ok.html"
#define NETWORK_PAGE    "/portal/apple-network.html"
#define STATION_PAGE    "/portal/station.html"


#define DEFAULT_DAEMON 1
#define DEFAULT_DEBUGLEVEL __LOG_MSG__//__LOG_DEBUG__
#define DEFAULT_HTTPDMAXCONN 10
#define DEFAULT_GATEWAYID NULL
#define DEFAULT_GATEWAYPORT 2060
#define DEFAULT_GATEWAYREDIRECTPORT 2061
/*add@2014.8.15*/
#define DEFAULT_GATEWAYFIRSTPAGEHANDLEPORT 8062

#define DEFAULT_HTTPDNAME "WiFiDog"
#define DEFAULT_CLIENTTIMEOUT 5
#define DEFAULT_CHECKINTERVAL 60
#define DEFAULT_LOG_SYSLOG 0
#define DEFAULT_SYSLOG_FACILITY LOG_DAEMON
#define DEFAULT_WDCTL_SOCK "/tmp/wdctl.sock"
#define DEFAULT_INTERNAL_SOCK "/tmp/wifidog.sock"
#define DEFAULT_AUTHSERVPORT 80
#define DEFAULT_AUTHSERVSSLPORT 443
/** Note that DEFAULT_AUTHSERVSSLAVAILABLE must be 0 or 1, even if the config file syntax is yes or no */
#define DEFAULT_AUTHSERVSSLAVAILABLE 0
/** Note:  The path must be prefixed by /, and must be suffixed /.  Put / for the server root.*/
#define DEFAULT_AUTHSERVPATH                "/wifidog/"
#define DEFAULT_AUTHSERVLOGINPATHFRAGMENT   "login/?"
#define DEFAULT_AUTHSERVPORTALPATHFRAGMENT  "portal/?"
#define DEFAULT_AUTHSERVMSGPATHFRAGMENT     "gw_message.php?"
#define DEFAULT_AUTHSERVPINGPATHFRAGMENT    "ping/?"
#define DEFAULT_AUTHSERVAUTHPATHFRAGMENT    "auth/?"
#define DEFAULT_CONFSERVAUTHPATHFRAGMENT    "conf/?"
#define DEFAULT_CMDSERVAUTHPATHFRAGMENT     "cmd/?"
#define DEFAUTL_CLIENTSERVAUTHPATHFRAGMENT  "client/?"
/*add@2014.8.26 for reporting flow of wan*/
#define DEFAUTL_WANFLOWREPORTPATHFRAGMENT   "/cmps/admin.php/guidecustomerhot/gwflowrecord?"

#define DEFAULT_TCP_TIMEOUT 10
#define DEFAULT_CLINET_MAX_ACCESS_NUM 2000
#define DEFAULT_DEV_CLINET_MAX_ACCESS_NUM 0 /*0表示不限制*/
/*@}*/ 

#define SERVER_MODE_DISABLE			0
#define SERVER_MODE_WIFIDOG			1
#define SERVER_MODE_THIRD_PARTY		2

/**
 * Information about the authentication server
 */
typedef struct _auth_serv_t {
    char *authserv_hostname;	/**< @brief Hostname of the central server */
    char *authserv_path;	/**< @brief Path where wifidog resides */
    char *authserv_login_script_path_fragment;	/**< @brief This is the script the user will be sent to for login. */
    char *authserv_portal_script_path_fragment;	/**< @brief This is the script the user will be sent to after a successfull login. */
    char *authserv_msg_script_path_fragment;	/**< @brief This is the script the user will be sent to upon error to read a readable message. */
    char *authserv_ping_script_path_fragment;	/**< @brief This is the ping heartbeating script. */
    char *authserv_auth_script_path_fragment;	/**< @brief This is the script that talks the wifidog gateway protocol. */
    char *authserv_conf_script_path_fragment;	/**< @brief This is the script that talks the wifidog gateway protocol. */
    char *authserv_cmd_script_path_fragment;	/**< @brief This is the script that talks the wifidog gateway protocol. */
    char *authserv_client_script_path_fragment; /**< @brief This is the script that talks the wifidog gateway protocol. */
	char *authserv_wan_flow_report_script_path_fragment; /*add@2014.8.26 for reporting flow of wan*/
	int authserv_http_port;	/**< @brief Http port the central server
				     listens on */
    int authserv_ssl_port;	/**< @brief Https port the central server
				     listens on */
    int authserv_use_ssl;	/**< @brief Use SSL or not */
    char *last_ip;	/**< @brief Last ip used by authserver */
    struct _auth_serv_t *next;
} t_auth_serv;

/**
 * Firewall rules
 */
typedef struct _firewall_rule_t {
    int block_allow;		/**< @brief 1 = Allow rule, 0 = Block rule */
    char *protocol;		/**< @brief tcp, udp, etc ... */
    char *port;			/**< @brief Port to block/allow */
    char *mask;			/**< @brief Mask for the rule *destination* */
    struct _firewall_rule_t *next;
} t_firewall_rule;

/**
 * Firewall rulesets
 */
typedef struct _firewall_ruleset_t {
    char			*name;
    t_firewall_rule		*rules;
    struct _firewall_ruleset_t	*next;
} t_firewall_ruleset;

/**
 * Trusted MAC Addresses
 */
typedef struct _trusted_mac_t {
    char   *mac;
    struct _trusted_mac_t *next;
} t_trusted_mac;


/**
 * Trusted IP Addresses
 */
typedef struct _trusted_ip_t {
    char   *ip;
    struct _trusted_ip_t *next;
} t_trusted_ip;


/**
 * Trusted IP Addresses
 */
typedef struct _trusted_wanhost_t {
		char	*host;
    char   *ip;
    struct _trusted_wanhost_t *next;
} t_trusted_wanhost;


/*add 2015-8-26 for 网间漫游*/
typedef enum
{
    MACRO_SWITCH_DISABLE = 0,
    MACRO_SWITCH_PERCEPT,
    MACRO_SWITCH_UNPERCEPT
}en_macro_hotspot_switch;

#define FREEAUTH_MACLIST_HASH_SIZE	512
#define FREEAUTH_MACLIST_LOCK()		pthread_mutex_lock(&config->freeauth_maclist_mutex)
#define FREEAUTH_MACLIST_UNLOCK()	pthread_mutex_unlock(&config->freeauth_maclist_mutex)
typedef enum
{
    FREEAUTH_MAC_OP_NONE = 0,
    FREEAUTH_MAC_OP_ADD,
    FREEAUTH_MAC_OP_DEL
}freeauth_mac_op;

typedef struct {
	unsigned char mac[6];
	freeauth_mac_op op;
	struct hlist_node mac_Hlist;
}freeauth_mac_node;

typedef struct _lan_info_t{
	char name[6];
	unsigned int uiIp;
	unsigned int uiMask;
	unsigned int uiDns;
	unsigned int uiSubDns;
	struct list_head stLanInfo;
} lan_info_t;

#define LANINFO_LIST_LOCK()	pthread_mutex_lock(&config->laninfo_list_mutex)
#define LANINFO_LIST_UNLOCK()	pthread_mutex_unlock(&config->laninfo_list_mutex)



/**
 * Configuration structure
 */
typedef struct {
    char configfile[255];	/**< @brief name of the config file */
    char *htmlmsgfile;		/**< @brief name of the HTML file used for messages */
    char *wdctl_sock;		/**< @brief wdctl path to socket */
    char *internal_sock;		/**< @brief internal path to socket */
    int daemon;			/**< @brief if daemon > 0, use daemon mode */
    int debuglevel;		/**< @brief Debug information verbosity */
    char *external_interface;	/**< @brief External network interface name for
				     firewall rules */
    char *gw_id;		/**< @brief ID of the Gateway, sent to central
				     server */
    char *gw_interface;		/**< @brief Interface we will accept connections on *//*LAN口名称，例如br0*/
    char *gw_address;		/**< @brief Internal IP address for our web server *//*LAN口IP地址*/
    int gw_port;		/**< @brief Port the webserver will run on */
    int gw_redirect_port; /** @终端重定向login页面 */
	int gw_first_page_handle_port; /*add@2014.8.15 first page hande. http server port*/

    /* BEGIN: Added by Zhou Shouya, 2014/1/24   PN:本机wan口 信息 */
    char *wan_interface;  	/*WAN口名称，例如eth0，ixp0，br0*/
	char *wan_address;		/*WAN口IP地址*/
    char *wan_mac;			/*WAN口MAC地址*/
    /* END:   Added by Zhou Shouya, 2014/1/24 */
    
    t_auth_serv	*auth_servers;	/**< @brief Auth servers list */
    char *ccs_server;
	char *snmp_server;
    char *httpdname;		/**< @brief Name the web server will return when
				     replying to a request */
    int httpdmaxconn;		/**< @brief Used by libhttpd, not sure what it
				     does */
    char *httpdrealm;		/**< @brief HTTP Authentication realm */
    char *httpdusername;	/**< @brief Username for HTTP authentication */
    char *httpdpassword;	/**< @brief Password for HTTP authentication */
    int clienttimeout;		/**< @brief How many CheckIntervals before a client
				     must be re-authenticated */
    int checkinterval;		/**< @brief Frequency the the client timeout check
				     thread will run. */
    int pingtimeouts; /**< @brief Frequency the ping timeout */
    int log_syslog;		/**< @brief boolean, wether to log to syslog */
    int syslog_facility;	/**< @brief facility to use when using syslog for
				     logging */
    t_firewall_ruleset	*rulesets;	/**< @brief firewall rules */
    t_trusted_mac *trustedmaclist; /**< @brief list of trusted macs */
    t_trusted_ip *trustediplist; /**< @brief list of trusted macs */
    t_trusted_wanhost *trustedwanhostlist; /**< @brief list of trusted macs */
    long long conf_ver;
	long long config_ver_tmp;    /**add @2014.8.13*/
    long long cmd_ver;
    long long  iUser_ver;		/*@2014.10.11 云平台通知网关用户下线 配置版本号*/

    int qos_enable;
    int tcp_timeout;
    int max_client; /*最大允许接入用户数:可配置*/
	int dev_max_client; /*最大允许接入用户数:产品定位相关*/
    char *model;
	int ext_debuglevel;
	en_macro_hotspot_switch macro_hotspot;/*add:for 网间漫游 2015-8-27*/
        int macro_hotspot_thread_status;/*网间漫游线程运行状态1:在运行  0:关闭*/
       
	unsigned int bw_up;  /*@ 2014.8.1 add config: bw_up*/
	unsigned int bw_down;  /*@ 2014.8.1 add config: bw_down*/
	unsigned long forcetimeout;  /*@ 2014.8.1 add config: forcetimeout*/
	unsigned long idletimeout;  /*@ 2014.8.1 add config: idletimeout*/
	unsigned int wan_flow_report_switch; /*@2014.8.26 add config: wan stat report switch*/
	unsigned int wan_flow_report_interval; /*@2014.8.26 add config: wan stat report interval*/
	int config_change_flag[32]; /*0: bw_up
									   *1: bw_down
									   *2: forcetimeout
									   *3: idletimeout
									   */
	#ifdef GW_X86_DEL
	int redirect_thread_num; /**@add 2014.8.16 for redirect thread pool*/
	#else
	#endif
	int child_process;
	int server_mode; /*0:禁用1: wifidog接口server 2: 第三方server*/
	char *wxl_ssid; /*微信连SSID*/
	char *wxl_bssid; /*微信连BSSID*/

	int uiHttpadswitch; /*HTTP攻击开关*/
	int uiHttpadwldura; /*HTTP攻击临时白名单持续时间*/
	int uiHttpadbldura; /*HTTP攻击临时黑名单持续时间*/
	int uiHttprdmaxcnt; /*一个周期的HTTP访问阀值，超过则认定为攻击性终端*/
	int uiHttpckIntvl;	/*HTTP攻击检测周期(秒)*/
	int uiHttpckStaLimit;	/*HTTP攻击终端数量门限*/
	
	int  uiSpanSsidAuthSwith;/*跨ssid认证开关 1: 跨ssid需要认证 0:跨ssid不需要认证*/
	struct hlist_head freeauth_maclist_HList[FREEAUTH_MACLIST_HASH_SIZE]; /*免认证mac名单列表*/
	pthread_mutex_t freeauth_maclist_mutex;	

	struct list_head stLanInfo; /*LAN口信息列表*/
	pthread_mutex_t laninfo_list_mutex; 
	unsigned int uiOption43; /*Option 43信息*/
} s_config;

/** @brief Get the current gateway configuration */
extern s_config *config_get_config(void);

/** @brief Initialise the conf system */
extern void config_init(void);
extern void config_read(const char *filename);

extern void config_validate(void);
extern t_auth_serv *get_auth_server(void);


/*系统配置相关*/



#ifdef OPENWRT
#define GATEWAY "gateway.@gateway[0]."
typedef struct {
	char hotspot_name[128];
	char hotspot_url[128];
	char hotspot_path[128];
	char Model[32];
	char WanInterface[32];
	char auth_ctrl[32];
	char ccs_server[128];
	char snmp_server[128];
	char servermode[128];
	char wxl_ssid[128];
	char wxl_bssid[128];
	char gateway_interface[32];
}wifidog_conf;


int get_wifidog_conf(void);
int set_wifidog_conf(void);


#endif

#define WIFIDOG_VLAN_FC_RULE      			"vlanfc_rule"
#define WIFIDOG_IOS_AUTO_PROTAL   			"ios_auto_portal"

extern int sys_config_set(char *name, char *value);
extern int sys_config_get(char *value, char *name, int len);

#define WIFIDOG_LOG_FILE				"/tmp/wd_sys.log"
#define WIFIDOG_USR_LOG_FILE			"/tmp/wd_usr.log"
#define WIFIDOG_SERVER_LOG_FILE			"/tmp/wd_srv.log"
#define WIFIDOG_PLATFORM_FILE			"/tmp/wd_sys.log" /*"/tmp/wd_platform.log"*/
#define WIFIDOG_REDIRECT_FILE			"/tmp/wd_sys.log" /*"/tmp/wd_redirect.log"*/
#define WIFIDOG_FIRSTPAGE_FILE			"/tmp/wd_sys.log" /*"/tmp/wd_firstpage.log"*/
#define WIFIDOG_MAINLOOP_FILE			"/tmp/wd_sys.log" /*"/tmp/wd_mainloop.log"*/
#define WIFIDOG_DEBUG_LOG_FILE			"/tmp/wd_debug.log"
#define WIFIDOG_VER_DOWN_LOG           "/tmp/wd_ver_down.log"
#ifdef GW_X86
#define WIFIDOG_DEBUG_USR_LOG_FILE		"/tmp/ramdisk0/wd_debug_usr.log"
#define WIFIDOG_APICALL_LOG_FILE		"/tmp/ramdisk0/wd_http.log"
#define WIFIDOG_HTTP_LOG_FILE			"/tmp/ramdisk0/wd_http.log"
#else
#define WIFIDOG_DEBUG_USR_LOG_FILE		"/tmp/wd_debug_usr.log"
#define WIFIDOG_APICALL_LOG_FILE		"/tmp/wd_http.log"
#define WIFIDOG_HTTP_LOG_FILE			"/tmp/wd_http.log"
#endif

#define WIFIDOG_MARCO_HOT_LOG "/tmp/wd_maco_hot.log"

#define WIFIDOG_STAINFO_AUDIT "/tmp/wd_stainfo_audit.log"

#define WIFIDOG_APP_AUTH "/tmp/wd_app_auth.log"

#define WIFIOG_SYN_CLOUD_LOG "/tmp/wd_syn_cloud.log"


#ifdef PCAP_SUPPORT
#define WIFIDOG_PCAP_LOG           "/tmp/wd_pcap.log"
#endif

#define WIFIDOG_LOCK_LOG           "/tmp/wd_lock.log"

#define WIFIDOG_SSID_AUTH "/tmp/wd_ssid_auth.log"

extern int sys_log_set(char *name, const char *format, ...);
extern int debug_log_set(char *name, int id, const char *format, ...);
extern int strmac_to_mac(char *str_mac, unsigned char *mac);
extern int addr_to_ip_string(const char *addr, char *ip, int len);
extern int strip_to_intip(char *srcip, char *dstip);
extern int strncat_format(char *str, int len, const char *format, ...);

extern int auth_mode_support_free(void);
extern int auth_mode_support_user(void);
extern int auth_mode_support_msg(void);
extern int auth_mode_support_qq(void);
extern int auth_mode_support_weibo(void);
extern int auth_mode_support_weixin(void);
extern int auth_mode_support_alipay(void);
extern int auth_mode_support_wxl(void);
#endif /* __CONF_H__ */
