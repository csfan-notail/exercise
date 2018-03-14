
#include "lte_log.h"

//#define USE_PRIVATE_LOGLEVEL
int g_iLteLogLevel;

void lte_log_set(int iLogLevel)
{
#ifdef USE_PRIVATE_LOGLEVEL
    g_iLteLogLevel = LTE_LOG_WARNING;
#else
    g_iLteLogLevel = iLogLevel;
#endif
    return;
}

int lte_log_get(void)
{
    return g_iLteLogLevel;
}

