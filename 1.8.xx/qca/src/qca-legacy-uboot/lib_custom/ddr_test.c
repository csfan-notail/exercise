/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : ddr_test.c
  版 本 号   : 初稿
  作    者   : 张平
  生成日期   : 2012年7月27日 星期五
  最近修改   :
  功能描述   : 内存稳定性测试
  函数列表   :
  修改历史   :
  1.日    期   : 2012年7月27日 星期五
    作    者   : 张平
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <common.h>
#include <command.h>

#include <asm/addrspace.h>
#include <watchdog.h>
/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
uint uiBadCount = 0; /* 坏块个数 */
const char ucTip[] = " command or paramater invalid.\n";
/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
#define DDR_TEST_START_ADDR 0x00000000
#define DDR_TEST_END_ADDR   (gd->ram_size)  //0x04000000 /* DDR总大小是64M */

#define DDR_TEST_WAITING_TIME 2
#define DDR_TEST_PROMPT "press '%c' to ddr test cmd in %d seconds."
#define DDR_TEST_KEY 'T'

#define READ_BUF_LEN	    100
#define NARG				10
#define DATA_LEN		    40
#define LINE_SHOW_LEN       6

DECLARE_GLOBAL_DATA_PTR;

#define	endtick(seconds) (get_ticks() + (uint64_t)(seconds) * get_tbclk())
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

int inline check_key_q(void)
{
    if(tstc())
    {
        if('q' == getc())
        {
            printf("\ninterrupt:key-'q'.\n");
            return 1;
        }
        return 0;
    }
    return 0;
}
void write_ddr_memory(volatile uint addr, uint data, uint nbyte,uint count)
{
    uint uiCounti,uiAddrTem;
    u8 flag=0;
    
	if((addr < DDR_TEST_START_ADDR) || ((addr + nbyte * count) > DDR_TEST_END_ADDR))
	{
		if(count < DDR_TEST_END_ADDR)
        {
            printf("parameter invalid---address. \n");
		    return ;
        }
        else
        {
            printf("Circulation write ddr data from %08x to %08x\n",addr,DDR_TEST_END_ADDR-1);
            count = DDR_TEST_END_ADDR;
            uiAddrTem = addr;
            flag=1;
        }
	}
    
    printf("write:");	
    for(uiCounti=0;uiCounti<count;uiCounti++)
    {
        /* printf("%08x",addr);	 2012-9-4 add by zhangping */
        if(1 == nbyte)
        {
           *(volatile u8*)addr = (u8)data;
        }
        else if(2 == nbyte)
        {
            *(volatile u16*)addr = (u16)data;
        }
        else
        {
            *(volatile u32*)addr = (u32)data;
        }
        addr += nbyte;
        if(((count-1)==uiCounti) && (1==flag)) /* 无限循环 */
        {
            uiCounti = 0;
            addr = uiAddrTem;
            printf("write from:%08x to %08x\n",addr,DDR_TEST_END_ADDR-1);
        }
        if(check_key_q())
        {
            return ;
        }
        /* printf("\b\b\b\b\b\b\b\b"); 2012-9-4 add by zhangping */
    }
    printf("write done.\n");
	return;
}


void read_ddr_memory(volatile uint addr,uint nbyte,uint count,uint linelen)
{
    uint counti,uiAddrTem;
    u8 flag=0;
    
    if((addr < DDR_TEST_START_ADDR) || ((addr + nbyte * count) > DDR_TEST_END_ADDR))
	{
        if(count < DDR_TEST_END_ADDR)
        {
            printf("parameter invalid---address. \n");
		    return ;
        }
        else
        {
            printf("Circulation read ddr data from %08x to %08x\n",DDR_TEST_START_ADDR,DDR_TEST_END_ADDR-1);
            count = DDR_TEST_END_ADDR;
            uiAddrTem = addr;
            flag=1;
        }
		
	}

    for(counti=0;counti<count;counti++)
    {
        if(1 == nbyte)
        {
           printf("%08x:%02x  ",addr,*(volatile u8*)addr); 
        }
        else if(2 == nbyte)
        {
           printf("%08x:%04x  ",addr,*(volatile u16*)addr);
        }
        else
        {
           printf("%08x:%08x  ",addr,*(volatile u32*)addr);
        }
        
        if(!((counti+1) % linelen))
        {
            printf("\n");
        }
        addr += nbyte;
        if(((count-1)==counti) && (1==flag)) /* 无限循环 */
        {
            counti = 0;
            addr = uiAddrTem;
            printf("read from:%08x to %08x\n",addr,DDR_TEST_END_ADDR-1);
        }
        if(check_key_q())
        {
            return ;
        }
    }
    printf("\n");
	return ;
}

int inline mt_write_ddr(volatile uint addr, uint data, uint nbyte)
{
    switch ( nbyte )
    {
        case 1 :
            *(volatile u8*)addr = (u8)data;
            break;
        case 2 :
            *(volatile u16*)addr = (u16)data;
            break;
        case 4 :
            *(volatile u32*)addr = (u32)data;
            break;
        default:
            return -1;
    }
    return 0;
}

uint inline mt_read_cmp_ddr(volatile uint addr, uint data, uint nbyte)
{
    uint tem;
    switch ( nbyte )
    {
        case 1 :
            tem = *(volatile u8*)addr;
            if(tem != data)
            {
                uiBadCount++;
                printf("%08x:%02x!=%02x,data is bad!\n",addr,tem,data); 
            }
            break;
        case 2 :
            tem = *(volatile u16*)addr;
            if(tem != data)
            {
                uiBadCount++;
                printf("%08x:%04x!=%04x,data is bad!\n",addr,tem,data); 
            }
            break;
        case 4 :
            tem = *(volatile u32*)addr;
            if(tem != data)
            {
                uiBadCount++;
                printf("%08x:%08x!=%08x,data is bad!\n",addr,tem,data); 
            }
            break;
        default:
            return 1;
    }
    
    return 0;
}


uint ddr_test_get_datasize(char* arg, int default_size)
{
	/* Check for a size specification .b, .w or .l.
	 */
	int len = strlen(arg);
    if((len > 5)|| (len == 5 && arg[2] != 'r'))
    {
        return 0;
    }
	else if(len > 2 && len < 6 && arg[len-2] == '.') {
		switch(arg[len-1]) {
		case 'b':
			return 1;
		case 'w':
			return 2;
		case 'l':
			return 4;
		default:
			return 0;
		}
	}
    
	return default_size;
}

int ddr_test_read_line(char*buf, int len)
{
	char c;
	char*p = buf;
	
	while(1)
	{
		c = getc();
		if ((p - buf + 1)== len)
		{
			*p ='\0' ;
			return len;
		}
		else if('\n'==c || '\r'==c )
		{
			puts ("\r\n");
			*p ='\0' ;
			return (p - buf);
		}
		else if ('\b' == c)
		{
			if (p != buf)
			{
				puts ("\b \b");
				p--;
			}
		}
		else
		{
			*p = c;
			p++;
			putc(c);
		}
	}
}

int  ddr_test_cmd_parse(char*line, char*argv[], int num)
{
	int argc;
	char*cp;
	int counter = 0;
	
	for (argc = 0; argc < num; argc++)
	{
		argv[argc] = NULL;
	}

	/*Remove blank character.*/
	while (*line == ' ' || *line == '\t')
	{
		line++;
	}
	
	/* analyze input command */
	for (argc = 0; argc < num;)
	{
		/* Skip leading white space */
		while (*line == ' ' || *line == '\t') {
			line++;
		}
		if (*line == '\0') {
			break;
		}

		/*Save*/
		argv[argc++] = line;
		counter++;
		
		/*Find the end of one arg.*/
		for (cp = line; *cp; cp++) 
		{
			if (*cp == ' ' || *cp == '\t') 
			{
				break;
			}
		}

		if (*cp == '\0') 
		{
			break;
		}

		/*Signal the end.*/
		*cp++ = '\0';

		/*Next turn.*/
		line = cp;
	}

	return counter;
}

int ddr_test_find_cmd(char *buf,char *argv[],uint *buflen)
{
    int argc;
    *buflen = ddr_test_read_line(buf, READ_BUF_LEN);
    if(!(*buflen))
    {
        return 0;
    }
	if (0 == buf[0])
	{
		return -1;
	}
	
	argc = ddr_test_cmd_parse(buf, argv, NARG);
    return argc;
}

void help(void)
{
	printf("=================Help information for commands=================\n");
    printf("?   -  alias for 'help'\n");
	printf("mr  -  mr[.b .w .l] addr [len]\n       read DDR memory.\n");
    printf("mw  -  mw[.b .w .l] addr data [len]\n       write DDR memory.\n");
    printf("mf  -  mf[.b .w .l] addr time count flag\n       dataline circulation write and compare DDR memory.\n");
    printf("mfr - mfr[.b .w .l] addr time count flag\n      dataline circulation compare DDR memory.\n");
    printf("mt  -  mt[.b .w .l] addr len time count data1 [data2 ...]\n       write and compare DDR memory.\n");
    printf("mtr - mtr[.b .w .l] addr len time count data1 [data2 ...]\n      compare DDR memory.\n");
    printf("ms  -  ms[.b .w .l] addr len time count gap data1 [data2 ...]\n       write and compare DDR memory with a gap.\n");
    printf("msr - msr[.b .w .l] addr len time count gap data1 [data2 ...]\n      compare DDR memory with a gap.\n");
}


/* ================命令处理函数==start============================= */
void ddr_test_mr(int argc,char *argv[])
{
    uint ddr_addr;
    uint data_count;
    uint size;
    
    size = ddr_test_get_datasize(argv[0],2);
   // printf("size:%d %s\n",size,argv[0]); /* tem add */
    if((!size) || (argc<2))
    {
        printf(ucTip);
        return ;
    }
    ddr_addr = simple_strtoul(argv[1],NULL,16);
    data_count = 1;
    if (argc > 2)
    {
        data_count = simple_strtoul(argv[2], NULL, 16);
    }
    if(0 == data_count)
    {
        printf("parameters error--len.\n");
        return ;
    }
	read_ddr_memory(ddr_addr,size,data_count,LINE_SHOW_LEN);		

    return ;
}

void ddr_test_mw(int argc,char *argv[])
{
    uint ddr_data;
    uint ddr_addr;
    uint data_count;
    uint size;
    
    size = ddr_test_get_datasize(argv[0],2);
    if((!size) || (argc<3))
    {
        printf(ucTip);
        return ;
    }
    ddr_addr = simple_strtoul(argv[1],NULL,16);
    ddr_data = simple_strtoul(argv[2],NULL,16);
    data_count = 1;
    if (argc > 3)
    {
        data_count = simple_strtoul(argv[3], NULL, 16);
    }
    if(0 == data_count)
    {
        printf("parameters error--len.\n");
        return ;
    }
	write_ddr_memory(ddr_addr,ddr_data,size,data_count);		

    return ;
}

void ddr_test_mxx(int argc,char *argv[])
{
    uint ddr_data[NARG];
    uint ddr_addr,ddr_tem;
    uint data_count,data_gap=0,p_count;
    uint wait_time;
    uint loop_count;
    uint size,i,loopi,datalen;
    
    size = ddr_test_get_datasize(argv[0],2);
    p_count = 7 - (argv[0][1] - 's');
   // printf("size:%d pcount:%d  %s \n",size,p_count,argv[0]); /* tem add */
    if((!size) || (argc < p_count))
    {
        printf(ucTip);
        return ;
    }
    
    argc -= p_count;
    argc += 1;
   // argv[0][3]='\0';
    
    ddr_addr = simple_strtoul(argv[1],NULL,16);
    ddr_tem = ddr_addr;
    data_count = simple_strtoul(argv[2],NULL,16);
    
    if((ddr_addr < DDR_TEST_START_ADDR) || ((ddr_addr + size * data_count) > DDR_TEST_END_ADDR))
	{
		printf("parameter invalid---address. \n");
		return;
	}
    if(0 == data_count)
    {
       printf("parameters error--len.\n");
       return ;
    }
    datalen=ddr_tem+size*data_count; /* ms命令长度限制 */
    wait_time = simple_strtoul(argv[3],NULL,10);
    loop_count = simple_strtoul(argv[4],NULL,16);
    
    if('s' == argv[0][1])
    {
        data_gap = simple_strtoul(argv[5],NULL,16);
    }
    
    if(argc > NARG)
    {
        printf("parameters error--data.\n");
        return ; 
    }
    for(i=0;i<argc;i++)
    {
        ddr_data[i] = simple_strtoul(argv[i+p_count-1],NULL,16);
    }
    
    for(loopi=0;loopi<loop_count;loopi++)
    {
        if('r' != argv[0][2])
        {
            printf("The %d times:writing...\n",(loopi+1));
            for(i=0;i<data_count;i++)
            {
                if(('s' == argv[0][1]) && (0 != i) && (0 == i%argc))
                {
                   ddr_addr+=(data_gap*size);
                }
                if((ddr_addr+size) > DDR_TEST_END_ADDR || (ddr_addr+size) > datalen)
                { 
                   break;
                }
                /* printf("%08x",ddr_addr); 2012-9-4 add by zhangping */
                mt_write_ddr(ddr_addr,ddr_data[(i%argc)],size);
                ddr_addr+=size;
                if(check_key_q())
                {
                    return ;
                }
                /* printf("\b\b\b\b\b\b\b\b"); 2012-9-4 add by zhangping */
            }
            printf("\n");
            if(0 != wait_time)
            {
                printf("The %d times:write over,read all the data and compare after %d ms.\n",(loopi+1),wait_time);
        	    udelay(wait_time * 1000);
                ddr_addr = ddr_tem;
            }
            else
            {
                printf("write over,you can read all the data and compare in any time.\n");
                return ;
            }
            
        }
        
        uiBadCount = 0;
        printf("The %d times:reading and comparing...\n",(loopi+1));
        for(i=0;i<data_count;i++)
        {	
            if(('s' == argv[0][1]) && (0 != i) && (0 == i%argc))
            {
               ddr_addr+=(data_gap*size);
            }
            if((ddr_addr+size) > DDR_TEST_END_ADDR || (ddr_addr+size) > datalen)
            {
               break;
            }
            /* printf("%08x",ddr_addr); 2012-9-4 add by zhangping */
            /* printf("\b\b\b\b\b\b\b\b"); 2012-9-4 add by zhangping */
            mt_read_cmp_ddr(ddr_addr,ddr_data[(i%argc)],size);	
            ddr_addr+=size;
            if(check_key_q())
            {
                return ;
            }
        }
        printf("\n");
        printf("The %d times:read and compare over,bad data count:%d\n",(loopi+1),uiBadCount);
        if('r' == argv[0][2])
        {
           printf("read all the data and compare after %d ms.\n",wait_time);
           udelay(wait_time * 1000);
        }
        ddr_addr = ddr_tem; 
        uiBadCount = 0;
    }
    uiBadCount = 0;
    return ;
}
uint loop_left_shift(uint value,uint nbit,uint nbyte,u8 dataflag)
{
    uint tem;

    tem = value >> (nbyte-nbit);
    value <<= nbit;
    value |= tem;
    if(dataflag)
    {
       return value; 
    }
    else
    {
        tem = 2;
        tem = (~value)&((tem<<(nbyte-1))-1);
        return tem;
    }
}
void ddr_test_mf(int argc,char *argv[])
{
    uint ddr_data[DATA_LEN];
    uint ddr_addr,ddr_tem,ddr_datatem;
    uint data_count;
    u8 dataflag;
    uint wait_time;
    uint loop_count;
    uint size,i,loopi;

    size = ddr_test_get_datasize(argv[0],2);
   // printf("size:%d %s \n",size,argv[0]); /* tem add */
    if((!size) || (argc < 5))
    {
        printf(ucTip);
        return ;
    }
    
    ddr_addr = simple_strtoul(argv[1],NULL,16);
    ddr_tem = ddr_addr;
    data_count = 8*size;
    if((ddr_addr < DDR_TEST_START_ADDR) || ((ddr_addr + size * data_count) > DDR_TEST_END_ADDR))
	{
		printf("parameter invalid---address. \n");
		return;
	}
    wait_time = simple_strtoul(argv[2],NULL,10);
    loop_count = simple_strtoul(argv[3],NULL,16);
    dataflag = simple_strtoul(argv[4],NULL,16);
    //printf("wt:%d count:%d flag:%d %s\n",wait_time,loop_count,dataflag,argv[3]); /* tem add */
    if((1!=dataflag) && (0!=dataflag))
    {
        printf(ucTip);
    }
    ddr_data[0] = 0;
    
    ddr_datatem = 0x00000001; 
    
    for(i=0;i<data_count;i++)
    {
        ddr_data[i+1] = loop_left_shift(ddr_datatem,i,data_count,dataflag);
        //printf("%x ",ddr_data[i+1]);/* tem add */
    }
    
    uiBadCount = 0;
    //printf("\n1count:%d \n",loop_count); /* tem add */
    for(loopi=0;loopi<loop_count;loopi++)
    {
        if('r' != argv[0][2])
        {
            printf("The %d times:writing...\n",(loopi+1));
            for(i=0;i<data_count;i++)
            {
                mt_write_ddr(ddr_addr,ddr_data[(i%(data_count+1))],size);
                ddr_addr+=size;
                if(check_key_q())
                {
                    return ;
                }
            }
            if(0 != wait_time)
            {
                printf("The %d times:write over,read all the data and compare after %d ms.\n",(loopi+1),wait_time);
        	    udelay(wait_time * 1000);
                ddr_addr = ddr_tem;
            }
            else
            {
                printf("write over,you can read all the data and compare in any time.\n");
                return ;
            }
            
        }
        
        printf("The %d times:reading and comparing...\n",(loopi+1));
        for(i=0;i<data_count;i++)
        {	
            mt_read_cmp_ddr(ddr_addr,ddr_data[(i%(data_count+1))],size);	
            ddr_addr+=size;
            if(check_key_q())
            {
                return ;
            }
        }
        printf("The %d times:read and compare over,bad data count:%d\n",(loopi+1),uiBadCount);
        if('r' == argv[0][2])
        {
           printf("read all the data and compare after %d ms.\n",wait_time);
           udelay(wait_time * 1000);
        }
        ddr_addr = ddr_tem; 
        uiBadCount = 0;
    }
    uiBadCount = 0;
    return ;
}
/* ================命令处理函数==end============================= */


void ddr_test_cmd_line(void)
{
    char buf[READ_BUF_LEN];
	char*argv[NARG];
	int argc;
    uint buflen;  

    /*进入命令行后，关闭看门狗*/
    WATCHDOG_CLOSE();

    printf("\n");
    for(;;)
    {
        printf("cmd #");
        
        argc = ddr_test_find_cmd(buf,argv,&buflen);

		/*命令提示*/
		if (!strcmp(argv[0], "help") || !strcmp(argv[0], "?"))
		{
			help();
		}
        else if('m' == argv[0][0] && 'r' == argv[0][1])
        {
            ddr_test_mr(argc,argv);
        }
        else if('m' == argv[0][0] && 'w' == argv[0][1])
        {
            ddr_test_mw(argc,argv);
        }
        else if('m' == argv[0][0] && ('t' == argv[0][1] || 's' == argv[0][1]))
        {
            ddr_test_mxx(argc,argv);
        }
        else if('m' == argv[0][0] && 'f' == argv[0][1])
        {
            ddr_test_mf(argc,argv);
        } 
        else if ('q' == argv[0][0])
		{
			break;
		}
        else
		{
            if(argc != 0)
            {
               printf(ucTip);
            }
		}
    }
}


void ddr_test_waiting_key(void)
{
    uchar ucEnterCmd = 0;
    uint64_t endtime = endtick(DDR_TEST_WAITING_TIME);
	
	printf(DDR_TEST_PROMPT,DDR_TEST_KEY,DDR_TEST_WAITING_TIME);

	/*等待输入*/
	while(get_ticks() < endtime)
	{
		if(tstc())
		{
			if(getc() == DDR_TEST_KEY)
			{
				ucEnterCmd = 1;
                break;
			}
		}
	}
    
    if(ucEnterCmd)
    {
        ddr_test_cmd_line();
    }
    else {
        putc('\n');
    }
    

	/*开启看门狗*/
    WATCHDOG_RESET();
	return;
	
}

