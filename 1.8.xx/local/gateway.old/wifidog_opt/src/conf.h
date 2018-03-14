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
#define PORTAL_FIRST_PAGE       "/portal/first_page"
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
#define FIRST_PAGE_TASK_NUM		1
#else
#define FIRST_PAGE_TASK_NUM		1
#endif


#define NOISE_FLOOR (-95)

#define AUTH_OK         "/portal/auth_ok.html"
#define STA_MAC_PAGE    "/portal/sta_mac.html"
#define LOGOUT_PAGE    "/portal/sta_logout.html"
#define STATION_PAGE    "/portal/station.html"


#define DEFAULT_DAEMON 1
#define DEFAULT_DEBUGLEVEL __LOG_WARNING__ //__LOG_DEBUG__
#define DEFAULT_HTTPDMAXCONN 10
#define DEFAULT_GATEWAYID NULL
#define DEFAULT_GATEWAYPORT 2060
#define DEFAULT_GATEWAYREDIRECTPORT 2061
/*add@2014.8.15*/
#define DEFAULT_GATEWAYFIRSTPAGEHANDLEPORT 8062
#define DEFAULT_GET_STA_KEY_PORT 81

#define DEFAULT_HTTPDNAME "WiFiDog"
#define DEFAULT_CLIENTAGESWITCH 1
#define DEFAULT_CLIENTTIMEOUT 5
#define DEFAULT_CHECKINTERVAL 60
#define DEFAULT_STAKEY_INTVAL 300
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
#define DEFAULT_CLINET_MAX_ACCESS_NUM 200
#define DEFAULT_DEV_CLINET_MAX_ACCESS_NUM 0 /*0表示不限制*/
/*@}*/ 
#ifdef OPENWRT
#define MAX_MAC_BAW_NUM			512
#define BAW_NUM_PER_CONFIG		8
#define BBLINK_WHITE_LIST		GATEWAY"bb_white_mac_"
#define BBLINK_WHITE_LIST_NUM	GATEWAY"bb_white_mac_num"
#define BBLINK_BLACK_LIST		GATEWAY"bb_black_mac_"
#define BBLINK_BLACK_LIST_NUM	GATEWAY"bb_black_mac_num"

#define WD_MAC_NUM_MAX          64
#define NVRAM_WD_BLACK_MAC_HEAD GATEWAY"wd_black_mac"
#define NVRAM_WD_BLACK_MAC_NUM  GATEWAY"wd_black_mac_num"
#define NVRAM_WD_BLACK_MAC_LIST GATEWAY"wd_black_mac_list"

#define NVRAM_WD_WHITE_MAC_HEAD GATEWAY"wd_white_mac"
#define NVRAM_WD_WHITE_MAC_NUM  GATEWAY"wd_white_mac_num"
#define NVRAM_WD_WHITE_MAC_LIST GATEWAY"wd_white_mac_list"

#define WD_URL_NUM_MAX          64
#define NVRAM_WD_BLACK_URL_HEAD GATEWAY"wd_black_url"
#define NVRAM_WD_BLACK_URL_NUM  GATEWAY"wd_black_url_num"
#define NVRAM_WD_BLACK_URL_LIST GATEWAY"wd_black_url_list"

#define NVRAM_WD_WHITE_URL_HEAD GATEWAY"wd_white_url"
#define NVRAM_WD_WHITE_URL_NUM  GATEWAY"wd_white_url_num"
#define NVRAM_WD_WHITE_URL_LIST GATEWAY"wd_white_url_list"



#define CLIENT_AGE_SWITCH  GATEWAY"ClientAgeSwitch"

#define CLIENT_TIME_OUT  GATEWAY"ClientTimeOut"

#define QOS_ENABLE  GATEWAY"QosEnable"

#define FLOW_CTL_UP  GATEWAY"FlowCtlUp"

#define FLOW_CTL_DOWN  GATEWAY"FlowCtlDown"

#define FLOW_CTL_WHITE_UP  GATEWAY"FlowCtlWhiteUp"

#define FLOW_CTL_WHITE_DOWN  GATEWAY"FlowCtlWhiteDown"

#define WD_STAKEY_INTVAL  GATEWAY"wd_stakey_intval"



#else
#define MAX_MAC_BAW_NUM			512
#define BAW_NUM_PER_CONFIG		8
#define BBLINK_WHITE_LIST		"bb_white_mac_"
#define BBLINK_WHITE_LIST_NUM	"bb_white_mac_num"
#define BBLINK_BLACK_LIST		"bb_black_mac_"
#define BBLINK_BLACK_LIST_NUM	"bb_black_mac_num"

#define WD_MAC_NUM_MAX          64
#define NVRAM_WD_BLACK_MAC_HEAD "wd_black_mac"
#define NVRAM_WD_BLACK_MAC_NUM  "wd_black_mac_num"
#define NVRAM_WD_BLACK_MAC_LIST "wd_black_mac_list"

#define NVRAM_WD_WHITE_MAC_HEAD "wd_white_mac"
#define NVRAM_WD_WHITE_MAC_NUM  "wd_white_mac_num"
#define NVRAM_WD_WHITE_MAC_LIST "wd_white_mac_list"

#define WD_URL_NUM_MAX          64
#define NVRAM_WD_BLACK_URL_HEAD "wd_black_url"
#define NVRAM_WD_BLACK_URL_NUM  "wd_black_url_num"
#define NVRAM_WD_BLACK_URL_LIST "wd_black_url_list"

#define NVRAM_WD_WHITE_URL_HEAD "wd_white_url"
#define NVRAM_WD_WHITE_URL_NUM  "wd_white_url_num"
#define NVRAM_WD_WHITE_URL_LIST "wd_white_url_list"

#endif
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
    char *gw_interface;		/**< @brief Interface we will accept connections on */
    char *gw_address;		/**< @brief Internal IP address for our web
				     server */
    int gw_port;		/**< @brief Port the webserver will run on */
    int gw_redirect_port; /** @终端重定向login页面 */
	int gw_first_page_handle_port; /*add@2014.8.15 first page hande. http server port*/
    int gw_sta_key_port; /** @终端重定向login页面 */

    /* BEGIN: Added by Zhou Shouya, 2014/1/24   PN:本机wan口 信息 */
    char *wan_interface;
    char *wan_mac;
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
	int ios_auto_portal;

	unsigned int bw_up;  /*@ 2014.8.1 add config: bw_up*/
	unsigned int bw_down;  /*@ 2014.8.1 add config: bw_down*/
	unsigned int bw_white_up;  /*@ 2015.04.21 add config: 白名单流控*/
	unsigned int bw_white_down;  /*@ 2015.04.21 add config: 白名单流控*/
	int           iAgeingSwitch;
	unsigned long forcetimeout;  /*@ 2014.8.1 add config: forcetimeout*/
	unsigned long idletimeout;  /*@ 2014.8.1 add config: idletimeout*/
	int           iStaKeyIntval;
	char          acStaKey[256];
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
} s_config;

/** @brief Get the current gateway configuration */
extern s_config *config_get_config(void);

/** @brief Initialise the conf system */
extern void config_init(void);
extern void config_read(const char *filename);

extern void config_validate(void);
extern t_auth_serv *get_auth_server(void);


/*系统配置相关*/




typedef struct {
	char hotspot_name[128];
	char hotspot_url[128];
	char hotspot_path[128];
	char Model[32];
	char WanInterface[32];
	char ios_auto_portal[32];
	char ccs_server[128];
	char snmp_server[128];
}wifidog_conf;


int get_wifidog_conf(void);
int set_wifidog_conf(void);



#define WIFIDOG_VLAN_FC_RULE      			GATEWAY"vlanfc_rule"
#define WIFIDOG_IOS_AUTO_PROTAL   			GATEWAY"ios_auto_portal"

extern int sys_config_set(char *name, char *value);
extern int sys_config_get(char *value, char *name);

extern int factory_config_get(char *value, char *name);

#define WIFIDOG_LOG_FILE				"/tmp/wd_sys.log"
#define WIFIDOG_USR_LOG_FILE			"/tmp/wd_usr.log"
#define WIFIDOG_SERVER_LOG_FILE			"/tmp/wd_srv.log"
#define WIFIDOG_PLATFORM_FILE			"/tmp/wd_platform.log"
#define WIFIDOG_REDIRECT_FILE			"/tmp/wd_redirect.log"
#define WIFIDOG_FIRSTPAGE_FILE			"/tmp/wd_firstpage.log"
#define WIFIDOG_MAINLOOP_FILE			"/tmp/wd_mainloop.log"
#define WIFIDOG_DEBUG_LOG_FILE			"/tmp/wd_debug.log"

extern int sys_log_set(char *name, const char *format, ...);
extern int strmac_to_mac(char *str_mac, unsigned char *mac);

#endif /* __CONF_H__ */
