/*
*blackbox.h
*/

#ifndef  BLACK_BOX_H
#define  BLACK_BOX_H

/******************************************************************************
*
* Number Macro defination
*/
#define bb_ENTRY_FILENAME "bb"
#define user_file_ENTRY_FILENAME	"userfile"


#define get_board_version()	0

#define LOG_AREA_SIZE	(1 << 20) 	/* 1M */
#define LOG_AREA_NUM 	2 

#define BB_SOFT_VERSION "0.2"
#define BB_ENTRY_BUF_SIZE (1<<10)
#define BB_ENTRY_NUM_PER_AREA (LOG_AREA_SIZE/BB_ENTRY_BUF_SIZE)
#define BB_ENTRY_NUM (BB_ENTRY_NUM_PER_AREA*LOG_AREA_NUM)

#define BB_BUF_MASK	(LOG_AREA_SIZE-1)

/*
#define PORT_NAME_LEN 15
#define RC_SOFT_VERSION "2"

#define u32 unsigned long
#define u16 unsigned short
#define u8 unsigned char
*/

#define LOG_MAX_ITEM_MSG_LENGTH 128

#define PROTECT_PAGE_SIZE	 2048
#define TRACE_SIZE 	2048


#define MAX_REBOOT_RECORD_NUM   16

#define REBOOT_HARDRESET        0    
#define REBOOT_SOFTRESET        1


/******************************************************************************
*
* typedef enum
*/


/******************************************************************************
*
* MACRO Functions
*/

/******************************************************************************
*
* typedef data type
*/

/******************************************************************************
*
* typedef structure
*/
typedef struct bb_log_area_tag
{
	unsigned long bb_log_start;	/* Index into log_buf: next char to be read by syslog() */
	unsigned long bb_con_start;	/* Index into log_buf: next char to be sent to consoles */
	unsigned long bb_log_end;	/* Index into log_buf: most-recently-written-char + 1 */
	unsigned long bb_logged_chars; /* Number of chars produced since last read+clear operation */

	int trace_area_left_space;
	char	trace_area[TRACE_SIZE];
	char	p[PROTECT_PAGE_SIZE + LOG_AREA_SIZE];	
}bb_log_area_t;

typedef struct bb_log_tag
{
	unsigned char 	log_area;
	unsigned char 	reboot_count;
	unsigned char 	rsv1[30];
	bb_log_area_t log[LOG_AREA_NUM];
}bb_log_t;

struct bb_iter_state
{
	int bb_first_area;
	int bb_recent_area; 	 
	char bb_buf[BB_ENTRY_BUF_SIZE+64];
	struct bb_iter_entry* pEntry;
};

struct bb_iter_entry
{
	int off;
};

typedef struct rc_log_item_tag
{
    u_int16_t   usModuleId;
    u_int16_t   usType;
    u_int16_t   usLevel;
    unsigned char rsv1[2];	
    struct timeval  tsCurTime;      /* time based on 1970-01-01 00:00:00 */
    struct timeval  tsRsvTime;      /* no use now */

    unsigned char rsv2[32];
    char acMessage[LOG_MAX_ITEM_MSG_LENGTH];

} rc_log_item_t;

typedef struct reboot_Info

{
    int         iType; /* 0 -- hardReset; 1 -- softReset */
    int         iUptime;

#ifdef __KERNEL__
    struct
    {
      int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
      int tm_min;			/* Minutes.	[0-59] */
      int tm_hour;			/* Hours.	[0-23] */
      int tm_mday;			/* Day.		[1-31] */
      int tm_mon;			/* Month.	[0-11] */
      int tm_year;			/* Year	- 1900.  */
      int tm_wday;			/* Day of week.	[0-6] */
      int tm_yday;			/* Days in year.[0-365]	*/
      int tm_isdst;			/* DST.		[-1/0/1]*/

      long int tm_gmtoff;		/* Seconds east of UTC.  */
      char *tm_zone;	/* Timezone abbreviation.  */
    } stCurTime;
#else    
    struct tm   stCurTime; 
#endif
    char        acReason[128];
}T_RebootInfo;

typedef struct usr_file_info
{
    int             iCheck;
#define REBOOT_CHECK 0x01010101
    unsigned char   ucRebootCount;   /*  */
    unsigned char   ucRecordPos;
    int             iFlag;          /* 0 -- hardReset; 1 -- softReset */  
    T_RebootInfo    stRebootInfo[MAX_REBOOT_RECORD_NUM];
    //int             iResv[256];
}USR_FILE_INFO_T;


#define REBOOT_SHOW_GET_ACCESS  "/tmp/showreboot.flag"
#define REBOOT_USER_FILE        "/proc/userfile"
#define REBOOT_RECORD_FILE      "/tmp/rebootrecord"
#define REBOOT_SOFTRESET_FILE   "/tmp/softreboot"
#define REBOOT_HARDRESET_FILE   "/tmp/hardreboot"
typedef rc_log_item_t LOG_ITEM_T;
#define AP_MISC_SUCCESS         0
#define AP_MISC_ERROR           -1

#endif /* BLACK_BOX_H */

