/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : gpioex_ioctl.c
  �� �� ��   : ����
  ��    ��   : Zhong Huaqing
  ��������   : 2012��8��15�� ������
  ����޸�   :
  ��������   : GPIOEXģ��IO�ӿڣ����ں��û�̬���н���.
               ˵��: ���ļ��У���ʵ���κ�ʵ���ԵĹ��ܣ�����Ϊһ������ͨ����
  �����б�   :
  
  �޸���ʷ   :
  1.��    ��   : 2012��8��15�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "gpioex_main.h"

#include "gpioex_ioctl.h"
#include "gpioex_ops.h"
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

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                       *
 *----------------------------------------------*/
AP_ERROR_CODE_E gpioex_ioctl_get_version(GPIOEX_IOCTL_VERSION_T *pstVersion)
{
    strncpy(pstVersion->acModVersion, GPIOEX_MOD_VERSION, GPIOEX_VERSION_SIZE);
    strncpy(pstVersion->acModDepVersion, GPIOEX_MOD_DEP_VERSION, GPIOEX_VERSION_SIZE);
    return AP_E_NONE;
}


LOCAL AP_ERROR_CODE_E gpioex_ioctl_set_debug_level(GPIOEX_IOCTL_DEBUG_T *pstDebug)
{
    if (NULL == pstDebug)
    {
        return AP_E_PARAM;
    }
    return gpioex_set_debugLevel(pstDebug->uiDebugLevel);
}

LOCAL AP_ERROR_CODE_E gpioex_ioctl_get_debug_level(GPIOEX_IOCTL_DEBUG_T *pstDebug)
{
    if (NULL == pstDebug)
    {
        return AP_E_PARAM;
    }
    pstDebug->uiDebugLevel = gpioex_get_debugLevel();
    return AP_E_NONE;
}

LOCAL AP_ERROR_CODE_E gpioex_ioctl_mode_all_set(GPIOEX_IOCTL_MODE_ALL_T *pstMode)
{
    if (NULL == pstMode)
    {
        return AP_E_PARAM;
    }
    return gpioex_mode_all_set(pstMode->uiValue);
}

LOCAL AP_ERROR_CODE_E gpioex_ioctl_mode_all_get(GPIOEX_IOCTL_MODE_ALL_T *pstMode)
{
    if (NULL == pstMode)
    {
        return AP_E_PARAM;
    }
    return gpioex_mode_all_get(&pstMode->uiValue);
}
LOCAL AP_ERROR_CODE_E gpioex_ioctl_mode_set(GPIOEX_IOCTL_MODE_T *pstMode)
{
    if (NULL == pstMode)
    {
        return AP_E_PARAM;
    }
    return gpioex_mode_set(pstMode->uiNum,pstMode->uiValue);
}
LOCAL AP_ERROR_CODE_E gpioex_ioctl_mode_get(GPIOEX_IOCTL_MODE_T *pstMode)
{
    if (NULL == pstMode)
    {
        return AP_E_PARAM;
    }
    return gpioex_mode_get(pstMode->uiNum,&pstMode->uiValue);
}

LOCAL AP_ERROR_CODE_E gpioex_ioctl_state_all_set(GPIOEX_IOCTL_STATE_ALL_T *pstState)
{
    if (NULL == pstState)
    {
        return AP_E_PARAM;
    }
    return gpioex_state_all_set(pstState->uiValue);
}
LOCAL AP_ERROR_CODE_E gpioex_ioctl_state_all_get(GPIOEX_IOCTL_STATE_ALL_T *pstState)
{
    if (NULL == pstState)
    {
        return AP_E_PARAM;
    }
    return gpioex_state_all_get(&pstState->uiValue);
}
LOCAL AP_ERROR_CODE_E gpioex_ioctl_state_set(GPIOEX_IOCTL_STATE_T *pstState)
{
    if (NULL == pstState)
    {
        return AP_E_PARAM;
    }
    return gpioex_state_set(pstState->uiNum,pstState->uiValue);
}

LOCAL AP_ERROR_CODE_E gpioex_ioctl_state_get(GPIOEX_IOCTL_STATE_T *pstState)
{
    if (NULL == pstState)
    {
        return AP_E_PARAM;
    }
    return gpioex_state_get(pstState->uiNum,&pstState->uiValue);
}

/*****************************************************************************
 �� �� ��  : gpioex_ioctl_handler
 ��������  : IO����ͳһ�ӿ�
 �������  : void  *pstArg  --�û�̬�ռ��ָ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��15�� ������
    ��    ��   : Zhong Huaqing
    �޸�����   : �����ɺ���

*****************************************************************************/
AP_ERROR_CODE_E gpioex_ioctl_handler(void *pstArg)
{
	LOCAL GPIOEX_IOCTL_T stIoctl;
	BOOL bGetOps = FALSE; /*ȡֵΪ��ʱ����ʾ��Ҫ���û�̬��������*/
	AP_ERROR_CODE_E enRet = AP_E_NONE;
	
    if (NULL == pstArg)
    {
        return AP_E_PARAM;
    }
    
	if (copy_from_user(&stIoctl, pstArg, sizeof(GPIOEX_IOCTL_T)))
	{
	    GPIOEX_LOG(GPIOEX_LOG_ERR, "Copy from user failed.\n");
		return AP_E_DEFAULT_FAIL;
	}
//	GPIOEX_LOG(GPIOEX_LOG_DEBUG, "stIoctl.enCmd = %d\n", stIoctl.enCmd);
	
	
	switch (stIoctl.enCmd)
	{
	    /*��ֲʱ����������������Ҫ����*/
        case GPIOEX_IOCTL_CMD_GET_VERSION: 
            enRet = gpioex_ioctl_get_version(&stIoctl.unData.stVersion);
            bGetOps = TRUE; 
            break;
            
	    case GPIOEX_IOCTL_CMD_SET_DEBUG_LEVEL:
	        enRet = gpioex_ioctl_set_debug_level(&stIoctl.unData.stDebug); 
	        break;
	    case GPIOEX_IOCTL_CMD_GET_DEBUG_LEVEL:
	        enRet = gpioex_ioctl_get_debug_level(&stIoctl.unData.stDebug); 
	        bGetOps = TRUE;
	        break;
	        
	    case GPIOEX_IOCTL_CMD_ALL_MODE_SET: 
	        enRet = gpioex_ioctl_mode_all_set(&stIoctl.unData.stAllMode); 
	        break;
        case GPIOEX_IOCTL_CMD_ALL_MODE_GET: 
            enRet = gpioex_ioctl_mode_all_get(&stIoctl.unData.stAllMode); 
            bGetOps = TRUE; 
            break;
   
	    case GPIOEX_IOCTL_CMD_MODE_SET: 
	        enRet = gpioex_ioctl_mode_set(&stIoctl.unData.stMode); 
	        break;
        case GPIOEX_IOCTL_CMD_MODE_GET: 
            enRet = gpioex_ioctl_mode_get(&stIoctl.unData.stMode); 
            bGetOps = TRUE; 
            break;
   
	    case GPIOEX_IOCTL_CMD_ALL_STATE_SET: 
	        enRet = gpioex_ioctl_state_all_set(&stIoctl.unData.stAllState); 
	        break;
        case GPIOEX_IOCTL_CMD_ALL_STATE_GET: 
            enRet = gpioex_ioctl_state_all_get(&stIoctl.unData.stAllState); 
            bGetOps = TRUE; 
            break;
   
	    case GPIOEX_IOCTL_CMD_STATE_SET: 
	        enRet = gpioex_ioctl_state_set(&stIoctl.unData.stState); 
	        break;
        case GPIOEX_IOCTL_CMD_STATE_GET: 
            enRet = gpioex_ioctl_state_get(&stIoctl.unData.stState); 
            bGetOps = TRUE; 
            break;
   
   
        default:
			GPIOEX_LOG(GPIOEX_LOG_ERR, "Unknown cmd %d\n", stIoctl.enCmd);
			return  AP_E_PARAM;
			break;
	}
	
	if (AP_E_NONE != enRet)
	{
	    return AP_E_DEFAULT_FAIL;
	}
	
	if (bGetOps)
	{
    	if (copy_to_user(pstArg, &stIoctl, sizeof(GPIOEX_IOCTL_T)))
    	{
    		GPIOEX_LOG(GPIOEX_LOG_ERR, "Copy to user failed.\n");
    		return AP_E_DEFAULT_FAIL;
    	}
	}
	
	return AP_E_NONE;
}


