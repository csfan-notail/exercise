/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : tool.c
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/5/29
  Last Modified :
  Description   : 公共工具函数接口
  Function List :
  History       :
  1.Date        : 2013/5/29
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tool.h"
#include "ap_types.h"
#include "pal_log.h"

int strmac_to_mac(char *str_mac, unsigned char *mac)
{
	int i;
	unsigned char val1 = 0, val2 = 0;
	
	/*str_mac: xx:xx:xx:xx:xx:xx*/
	for(i=0; i<17; ++i)
	{
		switch(i%3)
		{
		case 0:	
			if((str_mac[i] >= 'a') && (str_mac[i] <= 'f')){
				val2 = str_mac[i] - 'a' + 0xA;
			}
			else if((str_mac[i] >= 'A') && (str_mac[i] <= 'F')){
				val2 = str_mac[i] - 'A' + 0xA;
			}
			else if((str_mac[i] >= '0') && (str_mac[i] <= '9')){
				val2 = str_mac[i] - '0';
			}
			else
				return -1;
			break;
		case 1:
			if((str_mac[i] >= 'a') && (str_mac[i] <= 'f')){
				val1 = str_mac[i] - 'a' + 0xA;
			}
			else if((str_mac[i] >= 'A') && (str_mac[i] <= 'F')){
				val1 = str_mac[i] - 'A' + 0xA;
			}
			else if((str_mac[i] >= '0') && (str_mac[i] <= '9')){
				val1 = str_mac[i] - '0';
			}
			else
				return -1;
			if(i<2)
				mac[0] = val1 + val2*16;
			else if(i<5)
				mac[1] = val1 + val2*16;
			else if(i<8)
				mac[2] = val1 + val2*16;
			else if(i<11)
				mac[3] = val1 + val2*16;
			else if(i<14)
				mac[4] = val1 + val2*16;
			else
				mac[5] = val1 + val2*16;
			break;
		case 2:
			if(str_mac[i] != ':')
				return -1;
			break;
		default:
			break;
		}
	}

	return 0; 
}


/*****************************************************************************
 Prototype    : string_mac_tools
 Description  : 获取MAC地址
              : dstmac is char srcmac[6]
                srcmac is format 02:02:02:02:02:02 or 02-02-02-02-02-02
 Input        : char *srcmac, char *dstmac
 Output       : None
 Return Value : int
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/5/29
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
int string_mac_tools(char *srcmac, char *dstmac)
{
    int iRet;
    int loop;
    int mac[6];

    if(NULL == srcmac || NULL == dstmac)
    {
        return ERROR;
    }

    if(17 != strlen(srcmac))
    {
        return ERROR;
    }

    iRet = 0;
    memset(mac, 0, 6);
    iRet = sscanf(srcmac, "%02x:%02x:%02x:%02x:%02x:%02x",
        &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
    if(6 != iRet)
    {
        iRet = sscanf(srcmac, "%02x-%02x-%02x-%02x-%02x-%02x",
            &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
        if(6 != iRet)
        {
            return ERROR;
        }
    }

    for(loop = 0; loop < 6; loop++)
    {
        dstmac[loop] = mac[loop];
    }

    return OK;
}

/*****************************************************************************
 Prototype    : string_ip_tools
 Description  : 获取Ip地址
 Input        : char *srcip  
                char *dstip  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/5/29
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
int string_ip_tools(char *srcip, char *dstip)
{
    int iRet;
    int loop;
    int ip[4];

    if(NULL == srcip || NULL == dstip)
    {
        return ERROR;
    }
    iRet = 0;
    iRet = sscanf(srcip, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);
    if(4 != iRet)
    {
        return ERROR;
    }
    for(loop = 0; loop < 4; loop++)
    {
        dstip[loop] = ip[loop];
    }

    return OK;
}

/*****************************************************************************
 Prototype    : array_string_to_int
 Description  : src:1,2,3,4,11,23 -> 1 2 3 4 11 23
 Input        : char *src  
                char *dst   
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/6/20
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
int array_string_to_int(char *src , char *dst, int *total)
{
    char *start, *end;
    char *buff;
    int flag;
    
    if(NULL == src || NULL == dst)
    {
        return ERROR;
    }
    buff = NULL;
    buff = (char *)malloc(strlen(src)+1);       /* modify by zhz 2013-12-27 */
    if(NULL == buff)
    {
        return ERROR;
    }
    memcpy(buff, src, strlen(src)+1);

    flag = 0;
    (*total) = 0;
    start = end = buff;
    while('\0' != *end)
    {
        flag = 1;
        if(*end != ',')
        {
            end++;
            continue;
        }
        else
        {
            *end = '\0';
            end++;
            *dst = atoi(start);
            dst++;
            start = end;
            (*total)++;
        }
    }
    if(1 == flag)
    {
        *dst = atoi(start);
        (*total)++;
    }
    if(NULL != buff)
    {
        free(buff);
    }
    return OK;
}

/*****************************************************************************
 Prototype    : array_string_to_integer
 Description  : 
 Input        : char *src   
                int *dst    
                int *total  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/7/16
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
int array_string_to_integer(char *src , int *dst, int *total)
{
    char *start, *end;
    char *buff;
    int flag;
    
    if(NULL == src || NULL == dst)
    {
        return ERROR;
    }
    buff = NULL;
    buff = (char *)malloc(strlen(src)+1);
    if(NULL == buff)
    {
        return ERROR;
    }
    memcpy(buff, src, strlen(src)+1);

    flag = 0;
    (*total) = 0;
    start = end = buff;
    while('\0' != *end)
    {
        flag = 1;
        if(*end != ',')
        {
            end++;
            continue;
        }
        else
        {
            *end = '\0';
            end++;
            *dst = atoi(start);
            dst++;
            start = end;
            (*total)++;
        }
    }
    if(1 == flag)
    {
        *dst = atoi(start);
        (*total)++;
    }
    if(NULL != buff)
    {
        free(buff);
    }
    return OK;
}

/*****************************************************************************
 Prototype    : array_string_del_someone
 Description  : 删除某个字符串，并按顺序返回
 Input        : char *src  
                int id     
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/6/24
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
int array_string_del_someone(char *src, int id)
{
    char tmp[16];
    char *list;
    int len;
    int total, loop, flag;

    if(NULL == src)
    {
        return ERROR;
    }
    len = strlen(src);
    list = NULL;
    list = (char *)malloc(len);
    if(NULL == list)
    {
        return ERROR;
    }
    array_string_to_int(src, list, &total);
    flag = 0;
    memset(src, 0, len);
    for(loop = 0; loop < total; loop++)
    {
        if(id != list[loop])
        {
            sprintf(tmp, "%d,", list[loop]);
            strcat(src, tmp);
        }
    }
    src[strlen(src)-1] = '\0';
    if(NULL != list)
    {
        free(list);
    }
    return OK;
}

/*****************************************************************************
 Prototype    : array_string_add_someone
 Description  : 添加某个字符
 Input        : char *src  
                int id     
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/6/24
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
int array_string_add_someone(char *src, int id)
{
    char tmp[16];
    char *list;
    int len;
    int total, loop, flag;

    if(NULL == src)
    {
        return ERROR;
    }
    len = strlen(src);
    list = NULL;
    list = (char *)malloc(len);
    if(NULL == list)
    {
        return ERROR;
    }
    array_string_to_int(src, list, &total);
    flag = 0;
    memset(src, 0, len);
    if(0 == total)
    {
        sprintf(tmp, "%d,", id);
        strcat(src, tmp);
    }
    else
    {
        for(loop = 0; loop < total; loop++)
        {
            if(id < list[loop] && 0 == flag)
            {
                sprintf(tmp, "%d,%d,", id, list[loop]);
                flag = 1;
            }
            else
            {
                sprintf(tmp, "%d,", list[loop]);
            }
            strcat(src, tmp);
        }
        if(0 == flag)
        {
            sprintf(tmp, "%d,", id);
            strcat(src, tmp);
        }
    }
    src[strlen(src)-1] = '\0';
    if(NULL != list)
    {
        free(list);
    }
    return OK;
}

/*****************************************************************************
 Prototype    : pwd_encryption
 Description  : 账户密码加密
 Input        : char *str       
                int key_format 
                int len 字符串的长度
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/7/4
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
int pwd_encryption(char *str, int key_format, int len)
{
    int loop;
    
    if(NULL == str)
    {
        return ERROR;
    }
    for(loop = 0; loop < len; loop++)
    {
        *(str+loop) += key_format;
    }
    return OK;
}

/*****************************************************************************
 Prototype    : pwd_decryption
 Description  : 
 Input        : char *str       
                int key_format  
                int len         
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/7/4
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
int pwd_decryption(char *str, int key_format, int len)
{
    int loop;
    
    if(NULL == str)
    {
        return ERROR;
    }
    for(loop = 0; loop < len; loop++)
    {
        *(str+loop) -= key_format;
    }
    return OK;
}

static int Len_string(char * str)
{
	int i = 0;
	while (*(str+i))
	{
		i++;
	}
	return i;
}

/*****************************************************************************
 Prototype    : ReplaceToString
 Description  : 
 Input        : char * source  
                char *S_str    
                char * D_str   
 Output       : None
 Return Value : char
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/8/26
    Author       : Zhou Shouya
    Modification : Created function

*****************************************************************************/
char *ReplaceToString(char * source, char *S_str, char * D_str)   
{
	int len_source;
	int len_S_str;
	int len_D_str;
	char * s = source;
	char * temp;
	char * ss;
	char * dd;
	int mark1;
	int mark2 = 0;
	int i = 0,j = 0,k,start = 0,end;	
	while (*s)
	{
		ss = S_str;
		dd = D_str;
		temp = S_str;
		mark1 = 0;
		i = 0;
		j = 0;
		end = 0;
		len_source = Len_string(source);
		len_S_str = Len_string(S_str);
		len_D_str = Len_string(D_str);
		while (1)
		{  
		    while ((*(s + i) == *(temp + j)) && (*(s + i) && *(temp + j)))
			{
				i++;
				j++;
				continue;
			}
			if (j == len_S_str)
			{
				mark2 = 1;
				mark1 = 1;
				break;
			}
			if (*(s+i) == '\0')	break;
			s++;
			start++;
			i = 0;
			j = 0;
		}
		if( mark1 == 0 && mark2 == 0)
		{
			break;
		}
		if (mark1 == 0 && mark2 != 0) break;

		end = start + len_S_str;

		if (len_S_str == len_D_str)
		{
			while (*dd)
			{
				*(source+start) = *dd;
				start++;
				dd++;
			}
		}
		if (len_S_str > len_D_str)
		{
			for (k = end; source[k]; k++)
			{
				source[k-(len_S_str-len_D_str)] = source[k];
			}
			source[k-(len_S_str-len_D_str)] = '\0';
			while (*dd)
			{
				*(source+start) = *dd;
				start++; 
				dd++;
			}
		}
		if (len_S_str < len_D_str)
		{
			for (k = len_source-1; k >= end; k--)
			{
				source[k+(len_D_str-len_S_str)] = source[k];
			}
			source[len_source+(len_D_str-len_S_str)] = '\0';
			while (*dd)
			{
				*(source+start) = *dd;
				start++;
				dd++;
			}
		}
		s = s+len_D_str;
	}

	return source;
}

void string_foramt_rates(char *rate_list, int length)
{
    char *buff, *start, *end;
    char temp[32];
    int len;
    
    if(NULL == rate_list)
    {
        return;
    }

    len = strlen(rate_list);
    if(len <= 0)
    {
        return;
    }

    buff = NULL;
    buff = (char *)malloc(len+64);
    if(NULL == buff)
    {
        return;
    }
    memset(buff, 0, len+64);

    start = rate_list;
    end = start;
    while('\0' != *end)
    {
        if(',' == *end)
        {
            *end = '\0';
            sprintf(temp, "'%s',", start);
            strcat(buff, temp);
            start=++end;
        }
        end++;
    }
    sprintf(temp, "'%s'", start);
    strcat(buff, temp);
    memset(rate_list, 0, length);
    memcpy(rate_list, buff, strlen(buff));
	free(buff);
    return;
}

/*读取执行命令的返回值，返回值为一行字符串 */
char *cmd_return_str(char *cmd)
{
    FILE *pf;
    
    static char buf[128];
    char *tmp = buf;

    pf = popen(cmd, "r");
    if(NULL == pf)
        return 0;
    memset(buf, 0, sizeof(buf));

    fgets(buf, sizeof(buf), pf);
    pclose(pf);
    while('\n' != *(++tmp));
    *tmp = '\0';
    return buf;
}



//分割字符串
char** split_str(char * str, char sym)
{
    const int ini = 10;
    int size = ini;
    char ** str_table;
    int line = 0;
    if (!str || !*str) return 0;

    str_table = (char **)malloc(sizeof(char*) * size);
    while (1)
    {
        if (line >= size - 1)
        {
            size *= 2;
            str_table = (char **)realloc(str_table, sizeof(char*) * size);
        }
        str_table[line++] = str;
        while (*str && *str != sym)
        {
            str++;
        }
        if (!*str)
        {
            break;
        }
        else
        {
            *str = '\0';
            while (*++str == sym);
        }
    }
    str_table[line] = 0;
    return str_table;
}

/*spilt string  */
int string_spilt(char *buf,char **str)
{
	int i=0,line=0,row=0;
	int len=0;
	len=strlen(buf);

	while('\0' !=buf[i] )
	{
		if(' ' != buf[i])
		{
			gw_log(LOG_DEBUG,"i %d  line %d row : %d",i,line,row);
			str[line][row] = buf[i];
			++row;
		}
		else
		{
			gw_log(LOG_DEBUG,"i %d",i);
			if(i+1<=len){
				gw_log(LOG_DEBUG,"i %d  line %d",i,line);
				if(' ' !=buf[i+1]){
				str[line][row]='\0';
				gw_log(LOG_DEBUG,"str: %s",str[line]);
				++line;
				row=0;	
				}
			}
		}
		i++;  
	}
	return line;
}


/*xx:xx:xx ----> ms*/
void time_convert_1(char *timefrom,unsigned long  timeto)
{	
	char**str;
	int str_num;
	int i;
	int hour=0,min=0,sec=0;
	gw_log(LOG_DEBUG,"timefrom: %s",timefrom);
	str=split_str(timefrom,':');
	if(str)
	{
		for(i=0;str[i];i++)
		{
			gw_log(LOG_DEBUG,"str[%d]: %s",i,str[i]);
		}
		if(i==1)
		{
			sec=atoi(str[0]);
			timeto = sec*100;
		}
		else if(i==2)
		{
			sec=atoi(str[1]);
			min=atoi(str[0]);
			timeto = (min*60+ sec)*100;
		}
		else if(i==3)
		{	
			sec=atoi(str[2]);
			min=atoi(str[1]);
			hour=atoi(str[0]);
			timeto = (hour*60*60+ min*60+ sec)*100;
			
		}
		gw_log(LOG_DEBUG,"timeto: %ld",timeto);
		free(str);
	}
	return ;
}


