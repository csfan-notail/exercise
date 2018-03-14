/******************************************************************************

                  版权所有 (C), 2001-2011, 上海寰创通信股份有限公司

 ******************************************************************************
  文 件 名   : pal_data.h
  版 本 号   : 初稿
  作    者   : 吴文钊
  生成日期   : 2012年9月11日 星期一
  最近修改   :
  功能描述   : 
  函数列表   :
  修改历史   :
  1.日    期   : 2012年9月11日 星期一
    作    者   : 吴文钊
    修改内容   : 创建文件

  2.日    期   : 2012年9月11日 星期三
    作    者   : 吴文钊
    修改内容   : 增加nvram宏定义
******************************************************************************/

#ifndef __AP_CONF_H__
#define __AP_CONF_H__


/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
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

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 基础类型定义                                 *
 *----------------------------------------------*/
 
/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
 
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
/* nvram宏定义 */
#define NVRAM_USERNAME               "UserName"               //登录用户名
#define NVRAM_WEBPASSWORD            "webPassword"            //登录密码
#define NVRAM_SYSDEVICENAME          "sysDeviceName"          //设备名
#define NVRAM_NETWANIPADDR           "netWanIpAddr"           //WAN口ip地址
#define NVRAM_NETWANNETMASK          "netWanNetMask"          //WAN口子网掩码
#define NVRAM_NETWANDEFAULTGW        "netWanDefaultGw"        //WAN口网关地址
#define NVRAM_WANPRIMARYDNS          "WanPrimaryDns"          //WAN口主DNS服务器
#define NVRAM_WANSECONDARYDNS        "WanSecondaryDns"        //WAN口备DNS服务器
#define NVRAM_SYSLANIPADDR           "sysLanIpAddr"           //LAN口ip地址
#define NVRAM_SYSLANNETMASK          "sysLanNetMask"          //LAN口子网掩码
#define NVRAM_SYSLANDEFAULTGW        "sysLanDefaultGw"        //LAN口网关地址
#define NVRAM_DHCPSTARTIPADDR        "dhcpStartIpAddr"        //dhcp地址池起始地址
#define NVRAM_DHCPENDIPADDR          "dhcpEndIpAddr"          //dhcp地址池结束地址
#define NVRAM_DHCPNETMASK            "dhcpNetMask"            //dhcp分配子网掩码
#define NVRAM_DHCPDEFAULTGW          "dhcpDefaultGw"          //dhcp分配网关
#define NVRAM_DHCPPRIMARYDNS         "dhcpPrimaryDns"         //dhcp分配主DNS服务器
#define NVRAM_DHCPSECONDARYDNS       "dhcpSecondaryDns"       //dhcp分配备DNS服务器
#define NVRAM_DHCPLEASE              "dhcpLease"              //dhcp有效时长
#define NVRAM_WIFIABGMODE            "wifiAbgMode"            //无线802.11模式
#define NVRAM_WIFISHORTGI            "wifiShortGi"            //SGI功能开关
#define NVRAM_WIFIAMPDU              "wifiAmpdu"              //A-MPDU功能开关
#define NVRAM_WIFIAMSDU              "wifiAmsdu"              //A-MSDU功能开关
#define NVRAM_WIFIMODULECHANNELLIST  "wifiModuleChannelList"  //信道列表
#define NVRAM_WIFICURTXPOWER         "wifiCurTxPower"         //发射功率
#define NVRAM_FLOWCTRLSWITCH         "flowCtrlSwitch"         //流控开关
#define NVRAM_FLOWENDUSERMAXUPLOAD   "flowEndUserMaxUpload"   //流控上传流量
#define NVRAM_FLOWENDUSERMAXDOWNLOAD "flowEndUserMaxDownload" //流控下载流量
#define NVRAM_L2TPSWITCH             "l2tpSwitch"             //l2tp控制开关, ADD BY LYK

/* BEGIN: Added by lvjianlin, 2013/6/18   PN:lte-fi dns */
#define NVRAM_ACDNSCONFIG            "acdnsconfig"            //config区域域名
#define NVRAM_ACDNSVALUE             "acdnsvalue"             //有效域名
#define NVRAM_ACDNSVALID             "acdnsvalid"             //下标标识
/* END:   Added by lvjianlin, 2013/6/18   PN:lte-fi dns */
#if 0
//新加入begin
WifiLinkState//连接状态
WifiPower//信号强度
CommunSystem//通信制式
LteApn//APN配置
LtePlmn//PLMN状态

LteSwitch=0//接入网络开关
LteAutoSwitch=0//自动接入开关
LteApnSwitch=//接入APN
//新加入end

UserName=admin//登录用户名
webPassword=admin//登录密码
sysDeviceName=cpe//设备名
netWanIpAddr=192.168.1.5//WAN口ip地址
netWanNetMask=255.255.255.0//WAN口子网掩码
netWanDefaultGw=192.168.1.254//WAN口网关地址
sysUpLinkSwitch=0//无
sysUpLinkIP1=192.168.1.253//无
sysUpLinkIP2=192.168.1.254//无
sysUpLinkInterval=10//无
sysUpLinkTimeoutNum=0//无
sysUpLinkTimeoutop1=1//无
sysUpLinkTimeoutop2=1//无
sysUpLinkTimeoutop3=1//无
sysUpLinkTimeoutop4=1//无
sysNetMode=0//无
sysLanIpAddr=192.168.1.1//LAN口ip地址
sysLanNetMask=255.255.255.0//LAN口子网掩码
sysLanDefaultGw=192.168.1.254//LAN口网关地址
dhcpStartIpAddr=192.168.1.100//dhcp地址池起始地址
dhcpEndIpAddr=192.168.1.200//dhcp地址池结束地址
dhcpNetMask=255.255.255.0//dhcp分配子网掩码
dhcpDefaultGw=192.168.1.1//dhcp分配网关
dhcpPrimaryDns=192.168.1.1//dhcp分配主DNS服务器
新加//dhcpSecondaryDns=192.168.1.1//dhcp分配备DNS服务器
dhcpLease=864000//dhcp有效时长
sysWanIpMode=0//
WanPrimaryDns=0.0.0.0//WAN口主DNS服务器
WanSecondaryDns=0.0.0.0//WAN口备DNS服务器
pppoe_username= //
pppoe_passwd= //
wifi0AbgMode=6//无线802.11模式
wifi0CwmMode=1//
wifi0ShortGi=1//SGI功能开关
wifi0Ampdu=1//A-MPDU功能开关
wifi0Amsdu=1//A-MSDU功能开关
wifi0ChannelMode=1//无
wifi0ModuleChannelSet=0// 
wifi0ModuleChannelList=1,2,3,4,5,6,7,8,9,10,11,12,13//信道列表
wifi0CurTxPower=20//发射功率
wifi0BeaconInterval=100//无
wifi0DtimInterval=1//无
wifi0FragmentationThreshold=2346//无
wifi0RtsThreshold=2346//无
wifi0CtsProtectionMode=0//无
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
flowCtrlSwitch=1//流控开关
flowEndUserMaxUpload=0//流控上传流量
flowEndUserMaxDownload=512//流控下载流量
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
 * 枚举定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 结构定义                                     *
 *----------------------------------------------*/

#endif
