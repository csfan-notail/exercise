/******************************************************************************

                  版权所有 (C), 2001-2011, 神州数码网络有限公司

 ******************************************************************************
  文 件 名   : pal.c
  版 本 号   : 初稿
  作    者   : liyoukun
  生成日期   : 2015年2月2日星期一
  最近修改   :
  功能描述   : 中间层函数
  函数列表   :
              lte_sig_info_get
              lte_timer_sig_info_get
  修改历史   :
  1.日    期   : 2015年2月2日星期一
    作    者   : liyoukun
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "lte_com.h"
#include "lte_log.h"


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
extern LTE_STATE_INFO_T g_stLteStateInfo;
extern LTE_MODULE_OPS_T *g_pstLte_module_ops_t;

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 ********      定时器获取状态函数      **********
 *----------------------------------------------*/
LTE_RET_E lte_timer_oper_info_get(LTE_STATE_INFO_T *pstLteStateInfo)
{
    LTE_OPER_INFO_T stOperInfo = {0};
    LTE_LOG(LTE_LOG_INFO, "lte_timer_oper_info_get enter!\n");
    if(NULL == pstLteStateInfo)
    {
        LTE_LOG(LTE_LOG_ERR, "input param is NULL pointer!");
        return LTE_FAIL;
    }
    /* 从lte com口中获取pinstate信息 */
    if(NULL == g_pstLte_module_ops_t->lte_oper_info_get)
    {
        LTE_LOG(LTE_LOG_ERR, "ops func is NULL");
        return LTE_FAIL;
    }
    //g_pstLte_module_ops_t->lte_oper_info_get((void *)(&stOperInfo), sizeof(LTE_OPER_INFO_T));
    strncpy(pstLteStateInfo->acOperInfo, stOperInfo.acOperName, sizeof(pstLteStateInfo->acOperInfo));
    LTE_LOG(LTE_LOG_INFO, "Oper Info:%s\n", pstLteStateInfo->acOperInfo);
    return LTE_OK;
}
LTE_RET_E lte_timer_sig_info_get(LTE_STATE_INFO_T *pstLteStateInfo)
{
    LTE_SIGNAL_T stSignal = {0};
    LTE_LOG(LTE_LOG_INFO,"######################lte_timer_sig_info_get\n");
    if(NULL == pstLteStateInfo)
    {
        LTE_LOG(LTE_LOG_ERR, "input param is NULL pointer!");
        return LTE_FAIL;
    }
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_signal_get)
    {
        LTE_LOG(LTE_LOG_ERR, "ops func is NULL");
        return LTE_FAIL;
    }
    //g_pstLte_module_ops_t->lte_signal_get((void *)(&stSignal), sizeof(LTE_SIGNAL_T));
    pstLteStateInfo->iSignal = stSignal.iSignal;
    LTE_LOG(LTE_LOG_INFO,"signal value:%d\n", pstLteStateInfo->iSignal);
    return LTE_OK;
}
LTE_RET_E lte_timer_link_info_get(LTE_STATE_INFO_T *pstLteStateInfo)
{
    LTE_LINKSTATE_T stLinkState = {0};
    LTE_LOG(LTE_LOG_INFO,"######################lte_timer_link_info_get\n");
    if(NULL == pstLteStateInfo)
    {
        LTE_LOG(LTE_LOG_ERR, "input param is NULL pointer!");
        return LTE_FAIL;
    }
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_link_state_get)
    {
        LTE_LOG(LTE_LOG_ERR, "ops func is NULL");
        return LTE_FAIL;
    }
    //g_pstLte_module_ops_t->lte_link_state_get((void *)(&stLinkState), sizeof(LTE_LINKSTATE_T));
    pstLteStateInfo->enLinkState = stLinkState.enLinkstate;
    LTE_LOG(LTE_LOG_INFO,"link state:%d\n", pstLteStateInfo->enLinkState);
    return LTE_OK;
}

LTE_RET_E lte_timer_simstate_info_get(LTE_STATE_INFO_T *pstLteStateInfo)
{
    LTE_SIM_STATE_T stSimState = {0};
    LTE_LOG(LTE_LOG_INFO,"######################lte_timer_simstate_info_get\n");
    if(NULL == pstLteStateInfo)
    {
        LTE_LOG(LTE_LOG_ERR, "input param is NULL pointer!");
        return LTE_FAIL;
    }
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_sim_state_get)
    {
        LTE_LOG(LTE_LOG_ERR, "ops func is NULL");
        return LTE_FAIL;
    }
    //g_pstLte_module_ops_t->lte_sim_state_get((void *)(&stSimState), sizeof(LTE_SIM_STATE_T));
    pstLteStateInfo->enSimState = stSimState.enSimState;
    LTE_LOG(LTE_LOG_INFO,"sim state:%d[0:sim not found;1:sim ready]\n", pstLteStateInfo->enSimState);
    return LTE_OK;
}
LTE_RET_E lte_timer_accessnet_info_get(LTE_STATE_INFO_T *pstLteStateInfo)
{
    LTE_ACCESS_NETWORK_T stAccessNet = {0};
    LTE_LOG(LTE_LOG_INFO,"######################lte_timer_accessnet_info_get\n");
    if(NULL == pstLteStateInfo)
    {
        LTE_LOG(LTE_LOG_ERR, "input param is NULL pointer!");
        return LTE_FAIL;
    }
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_access_net_get)
    {
        LTE_LOG(LTE_LOG_ERR, "ops func is NULL");
        return LTE_FAIL;
    }
    //g_pstLte_module_ops_t->lte_access_net_get((void *)(&stAccessNet), sizeof(LTE_ACCESS_NETWORK_T));
    pstLteStateInfo->enAccessNet = stAccessNet.enAccessNet;
    LTE_LOG(LTE_LOG_INFO,"access net:%d[0:Null net;1:2G net;2:3G net;3:4G net]\n", pstLteStateInfo->enAccessNet);
    return LTE_OK;
}

LTE_RET_E lte_timer_cellid_info_get(LTE_STATE_INFO_T *pstLteStateInfo)
{
    LTE_CELLID_T stCellId = {0};
    LTE_LOG(LTE_LOG_INFO,"######################lte_timer_accessnet_info_get\n");
    if(NULL == pstLteStateInfo)
    {
        LTE_LOG(LTE_LOG_ERR, "input param is NULL pointer!");
        return LTE_FAIL;
    }
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_cellid_get)
    {
        LTE_LOG(LTE_LOG_ERR, "ops func is NULL");
        return LTE_FAIL;
    }
    strncpy(pstLteStateInfo->acCellId, stCellId.acCellID, sizeof(pstLteStateInfo->acCellId));
    LTE_LOG(LTE_LOG_INFO,"CellId:%s\n", pstLteStateInfo->acCellId);
    return LTE_OK;
}
LTE_RET_E lte_timer_lte_info_get(LTE_STATE_INFO_T *pstLteStateInfo)
{
    LTE_INFO_T stLteInfo = {0};
    LTE_LOG(LTE_LOG_INFO,"######################lte_timer_lte_info_get\n");
    if(NULL == pstLteStateInfo)
    {
        LTE_LOG(LTE_LOG_ERR, "input param is NULL pointer!");
        return LTE_FAIL;
    }
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_info_get)
    {
        LTE_LOG(LTE_LOG_ERR, "ops func is NULL");
        return LTE_FAIL;
    }
    //g_pstLte_module_ops_t->lte_info_get((void *)(&stLteInfo), sizeof(LTE_INFO_T));
    if(strlen(stLteInfo.stCellInfo.acCellId) > 4)
    {
        strncpy(pstLteStateInfo->acCellId, stLteInfo.stCellInfo.acCellId, sizeof(pstLteStateInfo->acCellId));
    }
    strncpy(pstLteStateInfo->acBand, stLteInfo.stCellInfo.acBand, sizeof(pstLteStateInfo->acBand));
    strncpy(pstLteStateInfo->acPhyCellId, stLteInfo.stCellInfo.acPhyCeillid, sizeof(pstLteStateInfo->acPhyCellId));
    pstLteStateInfo->iChannel = stLteInfo.stCellInfo.iChannel;
    pstLteStateInfo->iRSRP = stLteInfo.stCellInfo.iRsrp;
    pstLteStateInfo->iRSRQ = stLteInfo.stCellInfo.iRsrq;
    pstLteStateInfo->iSinr = stLteInfo.stCellInfo.iSinr;
    pstLteStateInfo->iRssi = stLteInfo.stCellInfo.iRssi;
    LTE_LOG(LTE_LOG_INFO,"CellId:%s\n", pstLteStateInfo->acCellId);
    LTE_LOG(LTE_LOG_INFO,"PhyCellId:%s\n", pstLteStateInfo->acPhyCellId);
    LTE_LOG(LTE_LOG_INFO,"band:%s\n", pstLteStateInfo->acBand);
    LTE_LOG(LTE_LOG_INFO,"Channel:%d\n", pstLteStateInfo->iChannel);
    LTE_LOG(LTE_LOG_INFO,"RSRP:%d\n", pstLteStateInfo->iRSRP);
    LTE_LOG(LTE_LOG_INFO,"RSRQ:%d\n", pstLteStateInfo->iRSRQ);
    LTE_LOG(LTE_LOG_INFO,"RSSI:%d\n", pstLteStateInfo->iRssi);
    LTE_LOG(LTE_LOG_INFO,"SINR:%d\n", pstLteStateInfo->iSinr);
    return LTE_OK;
}


/*----------------------------------------------*
 ********        fd获取状态接口        **********
 *----------------------------------------------*/
LTE_RET_E lte_pinstate_info_get(void *pvData)
{
    LTE_PIN_STATE_T *pstPinState = (LTE_PIN_STATE_T *)pvData;

    if(1 != g_stLteStateInfo.iPinStaticFlag)
    {
        return LTE_FAIL;
    }
    pstPinState->enState = g_stLteStateInfo.enPinState;
    
    return LTE_OK;
} 
LTE_RET_E lte_pinswstate_info_get(void *pvData)
{
    LTE_PIN_SW_STATE_T *pstPinSwState = (LTE_PIN_SW_STATE_T *)pvData;

    if(1 == g_stLteStateInfo.iPinSwStaticFlag)
    {
        pstPinSwState->enState = g_stLteStateInfo.enPinSwState;
    }
    else 
    {
        return LTE_FAIL;
    }
    
    return LTE_OK;
} 
LTE_RET_E lte_curnettype_info_get(void *pvData)
{
    LTE_NET_TYPE_T *pstCurNetType = (LTE_NET_TYPE_T *)pvData;

    if(1 == g_stLteStateInfo.iNetTypeStaticFlag)
    {
        pstCurNetType->enNetType = g_stLteStateInfo.enCurNetType;
        LTE_LOG(LTE_LOG_INFO,"lte_curnettype_info_get:%d\n", pstCurNetType->enNetType);
    }
    else
    {
        return LTE_FAIL;
    }
    return LTE_OK;
}

LTE_RET_E lte_sig_info_get(void *pvData)
{
    LTE_SIGNAL_T *pstSignal = (LTE_SIGNAL_T *)pvData;

    if(1 == g_stLteStateInfo.iSignalStaticFlag)
    {
        pstSignal->iSignal = g_stLteStateInfo.iSignal;
    }
    else
    {
        return LTE_FAIL;
    }

    
    return LTE_OK;
}
LTE_RET_E lte_linkstate_info_get(void *pvData)
{
    LTE_LINKSTATE_T *pstLinkState = (LTE_LINKSTATE_T *)pvData;

    if( 1 == g_stLteStateInfo.iLinkStaticFlag )
    {
        pstLinkState->enLinkstate = g_stLteStateInfo.enLinkState;
    }
    else
    {
        return LTE_FAIL;
    }
    return LTE_OK;
}

LTE_RET_E lte_imei_info_get(void *pvData)
{
    LTE_IMEI_T *pstImei = (LTE_IMEI_T *)pvData;

    if(1 == g_stLteStateInfo.iIMEIStaticFlag)
    {
        strncpy(pstImei->acImei, g_stLteStateInfo.acImei, sizeof(pstImei->acImei));
    }
    else
    {   
        return LTE_FAIL;
    }
    return LTE_OK;
}
LTE_RET_E lte_cellid_info_get(void *pvData)
{
    LTE_CELLID_T *pstCellid = (LTE_CELLID_T *)pvData;

    if(1 == g_stLteStateInfo.iCellIDStaticFlag)
    {
        strncpy(pstCellid->acCellID, g_stLteStateInfo.acCellId, sizeof(pstCellid->acCellID));
    }
    else
    {   
        return LTE_FAIL;
    }
    return LTE_OK;
}

LTE_RET_E lte_iccid_info_get(void *pvData)
{
    LTE_ICCID_T *pstIccid = (LTE_ICCID_T *)pvData;

    if(1 == g_stLteStateInfo.iICCIDStaticFLag)
    {
        strncpy(pstIccid->acICCID, g_stLteStateInfo.acIccid, sizeof(pstIccid->acICCID));
    }
    else
    {   
        return LTE_FAIL;
    }
    return LTE_OK;
}
LTE_RET_E lte_imsi_info_get(void *pvData)
{
    LTE_IMSI_T *pstImsi = (LTE_IMSI_T *)pvData;

    if(1 == g_stLteStateInfo.iIMSIStaticFLag)
    {
        strncpy(pstImsi->acIMSI, g_stLteStateInfo.acImsi, sizeof(pstImsi->acIMSI));
    }
    else
    {   
        return LTE_FAIL;
    }
    return LTE_OK;
}
LTE_RET_E lte_simstate_info_get(void *pvData)
{
    LTE_SIM_STATE_T *pstSimState = (LTE_SIM_STATE_T *)pvData;

    if(1 == g_stLteStateInfo.iIMSIStaticFLag)
    {
        pstSimState->enSimState = g_stLteStateInfo.enSimState;
    }
    else
    {   
        return LTE_FAIL;
    }
    return LTE_OK;
}
LTE_RET_E lte_info_get(void *pvData)
{
    LTE_INFO_T *pstLteInfo = (LTE_INFO_T *)pvData;

    if(1 == g_stLteStateInfo.iLteInfoStaticFLag)
    {
        strncpy(pstLteInfo->stCellInfo.acCellId, g_stLteStateInfo.acCellId, sizeof(pstLteInfo->stCellInfo.acCellId));
        
        strncpy(pstLteInfo->stCellInfo.acBand, g_stLteStateInfo.acBand, sizeof(pstLteInfo->stCellInfo.acBand));
        strncpy(pstLteInfo->stCellInfo.acPhyCeillid, g_stLteStateInfo.acPhyCellId, sizeof(pstLteInfo->stCellInfo.acPhyCeillid));
        pstLteInfo->stCellInfo.iChannel = g_stLteStateInfo.iChannel;
        pstLteInfo->stCellInfo.iRsrp = g_stLteStateInfo.iRSRP;
        pstLteInfo->stCellInfo.iRsrq = g_stLteStateInfo.iRSRQ;
        pstLteInfo->stCellInfo.iSinr = g_stLteStateInfo.iSinr;
        pstLteInfo->stCellInfo.iRssi = g_stLteStateInfo.iRssi;
    }
    else
    {   
        return LTE_FAIL;
    }
    return LTE_OK;
}

LTE_RET_E lte_accessnet_info_get(void *pvData)
{
    LTE_ACCESS_NETWORK_T *pstAccessNet = (LTE_ACCESS_NETWORK_T *)pvData;

    if(1 == g_stLteStateInfo.iACCNetStaticFlag)
    {
        pstAccessNet->enAccessNet = g_stLteStateInfo.enAccessNet;
    }
    else
    {   
        return LTE_FAIL;
    }

    return LTE_OK;
}

LTE_RET_E lte_bands_info_get(void *pvData)
{
    LTE_BANDS_T *pstBands = (LTE_BANDS_T *)pvData;

    strncpy(pstBands->bands, g_stLteStateInfo.acBands, sizeof(pstBands->bands));
    pstBands->iClcBandAble = g_stLteStateInfo.iClcBandAble;
    return LTE_OK;
}
LTE_RET_E lte_clccellable_info_get(void *pvData)
{
    LTE_CLKCELL_ENABLE_T *pstClcCellInfo = (LTE_CLKCELL_ENABLE_T *)pvData;

    pstClcCellInfo->iClkCellEnable = g_stLteStateInfo.iClkCellEnable;
    return LTE_OK;
}
LTE_RET_E lte_oper_info_get(void *pvData)
{
    LTE_OPER_INFO_T *pstOperInfo = (LTE_OPER_INFO_T *)pvData;

    
    if(1 == g_stLteStateInfo.iOperInfoStaticFlag)
    {
        strncpy(pstOperInfo->acOperName, g_stLteStateInfo.acOperInfo, sizeof(pstOperInfo->acOperName));
    }
    else
    {   
        return LTE_FAIL;
    }
    return LTE_OK;
}
LTE_RET_E lte_nettype_info_get(void *pvData)
{
    LTE_NET_TYPE_T *pstSupNetType = (LTE_NET_TYPE_T *)pvData;

    pstSupNetType->enNetType = g_stLteStateInfo.enSupNetType;
    pstSupNetType->enNetTypePri = g_stLteStateInfo.enNetTypePri;
    return LTE_OK;
}
LTE_RET_E lte_supsms_info_get(void *pvData)
{
    LTE_SUPPORT_SMS_T *pstSupSms = (LTE_SUPPORT_SMS_T *)pvData;

    pstSupSms->enSupSms = g_stLteStateInfo.enSupSms;
    return LTE_OK;
}

LTE_RET_E lte_version_info_get(void *pvData)
{
    LTE_MODULE_VERSION_T *pstVersion = (LTE_MODULE_VERSION_T *)pvData;

    if(1 == g_stLteStateInfo.iVersionStaticFlag)
    {
        strncpy(pstVersion->acVersion, g_stLteStateInfo.acLteVersion, sizeof(pstVersion->acVersion));
        pstVersion->iVersionIndex = g_stLteStateInfo.iVersionIndex;
    }
    else
    {   
        return LTE_FAIL;
    }
    
    return LTE_OK;
}

LTE_RET_E lte_enodebid_info_get(void *pvData)
{
    LTE_ENODB_ID_T *pstEnodebId = (LTE_ENODB_ID_T *)pvData;

    if(1 == g_stLteStateInfo.iEnodebIDStaticFlag)
    {
        strncpy(pstEnodebId->acEnodebId, g_stLteStateInfo.acEnodebID, sizeof(pstEnodebId->acEnodebId));
    }
    else
    {   
        return LTE_FAIL;
    }
    
    return LTE_OK;
}

LTE_RET_E lte_tac_info_get(void *pvData)
{
    LTE_TAC_T *pstTac = (LTE_TAC_T *)pvData;

    if(1 == g_stLteStateInfo.iTacStaticFlag)
    {
        strncpy(pstTac->acTac, g_stLteStateInfo.acTac, sizeof(pstTac->acTac));
    }
    else
    {   
        return LTE_FAIL;
    }
    
    return LTE_OK;
}
LTE_RET_E lte_lcid_info_get(void *pvData)
{
    LTE_LCID_T *pstLcid = (LTE_LCID_T *)pvData;

    if(1 == g_stLteStateInfo.iLcidStaticFlag)
    {
        strncpy(pstLcid->acLcid, g_stLteStateInfo.acLcid, sizeof(pstLcid->acLcid));
    }
    else
    {   
        return LTE_FAIL;
    }
    
    return LTE_OK;
}

LTE_RET_E lte_allcid_info_get(void *pvData)
{
    LTE_ALL_CELLID_T *pstAllCid = (LTE_ALL_CELLID_T *)pvData;

    if(1 == g_stLteStateInfo.iLcidStaticFlag)
    {
        strncpy(pstAllCid->acCellId, g_stLteStateInfo.acCellId, sizeof(pstAllCid->acCellId));
        strncpy(pstAllCid->acEnodebId, g_stLteStateInfo.acEnodebID, sizeof(pstAllCid->acEnodebId));
        strncpy(pstAllCid->acTac, g_stLteStateInfo.acTac, sizeof(pstAllCid->acTac));
        strncpy(pstAllCid->acLcid, g_stLteStateInfo.acLcid, sizeof(pstAllCid->acLcid));
    }
    else
    {   
        return LTE_FAIL;
    }
    
    return LTE_OK;
}

LTE_RET_E lte_mac_info_get(void *pvData)
{
    LTE_MOUDLE_MAC_T *pstMac = (LTE_MOUDLE_MAC_T *)pvData;

    if(1 == g_stLteStateInfo.iMacStaticFlag)
    {
        strncpy(pstMac->acMac, g_stLteStateInfo.acMac, sizeof(pstMac->acMac));
        
    }
    else
    {   
        return LTE_FAIL;
    }
    
    return LTE_OK;
}

LTE_RET_E lte_sim_type_get(void *pvData)
{
    LTE_SIM_RANDOM_NUM_T *pstRandNum = (LTE_SIM_RANDOM_NUM_T *)pvData;

    if(1 == g_stLteStateInfo.iSimRandStaticFlag)
    {
        strncpy(pstRandNum->cRandomNum, g_stLteStateInfo.stSimRandNum.cRandomNum, sizeof(pstRandNum->cRandomNum)); 
        pstRandNum->iSIMType = g_stLteStateInfo.stSimRandNum.iSIMType;
    }
    else
    {   
        return LTE_FAIL;
    }
    
    return LTE_OK;
}

LTE_RET_E lte_sim_unlock_get(void *pvData)
{
    LTE_SIM_UNLOCK_STATE_T *pstUnlockState = (LTE_SIM_UNLOCK_STATE_T *)pvData;

    if(1 == g_stLteStateInfo.iSimUnlockStateFlag)
    {
        pstUnlockState->iIsUnlock = g_stLteStateInfo.iSimUnlockState;
    }
    else
    {   
        return LTE_FAIL;
    }
    
    return LTE_OK;
}


/*----------------------------------------------*
 ********         功能   接口          **********
 *----------------------------------------------*/
#if 0
/*****************************************************************************
 函 数 名  : lte_static_info_get
 功能描述  : 获取lte静态信息（开机后不会自动发生变化的信息），
             包括模块版本，ICCID，imei，imsi，apn等
 输入参数  : LTE_STATE_INFO_T *pstLteStateInfo  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年2月11日星期三
    作    者   : liyoukun
    修改内容   : 新生成函数

*****************************************************************************/
LTE_RET_E lte_static_info_get(LTE_STATE_INFO_T *pstLteStateInfo)
{
    LTE_ICCID_T stICCID = {0};
    LTE_IMSI_T  stImsi = {0};
    LTE_IMEI_T  stImei = {0};
    LTE_BANDS_T stBands = {0};
    LTE_PIN_STATE_T stPinState = {0};
    LTE_PIN_SW_STATE_T stPinSwState = {0};
    LTE_CLKCELL_ENABLE_T stClcCellAble = {0};
    LTE_NET_TYPE_T stSupNetType = {0};
    LTE_NET_TYPE_T stCurNetType = {0};
    LTE_SUPPORT_SMS_T stSupSms = {0};

    LTE_MODULE_VERSION_T stVersion = {0};
    if(NULL == pstLteStateInfo)
    {
        LTE_LOG(LTE_LOG_ERR, "input param is NULL pointer!");
        return LTE_FAIL;
    }
    lte_config_power_off();
    sleep(1);
    lte_config_power_on();
    
    /* 从lte com口中获取Iccid信息 */
    if(NULL == g_pstLte_module_ops_t->lte_iccid_get)
    {
        LTE_LOG(LTE_LOG_ERR, "ops func is NULL");
        return LTE_FAIL;
    }
    //g_pstLte_module_ops_t->lte_iccid_get((void *)(&stICCID), sizeof(LTE_ICCID_T));
    strncpy(pstLteStateInfo->acIccid, stICCID.acICCID, sizeof(pstLteStateInfo->acIccid));
    LTE_LOG(LTE_LOG_INFO,"ICCID:%s %s\n", pstLteStateInfo->acIccid, stICCID.acICCID);
    sleep(1);
    
    /* 从lte com口中获取IMSI信息 */
    if(NULL == g_pstLte_module_ops_t->lte_imsi_get)
    {
        LTE_LOG(LTE_LOG_ERR, "ops func is NULL");
        return LTE_FAIL;
    }
    //g_pstLte_module_ops_t->lte_imsi_get((void *)(&stImsi), sizeof(LTE_IMSI_T));
    strncpy(pstLteStateInfo->acImsi, stImsi.acIMSI, sizeof(pstLteStateInfo->acImsi));
    LTE_LOG(LTE_LOG_INFO,"IMSI:%s %s######\n", pstLteStateInfo->acImsi, stImsi.acIMSI);
    sleep(1);
    
    /* 从lte com口中获取IMEI信息 */
    if(NULL == g_pstLte_module_ops_t->lte_imei_get)
    {
        LTE_LOG(LTE_LOG_ERR, "ops func is NULL");
        return LTE_FAIL;
    }
    //g_pstLte_module_ops_t->lte_imei_get((void *)(&stImei), sizeof(LTE_IMEI_T));
    strncpy(pstLteStateInfo->acImei, stImei.acImei, sizeof(pstLteStateInfo->acImei));
    LTE_LOG(LTE_LOG_INFO,"IMEI:%s %s######\n", pstLteStateInfo->acImei, stImei.acImei);
    sleep(1);
    
    /* 从lte com口中获取模块版本信息 */
    if(NULL == g_pstLte_module_ops_t->lte_version_get)
    {
        LTE_LOG(LTE_LOG_ERR, "ops func is NULL");
        return LTE_FAIL;
    }
    //g_pstLte_module_ops_t->lte_version_get((void *)(&stVersion), sizeof(stVersion));
    strncpy(pstLteStateInfo->acLteVersion, stVersion.acVersion, sizeof(pstLteStateInfo->acLteVersion));
    pstLteStateInfo->iVersionIndex = stVersion.iVersionIndex;
    LTE_LOG(LTE_LOG_INFO,"version:%s %d######\n", pstLteStateInfo->acLteVersion, pstLteStateInfo->iVersionIndex);

    /* 获取支持接入的band */
    if(NULL == g_pstLte_module_ops_t->lte_bands_get)
    {
        LTE_LOG(LTE_LOG_ERR, "ops func is NULL");
        return LTE_FAIL;
    }
    //g_pstLte_module_ops_t->lte_bands_get((void *)(&stBands), sizeof(stBands));
    strncpy(pstLteStateInfo->acBands, stBands.bands, sizeof(pstLteStateInfo->acBands));
    pstLteStateInfo->iClcBandAble = stBands.iClcBandAble;
    LTE_LOG(LTE_LOG_INFO,"Bands:%s ClCBandEnable:%d######\n", pstLteStateInfo->acBands, pstLteStateInfo->iClcBandAble);

     /* 获取是否支持锁小区 */
    if(NULL == g_pstLte_module_ops_t->lte_clck_cell_get)
    {
        LTE_LOG(LTE_LOG_ERR, "ops func is NULL");
        return LTE_FAIL;
    }
    //g_pstLte_module_ops_t->lte_clck_cell_get((void *)(&stClcCellAble), sizeof(stClcCellAble));
    pstLteStateInfo->iClkCellEnable = stClcCellAble.iClkCellEnable;
    LTE_LOG(LTE_LOG_INFO,"ClCCellEnable:%d######\n", pstLteStateInfo->iClkCellEnable);

    /* get_pin_sw_state */
    if(NULL == g_pstLte_module_ops_t->lte_pin_switch_get)
    {
        LTE_LOG(LTE_LOG_ERR, "ops func is NULL");
        return LTE_FAIL;
    }
    //g_pstLte_module_ops_t->lte_pin_switch_get((void *)(&stPinSwState), sizeof(stPinSwState));
    pstLteStateInfo->enPinSwState = stPinSwState.enState;
    LTE_LOG(LTE_LOG_INFO,"Pin SW:%d[0:close pin;1:open pin close]######\n", pstLteStateInfo->enPinSwState);
    /* 从lte com口中获取pinstate信息 */
    if(NULL == g_pstLte_module_ops_t->lte_pin_state_get)
    {
        LTE_LOG(LTE_LOG_ERR, "ops func is NULL");
        return LTE_FAIL;
    }
    //g_pstLte_module_ops_t->lte_pin_state_get((void *)(&stPinState), sizeof(LTE_PIN_STATE_T));
    pstLteStateInfo->enPinState = stPinState.enState;
    LTE_LOG(LTE_LOG_INFO,"Pin state:%d[0:Ready;1:Pin;2:Puk]\n", pstLteStateInfo->enPinState);
    /* 获取支持网络类型信息 */
    if(NULL == g_pstLte_module_ops_t->lte_nettype_info_get)
    {
        LTE_LOG(LTE_LOG_ERR, "ops func is NULL");
        return LTE_FAIL;
    }
    //g_pstLte_module_ops_t->lte_nettype_info_get((void *)(&stSupNetType), sizeof(LTE_NET_TYPE_T));
    pstLteStateInfo->enSupNetType = stSupNetType.enNetType;
    pstLteStateInfo->enNetTypePri = stSupNetType.enNetTypePri;
    LTE_LOG(LTE_LOG_INFO,"Sup Net Type:%d, Net Pri:%d\n", pstLteStateInfo->enSupNetType, pstLteStateInfo->enNetTypePri);
    /* 获取当前网络类型信息 */
    if(NULL == g_pstLte_module_ops_t->lte_net_type_get)
    {
        LTE_LOG(LTE_LOG_ERR, "ops func is NULL");
        return LTE_FAIL;
    }
    //g_pstLte_module_ops_t->lte_net_type_get((void *)(&stCurNetType), sizeof(LTE_NET_TYPE_T));
    pstLteStateInfo->enCurNetType = stCurNetType.enNetType;
    LTE_LOG(LTE_LOG_INFO,"Cur Net Type:%d\n", pstLteStateInfo->enCurNetType);

    /* 获取是否支持短信功能 */
    if(NULL == g_pstLte_module_ops_t->lte_sms_supinfo_get)
    {
        LTE_LOG(LTE_LOG_ERR, "ops func is NULL");
        return LTE_FAIL;
    }
    //g_pstLte_module_ops_t->lte_sms_supinfo_get((void *)(&stSupSms), sizeof(LTE_SUPPORT_SMS_T));
    pstLteStateInfo->enSupSms = stSupSms.enSupSms;
    LTE_LOG(LTE_LOG_INFO,"Sup sms state:%d[0:not sup;1:sup]\n", pstLteStateInfo->enSupSms);
    return LTE_OK;
}
#endif
LTE_RET_E lte_real_pinstate_get(void *pvData, char *pcBuf)
{
    LTE_PIN_STATE_T stPinState = {0};
    
    LTE_PIN_STATE_T *pstPinState = (LTE_PIN_STATE_T *)pvData;
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_pinstate_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_pinstate_get enter!\n");    
        
    /* 从lte com口中获取pinstate信息 */
    if(NULL == g_pstLte_module_ops_t->lte_pin_state_get)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_pinstate_get ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_pin_state_get((void *)(&stPinState), pcBuf);
    pstPinState->enState = stPinState.enState;
    LTE_LOG(LTE_LOG_INFO, "Pin state:%d[0:Ready;1:Pin;2:Puk]\n", pstPinState->enState);
    g_stLteStateInfo.enPinState = stPinState.enState;
    g_stLteStateInfo.iPinStaticFlag = 1;
    return LTE_OK;
}
LTE_RET_E lte_real_pinswstate_get(void *pvData, char *pcBuf)
{
    LTE_PIN_SW_STATE_T stPinSwState = {0};
    
    LTE_PIN_SW_STATE_T *pstPinSwState = (LTE_PIN_SW_STATE_T *)pvData;
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_pinswstate_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_pinswstate_get enter!\n");    
        
    /* 从lte com口中获取pinstate信息 */
    if(NULL == g_pstLte_module_ops_t->lte_pin_switch_get)
    {
        LTE_LOG(LTE_LOG_ERR, "ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_pin_switch_get((void *)(&stPinSwState), pcBuf);
    pstPinSwState->enState = stPinSwState.enState;
    LTE_LOG(LTE_LOG_INFO, "PinSW state:%d[0:pin close;1:Pin open]\n", pstPinSwState->enState);
    g_stLteStateInfo.enPinSwState = stPinSwState.enState;
    g_stLteStateInfo.iPinSwStaticFlag = 1;
    return LTE_OK;
}
LTE_RET_E lte_real_supnet_get(void *pvData, char *pcBuf)
{
    LTE_NET_TYPE_T stNettype = {0};
    
    LTE_NET_TYPE_T *pstNettype = (LTE_NET_TYPE_T *)pvData;
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_supnet_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_supnet_get enter!\n");    
        
    /* 从lte com口中获取pinstate信息 */
    if(NULL == g_pstLte_module_ops_t->lte_net_type_get)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_supnet_get ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_net_type_get((void *)(&stNettype), pcBuf);
    pstNettype->enNetType = stNettype.enNetType;
    LTE_LOG(LTE_LOG_INFO, "signal:%d\n", pstNettype->enNetType);
    g_stLteStateInfo.enSupNetType = pstNettype->enNetType;
    g_stLteStateInfo.iNetTypeStaticFlag = 1; 
    return LTE_OK;
}

LTE_RET_E lte_real_signal_get(void *pvData, char *pcBuf)
{
    LTE_SIGNAL_T stSignal = {0};
    
    LTE_SIGNAL_T *pstSignal = (LTE_SIGNAL_T *)pvData;
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_signal_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_signal_get enter!\n");    
        
    /* 从lte com口中获取pinstate信息 */
    if(NULL == g_pstLte_module_ops_t->lte_signal_get)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_signal_get ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_signal_get((void *)(&stSignal), pcBuf);
    pstSignal->iSignal = stSignal.iSignal;
    LTE_LOG(LTE_LOG_INFO, "signal:%d\n", pstSignal->iSignal);
    g_stLteStateInfo.iSignal = stSignal.iSignal;
    g_stLteStateInfo.iSignalStaticFlag = 1;
    return LTE_OK;
}
LTE_RET_E lte_real_network_prepare(void *pvData, char *pcBuf)
{
    LTE_NETWORK_PREPARE_T stNwState = {0};
    
    LTE_NETWORK_PREPARE_T *pstNwState = (LTE_NETWORK_PREPARE_T *)pvData;
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_network_prepare input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_network_prepare enter!\n");    
    /* 从lte com口中获取linkstate信息 */
    if(NULL == g_pstLte_module_ops_t->lte_net_work_prepare)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_network_prepare ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_net_work_prepare((void *)(&stNwState), pcBuf);
    strncpy(pstNwState->acNetWorkStatus, stNwState.acNetWorkStatus, sizeof(pstNwState->acNetWorkStatus));
    LTE_LOG(LTE_LOG_INFO, "nw state:%s\n", pstNwState->acNetWorkStatus);

    return LTE_OK;
}

LTE_RET_E lte_real_nwstatus_get(void *pvData, char *pcBuf)
{
    LTE_NETWORK_PREPARE_T stNwState = {0};
    
    LTE_NETWORK_PREPARE_T *pstNwState = (LTE_NETWORK_PREPARE_T *)pvData;
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_nwstatus_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_nwstatus_get enter!\n");    
        
    /* 从lte com口中获取linkstate信息 */
    if(NULL == g_pstLte_module_ops_t->lte_net_work_status_get)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_nwstatus_get ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_net_work_status_get((void *)(&stNwState), pcBuf);
    strncpy(pstNwState->acNetWorkStatus, stNwState.acNetWorkStatus, sizeof(pstNwState->acNetWorkStatus));
    LTE_LOG(LTE_LOG_INFO, "nw state:%s\n", pstNwState->acNetWorkStatus);

    return LTE_OK;
}

LTE_RET_E lte_real_linkstate_get(void *pvData, char *pcBuf)
{
    LTE_LINKSTATE_T stLinkState = {0};
    
    LTE_LINKSTATE_T *pstLinkState = (LTE_LINKSTATE_T *)pvData;
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_linkstate_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_linkstate_get enter!\n");    
    /* 从lte com口中获取linkstate信息 */
    if(NULL == g_pstLte_module_ops_t->lte_link_state_get)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_linkstate_get ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_link_state_get((void *)(&stLinkState), pcBuf);
    pstLinkState->enLinkstate = stLinkState.enLinkstate;
    LTE_LOG(LTE_LOG_INFO, "LinkState:%d\n", pstLinkState->enLinkstate);
    g_stLteStateInfo.enLinkState= stLinkState.enLinkstate;
    g_stLteStateInfo.iLinkStaticFlag = 1;
    return LTE_OK;
}
LTE_RET_E lte_real_imei_get(void *pvData, char *pcBuf)
{
    LTE_IMEI_T stImei = {0};
    
    LTE_IMEI_T *pstImei = (LTE_IMEI_T *)pvData;
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_imei_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_imei_get enter!\n");     
    /* 从lte com口中获取linkstate信息 */
    if(NULL == g_pstLte_module_ops_t->lte_imei_get)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_imei_get ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_imei_get((void *)(&stImei), pcBuf);
    strncpy(pstImei->acImei, stImei.acImei, sizeof(pstImei->acImei));
    LTE_LOG(LTE_LOG_INFO, "IMEI:%s\n", pstImei->acImei);
    strncpy(g_stLteStateInfo.acImei, stImei.acImei, sizeof(g_stLteStateInfo.acImei));
    g_stLteStateInfo.iIMEIStaticFlag = 1;
    return LTE_OK;
}
LTE_RET_E lte_real_cellid_get(void *pvData, char *pcBuf)
{
    LTE_CELLID_T stCellID = {0};
    
    LTE_CELLID_T *pstCellID = (LTE_CELLID_T *)pvData;
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_cellid_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_cellid_get enter!\n");    
        
    /* 从lte com口中获取linkstate信息 */
    if(NULL == g_pstLte_module_ops_t->lte_cellid_get)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_cellid_get ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_cellid_get((void *)(&stCellID), pcBuf);
    strncpy(pstCellID->acCellID, stCellID.acCellID, sizeof(pstCellID->acCellID));
    LTE_LOG(LTE_LOG_INFO, "CELLID:%s\n", pstCellID->acCellID);
    if(strlen(stCellID.acCellID) > 2)
    {
        strncpy(g_stLteStateInfo.acCellId, stCellID.acCellID, sizeof(g_stLteStateInfo.acCellId));
    }
    g_stLteStateInfo.iCellIDStaticFlag = 1;
    return LTE_OK;
}

LTE_RET_E lte_real_iccid_get(void *pvData, char *pcBuf)
{
    LTE_ICCID_T stIccid = {0};
    
    LTE_ICCID_T *pstIccid = (LTE_ICCID_T *)pvData;
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_iccid_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_iccid_get enter!\n");    
        
    /* 从lte com口中获取iccid信息 */
    if(NULL == g_pstLte_module_ops_t->lte_iccid_get)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_iccid_get ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_iccid_get((void *)(&stIccid), pcBuf);
    strncpy(pstIccid->acICCID, stIccid.acICCID, sizeof(pstIccid->acICCID));
    LTE_LOG(LTE_LOG_INFO, "iccid:%s\n", pstIccid->acICCID);
    strncpy(g_stLteStateInfo.acIccid, stIccid.acICCID, sizeof(g_stLteStateInfo.acIccid));
    g_stLteStateInfo.iICCIDStaticFLag = 1;
    return LTE_OK;
}


LTE_RET_E lte_real_imsi_get(void *pvData, char *pcBuf)
{
    LTE_IMSI_T stImsi = {0};
    
    LTE_IMSI_T *pstImsi = (LTE_IMSI_T *)pvData;
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_imsi_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_imsi_get enter!\n");    
        
    /* 从lte com口中获取linkstate信息 */
    if(NULL == g_pstLte_module_ops_t->lte_imsi_get)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_imsi_get ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_imsi_get((void *)(&stImsi), pcBuf);
    strncpy(pstImsi->acIMSI, stImsi.acIMSI, sizeof(pstImsi->acIMSI));
    LTE_LOG(LTE_LOG_INFO, "IMSI:%s\n", pstImsi->acIMSI);
    strncpy(g_stLteStateInfo.acImsi, stImsi.acIMSI, sizeof(g_stLteStateInfo.acImsi));
    g_stLteStateInfo.iIMSIStaticFLag = 1;
    return LTE_OK;
}
LTE_RET_E lte_real_simstate_get(void *pvData, char *pcBuf)
{
    LTE_SIM_STATE_T stSimState = {0};
    
    LTE_SIM_STATE_T *pstSimState = (LTE_SIM_STATE_T *)pvData;
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_simstate_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_simstate_get enter!\n");    
    /* 从lte com口中获取simstate信息 */
    if(NULL == g_pstLte_module_ops_t->lte_sim_state_get)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_simstate_get ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_sim_state_get((void *)(&stSimState), pcBuf);
    pstSimState->enSimState = stSimState.enSimState;
    LTE_LOG(LTE_LOG_INFO, "sim state:%d\n", pstSimState->enSimState);
    g_stLteStateInfo.enSimState = stSimState.enSimState;
    
    g_stLteStateInfo.iSimStaticFlag = 1;
    
    return LTE_OK;
}
LTE_RET_E lte_real_lteinfo_get(void *pvData, char *pcBuf)
{
    LTE_INFO_T stLteinfo = {0};
    
    LTE_INFO_T *pstLteinfo = (LTE_INFO_T *)pvData;
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_lteinfo_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_lteinfo_get enter!\n");         
    /* 从lte com口中获取access net信息 */
    if(NULL == g_pstLte_module_ops_t->lte_info_get)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_lteinfo_get ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_info_get((void *)(&stLteinfo), pcBuf);
    strncpy(pstLteinfo->stCellInfo.acBand, stLteinfo.stCellInfo.acBand, sizeof(pstLteinfo->stCellInfo.acBand));
    strncpy(pstLteinfo->stCellInfo.acCellId, stLteinfo.stCellInfo.acCellId, sizeof(pstLteinfo->stCellInfo.acCellId));
    strncpy(pstLteinfo->stCellInfo.acPhyCeillid, stLteinfo.stCellInfo.acPhyCeillid, sizeof(pstLteinfo->stCellInfo.acPhyCeillid));
    pstLteinfo->stCellInfo.iChannel = stLteinfo.stCellInfo.iChannel;
    pstLteinfo->stCellInfo.iRsrp = stLteinfo.stCellInfo.iRsrp;
    pstLteinfo->stCellInfo.iRsrq = stLteinfo.stCellInfo.iRsrq;
    pstLteinfo->stCellInfo.iRssi = stLteinfo.stCellInfo.iRssi;
    pstLteinfo->stCellInfo.iSinr = stLteinfo.stCellInfo.iSinr;
    //g_stLteStateInfo.enAccessNet = stAccessNet.enAccessNet;

    //g_stLteStateInfo.iLteInfoStaticFLag = 1;
    return LTE_OK;
}

LTE_RET_E lte_real_accessnet_get(void *pvData, char *pcBuf)
{
    LTE_ACCESS_NETWORK_T stAccessNet = {0};
    
    LTE_ACCESS_NETWORK_T *pstAccessNet = (LTE_ACCESS_NETWORK_T *)pvData;
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_accessnet_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_accessnet_get enter!\n");     
    /* 从lte com口中获取access net信息 */
    if(NULL == g_pstLte_module_ops_t->lte_access_net_get)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_accessnet_get ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_access_net_get((void *)(&stAccessNet), pcBuf);
    pstAccessNet->enAccessNet = stAccessNet.enAccessNet;
    LTE_LOG(LTE_LOG_INFO, "access net:%d\n", pstAccessNet->enAccessNet);
    g_stLteStateInfo.enAccessNet = stAccessNet.enAccessNet;
    
    g_stLteStateInfo.iACCNetStaticFlag = 1;
    
    return LTE_OK;
}
LTE_RET_E lte_real_operinfo_get(void *pvData, char *pcBuf)
{
    LTE_OPER_INFO_T stOperInfo = {0};
    
    LTE_OPER_INFO_T *pstOperInfo = (LTE_OPER_INFO_T *)pvData;
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_operinfo_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_operinfo_get enter!\n");      
    /* 从lte com口中获取access net信息 */
    if(NULL == g_pstLte_module_ops_t->lte_oper_info_get)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_operinfo_get ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_oper_info_get((void *)(&stOperInfo), pcBuf);
    strncpy(pstOperInfo->acOperName, stOperInfo.acOperName, sizeof(pstOperInfo->acOperName));
    LTE_LOG(LTE_LOG_INFO, "oper info:%s\n", pstOperInfo->acOperName);

    strncpy(g_stLteStateInfo.acOperInfo, stOperInfo.acOperName, sizeof(g_stLteStateInfo.acOperInfo));
    
    g_stLteStateInfo.iOperInfoStaticFlag = 1;
    
    return LTE_OK;
}
LTE_RET_E lte_real_modversion_get(void *pvData, char *pcBuf)
{
    LTE_MODULE_VERSION_T stVersion = {0};
    
    LTE_MODULE_VERSION_T *pstVersion = (LTE_MODULE_VERSION_T *)pvData;
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_modversion_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_modversion_get enter!\n");    
    /* 从lte com口中获取模块版本信息 */
    if(NULL == g_pstLte_module_ops_t->lte_version_get)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_modversion_get ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_version_get((void *)(&stVersion), pcBuf);
    strncpy(pstVersion->acVersion, stVersion.acVersion, sizeof(pstVersion->acVersion));
    pstVersion->iVersionIndex = stVersion.iVersionIndex;
    LTE_LOG(LTE_LOG_INFO, "version:%s, index:%d len:%d\n", pstVersion->acVersion, pstVersion->iVersionIndex, strlen(pstVersion->acVersion));

    strncpy(g_stLteStateInfo.acLteVersion, stVersion.acVersion, sizeof(g_stLteStateInfo.acLteVersion));
    g_stLteStateInfo.iVersionIndex = stVersion.iVersionIndex;
    
    g_stLteStateInfo.iVersionStaticFlag = 1;
    
    return LTE_OK;
}
LTE_RET_E lte_real_enodebid_get(void *pvData, char *pcBuf)
{
    LTE_ENODB_ID_T stEnodebID = {0};
        
    LTE_ENODB_ID_T *pstEnodebID = (LTE_ENODB_ID_T *)pvData;
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_enodebid_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_enodebid_get enter!\n");    
    /* 从lte com口中获取模块版本信息 */
    if(NULL == g_pstLte_module_ops_t->lte_enodebid_get)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_enodebid_get ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_enodebid_get((void *)(&stEnodebID), pcBuf);
    strncpy(pstEnodebID->acEnodebId, stEnodebID.acEnodebId, sizeof(pstEnodebID->acEnodebId));

    LTE_LOG(LTE_LOG_INFO, "enodebid:%s len:%d\n", pstEnodebID->acEnodebId, strlen(pstEnodebID->acEnodebId));
    if(strlen(stEnodebID.acEnodebId) > 2)
    {
        strncpy(g_stLteStateInfo.acEnodebID, stEnodebID.acEnodebId, sizeof(g_stLteStateInfo.acEnodebID));
    }
    g_stLteStateInfo.iEnodebIDStaticFlag = 1;
    
    return LTE_OK;

}
LTE_RET_E lte_real_tac_get(void *pvData, char *pcBuf)
{
    LTE_TAC_T stTacInfo = {0};
        
    LTE_TAC_T *pstTacInfo = (LTE_TAC_T *)pvData;
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_tac_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_tac_get enter!\n");
    /* 从lte com口中获取模块版本信息 */
    if(NULL == g_pstLte_module_ops_t->lte_tac_get)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_tac_get ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_tac_get((void *)(&stTacInfo), pcBuf);
    strncpy(pstTacInfo->acTac, stTacInfo.acTac, sizeof(pstTacInfo->acTac));
    LTE_LOG(LTE_LOG_INFO, "tac:%s len:%d\n", pstTacInfo->acTac, strlen(pstTacInfo->acTac));
    if(strlen(stTacInfo.acTac) > 2)
    {
        strncpy(g_stLteStateInfo.acTac, stTacInfo.acTac, sizeof(g_stLteStateInfo.acTac));
        }
    g_stLteStateInfo.iTacStaticFlag = 1;
    
    return LTE_OK;

}
LTE_RET_E lte_real_lcid_get(void *pvData, char *pcBuf)
{
    LTE_LCID_T stLcidInfo = {0};
        
    LTE_LCID_T *pstLcidInfo = (LTE_LCID_T *)pvData;

    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_lcid_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_lcid_get enter!\n");
        
    /* 从lte com口中获取模块版本信息 */
    if(NULL == g_pstLte_module_ops_t->lte_lcid_get)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_lcid_get ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_lcid_get((void *)(&stLcidInfo), pcBuf);
    strncpy(pstLcidInfo->acLcid, stLcidInfo.acLcid, sizeof(pstLcidInfo->acLcid));
    LTE_LOG(LTE_LOG_INFO, "lcid:%s len:%d\n", pstLcidInfo->acLcid, strlen(pstLcidInfo->acLcid));
    if(strlen(stLcidInfo.acLcid) > 2)
    {
        strncpy(g_stLteStateInfo.acLcid, stLcidInfo.acLcid, sizeof(g_stLteStateInfo.acLcid));
    }
    g_stLteStateInfo.iLcidStaticFlag = 1;
    
    return LTE_OK;
}

LTE_RET_E lte_real_all_cellid_get(void *pvData, char *pcBuf)
{
    LTE_ALL_CELLID_T stAllCidInfo = {0};
        
    LTE_ALL_CELLID_T *pstAllCidInfo = (LTE_ALL_CELLID_T *)pvData;

    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_lcid_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_all_cellid_get enter!\n");
        
    /* 从lte com口中获取模块版本信息 */
    if(NULL == g_pstLte_module_ops_t->lte_all_cellid_get)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_all_cellid_get ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_all_cellid_get((void *)(&stAllCidInfo), pcBuf);
    strncpy(pstAllCidInfo->acCellId, stAllCidInfo.acCellId, sizeof(pstAllCidInfo->acCellId));
    strncpy(pstAllCidInfo->acEnodebId, stAllCidInfo.acEnodebId, sizeof(pstAllCidInfo->acEnodebId));
    strncpy(pstAllCidInfo->acLcid, stAllCidInfo.acLcid, sizeof(pstAllCidInfo->acLcid));
    strncpy(pstAllCidInfo->acTac, stAllCidInfo.acTac, sizeof(pstAllCidInfo->acTac));
    
    g_stLteStateInfo.iAllCidStaticFlag = 1;
    
    return LTE_OK;
}


LTE_RET_E lte_real_mac_get(void *pvData, char *pcBuf)
{
    LTE_MOUDLE_MAC_T stMacInfo = {0};
        
    LTE_MOUDLE_MAC_T *pstMacInfo = (LTE_MOUDLE_MAC_T *)pvData;

    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_lcid_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_mac_get enter!\n");
        
    /* 从lte com口中获取模块版本信息 */
    if(NULL == g_pstLte_module_ops_t->lte_mac_get)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_mac_get ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_mac_get((void *)(&stMacInfo), pcBuf);
    strncpy(pstMacInfo->acMac, stMacInfo.acMac, sizeof(pstMacInfo->acMac));

    strncpy(g_stLteStateInfo.acMac, stMacInfo.acMac, sizeof(g_stLteStateInfo.acMac));

    g_stLteStateInfo.iMacStaticFlag= 1;
    
    return LTE_OK;
}

LTE_RET_E lte_real_sim_type_get(void *pvData, char *pcBuf)
{
    int iRet = 0;
    LTE_SIM_RANDOM_NUM_T *pstRandNum = (LTE_SIM_RANDOM_NUM_T *)pvData;
    
    if(NULL == g_pstLte_module_ops_t->lte_sim_type_get)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_sim_type_get ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_sim_type_get(pvData, pcBuf);
    g_stLteStateInfo.stSimRandNum.iSIMType = pstRandNum->iSIMType;
    strncpy(g_stLteStateInfo.stSimRandNum.cRandomNum, pstRandNum->cRandomNum, sizeof(g_stLteStateInfo.stSimRandNum.cRandomNum));
    g_stLteStateInfo.iSimRandStaticFlag = 1;

    return iRet;
}

LTE_RET_E lte_real_sim_unlock_get(void *pvData, char *pcBuf)
{
    
    LTE_SIM_UNLOCK_STATE_T *pstUnlockState = (LTE_SIM_UNLOCK_STATE_T *)pvData;

    pstUnlockState->iIsUnlock = 0;
    g_stLteStateInfo.iSimUnlockState = 0;
    g_stLteStateInfo.iSimUnlockStateFlag = 1;

    return LTE_OK;
}

LTE_RET_E lte_real_ip_gw_dns_get(void *pvData, char *pcBuf)
{
    int iRet = 0;
    LTE_IP_GATEWAY_DNS_T *pstIpGwDns = (LTE_IP_GATEWAY_DNS_T *)pvData;
    
    if(NULL == g_pstLte_module_ops_t->lte_ip_gw_dns_get)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_real_ip_gw_dns_get ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_ip_gw_dns_get(pvData, pcBuf);

    return iRet;
}

LTE_RET_E lte_real_all_oper_get(void *pvData, char *pcBuf)
{
    int iRet = 0;
    LTE_ALL_PLMN_MODE_T *pstAllPlmnMode = (LTE_ALL_PLMN_MODE_T *)pvData;
    
    if(NULL == g_pstLte_module_ops_t->lte_all_plmn_mode_get)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_real_all_oper_get ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_all_plmn_mode_get(pvData, pcBuf);

    return iRet;
}

LTE_RET_E lte_real_pin_input_remain_get(void *pvData, char *pcBuf)
{
    int iRet = 0;
    LTE_PIN_INPUT_REMAIN_T *pstPinIputRemain = (LTE_PIN_INPUT_REMAIN_T *)pvData;
    
    if(NULL == g_pstLte_module_ops_t->lte_pin_input_remain)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_real_pin_input_remain_get ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_pin_input_remain(pvData, pcBuf);

    return iRet;
}

LTE_RET_E lte_real_post_route_get(void *pvData, char *pcBuf)
{
    int iRet = 0;
    //LTE_POST_ROUTE_SW_T *pstPostRouteSw = (LTE_POST_ROUTE_SW_T *)pvData;
    
    if(NULL == g_pstLte_module_ops_t->lte_post_route_get)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_real_pin_input_remain_get ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_post_route_get(pvData, pcBuf);

    return iRet;
}



/*LTE_RET_E lte_real_eps_qos_get(void *pvData, char *pcBuf)
{
    int iRet = 0;
    if(NULL == g_pstLte_module_ops_t->lte_qos_get)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_real_pin_input_remain_get ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_qos_get(pvData, pcBuf);

    return iRet;
}*/




LTE_RET_E lte_real_eps_qos_set(char *pcBuf)
{
    int iRet = 0;
    
    if(NULL == g_pstLte_module_ops_t->lte_qos_set)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_real_eps_qos_set ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_qos_set(NULL, pcBuf);

    return iRet;
}


LTE_RET_E lte_real_eps_qos_get(void *pvData, char *pcBuf)
{
    LTE_QOS_DATA_S stQosData = {0};
    
    LTE_QOS_DATA_S *pstQosData = (LTE_QOS_DATA_S *)pvData;
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_eps_qos_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_simstate_get enter!\n");    
    /* 从lte com口中获取qos信息 */
    if(NULL == g_pstLte_module_ops_t->lte_qos_get)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_eps_qos_get ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_qos_get((void *)(&stQosData), pcBuf);
    pstQosData->iQosCid = stQosData.iQosCid;
	pstQosData->iQosQci = stQosData.iQosQci;
	pstQosData->iQosDlGbr = stQosData.iQosDlGbr;
	pstQosData->iQosUlGbr = stQosData.iQosUlGbr;
	pstQosData->iQosDlMbr = stQosData.iQosDlMbr;
	pstQosData->iQosUlMbr = stQosData.iQosUlMbr;
   
    
    return LTE_OK;
}



/*----------------------------------------------*
 ********         模块配置接口         **********
 *----------------------------------------------*/


/* 配置pin开关 */
LTE_RET_E lte_config_pinsw_set(char *pcBuf)
{
    int iRet = 0;
    LTE_PIN_SW_STATE_T stPinSwState = {0};
    
    LTE_LOG(LTE_LOG_INFO, "lte_config_pinsw_set enter!\n");
    if(NULL == g_pstLte_module_ops_t->lte_pin_switch)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_config_pinsw_set ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_pin_switch(NULL, pcBuf);
    return iRet;
}
/* 解pin */
LTE_RET_E lte_config_pin_unlock(char *pcBuf)
{
    int iRet = 0;
    LTE_PIN_SW_STATE_T stPinSwState = {0};
    
    LTE_LOG(LTE_LOG_INFO, "lte_config_pin_unlock enter!\n");
    if(NULL == g_pstLte_module_ops_t->lte_pin_unlock)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_config_pin_unlock ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_pin_unlock(NULL, pcBuf);
    return iRet;
}
/* 修改pin */
LTE_RET_E lte_config_pin_alter(char *pcBuf)
{
    int iRet = 0;
    LTE_PIN_SW_STATE_T stPinSwState = {0};
    
    LTE_LOG(LTE_LOG_INFO, "lte_config_pin_alter enter!\n");
    if(NULL == g_pstLte_module_ops_t->lte_pin_code_alter)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_config_pin_alter ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_pin_code_alter(NULL, pcBuf);
    return iRet;
}

LTE_RET_E lte_config_power_off(char *pcBuf)
{
    int iRet = 0;
    LTE_LOG(LTE_LOG_INFO, "lte_config_power_off enter!\n");
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_power_off)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_config_power_off ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_power_off(NULL, pcBuf);
    
    return iRet;
}
LTE_RET_E lte_config_power_on(char *pcBuf)
{
    int iRet = 0;
    LTE_LOG(LTE_LOG_INFO, "lte_config_power_on enter!\n");
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_power_on)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_config_power_on ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_power_on(NULL, pcBuf);
    
    return iRet;
}

LTE_RET_E lte_config_connect(char *pcBuf)
{
    int iRet = 0;
    LTE_LOG(LTE_LOG_INFO, "lte_config_connect enter!\n");
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_connect)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_config_connect ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_connect(NULL, pcBuf);
    
    return iRet;
}
LTE_RET_E lte_config_disconnect(char *pcBuf)
{
    int iRet = 0;
    LTE_LOG(LTE_LOG_INFO, "lte_config_disconnect enter!\n");
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_disconnect)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_config_disconnect ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_disconnect(NULL, pcBuf);
    
    return iRet;
}

LTE_RET_E lte_config_nettype_set(char *pcBuf)
{
    int iRet = 0;
    LTE_LOG(LTE_LOG_INFO, "lte_config_nettype_set enter!\n");
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_net_type_set)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_config_nettype_set ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_net_type_set(NULL, pcBuf);
    
    return iRet;
}
LTE_RET_E lte_config_cent_code_set(char *pcBuf)
{
    int iRet = 0;
    LTE_LOG(LTE_LOG_INFO, "lte_config_cent_code_set enter!\n");
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_sms_cent_code_set)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_config_cent_code_set ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_sms_cent_code_set(NULL, pcBuf);
    
    return iRet;
}
LTE_RET_E lte_config_smsmode_set(char *pcBuf)
{
    int iRet = 0;
    LTE_LOG(LTE_LOG_INFO, "lte_config_smsmode_set enter!\n");
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_sms_mode_set)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_config_smsmode_set ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_sms_mode_set(NULL, pcBuf);
    
    return iRet;
}
LTE_RET_E lte_config_sms_init(char *pcBuf)
{
    int iRet = 0;
    LTE_LOG(LTE_LOG_INFO, "lte_config_sms_init enter!\n");
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_sms_func_init)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_config_sms_init ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_sms_func_init(NULL, pcBuf);
    
    return iRet;
}
LTE_RET_E lte_config_sms_recv(void *pvData, char *pcBuf)
{
    int iRet = 0;
    LTE_SMS_RECV_T stSmsRecv = {0};
    
    LTE_SMS_RECV_T *pstSmsRecv = (LTE_SMS_RECV_T *)pvData;
    LTE_LOG(LTE_LOG_INFO, "lte_config_sms_recv enter!\n");
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_sms_recv)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_config_sms_send ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_sms_recv((void *)(&stSmsRecv), pcBuf);
    strncpy(pstSmsRecv->acSmsRecvFile, stSmsRecv.acSmsRecvFile, sizeof(pstSmsRecv->acSmsRecvFile));
    LTE_LOG(LTE_LOG_INFO, "lte_config_sms_recv:recv file--%s", pstSmsRecv->acSmsRecvFile);
    return iRet;
}
LTE_RET_E lte_config_sms_send(char *pcBuf)
{
    int iRet = 0;
    LTE_LOG(LTE_LOG_INFO, "lte_config_sms_send enter!\n");
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_sms_send)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_config_sms_send ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_sms_send(NULL, pcBuf);
    
    return iRet;
}
LTE_RET_E lte_config_sms_delete(char *pcBuf)
{
    int iRet = 0;
    LTE_LOG(LTE_LOG_INFO, "lte_config_sms_delete enter!\n");
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_sms_del)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_config_sms_delete ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_sms_del(NULL, pcBuf);
    
    return iRet;
}

LTE_RET_E lte_config_pdp_set(char *pcBuf)
{
    int iRet = 0;
    LTE_LOG(LTE_LOG_INFO, "lte_config_pdp_set enter!\n");
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_sms_ip_stack_set)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_config_pdp_set ops func is NULL!");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_sms_ip_stack_set(NULL, pcBuf);
    
    return iRet;
}

/* 配置apn */
LTE_RET_E lte_config_apn_set(char *pcBuf)
{
    int iRet = 0;
    
    if(NULL == g_pstLte_module_ops_t->lte_apn_set)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_config_apn_set ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_apn_set(NULL, pcBuf);

    
    return iRet;
}

LTE_RET_E lte_config_clck_band_set(char *pcBuf)
{
    int iRet = 0;
    LTE_LOG(LTE_LOG_INFO, "lte_config_clck_band_set enter!\n");
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_clck_bands)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_config_clck_band_set ops func is NULL!");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_clck_bands(NULL, pcBuf);
    
    return iRet;
}
LTE_RET_E lte_config_clck_cell_set(char *pcBuf)
{
    int iRet = 0;
    LTE_LOG(LTE_LOG_INFO, "lte_config_clck_cell_set enter!\n");
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_clck_cell)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_config_clck_cell_set ops func is NULL!");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_clck_cell(NULL, pcBuf);
    
    return iRet;
}

LTE_RET_E lte_config_gps_start_set(char *pcBuf)
{
    int iRet = 0;
    LTE_LOG(LTE_LOG_INFO, "lte_config_gps_start_set enter!\n");
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_gps_start)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_config_gps_start_set ops func is NULL!");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_gps_start(NULL, pcBuf);
    
    return iRet;
}

LTE_RET_E lte_config_gps_stop_set(char *pcBuf)
{
    int iRet = 0;
    LTE_LOG(LTE_LOG_INFO, "lte_config_gps_start_set enter!\n");
    /* 从lte com口中获取signal信息 */
    if(NULL == g_pstLte_module_ops_t->lte_gps_stop)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_config_gps_start_set ops func is NULL!");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_gps_stop(NULL, pcBuf);
    
    return iRet;
}

LTE_RET_E lte_config_mac_set(char *pcBuf)
{
    int iRet = 0;
    
    if(NULL == g_pstLte_module_ops_t->lte_mac_set)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_config_apn_set ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_mac_set(NULL, pcBuf);

    return iRet;
}

LTE_RET_E lte_real_sim_unlock(void *pvData, char *pcBuf)
{
    int iRet = 0;
    
    if(NULL == g_pstLte_module_ops_t->lte_sim_unlock)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_config_apn_set ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_sim_unlock(pvData, pcBuf);
    if(LTE_OK == iRet)
    {
        g_stLteStateInfo.iSimUnlockState = 1;
    }
    else
    {
        g_stLteStateInfo.iSimUnlockState = 0;
    }
    g_stLteStateInfo.iSimUnlockStateFlag = 1;

    return iRet;
}

LTE_RET_E lte_real_sim_disable_unlock(char *pcBuf)
{
    int iRet = 0;
    
    if(NULL == g_pstLte_module_ops_t->lte_sim_disable_unlock)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_config_apn_set ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_sim_disable_unlock(NULL, pcBuf);
    if(LTE_OK == iRet)
    {
        g_stLteStateInfo.iSimUnlockState = 0;
    }

    return iRet;
}

LTE_RET_E lte_real_sounet_mode_set(char *pcBuf)
{
    int iRet = 0;
    
    if(NULL == g_pstLte_module_ops_t->lte_sounet_mode_set)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_sounet_mode_set ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_sounet_mode_set(NULL, pcBuf);

    return iRet;
}

LTE_RET_E lte_real_sim_puk_unlock(char *pcBuf)
{
    int iRet = 0;
    
    if(NULL == g_pstLte_module_ops_t->lte_sim_puk_unlock)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_sounet_mode_set ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_sim_puk_unlock(NULL, pcBuf);

    return iRet;
}

LTE_RET_E lte_real_post_route_set(char *pcBuf)
{
    int iRet = 0;
    
    if(NULL == g_pstLte_module_ops_t->lte_post_route_set)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_post_route_set ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_post_route_set(NULL, pcBuf);

    return iRet;
}
LTE_RET_E lte_real_nv_datalock(char *pcBuf)
{
    int iRet = 0;
    
    if(NULL == g_pstLte_module_ops_t->lte_nv_datalock)
    {
        LTE_LOG(LTE_LOG_ERR,"nv_datalock ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_nv_datalock(NULL, pcBuf);

    return iRet;
}

LTE_RET_E lte_real_remove_attach(char *pcBuf)
{
    int iRet = 0;
    
    if(NULL == g_pstLte_module_ops_t->lte_remove_attach)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_remove_attach ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_remove_attach(NULL, pcBuf);

    return iRet;
}

LTE_RET_E lte_real_dload_prot_ver_get(void *pvData, char *pcBuf)
{
    int iRet = 0;

    if(NULL == g_pstLte_module_ops_t->lte_dload_prot_ver_get)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_real_dload_prot_ver_get ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_dload_prot_ver_get(pvData, pcBuf);

    return iRet;
}
LTE_RET_E lte_real_dload_ver_info_get(void *pvData, char *pcBuf)
{
    int iRet = 0;

    if(NULL == g_pstLte_module_ops_t->lte_dload_ver_info_get)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_real_dload_ver_info_get ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_dload_ver_info_get(pvData, pcBuf);

    return iRet;
}
LTE_RET_E lte_real_nv_backup(void *pvData, char *pcBuf)
{
    int iRet = 0;

    if(NULL == g_pstLte_module_ops_t->lte_nv_backup)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_real_nv_backup ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_nv_backup(pvData, pcBuf);

    return iRet;
}
LTE_RET_E lte_real_nv_restore(void *pvData, char *pcBuf)
{
    int iRet = 0;

    if(NULL == g_pstLte_module_ops_t->lte_nv_restore)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_real_nv_restore ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_nv_restore(pvData, pcBuf);

    return iRet;
}
LTE_RET_E lte_real_dload_mode(char *pcBuf)
{
    int iRet = 0;
    
    if(NULL == g_pstLte_module_ops_t->lte_dload_mode)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_dload_mode ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_dload_mode(NULL, pcBuf);

    return iRet;
}
LTE_RET_E lte_real_data_mode(char *pcBuf)
{
    int iRet = 0;
    
    if(NULL == g_pstLte_module_ops_t->lte_data_mode)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_data_mode ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_data_mode(NULL, pcBuf);

    return iRet;
}
LTE_RET_E lte_real_reset(char *pcBuf)
{
    int iRet = 0;
    
    if(NULL == g_pstLte_module_ops_t->lte_reset)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_reset ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_reset(NULL, pcBuf);

    return iRet;
}

LTE_RET_E lte_real_sim_switch_set(char *pcBuf)
{
    int iRet = 0;
    
    if(NULL == g_pstLte_module_ops_t->lte_sim_switch_set)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_sim_switch_set ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_sim_switch_set(NULL, pcBuf);

    return iRet;
}
LTE_RET_E lte_real_sim_switch_get(void *pvData, char *pcBuf)
{
    LTE_SIM_SWITCH_T stSimSwitch = {0};
    
    LTE_SIM_SWITCH_T *pstSimSwitch = (LTE_SIM_SWITCH_T *)pvData;
    if(NULL == pvData)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_sim_switch_get input param is NULL pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_real_sim_switch_get enter!\n");    

    if(NULL == g_pstLte_module_ops_t->lte_sim_switch_get)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_real_eps_qos_get ops func is NULL");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t->lte_sim_switch_get((void *)(&stSimSwitch), pcBuf);
    
    pstSimSwitch->iSimSw = stSimSwitch.iSimSw;
   
    return LTE_OK;
}
LTE_RET_E lte_real_sim_switch_soft_set(char *pcBuf)
{
    int iRet = 0;
    
    if(NULL == g_pstLte_module_ops_t->lte_sim_switch_soft_set)
    {
        LTE_LOG(LTE_LOG_ERR,"lte_real_sim_switch_soft_set ops func is NULL");
        return LTE_FAIL;
    }
    iRet = g_pstLte_module_ops_t->lte_sim_switch_soft_set(NULL, pcBuf);

    return iRet;
}

/* ################################# */
/* ################################# */
/* ###### get lte static info ###### */
/* ################################# */
/* ################################# */
LTE_RET_E lte_info_static_get(int iFuncCmd, LTE_SEND_MSG_T *pstLteSendMsg)
{
    int iRet = 0;
    
    if(NULL == pstLteSendMsg)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_info_real_get input param is NULL param!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_info_real_get enter:FuncCmd:%d!\n", iFuncCmd);
    switch(iFuncCmd)
    {
        case LTE_PIN_STATE:
            iRet = lte_pinstate_info_get(&(pstLteSendMsg->uLteCommand.stPinStateInfo));
            break;
        case LTE_PIN_SW_GET:
            iRet = lte_pinswstate_info_get(&(pstLteSendMsg->uLteCommand.stPinSwInfo));
            break;    
        case LTE_NETTYPE_GET:
            iRet = lte_curnettype_info_get(&(pstLteSendMsg->uLteCommand.stCurNetTypeInfo));
            break;
        case LTE_SIGNAL_GET:
            iRet = lte_sig_info_get(&(pstLteSendMsg->uLteCommand.stSignalInfo));
            break;
        case LTE_POWER_ON:
            //iRet = lte_config_power_on();
            break;
        case LTE_POWER_OFF:
            //iRet = lte_config_power_off();
            break;   
        case LTE_NETWORK_STATUS_GET:
            //iRet = lte_real_nwstatus_get(&(pstLteSendMsg->uLteCommand.stNwStatusInfo), sizeof(pstLteSendMsg->uLteCommand.stNwStatusInfo));
            break;    
        case LTE_LINKSTATE_GET:
            iRet = lte_linkstate_info_get(&(pstLteSendMsg->uLteCommand.stLinkInfo));
            break;      
        case LTE_IMEI_GET:
            iRet = lte_imei_info_get(&(pstLteSendMsg->uLteCommand.stImeiInfo));
            break;  
        case LTE_CELLID_GET:
            iRet = lte_cellid_info_get(&(pstLteSendMsg->uLteCommand.stCellIdInfo));
            break;     
        case LTE_ICCID_GET:
            iRet = lte_iccid_info_get(&(pstLteSendMsg->uLteCommand.stIccidInfo));
            break;  
        case LTE_IMSI_GET:
            iRet = lte_imsi_info_get(&(pstLteSendMsg->uLteCommand.stImsiInfo));
            break;
        case LTE_SIM_STATE_GET:
            iRet = lte_simstate_info_get(&(pstLteSendMsg->uLteCommand.stSimStateInfo));
            break;    
        case LTE_INFO_GET:
            iRet = lte_info_get(&(pstLteSendMsg->uLteCommand.stLteInfo));
            break;     
        case LTE_ACCESS_NET_GET:
            iRet = lte_accessnet_info_get(&(pstLteSendMsg->uLteCommand.stAccessNetInfo));
            break;    
        case LTE_BANDS_GET:
            iRet = lte_bands_info_get(&(pstLteSendMsg->uLteCommand.stBandsInfo));
            break;    
        case LTE_CLCK_CELL_GET:
            iRet = lte_clccellable_info_get(&(pstLteSendMsg->uLteCommand.stClckCellAbleInfo));
            break;    
        case LTE_OPER_INFO_GET:
            iRet = lte_oper_info_get(&(pstLteSendMsg->uLteCommand.stOperInfo));
            break;   
        case LTE_NETTYPE_INFO_GET:
            iRet = lte_nettype_info_get(&(pstLteSendMsg->uLteCommand.stSupNetTypeInfo));
            break;  
        case LTE_SMS_SUPP:
            iRet = lte_supsms_info_get(&(pstLteSendMsg->uLteCommand.stSupSmsInfo));
            break;    
        case LTE_VERSION_GET:
            iRet = lte_version_info_get(&(pstLteSendMsg->uLteCommand.stVersionInfo));
            break;
        case LTE_ENODEBID_GET:
            iRet = lte_enodebid_info_get(&(pstLteSendMsg->uLteCommand.stEnodeBInfo));
            break;
        case LTE_TAC_GET:
            iRet = lte_tac_info_get(&(pstLteSendMsg->uLteCommand.stTacInfo));
            break;
        case LTE_LCID_GET:
            iRet = lte_lcid_info_get(&(pstLteSendMsg->uLteCommand.stLcidInfo));
            break;
        case LTE_ALL_CELLID_GET:
            iRet = lte_allcid_info_get(&(pstLteSendMsg->uLteCommand.stAllcidInfo));
            break;
        case LTE_MOUDLE_MAC_GET:
            iRet = lte_mac_info_get(&(pstLteSendMsg->uLteCommand.stMacInfo));
            break;
        case LTE_SIM_TYPE_GET:
            iRet = lte_sim_type_get(&(pstLteSendMsg->uLteCommand.stSimRandomNum));
            break;
        case LTE_SIM_UNLOCK_GET:
            iRet = lte_sim_unlock_get(&(pstLteSendMsg->uLteCommand.stSimUnlockState));
            break;    
        default :
            LTE_LOG(LTE_LOG_ALERT, "This lte ops cmd[%d] do not exist!", iFuncCmd);
            iRet = LTE_FAIL;
            break;
    }
    
    if(LTE_OK == iRet)
    {
        pstLteSendMsg->iResult = 1;
    }
    else
    {
        pstLteSendMsg->iResult = 0;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_info_static_get:[Result:%d]\n", pstLteSendMsg->iResult);
    return iRet;
}
/* ################################# */
/* ################################# */
/* ####### get lte real info ####### */
/* ################################# */
/* ################################# */

LTE_RET_E lte_info_real_get(int iFuncCmd, LTE_SEND_MSG_T *pstLteSendMsg, char *pcBuf)
{
    int iRet = 0;
    if(NULL == pstLteSendMsg)
    {
        LTE_LOG(LTE_LOG_ERR, "lte_info_real_get input param is null pointer!\n");
        return LTE_FAIL;
    }
    LTE_LOG(LTE_LOG_INFO, "lte_info_real_get enter:FuncCmd:%d!\n", iFuncCmd);

    switch(iFuncCmd)
    {
        case LTE_PIN_STATE:
            iRet = lte_real_pinstate_get(&(pstLteSendMsg->uLteCommand.stPinStateInfo), pcBuf);
            break;
        case LTE_PIN_SWITCH:
            iRet = lte_config_pinsw_set(pcBuf);
            break;    
        case LTE_PIN_SW_GET:
            iRet = lte_real_pinswstate_get(&(pstLteSendMsg->uLteCommand.stPinSwInfo), pcBuf);
            break;    
        case LTE_PIN_UNLOCK:
            iRet = lte_config_pin_unlock(pcBuf);
            break;    
        case LTE_PIN_ALTER:
            iRet = lte_config_pin_alter(pcBuf);
            break;    
        case LTE_NETTYPE_GET:
            iRet = lte_real_supnet_get(&(pstLteSendMsg->uLteCommand.stSupNetTypeInfo), pcBuf);
            break;    
        case LTE_SIGNAL_GET:
            iRet = lte_real_signal_get(&(pstLteSendMsg->uLteCommand.stSignalInfo), pcBuf);
            break;
        case LTE_POWER_ON:
            iRet = lte_config_power_on(pcBuf);
            break;
        case LTE_POWER_OFF:
            iRet = lte_config_power_off(pcBuf);
            break;   
        case LTE_NETTYPE_SET:
            iRet = lte_config_nettype_set(pcBuf);
            break;    
        case LTE_NETWORK_PREPARE:
            iRet = lte_real_network_prepare(&(pstLteSendMsg->uLteCommand.stNwStatusInfo), pcBuf);
            break;    
        case LTE_NETWORK_STATUS_GET:
            iRet = lte_real_nwstatus_get(&(pstLteSendMsg->uLteCommand.stNwStatusInfo), pcBuf);
            break;    
        case LTE_CONNECT:
            iRet = lte_config_connect(pcBuf);
            break;
        case LTE_DISCONNECT:
            iRet = lte_config_disconnect(pcBuf);
            break;    
        case LTE_LINKSTATE_GET:
            iRet = lte_real_linkstate_get(&(pstLteSendMsg->uLteCommand.stLinkInfo), pcBuf);
            break;      
        case LTE_IMEI_GET:
            iRet = lte_real_imei_get(&(pstLteSendMsg->uLteCommand.stImeiInfo), pcBuf);
            break;  
        case LTE_CELLID_GET:
            iRet = lte_real_cellid_get(&(pstLteSendMsg->uLteCommand.stCellIdInfo), pcBuf);
            break;     
        case LTE_ICCID_GET:
            iRet = lte_real_iccid_get(&(pstLteSendMsg->uLteCommand.stIccidInfo), pcBuf);
            break;  
        case LTE_IMSI_GET:
            iRet = lte_real_imsi_get(&(pstLteSendMsg->uLteCommand.stImsiInfo), pcBuf);
            break;
        case LTE_SIM_STATE_GET:
            iRet = lte_real_simstate_get(&(pstLteSendMsg->uLteCommand.stSimStateInfo), pcBuf);
            break;    
        case LTE_INFO_GET:
            iRet = lte_real_lteinfo_get(&(pstLteSendMsg->uLteCommand.stLteInfo), pcBuf);
            break;     
        case LTE_ACCESS_NET_GET:
            iRet = lte_real_accessnet_get(&(pstLteSendMsg->uLteCommand.stAccessNetInfo), pcBuf);
            break;    
        case LTE_BANDS_GET:
            break;    
        case LTE_CLCK_BANDS_SET:
            iRet = lte_config_clck_band_set(pcBuf);
            break;     
        case LTE_CLCK_CELL_GET:
            break;    
        case LTE_CLCK_CELL_SET:
            iRet = lte_config_clck_cell_set(pcBuf);
            break;    
        case LTE_OPER_INFO_GET:
            iRet = lte_real_operinfo_get(&(pstLteSendMsg->uLteCommand.stOperInfo), pcBuf);
            break;   
        case LTE_NETTYPE_INFO_GET:
            break;  
        case LTE_GPS_START:
            iRet = lte_config_gps_start_set(pcBuf);
            break; 
        case LTE_GPS_STOP:
            iRet = lte_config_gps_stop_set(pcBuf);
            break;
        case LTE_SMS_CENT_CODE_SET:
            iRet = lte_config_cent_code_set(pcBuf);
            break;   
        case LTE_SMS_MODE__SET:
            iRet = lte_config_smsmode_set(pcBuf);
            break;    
        case LTE_SMS_FUNC_INIT:
            iRet = lte_config_sms_init(pcBuf);
            break; 
        case LTE_SMS_RECV:
            iRet = lte_config_sms_recv(&(pstLteSendMsg->uLteCommand.stSmsRecvInfo), pcBuf);
            break;  
        case LTE_SMS_SEND:
            iRet = lte_config_sms_send(pcBuf);
            break;
        case LTE_SMS_DELE:
            iRet = lte_config_sms_delete(pcBuf);
            break;    
        case LTE_SMS_SUPP:
            break;  
        case LTE_IPV6_PDP_SET:
            iRet = lte_config_pdp_set(pcBuf);
            break;    
        case LTE_APN_SET:
            iRet = lte_config_apn_set(pcBuf);
            break;    
        case LTE_VERSION_GET:
            iRet = lte_real_modversion_get(&(pstLteSendMsg->uLteCommand.stVersionInfo), pcBuf);
            break;
        case LTE_ENODEBID_GET:
            iRet = lte_real_enodebid_get(&(pstLteSendMsg->uLteCommand.stEnodeBInfo), pcBuf);
            break;
        case LTE_TAC_GET:
            iRet = lte_real_tac_get(&(pstLteSendMsg->uLteCommand.stTacInfo), pcBuf);
            break;  
        case LTE_LCID_GET:
            iRet = lte_real_lcid_get(&(pstLteSendMsg->uLteCommand.stLcidInfo), pcBuf);
            break;   
        case LTE_ALL_CELLID_GET:
            iRet = lte_real_all_cellid_get(&(pstLteSendMsg->uLteCommand.stAllcidInfo), pcBuf);
            break;
        case LTE_MOUDLE_MAC_SET:
            iRet = lte_config_mac_set(pcBuf);
            break; 
        case LTE_MOUDLE_MAC_GET:
            iRet = lte_real_mac_get(&(pstLteSendMsg->uLteCommand.stMacInfo), pcBuf);
            break;
        case LTE_SIM_UNLOCK:
            iRet = lte_real_sim_unlock(&(pstLteSendMsg->uLteCommand.stSimRandomNum), pcBuf);
            break;
        case LTE_SIM_DISABLE_UNLOCK:
            iRet = lte_real_sim_disable_unlock(pcBuf);
            break;
        case LTE_SIM_TYPE_GET:
            iRet = lte_real_sim_type_get(&(pstLteSendMsg->uLteCommand.stSimRandomNum), pcBuf);
            break;
        case LTE_SIM_UNLOCK_GET:
            iRet = lte_real_sim_unlock_get(&(pstLteSendMsg->uLteCommand.stSimUnlockState), pcBuf);
            break;
        case LTE_IP_GATEWAY_DNS_GET:
            iRet = lte_real_ip_gw_dns_get(&(pstLteSendMsg->uLteCommand.stIpGatewayDns), pcBuf);
            break;
        case LTE_ALL_PLMN_MODE_GET:
            iRet = lte_real_all_oper_get(&(pstLteSendMsg->uLteCommand.stAllPlmnMode), pcBuf);
            break;
        case LTE_SOUNET_MODE_SET:
            iRet = lte_real_sounet_mode_set(pcBuf);
            break;
        case LTE_SIM_UNLOCK_PUK:
            iRet = lte_real_sim_puk_unlock(pcBuf);
            break;
        case LTE_PIN_INPUT_REMAIN_GET:
            iRet = lte_real_pin_input_remain_get(&(pstLteSendMsg->uLteCommand.stPinInputRemain), pcBuf);
            break;
        case LTE_POST_ROUTE_SET:
            iRet = lte_real_post_route_set(pcBuf);
            break;
        case LTE_NV_DATALOCK:
            iRet = lte_real_nv_datalock(pcBuf);
            break;
        case LTE_POST_ROUTE_GET:
            iRet = lte_real_post_route_get(&(pstLteSendMsg->uLteCommand.stPostRouteSw), pcBuf);
            break;
        case LTE_REMOVE_ATTACH:
            iRet = lte_real_remove_attach(pcBuf);
            break;
		case LTE_QOS_DATA_SET:
            iRet = lte_real_eps_qos_set(pcBuf);
            break;
		case LTE_QOS_DATA_GET:
            iRet = lte_real_eps_qos_get(&(pstLteSendMsg->uLteCommand.stQosData), pcBuf);
            break;
		case LTE_DLOAD_PROT_VER_GET:
            iRet = lte_real_dload_prot_ver_get(&(pstLteSendMsg->uLteCommand.stDloadProtVer),pcBuf);
            break;
        case LTE_DLOAD_VER_INFO_GET:
            iRet = lte_real_dload_ver_info_get(&(pstLteSendMsg->uLteCommand.stDloadVerInfo),pcBuf);
            break;
        case LTE_NV_BACKUP:
            iRet = lte_real_nv_backup(&(pstLteSendMsg->uLteCommand.stNvBackup),pcBuf);
            break;
        case LTE_NV_RESTORE:
            iRet = lte_real_nv_restore(&(pstLteSendMsg->uLteCommand.stNvRestore),pcBuf);
            break;
        case LTE_DLOAD_MODE:
            iRet = lte_real_dload_mode(pcBuf);
            break;
        case LTE_DATA_MODE:
            iRet = lte_real_data_mode(pcBuf);
            break;
        case LTE_RESET:
            iRet = lte_real_reset(pcBuf);
            break;
        case LTE_SIM_SWITCH_GET:
            iRet = lte_real_sim_switch_get(&(pstLteSendMsg->uLteCommand.stSimSwitch), pcBuf);
            break;
		case LTE_SIM_SWITCH_SET:
            iRet = lte_real_sim_switch_set(pcBuf);
            break;
		case LTE_SIM_SWITCH_SOFT_SET:
            iRet = lte_real_sim_switch_soft_set(pcBuf);
            break;        
        default :
            LTE_LOG(LTE_LOG_ALERT, "This lte ops cmd[%d] do not exist!", iFuncCmd);
            iRet = LTE_FAIL;
            break;
    }
    if(LTE_OK == iRet)
    {
        pstLteSendMsg->iResult = 1;
    }
    else
    {
        pstLteSendMsg->iResult = 0; /* 数据错误 */
    }
    return iRet;
}

/* ################################# */
/* ################################# */
/* #########  devops init  ######### */
/* ################################# */
/* ################################# */
LTE_RET_E lte_module_init(LTE_PLATFORM_E enLtePlatformType, char *pcAtCom, int iLen)
{
    LTE_RET_E enRet;
    if(NULL == pcAtCom)
    {
        return LTE_FAIL;
    }
    switch(enLtePlatformType)
    {
        #ifdef __ZTE_MICRO__
        case M_ZTE_MICRO:
        {
            enRet = zte_micro_B57_5_init(pcAtCom, iLen);
            if (LTE_OK != enRet)
            {
                  LTE_LOG(LTE_LOG_ALERT, "ZTE microelectronics platform init. ret %d", enRet);
                  return enRet;
            }
            LTE_LOG(LTE_LOG_INFO, "ZTE microelectronics platform init success");
        }
        break;
        #endif
        #ifdef __ARCCRA__
        case M_ARCCRA:
        {
            enRet = arccra_init(pcAtCom, iLen);
            if (LTE_OK != enRet)
            {
                  LTE_LOG(LTE_LOG_ALERT, "arccra microelectronics platform init. ret %d", enRet);
                  return enRet;
            }
            LTE_LOG(LTE_LOG_INFO, "arccra microelectronics platform init success");
            
            break;
        }
        case M_ARCCRA_SECOND:
        {
            enRet = arccra_init(pcAtCom, iLen);
            if (LTE_OK != enRet)
            {
                  LTE_LOG(LTE_LOG_ALERT, "arccra 4 mode microelectronics platform init. ret %d", enRet);
                  return enRet;
            }
            LTE_LOG(LTE_LOG_INFO, "arccra 4 mode microelectronics platform init success");
            
            break;
        }
        #endif

        #ifdef __LONGCHEER__
        case M_LONGCHEER:
        {
            enRet = longcheer_init(pcAtCom, iLen);
            if (LTE_OK != enRet)
            {
                LTE_LOG(LTE_LOG_ALERT, "longcheer platform init. ret %d", enRet);
                return enRet;
            }
            LTE_LOG(LTE_LOG_INFO, "longcheer platform init success");
            break;
        }
        #endif

        #ifdef __LEADCORE__
        case M_LEADCORE:
        {
            enRet = leadcore_init(pcAtCom, iLen);
        	if (LTE_OK != enRet)
        	{
        		LTE_LOG(LTE_LOG_ALERT, "leadcore platform init. ret %d", enRet);
        		return enRet;
        	}
            LTE_LOG(LTE_LOG_INFO, "leadcore platform init success");
            break;
        }
        #endif

        #ifdef __YUGA__
        case M_YUGA:
        {
            enRet = yuga_init(pcAtCom, iLen);
        	if (LTE_OK != enRet)
        	{
        		LTE_LOG(LTE_LOG_ALERT, "yuga platform init. ret %d", enRet);
        		return enRet;
        	}
            LTE_LOG(LTE_LOG_INFO, "yuga platform init success");
            break;
        }
        #endif

        #ifdef __HUAWEI__
        case M_HUAWEI:
        {
            enRet = huawei_init(pcAtCom, iLen);
        	if (LTE_OK != enRet)
        	{
        		LTE_LOG(LTE_LOG_ALERT, "huawei platform init. ret %d", enRet);
        		return enRet;
        	}
            LTE_LOG(LTE_LOG_INFO, "huawei platform init success");
            break;
        }
        #endif

        #ifdef __MARVELL__
        case M_MARVELL:
        {
            enRet = marvell_init(pcAtCom, iLen);
        	if (LTE_OK != enRet)
        	{
        		LTE_LOG(LTE_LOG_ALERT, "marvell platform init. ret %d", enRet);
        		return enRet;
        	}
            LTE_LOG(LTE_LOG_INFO, "marvell platform init success");
            break;
        }
        #endif
        
        #ifdef __HUAWEI4G__
        case M_HUAWEI4G_NETCARD:
        {
            enRet = huawei4G_init(pcAtCom, iLen);
        	if (LTE_OK != enRet)
        	{
        		LTE_LOG(LTE_LOG_ALERT, "huawei4G platform init. ret %d", enRet);
        		return enRet;
        	}
            LTE_LOG(LTE_LOG_INFO, "huawei4G platform init success");
            break;
        }
        #endif
        default:
        	LTE_LOG(LTE_LOG_ALERT, "unkown module platform ");
            return LTE_FAIL;
            break;
    }
    return enRet;
}



