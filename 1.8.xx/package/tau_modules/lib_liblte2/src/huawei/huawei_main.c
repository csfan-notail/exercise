#include <stdio.h>

#include "lte.h"
#include "lte_log.h"
#include "lte_common.h"

extern LTE_IOCTL_FUNC_T astHuaweiTable[];

LTE_RET_E huawei_init(void)
{
    LTE_RET_E enRet;

    enRet = lte_ioctl_register(astHuaweiTable);
    if (LTE_OK != enRet)
    {
        LTE_LOG(LTE_LOG_ERR, " LTE register ioctl table failed.");
        return enRet;
    }

    return LTE_OK;
}

