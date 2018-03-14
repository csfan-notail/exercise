#ifndef __VPN_CONTROL_H__
#define __VPN_CONTROL_H__


typedef enum
{
    VPN_CONNECT_OFF = 0,
    VPN_CONNECT_ON
}VPN_CONNECT_SWITCH_E;


#define VPN_NAME "lianlianvpn"

int vpn_control_xl2tpd(VPN_L2TP_CONTROL_INFO_T stVpnControl);
int vpn_control_xl2tpd_init(void);
int vpn_pass_set(VPN_PASS_CONTROL_INFO_T *t);



#endif

