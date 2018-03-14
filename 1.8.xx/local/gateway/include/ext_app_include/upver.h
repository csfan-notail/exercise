#ifndef __UPVER_H__
#define __UPVER_H__
#define MAX_REBOOT_RECORD_NUM   16
#define REBOOT_HARDRESET        0    
#define REBOOT_SOFTRESET        1
#define REBOOT_SHOW_GET_ACCESS  "/tmp/showreboot.flag"
#define REBOOT_USER_FILE        "/proc/userfile"
#define REBOOT_RECORD_FILE      "/tmp/rebootrecord"
#define REBOOT_SOFTRESET_FILE   "/tmp/softreboot"
#define REBOOT_HARDRESET_FILE   "/tmp/hardreboot"

#define AP_MISC_SUCCESS         0
#define AP_MISC_ERROR           -1

#ifdef AP93_OAM
#define FLASH_ERASE_SIZE    65536
#else
#define FLASH_ERASE_SIZE    131072
#endif
#define FLAG_POS_HIGH       65536

typedef struct reboot_Info

{
    int         iType; /* 0 -- hardReset; 1 -- softReset */
    int         iUptime;
    struct tm   stCurTime; 
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

/* LOGÎÄ¼þ±àºÅ */
#define LOG_FILE_CUR            1    /*  log since this reboot */
#define LOG_FILE_ALL            2     /* log since power on */

#define LOG_MAX_ITEM_MSG_LENGTH 128
typedef struct rc_log_item_tag
{
	unsigned short usModuleId;
	unsigned short usType;
	unsigned short usLevel;
	unsigned char rsv1[2];
	struct timeval  tsCurTime;      /* time based on 1970-01-01 00:00:00 */
	struct timeval  tsRsvTime;      /* no use now */

	unsigned char rsv2[32];
	char acMessage[LOG_MAX_ITEM_MSG_LENGTH];
} rc_log_item_t;

typedef rc_log_item_t LOG_ITEM_T;
#endif