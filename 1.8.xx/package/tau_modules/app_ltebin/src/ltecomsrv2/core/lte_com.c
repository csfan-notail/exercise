/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ���������������޹�˾

 ******************************************************************************
  �� �� ��   : lte_com.c
  �� �� ��   : ����
  ��    ��   : liyoukun
  ��������   : 2015��3��3�����ڶ�
  ����޸�   :
  ��������   : lte com���ļ������ڳ�ʼ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��3��3�����ڶ�
    ��    ��   : liyoukun
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  
#include <getopt.h>


#include "lte_com.h"
#include "lte_log.h"

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
static struct option const long_options[] = 
{
    {"-m lte module type, for example, longcher, zte-micro, arccra!",  required_argument,   NULL,'m'},
    {"-l log level!",                                                  required_argument,   NULL,'l'},    
    {"-h help",                                                        no_argument,         NULL,'h'},
    {NULL,                  0,              NULL, 0},
};

LTE_PLATFORM_E g_enLtePlatformType = M_TOTAL_NUM; /* ��¼ʹ��ģ����ͺ� */
LTE_MODULE_OPS_T *g_pstLte_module_ops_t = NULL;

LTE_EVENT_T g_stLteEventT = {0};  /* �¼��� */
LTE_STATE_INFO_T g_stLteStateInfo = {0};    /* ��̬��ȡ��lte��Ϣ�б� */


/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
void usage(void)
{
    int iCount = 0;
    LTE_LOG(LTE_LOG_ERR, "Usage:");
    while (long_options[iCount].name != NULL)
    {
        LTE_LOG(LTE_LOG_ERR, "%s\n", long_options[iCount].name);
        iCount++;
    }
    return ;
}

LTE_RET_E get_parameter(int argc,char** args)
{
    int iIndex = 0 ;
    int iChar ;
    //while((iChar = getopt_long(argc, args, "a:p:s:d:i:c:r:", long_options, &iIndex)) != EOF)
    while((iChar = getopt_long(argc, args, "m:l:h:", long_options, &iIndex)) != EOF)
    {
        switch (iChar)
        {
            case 'm':
            {
                g_enLtePlatformType = atoi(optarg);
                break;
            }
            case 'l':
            {
                g_iLteLogLevel = atoi(optarg);    
                break;
            }
            case 'h':
            {
                usage();    
                return LTE_FAIL;
            }
            default:
            {
                usage();
                return LTE_FAIL;
            }
        }
    }
    
    return LTE_OK;
}

int main(int argc, char** args)
{
    LTE_RET_E enRet = LTE_OK;
    int       iRet = 0;
    char      acCom[32] = {0};
    int       iComFd = 0;
    /* ��ȡ������� */
    enRet = get_parameter(argc, args);
    if(LTE_OK != enRet)
    {
        LTE_LOG(LTE_LOG_ERR, "get param failed!");
        return -1;
    }

    /* ģ��USB com��ע�� */
    iRet = lte_usb_com_register(g_enLtePlatformType, &g_stLteEventT);
    if(iRet < 0)
    {
        LTE_LOG(LTE_LOG_ERR, "register lte usb com failed!");
        return -1;
    }
    
    /* libeventע�� */
    enRet = libevent_register(&g_stLteEventT);
    if(LTE_OK != enRet)
    {
        LTE_LOG(LTE_LOG_ERR, "register libevent failed!");
        return -1;
    }
    return 0;
}

