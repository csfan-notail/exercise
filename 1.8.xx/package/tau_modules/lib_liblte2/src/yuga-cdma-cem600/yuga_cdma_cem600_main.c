#include <stdio.h>

#include "lte.h"
#include "lte_log.h"
#include "lte_common.h"

extern LTE_IOCTL_FUNC_T astyugacem600Table[];

LTE_RET_E yuga_cdma_cem600_init(void)
{
    LTE_RET_E enRet;

    enRet = lte_ioctl_register(astyugacem600Table);
    if (LTE_OK != enRet)
    {
        LTE_LOG(LTE_LOG_ERR, " LTE register ioctl table failed.");
        return enRet;
    }

    return LTE_OK;
}

