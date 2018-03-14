/* -----------------------------------------------------------------------------
 * Copyright (C) 2008 Robert Ernst <robert.ernst@linux-solutions.at>
 *
 * This file may be distributed and/or modified under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See COPYING for GPL licensing information.
 */



#ifndef __MINI_SNMPD_HEADER__
#define __MINI_SNMPD_HEADER__

#include <sys/types.h>
#include <netinet/in.h>
#include "ap_types.h"

#include "log.h"
/* -----------------------------------------------------------------------------
 * External defines
 */

extern INT32 g_iSnmpLogLevel;

/* -----------------------------------------------------------------------------
 * Project dependent defines
 */
#define FTP_PORT        21
#define CONFIG_FILE_DIR         "/tmp/tmp.bin"
#define UPDATE_IMAGE_LOG        "/tmp/auto_up_ver_info"
#define CURR_VERSION            "currVersion"
#define TARG_VERSION            "TargVersion"
#define FILE_TRANS_DIR          "/tmp/currVersion"

#define SW_FILE_DIR             "/tmp/"
#define SW_FILE_NAME            "tmp.bin"
#define LOG_FILE_DIR            "/tmp/"
#define LOG_FILE_NAME           "messages.txt"
#define SHELL_FILE_DIR          "/tmp/"
#define SHELL_FILE_NAME         "cmsshell.sh"


#define EXIT_OK								0
#define EXIT_ARGS							1
#define EXIT_SYSCALL						2

#define MAX_NR_CLIENTS						16
#define MAX_NR_OIDS							32//16->32
#define MAX_NR_SUBIDS						32//16->32
#define MAX_NR_DISKS						4
#define MAX_NR_INTERFACES					4
#define MAX_NR_VALUES						128

#define MAX_PACKET_SIZE						2048
#define MAX_STRING_SIZE						64


#define BYTE_BITNUM                         8
/* -----------------------------------------------------------------------------
 * SNMP dependent defines
 */

#define BER_TYPE_BOOLEAN					0x01
#define BER_TYPE_INTEGER					0x02
#define BER_TYPE_BIT_STRING					0x03
#define BER_TYPE_OCTET_STRING				0x04
#define BER_TYPE_NULL						0x05
#define BER_TYPE_OID						0x06
#define BER_TYPE_SEQUENCE					0x30

/*feng jing add:*/
#define BER_TYPE_IP                         0x40

#define BER_TYPE_COUNTER					0x41
#define BER_TYPE_GAUGE						0x42
#define BER_TYPE_TIME_TICKS					0x43

/*feng jing add:*/
#define BER_TYPE_COUNTER64                  0x46

#define BER_TYPE_NO_SUCH_OBJECT				0x80
#define BER_TYPE_NO_SUCH_INSTANCE			0x81
#define BER_TYPE_END_OF_MIB_VIEW			0x82
#define BER_TYPE_SNMP_GET					0xA0
#define BER_TYPE_SNMP_GETNEXT				0xA1
#define BER_TYPE_SNMP_RESPONSE				0xA2
#define BER_TYPE_SNMP_SET					0xA3
#define BER_TYPE_SNMP_GETBULK				0xA5
#define BER_TYPE_SNMP_INFORM				0xA6
#define BER_TYPE_SNMP_TRAP					0xA7
#define BER_TYPE_SNMP_REPORT				0xA8

#define SNMP_VERSION_1						0
#define SNMP_VERSION_2C						1
#define SNMP_VERSION_3						3

#define SNMP_STATUS_OK						0
#define SNMP_STATUS_TOO_BIG					1
#define SNMP_STATUS_NO_SUCH_NAME			2
#define SNMP_STATUS_BAD_VALUE				3
#define SNMP_STATUS_READ_ONLY				4
#define SNMP_STATUS_GEN_ERR					5
#define SNMP_STATUS_NO_ACCESS				6
#define SNMP_STATUS_WRONG_TYPE				7
#define SNMP_STATUS_WRONG_LENGTH			8
#define SNMP_STATUS_WRONG_ENCODING			9
#define SNMP_STATUS_WRONG_VALUE				10
#define SNMP_STATUS_NO_CREATION				11
#define SNMP_STATUS_INCONSISTENT_VALUE		12
#define SNMP_STATUS_RESOURCE_UNAVAILABLE	13
#define SNMP_STATUS_COMMIT_FAILED			14
#define SNMP_STATUS_UNDO_FAILED				15
#define SNMP_STATUS_AUTHORIZATION_ERROR		16
#define SNMP_STATUS_NOT_WRITABLE			17
#define SNMP_STATUS_INCONSISTENT_NAME		18



/* -----------------------------------------------------------------------------
 * Macros
 */
/* BEGIN: Added by Zhou Shouya, 2012/11/2   PN:–ﬁ∏ƒ»’÷æ */
#define __func__ __FUNCTION__
#define __line__ __LINE__
#define __file__ __FILE__

#ifdef SYSLOG
#define lprintf(level, format, arg...) \
    do \
    {  \
        app_log((char *)__func__, __line__, level, format, ##arg);\
    } while(0)
        
#else
#define lprintf(level, format...) \
	do { \
		if (g_verbose || (level != LOG_DEBUG)) { \
			fprintf(stderr, format); \
		} \
	} while (0)
#endif
/* END:   Added by Zhou Shouya, 2012/11/2 */

#define snmp_log(level, format, arg...) \
do{                                    \
    if (level <= g_iSnmpLogLevel)       \
    {                                  \
        syslog(level, "[line:%d] [func:%s]"format, __LINE__, __FUNCTION__, ##arg);\
    }                                  \
}while(0);

#ifdef __IPV4__
#define my_sockaddr_t		sockaddr_in
#define my_socklen_t		socklen_t
#define my_sin_addr			sin_addr
#define my_sin_port			sin_port
#define my_sin_family		sin_family
#define my_af_inet			AF_INET
#define my_pf_inet			PF_INET
#define my_in_addr_t		in_addr
#define my_in_port_t		in_port_t
#define my_inaddr_any		inaddr_any
#define my_inet_addrstrlen	INET_ADDRSTRLEN
#endif

#ifdef __IPV6__
#define my_sockaddr_t		sockaddr_in6
#define my_socklen_t		socklen_t
#define my_sin_addr			sin6_addr
#define my_sin_port			sin6_port
#define my_sin_family		sin6_family
#define my_af_inet			AF_INET6
#define my_pf_inet			PF_INET6
#define my_in_addr_t		in6_addr
#define my_in_port_t		in_port_t
#define my_inaddr_any		in6addr_any
#define my_inet_addrstrlen	INET6_ADDRSTRLEN
#endif

typedef enum 
{
    OM_E_NONE,                  /* 0, successful */
    OM_E_RADIOID,               /* invalid radio ID */
    OM_E_WLANID,                /* invalid WLAN ID */
    OM_E_IFID,                  /* invalid interface ID */
    OM_E_CMD_ID,                /* invalid command ID */
    OM_E_ASYNC_CMD_ID,          /* invalid async command ID */
    OM_E_EVENT_ID,              /* invalid event ID */
    OM_E_VALUE,                 /*Invalid value*/
    OM_E_FILE_NAME,             /*Invalid file name*/
    OM_E_RANGE,                 /* parameter exceed range */
    OM_E_PARAM,                 /* other invalid parameter */
    OM_E_RESOURCE,              /* fail to get resource*/
    OM_E_NOT_EXIST,             /* Object don't exist */
    OM_E_INIT,                  /* Feature not initialized. */
    OM_E_REINIT,                /* Feature initialized more than once. */
    OM_E_ORDER,                 /* Operator is not in order */
    OM_E_MEM_MALLOC,            /* memory malloc error */
    OM_E_UPGRADE_NOSPACE,       /* upgrade no space error */
    OM_E_NULLPTR,               /*an pointer point to NULL*/
    OM_E_SYSTEM_CMD,            /* system cmd fail */
    OM_E_DEFAULT_FAIL,          /*default failure*/
} OM_ERROR_CODE_E;


/* -----------------------------------------------------------------------------
 * Data types
 */
#if 0
typedef enum{
    SNMP_STATE_NONE = 0, 
    SNMP_STATE_DISCOVER,
    SNMP_STATE_UPDATE,
    SNMP_STATE_RUNNING,
    SNMP_STATE_MAX,
}SNMP_STATE_E;
#endif

typedef enum{
    DEV_STATE_NONE=0,
    DEV_STATE_RUNNING=1,
    DEV_STATE_DISCOVER=2,
    DEV_STATE_MAX
}DEV_STATE_E;

typedef enum {
	SNMP_UPDATE_STATE_NONE = 0,
	SNMP_UPDATE_STATE_SUCCESS,
	SNMP_UPDATE_STATE_RECV_ERR,
	SNMP_UPDATE_STATE_FILE_WRONG,
	SNMP_UPDATE_STATE_FLASH_ERR,
	SNMP_UPDATE_STATE_MAX,
}SNMP_UPDATE_STATE_E;

typedef enum{
    SNMP_FILE_NONE = 0,
    SNMP_FILE_SW,
    SNMP_FILE_CONFIG,
    SNMP_FILE_LOG,
    SNMP_FILE_CERT,
    SNMP_FILE_PERF,
    SNMP_FILE_SHELL,
    SNMP_FILE_MAX,
}SNMP_FILE_STATE_E;

typedef enum{
    SNMP_TRANS_NONE = 0,
    SNMP_TRANS_FTP,
    SNMP_TRANS_TFTP,
    SNMP_TRANS_HTTP,
    SNMP_TRANS_HTTPS,
    SNMP_TRANS_MAX,
}SNMP_TRANS_PROTO_E;

typedef enum{
    SNMP_LOG_CFG_CLEANUP = 0,
    SNMP_LOG_CFG_GET,
    SNMP_LOG_CFG_MAX,
}SNMP_LOG_CFG_E;

typedef enum{
    SNMP_DEVICE_NONE = 0,
    SNMP_DEVICE_REBOOT_RESET,
    SNMP_DEVICE_MAX,
}SNMP_DEVICE_STATE_E;

typedef enum{
    SNMP_L2TP_NONE = 0,
    SNMP_L2TP_TELNET_ON,
    SNMP_L2TP_TELNET_OFF,
    SNMP_L2TP_MAX,
}SNMP_L2TP_STATE_E;

typedef enum{
    SNMP_LOAD_NONE = 0,
    SNMP_LOAD_DOWN,
    SNMP_LOAD_UP,
    SNMP_LOAD_MAX,
}SNMP_LOAD_TYPE_E;

typedef struct client_s {
	time_t timestamp;
	int sockfd;
	struct my_in_addr_t addr;
	my_in_port_t port;
	unsigned char packet[MAX_PACKET_SIZE];
	size_t size;
	int outgoing;
} client_t;

typedef struct oid_s {
	unsigned int subid_list[MAX_NR_SUBIDS];
	short subid_list_length;
	short encoded_length;
} oid_t;

typedef struct data_s {
	unsigned char *buffer;
	short max_length;
	short encoded_length;
} data_t;

typedef struct value_s {
	oid_t oid;
	data_t data;
} value_t;

typedef struct request_s {
	char community[MAX_STRING_SIZE];
	int type;
	int version;
	int id;
	int non_repeaters;
	int max_repetitions;
	oid_t oid_list[MAX_NR_OIDS];
	int oid_list_length;

	/* feng jing add: */
	data_t data_list[MAX_NR_OIDS];
} request_t;

typedef struct response_s {
	int error_status;
	int error_index;
	value_t value_list[MAX_NR_VALUES];
	int value_list_length;
} response_t;

typedef struct loadinfo_s {
	unsigned int avg[3];
} loadinfo_t;

typedef struct meminfo_s {
	unsigned int total;
	unsigned int free;
	unsigned int shared;
	unsigned int buffers;
	unsigned int cached;
} meminfo_t;

typedef struct cpuinfo_s {
	unsigned int user;
	unsigned int nice;
	unsigned int system;
	unsigned int idle;
	unsigned int irqs;
	unsigned int cntxts;
} cpuinfo_t;

typedef struct diskinfo_s {
	unsigned int total[MAX_NR_DISKS];
	unsigned int free[MAX_NR_DISKS];
	unsigned int used[MAX_NR_DISKS];
	unsigned int blocks_used_percent[MAX_NR_DISKS];
	unsigned int inodes_used_percent[MAX_NR_DISKS];
} diskinfo_t;

typedef struct netinfo_s {
	unsigned int status[MAX_NR_INTERFACES];
	unsigned int rx_bytes[MAX_NR_INTERFACES];
	unsigned int rx_packets[MAX_NR_INTERFACES];
	unsigned int rx_errors[MAX_NR_INTERFACES];
	unsigned int rx_drops[MAX_NR_INTERFACES];
	unsigned int tx_bytes[MAX_NR_INTERFACES];
	unsigned int tx_packets[MAX_NR_INTERFACES];
	unsigned int tx_errors[MAX_NR_INTERFACES];
	unsigned int tx_drops[MAX_NR_INTERFACES];
} netinfo_t;

#ifdef __DEMO__
typedef struct demoinfo_s {
	unsigned int random_value_1;
	unsigned int random_value_2;
} demoinfo_t;
#endif



/* -----------------------------------------------------------------------------
 * Global variables
 */

extern in_port_t g_udp_port;
extern in_port_t g_tcp_port;
extern int g_timeout;
extern int g_auth;
extern int g_verbose;
extern int g_quit;
extern char *g_rocommunity;
extern char *g_rwcommunity;
extern char *g_description;
extern char *g_vendor;
extern char *g_location;
extern char *g_contact;
extern char *g_bind_to_device;
extern int g_stati_per;
extern int g_traps_switch;
#ifdef __IPV4__
extern const struct in_addr inaddr_any;
#endif

extern char *g_disk_list[MAX_NR_DISKS];
extern int g_disk_list_length;
extern char *g_interface_list[MAX_NR_INTERFACES];
extern int g_interface_list_length;
extern client_t g_udp_client;
extern client_t *g_tcp_client_list[MAX_NR_CLIENTS];
extern int g_tcp_client_list_length;
extern int g_udp_sockfd;
extern int g_tcp_sockfd;
extern value_t g_mib[MAX_NR_VALUES];
extern int g_mib_length;
extern time_t g_mib_timestamp;



/* -----------------------------------------------------------------------------
 * Module variables
 */

static const data_t m_null					= { (unsigned char *)"\x05\x00", 2, 2 };
static const data_t m_no_such_object		= { (unsigned char *)"\x80\x00", 2, 2 };
static const data_t m_no_such_instance		= { (unsigned char *)"\x81\x00", 2, 2 };
static const data_t m_end_of_mib_view		= { (unsigned char *)"\x82\x00", 2, 2 };


/* -----------------------------------------------------------------------------
 * Functions
 */

void dump_packet(const client_t *client);
void dump_mib(const value_t *value, int size);
void dump_response(const response_t *response);
char *oid_ntoa(const oid_t *oid);
oid_t *oid_aton(const char *str);
int oid_cmp(const oid_t *oid1, const oid_t *oid2);
int split(const char *str, char *delim, char **list, int max_list_length);
client_t *find_oldest_client(void);
int read_file(const char *filename, char *buffer, size_t size);
unsigned int read_value(const char *buffer, const char *prefix);
void read_values(const char *buffer, const char *prefix, unsigned int *values, int count);
int ticks_since(const struct timeval *tv_last, struct timeval *tv_now);

unsigned int get_process_uptime(void);
unsigned int get_system_uptime(void);
void get_loadinfo(loadinfo_t *loadinfo);
void get_meminfo(meminfo_t *meminfo);
void get_cpuinfo(cpuinfo_t *cpuinfo);
void get_diskinfo(diskinfo_t *diskinfo);
void get_netinfo(netinfo_t *netinfo);
#ifdef __DEMO__
void get_demoinfo(demoinfo_t *demoinfo);
#endif

int snmp_packet_complete(const client_t *client);
int snmp(client_t *client);
int snmp_element_as_string(const data_t *data, char *buffer, size_t size);

int mib_build(void);
int mib_update(int full);
int mib_find(const oid_t *oid);
int mib_findnext(const oid_t *oid);

BOOL get_cms_server_ip(void);


#endif



/* vim: ts=4 sts=4 sw=4 nowrap
 */
