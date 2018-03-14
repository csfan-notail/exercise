/******************************************************************
*  blackbox.c
*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/jiffies.h>
#include <linux/mm.h>
#include <linux/version.h>
#include "blackbox.h"

//#define RAM_SIZE 0x8000000  /*total ram size 128M*/
//#define RSV_MEM_SIZE 0x400000 /*rsv ram size 4M*/
unsigned int RAM_SIZE = 0x8000000;
unsigned int RSV_MEM_SIZE = 0x400000;

#define RSV_MEM_BASE_VIRT (PAGE_OFFSET+RAM_SIZE - RSV_MEM_SIZE) /*pos*/

#define USER_FILE_ITEM_NUM  20
#define USER_FILE_SIZE (sizeof(rc_log_item_t) * USER_FILE_ITEM_NUM)
#define USER_FILE_ADDR (PAGE_OFFSET + RAM_SIZE - sizeof(rc_log_item_t) * USER_FILE_ITEM_NUM)


/********************************************************************
*
* 	Debug tools
*/

/********************************************************************
*
*  Extern functions and variables
*/
extern void (*bb_log_char_hook)(char c); 
extern int (*add_reboot_rec)(char* msg);
//extern long current_kernel_upseconds(void);
/********************************************************************
*
*  Local functions declarations
*/
static void bb_log_char(char c);
int userfile_add_reboot_rec(char* msg);
/********************************************************************
*
*Extern varible declaration
*/
//extern struct proc_dir_entry proc_root;

/********************************************************************
*
*Local varible declaration
*/

static bb_log_t * bb_buf_log = NULL;

static struct proc_dir_entry *bb_Proc_File;
static struct proc_dir_entry *user_file_Proc_File;

static unsigned char ucBoardVer = 0;
static char *user_file_addr = NULL;

static bb_log_t *bb_buf = NULL;

int bb_init_finished = 0;

static USR_FILE_INFO_T uf_info;


/********************************************************************
*
*Function defination
*/

long current_kernel_upseconds(void)
{
    struct timespec ts;

    ktime_get_ts(&ts);
    return (long)ts.tv_sec;
}

/*将打印信息定向到高段内存空间*/
/*****************************************************************************
 函 数 名  : bb_buf_init
 功能描述  : 初始化函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年7月19日
    作    者   : WeiHonggang
    修改内容   : 新生成函数

*****************************************************************************/
void bb_buf_init(void)
{
	bb_log_area_t *log;

	bb_buf = (bb_log_t *)KSEG1ADDR(RSV_MEM_BASE_VIRT);
	
	if (bb_buf->log_area < LOG_AREA_NUM)
	{
		bb_buf->log_area = (bb_buf->log_area + 1) % LOG_AREA_NUM;
		log = &bb_buf->log[bb_buf->log_area];
		log->bb_log_start = 0;	
		log->bb_con_start = 0;	
		log->bb_log_end   = 0;	
		log->bb_logged_chars = 0;	
		log->trace_area_left_space = TRACE_SIZE;  		
		memset(log->trace_area, 0, TRACE_SIZE);
	} 
	else 
	{
		memset(bb_buf, 0 , sizeof(bb_log_t));
	}

	bb_buf->reboot_count++;
	bb_init_finished = 1;

	bb_log_char_hook = bb_log_char;

	return;

} 

/*****************************************************************************
 函 数 名  : bb_log_char
 功能描述  : 将打印信息输出到高段1M内存空间
 输入参数  : char c  
 输出参数  : 无
 返 回 值  : static
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年7月20日
    作    者   : WeiHonggang
    修改内容   : 新生成函数

*****************************************************************************/
static void bb_log_char(char c)
{ 
	bb_log_area_t *log;

	if (!bb_init_finished)
		return;

	log = &bb_buf->log[bb_buf->log_area % LOG_AREA_NUM];

	log->p[log->bb_log_end & BB_BUF_MASK] = c;
	log->bb_log_end++;
	if (log->bb_log_end  - log->bb_log_start >= LOG_AREA_SIZE)
		log->bb_log_end = log->bb_log_start;
	if (log->bb_logged_chars < LOG_AREA_SIZE)
		log->bb_logged_chars++;

	return;
}

/*****************************************************************************
 函 数 名  : get_bb_log
 功能描述  : 读取日志
 输入参数  : int log_area  
             char * buf    
             int len       
             int offset    
             int *ret      
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年7月20日
    作    者   : WeiHonggang
    修改内容   : 新生成函数

*****************************************************************************/
int get_bb_log(int log_area, char * buf, int len, int offset, int *ret)
{
	unsigned long i;
	//int do_clear = 0;
	char c;
	int error = 0;
	int bb_log_start;
	int bb_log_end;
	char *p;
	char overflow = 0;
	bb_log_area_t *log;

	log = &bb_buf->log[log_area % LOG_AREA_NUM];  

	bb_log_start = log->bb_log_start + offset;  
	bb_log_end   = log->bb_log_end;        
	p = log->p;
	if (log->bb_logged_chars >= LOG_AREA_SIZE)
		overflow = 1;               

	if (!buf || len <= 0)
		return 0;
  
	i = 0;

	if (!overflow)
	{
		while (!error && (bb_log_start < bb_log_end) && i < len)
		{
				c = p[bb_log_start & BB_BUF_MASK];
				bb_log_start++;
				*buf = c;
				buf++;
				i++;
		}
		if (bb_log_start >= bb_log_end)
		{
			*ret = 1;
		} 
		else     
		{
			*ret = 0; 
		}	    
	} 
	else
	{
		bb_log_start = bb_log_start & BB_BUF_MASK;
		while (!error && (bb_log_start < LOG_AREA_SIZE) && i < len)
		{
			c = p[bb_log_start & BB_BUF_MASK];
			bb_log_start++;
			*buf = c;
			buf++;
			i++;
		}    
		if (bb_log_start >= LOG_AREA_SIZE)
		{
			*ret = 1;
		}
		else     
		{
			*ret = 0; 
		}
	}

	if (!error)
		error = i;

  return error; 
}

/*****************************************************************************
 函 数 名  : get_bb_area
 功能描述  : 读取日志区域
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年7月20日
    作    者   : WeiHonggang
    修改内容   : 新生成函数

*****************************************************************************/
int get_bb_area(void)
{
	return bb_buf->log_area;
}
/*****************************************************************************
 函 数 名  : get_trace_area
 功能描述  : 获取trace 区域信息
 输入参数  : char log_area  
             int len        
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年7月20日
    作    者   : WeiHonggang
    修改内容   : 新生成函数

*****************************************************************************/
char* get_trace_area(char log_area, int len)
{
	bb_log_area_t *log;

	if (!bb_init_finished)
		return NULL;

	log = &bb_buf->log[bb_buf->log_area % LOG_AREA_NUM];
	if ((log->trace_area_left_space < len) || (TRACE_SIZE < log->trace_area_left_space))
	{
		return NULL;	
	}
	else 
	{
		log->trace_area_left_space -= len;
		return (char *)(log->trace_area + TRACE_SIZE - log->trace_area_left_space);
	}	
}

/*****************************************************************************
 函 数 名  : get_bb_reboot_count
 功能描述  : 读取重启次数
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年7月20日
    作    者   : WeiHonggang
    修改内容   : 新生成函数

*****************************************************************************/
int get_bb_reboot_count(void)
{
	return bb_buf->reboot_count;
}

/* --------------------------------------------------------------------------
										FS Interface (/proc/bb) using Seq File
	 -------------------------------------------------------------------------- */
static struct bb_iter_entry* bb_iter_entry_get(struct bb_iter_state *iter, loff_t off)
{
	if ((NULL == iter->pEntry) || (off >= BB_ENTRY_NUM))
	{
		return NULL;
	}
	return &iter->pEntry[off];

}
static void *bb_seq_start(struct seq_file *seq, loff_t *pos)
{
	struct bb_iter_state *iter = seq->private;
	int i;

	if (NULL == iter->pEntry)
	{
		iter->pEntry = kmalloc(sizeof(struct bb_iter_entry)*BB_ENTRY_NUM, GFP_KERNEL);
		memset(iter->pEntry, 0, sizeof(struct bb_iter_entry)*BB_ENTRY_NUM);
		for (i = 0; i < BB_ENTRY_NUM; i++)
		{
			iter->pEntry[i].off= i;
		}
	}
		
	return *pos ? bb_iter_entry_get(iter, *pos-1) : SEQ_START_TOKEN;
}

static void *bb_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	struct bb_iter_entry *entry = v;
	struct bb_iter_state *iter = seq->private;

	++*pos;

	/* first line after header */
	if (v == SEQ_START_TOKEN)
		entry = bb_iter_entry_get(iter, 0);
	else if (entry->off + 1 < BB_ENTRY_NUM)
		entry = bb_iter_entry_get(iter, entry->off + 1);
	else
			entry = NULL; 			 

	return entry;
}

static void bb_seq_stop(struct seq_file *seq, void *v)
{
	struct bb_iter_state *iter = seq->private;

	if (NULL != iter->pEntry)
	{
		kfree(iter->pEntry);
		iter->pEntry = NULL;
	}
		
	return;

}

static int bb_seq_show(struct seq_file *seq, void *v)
{
	struct bb_iter_state *iter = seq->private;
	struct bb_iter_entry *entry = v;
	int ret;
	int len = 0;
	int area;
	int off;
		
	if (v == SEQ_START_TOKEN) {
		seq_printf(seq, "\n================== BlackBoard Record (VER %s), Board Version (%d) ================== \n", \
				BB_SOFT_VERSION,	ucBoardVer);
		seq_printf(seq, "Board ver %d, Firware ver %d\n", get_board_version(), -1);


				
		seq_printf(seq, "Reboot Times: %d , Current Area %d\n", get_bb_reboot_count(), get_bb_area());				
				
		iter->bb_first_area = (get_bb_area() + 1) % LOG_AREA_NUM; 	
		iter->bb_recent_area = -1;

	}
	else {
		area = (iter->bb_first_area + entry->off / BB_ENTRY_NUM_PER_AREA) % LOG_AREA_NUM;
		off  = (entry->off % BB_ENTRY_NUM_PER_AREA) * BB_ENTRY_BUF_SIZE;
		if (area != iter->bb_recent_area)
		{
			seq_printf(seq, "\n=================== Record Area:  %d ===================\n", area); 
		
			iter->bb_recent_area = area;
		}
		len = get_bb_log( area, iter->bb_buf, BB_ENTRY_BUF_SIZE, off, &ret);
		/* ret == 1, this area has been read out */
		if (len > 0)
		{
			iter->bb_buf[len] = '\0';
			seq_printf(seq, "%s", iter->bb_buf);			
			}
	}
	return 0;
}

static struct seq_operations bb_seq_ops = {
	.start	= bb_seq_start,
	.next 	= bb_seq_next,
	.stop 	= bb_seq_stop,
	.show 	= bb_seq_show,
};

static int bb_seq_interface_open(struct inode *inode, struct file *file)
{
	struct seq_file *seq;
	int rc = -ENOMEM;
	struct bb_iter_state *s = kmalloc(sizeof(*s), GFP_KERNEL);
		
	if (!s)
		goto out;
		
	rc = seq_open(file, &bb_seq_ops);
	if (rc)
		goto out_kfree;
		
	seq	= file->private_data;
	seq->private = s;
	memset(s, 0, sizeof(*s));
	out:
		return rc;
	out_kfree:
		kfree(s);
	goto out;
		
}


static const struct file_operations File_Ops_4_bb_Proc_File = {
	.owner		= THIS_MODULE,
	.open		= bb_seq_interface_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release_private,
};

static void cleanup_bb(void)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,26)
		remove_proc_entry(bb_ENTRY_FILENAME, NULL);
#else
		remove_proc_entry(bb_ENTRY_FILENAME, &proc_root);
#endif
}

static int init_bb(void)
{
	int rv = 0;
	
	bb_Proc_File = create_proc_entry(bb_ENTRY_FILENAME, 0644, NULL);
	bb_Proc_File->proc_fops = &File_Ops_4_bb_Proc_File;
	bb_Proc_File->mode = S_IFREG | S_IRUGO | S_IWUSR;
	bb_Proc_File->uid = 0;
	bb_Proc_File->gid = 0;
	bb_Proc_File->size = 80;

	if (bb_Proc_File == NULL) {
		rv = -ENOMEM;
        cleanup_bb();
		printk(KERN_INFO "Error: Could not initialize /proc/bb\n");
	}

	bb_buf_log= (bb_log_t *)(PAGE_OFFSET + RAM_SIZE -RSV_MEM_SIZE);


	if((bb_buf_log->rsv1[8]==0) && (bb_buf_log->rsv1[9]==0)
		&&(bb_buf_log->rsv1[10]==0) && (bb_buf_log->rsv1[11]==0)
		&&(bb_buf_log->rsv1[12]==0) && (bb_buf_log->rsv1[13]==0)
		&&(bb_buf_log->rsv1[14]==0) && (bb_buf_log->rsv1[15]==0))
	{ 	 
		bb_buf_log->log_area= ((bb_buf_log->reboot_count+1)%2);
	 }

	return rv;
}

/*重启信息记录和操作*/
static ssize_t user_file_output(struct file *filp,	 
													 char *buff,	
													 size_t len,	 
													 loff_t * off)
{
		
	/* 
	* We return 0 to indicate end of file, that we have
	* no more information. Otherwise, processes will
	* continue to read from us in an endless loop. 
	*/

	if((len >= USER_FILE_SIZE) || (len == 0))
	{
		printk("len %d is invalid !\n", len);
		return 0;
	}
	if (*off >= USER_FILE_SIZE) {
		printk("loff_t %d is invalid !\n", (int)*off);
		return 0; 			 
	}
	if (*off + len > USER_FILE_SIZE) {
		len = *off + len - USER_FILE_SIZE;
	}
	if (copy_to_user(buff, user_file_addr+(*off), len)!= 0) {
		printk("copy_to_user failed in user_file_output !\n");
		return 0;
	}
	*off = * off + len; 			 
	return len;
}

static ssize_t user_file_input(struct file *filp, const char *buff, size_t len, loff_t * off)
{
	if((len >= USER_FILE_SIZE) || (len == 0))
	{
		printk("len %d is invalid !\n", len);
		return 0;
	}
	if (*off >= USER_FILE_SIZE) {
		printk("loff_t %d is invalid !\n", (int)*off);
		return 0; 			 
	}
	if (*off + len > USER_FILE_SIZE) {
		len = *off + len - USER_FILE_SIZE;
	}
	if (copy_from_user(user_file_addr+(*off), buff, len) !=0 ){
		printk("copy_from_user failed in user_file_input !\n");
		return 0;
	} 
	*off = *off + len;
		
	return len;
}

static int user_file_open(struct inode *inode, struct file *file)
{
	try_module_get(THIS_MODULE);
	return 0;
}

static int user_file_close(struct inode *inode, struct file *file)
{
	module_put(THIS_MODULE);
	return 0; 							 
}

static struct file_operations File_Ops_4_user_file_Proc_File = {
	.read 	 = user_file_output,
	.write	 = user_file_input,
	.open 	 = user_file_open,
	.release = user_file_close,
};

static void userfile_buf_init(void)
{
	user_file_addr = (char *)KSEG1ADDR(USER_FILE_ADDR);
	add_reboot_rec = userfile_add_reboot_rec;
	return;
} 

void cleanup_user_file_put(void)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,26)
	remove_proc_entry(user_file_ENTRY_FILENAME, NULL);
#else
	remove_proc_entry(user_file_ENTRY_FILENAME, &proc_root);
#endif
}

int init_userfile(void)
{
	int rv = 0;
	user_file_Proc_File = create_proc_entry(user_file_ENTRY_FILENAME, 0644, NULL);
	if (user_file_Proc_File == NULL) {
		rv = -ENOMEM;
		cleanup_user_file_put();
		printk("Error: Could not initialize /proc/userfile\n");
	}
	else 
	{
		user_file_Proc_File->proc_fops = &File_Ops_4_user_file_Proc_File;
		user_file_Proc_File->mode = S_IFREG | S_IRUGO | S_IWUSR;
		user_file_Proc_File->uid = 0;
		user_file_Proc_File->gid = 0;
		user_file_Proc_File->size = 80;
	}
		
	return 0;
}

int userfile_read(char*buf, int len)
{	
	if (len > USER_FILE_SIZE)
	{
		len = USER_FILE_SIZE;
	}
	memcpy(buf, user_file_addr, len);
	return len;
}

int userfile_write(char*buf, int len)
{	
	if (len > USER_FILE_SIZE)
	{
		len = USER_FILE_SIZE;
	}
	memcpy(user_file_addr, buf, len);
	return len;
}

int userfile_add_reboot_rec(char* msg)
{
	T_RebootInfo* pstRebootInfo= NULL;
	int idx;

	//struct timespec cur_time= current_kernel_time();
	
	userfile_read((char*)&uf_info, sizeof(USR_FILE_INFO_T));
	
	idx = uf_info.ucRebootCount % MAX_REBOOT_RECORD_NUM;
	pstRebootInfo = &uf_info.stRebootInfo[idx];

	pstRebootInfo->iUptime = current_kernel_upseconds();
	strncpy(pstRebootInfo->acReason, msg, 128);
	//printk( "%d: %s\n", pstRebootInfo->iUptime, pstRebootInfo->acReason);
	userfile_write((char*)&uf_info, sizeof(USR_FILE_INFO_T));
	return 0;
}

static int black_box_init(void)
{	
    bb_buf_init();
    init_bb();
    userfile_buf_init();
    init_userfile();
    printk("init black box success ram size=0x%x,rsv size=0x%x\n",RAM_SIZE,RSV_MEM_SIZE);
    return 0;

}

static void black_box_cleanup(void)
{	
	cleanup_bb();
	cleanup_user_file_put();
	bb_log_char_hook = NULL;
	add_reboot_rec = NULL;

	return;
}

module_init(black_box_init)
module_exit(black_box_cleanup)

MODULE_LICENSE("GPL");

/*****************************************************************************************************/
/*End of File*/
