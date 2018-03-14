#include <stdio.h>

#include "lte.h"
#include "lte_log.h"
#include "lte_common.h"

extern LTE_IOCTL_FUNC_T astThinkWillMi900Table[];

LTE_RET_E thinkwill_wcdma_mi900_init(void)
{
    LTE_RET_E enRet;

    enRet = lte_ioctl_register(astThinkWillMi900Table);
    if (LTE_OK != enRet)
    {
        LTE_LOG(LTE_LOG_ERR, " LTE register ioctl table failed.");
        return enRet;
    }

    return LTE_OK;
}

