
#include "om_include/om_net.h"
#include "reg_ioctl.h"
#include "ap_types.h"


int om_net_lan_link_reg_get( UINT32 uiPhyPortNum )
{
	INT32 uiRegBit = 0;
	INT32 enRet = 0;
	REGISTER_IOCTL_T stIoctl = {0};
	
	stIoctl.stRegData.enRegType = REGISTER_IOCTL_ADDR_SWITCH;
    stIoctl.stRegData.uiRegAddr = uiPhyPortNum;
    enRet = register_ioctl(&stIoctl, REGISTER_IOCTL_READ);
	if (AP_E_NONE == enRet)
    {
    	if( 0 == stIoctl.stRegData.uiRetValu )
        {
        	uiRegBit = ( stIoctl.stRegData.uiRegValu >> 8 )& 0x01 ;
        	if( 1 == uiRegBit )
        	{
        		return 1;
        	}
            //App_log(LOG_NOTICE,"regcmd switch read/write\n");
        }
    }
    else
    {
        App_log(LOG_ERR,"Call register_ioctl failed !\n");
    }
    
	return 0;	
}	
	
int om_net_lan_link_status( OM_NET_LINK_STA *pstPalLinkSta )
{
	
	if( NULL == pstPalLinkSta )
	{
		App_log(LOG_ERR," *ptPalLinkSta failed NULL !\n");
		return AP_E_DEFAULT_FAIL;
	}
	
	pstPalLinkSta->uiLinkPort1 = om_net_lan_link_reg_get(REG_SWITCH_PHY_PORT_1);
	pstPalLinkSta->uiLinkPort2 = om_net_lan_link_reg_get(REG_SWITCH_PHY_PORT_2);
	pstPalLinkSta->uiLinkPort3 = om_net_lan_link_reg_get(REG_SWITCH_PHY_PORT_3);
	pstPalLinkSta->uiLinkPort4 = om_net_lan_link_reg_get(REG_SWITCH_PHY_PORT_4);
	
	return 0;
}