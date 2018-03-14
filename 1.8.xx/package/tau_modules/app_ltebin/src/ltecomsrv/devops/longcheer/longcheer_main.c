#include <stdio.h>
#include <string.h>

#include "lte_com.h"
#include "lte_log.h"


#define ATCOM "/dev/ttyUSB2"

extern LTE_MODULE_OPS_T stLte_longcheer_ops_t;
extern LTE_MODULE_OPS_T *g_pstLte_module_ops_t;

LTE_RET_E longcheer_init(char *pcAtCom, int iLen)
{
    if(NULL == pcAtCom)
    {
        LTE_LOG(LTE_LOG_ALERT, "input param is NULL pointer!");
        return LTE_FAIL;
    }
    g_pstLte_module_ops_t = &stLte_longcheer_ops_t;

    strncpy(pcAtCom, ATCOM, iLen);

    return LTE_OK;
}

