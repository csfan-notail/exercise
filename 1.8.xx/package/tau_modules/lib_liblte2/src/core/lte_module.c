#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libusb.h"

#include "lte.h"
#include "lte_log.h"
 
static int g_iLteInitStatus = 0;

LTE_DEVICE_INFO g_acDevice[M_TOTAL_NUM] = {
    {M_LONGCHEER, 0x1c9e, 0x9b05, LTE_V33, "longcheer"}, /*龙旗平台*/
    {M_LEADCORE, 0x1ab7, 0x1761, LTE_V38, "leadcore"}, /* 联芯平台 */
    {M_THINKWILL, 0x19f5, 0x9013, LTE_V33, "thinkwill"}, /* 信可平台  */
    {M_ALTAIR, 0x216f, 0x0043, LTE_V33, "altair"}, /* 德明平台 */
    {M_ZTE, 0x19d2, 0x0117, LTE_V33, "zte"}, /* 中兴平台 */
    {M_ZTE_CDMA2000, 0x19d2, 0xffed, LTE_V33, "zte-cdma2000"}, /* 中兴CDMA2000平台 */
    {M_ARCCRA, 0x29e0, 0x0303, LTE_V33, "arccra"}, /* 乾润平台 */
    {M_THINKWILL_CDMA_MI660, 0x19f5, 0x9909, LTE_V33, "thinkwill-mi660"}, /* 信可cdma mi660平台 */
    {M_YUGA_CDMA_CEM600, 0x257a, 0x1601, LTE_V33, "yuga-cem600"}, /* 域格cdma cem600平台 */
    {M_ARCCRA_SECOND, 0x29e0, 0x0304, LTE_V33, "arccra-4-mode"}, /* 乾润4模平台 */
    {M_ZTE_MICRO, 0x19d2, 0x0199, LTE_V33, "zte-microelectronics"}, /* 中兴微电子 */
    {M_HUAWEI, 0x12d1, 0x1c25, LTE_V33, "huawei_wcdma"}, /* 华为 */
    {M_YUGA, 0x05c6, 0x9025, LTE_V33, "yuga 4g"}, /* 域格4g平台 */
    {M_MARVELL, 0x1286, 0x4e31, LTE_V33, "marvell"}, /* 马维尔平台 */ 
    {M_HUAWEI4G_STORAGE, 0x12d1, 0x14fe, LTE_V33, "huawei4G_storage"}, /* 华为4G平台 */
    {M_HUAWEI4G_NETCARD, 0x12d1, 0x1506, LTE_V33, "huawei4G"}, /* 华为4G平台 */ 
};

LTE_PLATFORM_E find_module_platform(struct libusb_device_descriptor *pstDesc)
{
	int loop = 0;
	
	for (loop = 0; loop < M_TOTAL_NUM; loop++)
	{
		if(pstDesc->idVendor == g_acDevice[loop].stDeviceDesc.usVendor && 
		   pstDesc->idProduct == g_acDevice[loop].stDeviceDesc.usProduct){

		   	return g_acDevice[loop].enDeviceNum;
		}
	}
	return M_TOTAL_NUM;
}

int get_module_platform(void)
{
	
	libusb_device **devs, *dev;
	struct libusb_device_descriptor desc;
	int ret, moduleNum, loop;
	ssize_t cnt;

	ret = libusb_init(NULL);
	if (ret < 0)
		return M_TOTAL_NUM;

	cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0){
		libusb_exit(NULL);
		return M_TOTAL_NUM;
	}

	loop = 0;
	while ((dev = devs[loop++]) != NULL) {
		
		ret = libusb_get_device_descriptor(dev, &desc);
		if (ret < 0) {
			LTE_LOG(LTE_LOG_ALERT, "failed to get device descriptor");
			return M_TOTAL_NUM;
		}
		moduleNum = find_module_platform(&desc);
		if (moduleNum < M_TOTAL_NUM)
			break;
	}
	
	
	libusb_free_device_list(devs, 1);
	libusb_exit(NULL);
	return moduleNum;
}

LTE_RET_E lte_device_info_get(LTE_DEVICE_INFO *pstDevInfo)
{
	if (pstDevInfo == NULL)
		return LTE_FAIL;
	
    	int iModule = M_TOTAL_NUM;

    	/*获取出来的信息和全局的结构体比较，确认平台*/
    	iModule = get_module_platform();

    	if (iModule < M_TOTAL_NUM){
		memcpy(pstDevInfo, &(g_acDevice[iModule]), sizeof(LTE_DEVICE_INFO));
	}else{
		LTE_LOG(LTE_LOG_ERR, "unkown lte module, cannot get device info");
		return LTE_FAIL;
	}

    	return LTE_OK;
}


