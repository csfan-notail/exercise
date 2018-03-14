#include <stdio.h>

#include "lte.h"
#include "lte_log.h"
#include "lte_common.h"

extern LTE_IOCTL_FUNC_T astYugaCLM920Table[];

LTE_RET_E yuga_init(void)
{
    LTE_RET_E enRet;

    enRet = lte_ioctl_register(astYugaCLM920Table);
    if (LTE_OK != enRet)
    {
        LTE_LOG(LTE_LOG_ERR, " LTE register ioctl table failed.");
        return enRet;
    }

    return LTE_OK;
}

