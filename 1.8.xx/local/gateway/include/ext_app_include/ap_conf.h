/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, �Ϻ�徴�ͨ�Źɷ����޹�˾

 ******************************************************************************
  �� �� ��   : pal_data.h
  �� �� ��   : ����
  ��    ��   : ������
  ��������   : 2012��9��11�� ����һ
  ����޸�   :
  ��������   : 
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��9��11�� ����һ
    ��    ��   : ������
    �޸�����   : �����ļ�

  2.��    ��   : 2012��9��11�� ������
    ��    ��   : ������
    �޸�����   : ����nvram�궨��
******************************************************************************/

#ifndef __AP_CONF_H__
#define __AP_CONF_H__


/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
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
 * �������Ͷ���                                 *
 *----------------------------------------------*/
 
/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
 
/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
/* nvram�궨�� */
#define NVRAM_USERNAME               "UserName"               //��¼�û���
#define NVRAM_WEBPASSWORD            "webPassword"            //��¼����
#define NVRAM_SYSDEVICENAME          "sysDeviceName"          //�豸��
#define NVRAM_NETWANIPADDR           "netWanIpAddr"           //WAN��ip��ַ
#define NVRAM_NETWANNETMASK          "netWanNetMask"          //WAN����������
#define NVRAM_NETWANDEFAULTGW        "netWanDefaultGw"        //WAN�����ص�ַ
#define NVRAM_WANPRIMARYDNS          "WanPrimaryDns"          //WAN����DNS������
#define NVRAM_WANSECONDARYDNS        "WanSecondaryDns"        //WAN�ڱ�DNS������
#define NVRAM_SYSLANIPADDR           "sysLanIpAddr"           //LAN��ip��ַ
#define NVRAM_SYSLANNETMASK          "sysLanNetMask"          //LAN����������
#define NVRAM_SYSLANDEFAULTGW        "sysLanDefaultGw"        //LAN�����ص�ַ
#define NVRAM_DHCPSTARTIPADDR        "dhcpStartIpAddr"        //dhcp��ַ����ʼ��ַ
#define NVRAM_DHCPENDIPADDR          "dhcpEndIpAddr"          //dhcp��ַ�ؽ�����ַ
#define NVRAM_DHCPNETMASK            "dhcpNetMask"            //dhcp������������
#define NVRAM_DHCPDEFAULTGW          "dhcpDefaultGw"          //dhcp��������
#define NVRAM_DHCPPRIMARYDNS         "dhcpPrimaryDns"         //dhcp������DNS������
#define NVRAM_DHCPSECONDARYDNS       "dhcpSecondaryDns"       //dhcp���䱸DNS������
#define NVRAM_DHCPLEASE              "dhcpLease"              //dhcp��Чʱ��
#define NVRAM_WIFIABGMODE            "wifiAbgMode"            //����802.11ģʽ
#define NVRAM_WIFISHORTGI            "wifiShortGi"            //SGI���ܿ���
#define NVRAM_WIFIAMPDU              "wifiAmpdu"              //A-MPDU���ܿ���
#define NVRAM_WIFIAMSDU              "wifiAmsdu"              //A-MSDU���ܿ���
#define NVRAM_WIFIMODULECHANNELLIST  "wifiModuleChannelList"  //�ŵ��б�
#define NVRAM_WIFICURTXPOWER         "wifiCurTxPower"         //���书��
#define NVRAM_FLOWCTRLSWITCH         "flowCtrlSwitch"         //���ؿ���
#define NVRAM_FLOWENDUSERMAXUPLOAD   "flowEndUserMaxUpload"   //�����ϴ�����
#define NVRAM_FLOWENDUSERMAXDOWNLOAD "flowEndUserMaxDownload" //������������
#define NVRAM_L2TPSWITCH             "l2tpSwitch"             //l2tp���ƿ���, ADD BY LYK

/* BEGIN: Added by lvjianlin, 2013/6/18   PN:lte-fi dns */
#define NVRAM_ACDNSCONFIG            "acdnsconfig"            //config��������
#define NVRAM_ACDNSVALUE             "acdnsvalue"             //��Ч����
#define NVRAM_ACDNSVALID             "acdnsvalid"             //�±��ʶ
/* END:   Added by lvjianlin, 2013/6/18   PN:lte-fi dns */
#if 0
//�¼���begin
WifiLinkState//����״̬
WifiPower//�ź�ǿ��
CommunSystem//ͨ����ʽ
LteApn//APN����
LtePlmn//PLMN״̬

LteSwitch=0//�������翪��
LteAutoSwitch=0//�Զ����뿪��
LteApnSwitch=//����APN
//�¼���end

UserName=admin//��¼�û���
webPassword=admin//��¼����
sysDeviceName=cpe//�豸��
netWanIpAddr=192.168.1.5//WAN��ip��ַ
netWanNetMask=255.255.255.0//WAN����������
netWanDefaultGw=192.168.1.254//WAN�����ص�ַ
sysUpLinkSwitch=0//��
sysUpLinkIP1=192.168.1.253//��
sysUpLinkIP2=192.168.1.254//��
sysUpLinkInterval=10//��
sysUpLinkTimeoutNum=0//��
sysUpLinkTimeoutop1=1//��
sysUpLinkTimeoutop2=1//��
sysUpLinkTimeoutop3=1//��
sysUpLinkTimeoutop4=1//��
sysNetMode=0//��
sysLanIpAddr=192.168.1.1//LAN��ip��ַ
sysLanNetMask=255.255.255.0//LAN����������
sysLanDefaultGw=192.168.1.254//LAN�����ص�ַ
dhcpStartIpAddr=192.168.1.100//dhcp��ַ����ʼ��ַ
dhcpEndIpAddr=192.168.1.200//dhcp��ַ�ؽ�����ַ
dhcpNetMask=255.255.255.0//dhcp������������
dhcpDefaultGw=192.168.1.1//dhcp��������
dhcpPrimaryDns=192.168.1.1//dhcp������DNS������
�¼�//dhcpSecondaryDns=192.168.1.1//dhcp���䱸DNS������
dhcpLease=864000//dhcp��Чʱ��
sysWanIpMode=0//
WanPrimaryDns=0.0.0.0//WAN����DNS������
WanSecondaryDns=0.0.0.0//WAN�ڱ�DNS������
pppoe_username= //
pppoe_passwd= //
wifi0AbgMode=6//����802.11ģʽ
wifi0CwmMode=1//
wifi0ShortGi=1//SGI���ܿ���
wifi0Ampdu=1//A-MPDU���ܿ���
wifi0Amsdu=1//A-MSDU���ܿ���
wifi0ChannelMode=1//��
wifi0ModuleChannelSet=0// 
wifi0ModuleChannelList=1,2,3,4,5,6,7,8,9,10,11,12,13//�ŵ��б�
wifi0CurTxPower=20//���书��
wifi0BeaconInterval=100//��
wifi0DtimInterval=1//��
wifi0FragmentationThreshold=2346//��
wifi0RtsThreshold=2346//��
wifi0CtsProtectionMode=0//��
wifi1AbgMode=6//
wifi1CwmMode=1//
wifi1ShortGi=1//
wifi1Ampdu=1//
wifi1Amsdu=1//
wifi1ChannelMode=1//
wifi1ModuleChannelSet=0//
wifi1ModuleChannelList=149,153,157,161,165//
wifi1CurTxPower=20//
wifi1BeaconInterval=100//
wifi1DtimInterval=1//
wifi1FragmentationThreshold=2346//
wifi1RtsThreshold=2346//
wifi1CtsProtectionMode=0//
wifiVapNum=2//
wifiVapList=1,2//
vap1.RadioId=1
vap1.WlanId=0
vap1.WorkMode=2
vap1.Ssid=managment
vap1.BroadcastSsid=1
vap1.PureN=0
vap1.AuthenticationType=1
vap1.SecurityMode=0
vap1.WpaAlgorithm=1
vap1.WepKey=0123456789
vap1.WpaPsk=wpa_shared_key
vap1.WepKeyLen=1
vap1.WpaRekeyPeriod=3600
vap1.WepKeyIndex=1
vap1.WepInputFormat=1
vap2.RadioId=2
vap2.WlanId=0
vap2.WorkMode=2
vap2.Ssid=managment
vap2.BroadcastSsid=1
vap2.PureN=0
vap2.AuthenticationType=1
vap2.SecurityMode=0
vap2.WpaAlgorithm=1
vap2.WepKey=0123456789
vap2.WpaPsk=wpa_shared_key
vap2.WepKeyLen=1
vap2.WpaRekeyPeriod=3600
vap2.WepKeyIndex=1
vap2.WepInputFormat=1
flowCtrlSwitch=1//���ؿ���
flowEndUserMaxUpload=0//�����ϴ�����
flowEndUserMaxDownload=512//������������
factory_clean=0//
passwd=ieee802.11//
module=//
softver=//
mtdblock5=test//
mtdblock4=test//
targetver=4//
boot_success=0//
bootnum=9//
NVRAM size: 2109/65532 bytes
#endif
/*----------------------------------------------*
 * ö�ٶ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ṹ����                                     *
 *----------------------------------------------*/

#endif
