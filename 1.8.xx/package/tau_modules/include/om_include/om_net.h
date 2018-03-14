#ifndef __OM_NET_H__
#define __OM_NET_H__

#include "ap_types.h"




typedef struct
{
	UINT32 uiLinkPort1;
	UINT32 uiLinkPort2;
	UINT32 uiLinkPort3;
	UINT32 uiLinkPort4;
}OM_NET_LINK_STA;


int om_net_lan_link_status(OM_NET_LINK_STA *pstPalLinkSta);


#endif
