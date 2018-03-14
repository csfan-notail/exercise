#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libusb.h"

#include "lte.h"
#include "lte_common.h"
#include "lte_log.h"
 
static int g_iLteInitStatus = 0;
int g_moduleID = M_TOTAL_NUM;

int lte_is_init(void)
{
    return g_iLteInitStatus;
}

void lte_init_set(void)
{
    g_iLteInitStatus = 1;
}

void lte_init_clear(void)
{
    g_iLteInitStatus = 0;
}

LTE_RET_E lte_core_init(void)
{
    LTE_RET_E enRet;
    
    LTE_LOG(LTE_LOG_INFO, "Init LTE core");

    enRet = lte_mutex_init();
    if (LTE_OK != enRet)
    {
        LTE_LOG(LTE_LOG_ALERT, "lte_mutex_init failed. ret %d", enRet);
        return enRet;
    }
    return LTE_OK;
}

LTE_RET_E lte_init(int iLogLevel)
{
    LTE_RET_E enRet;
    int iModule = M_TOTAL_NUM;

    if (lte_is_init())
    {
        LTE_LOG(LTE_LOG_INFO, "LTE module is initialized before.");
        return LTE_OK;
    }

    lte_log_set(iLogLevel);
    
    /*lte core初始化*/
    enRet = lte_core_init();
    if (LTE_OK != enRet)
    {
        LTE_LOG(LTE_LOG_ALERT, "lte_core_init failed. ret %d", enRet);
        return enRet;
    }   
    /*各模块初始化*/
    LTE_LOG(LTE_LOG_INFO, "Init LTE modules\n");
    /*lte 模块初始化*/

    /*获取出来的信息和全局的结构体比较，确认平台*/
    iModule = get_module_platform();
	g_moduleID = iModule;

    switch(iModule){
#ifdef __LONGCHEER__
	  case M_LONGCHEER:
	  {
		enRet = longcheer_init();
        	if (LTE_OK != enRet)
        	{
            		LTE_LOG(LTE_LOG_ALERT, "longcheer platform init. ret %d", enRet);
            		return enRet;
        	}
		LTE_LOG(LTE_LOG_INFO, "longcheer platform init success");
	  }
	  break;
#endif

#ifdef __LEADCORE__
	  case M_LEADCORE:
	  {
		enRet = leadcore_init();
        	if (LTE_OK != enRet)
        	{
            		LTE_LOG(LTE_LOG_ALERT, "leadcore platform init. ret %d", enRet);
            		return enRet;
        	}
		LTE_LOG(LTE_LOG_INFO, "leadcore platform init success");
	  }
	  break;
#endif

#ifdef __THINKWILL__
	  case M_THINKWILL:
	  {
		enRet = thinkwill_wcdma_mi900_init();
        	if (LTE_OK != enRet)
        	{
            		LTE_LOG(LTE_LOG_ALERT, "thinkwill platform init. ret %d", enRet);
            		return enRet;
        	}
		LTE_LOG(LTE_LOG_INFO, "thinkwill platform init success");
	  }
	  break;
#endif

#ifdef __ALTAIR__
	  case M_ALTAIR:
	  {
		enRet = altair_init();
        	if (LTE_OK != enRet)
        	{
            		LTE_LOG(LTE_LOG_ALERT, "altair platform init. ret %d", enRet);
            		return enRet;
        	}
		LTE_LOG(LTE_LOG_INFO, "altair platform init success");
	  }
	  break;
#endif

#ifdef __ZTE__
	  case M_ZTE:
	  {
		enRet = zte_wcdma_mf210_init();
        	if (LTE_OK != enRet)
        	{
            		LTE_LOG(LTE_LOG_ALERT, "zte platform init. ret %d", enRet);
            		return enRet;
        	}
		LTE_LOG(LTE_LOG_INFO, "zte platform init success");
	  }
	  break;
#endif
#ifdef __ZTE_CDMA2000__
        case M_ZTE_CDMA2000:
        {
          enRet = zte_cdma2000_mc2716_init();
              if (LTE_OK != enRet)
              {
                      LTE_LOG(LTE_LOG_ALERT, "zte cdma2000 platform init. ret %d", enRet);
                      return enRet;
              }
          LTE_LOG(LTE_LOG_INFO, "zte cdma2000 platform init success");
        }
        break;
#endif
#ifdef __ARCCRA__
        case M_ARCCRA:
        {
            enRet = arccra_init();
            if (LTE_OK != enRet)
            {
                    LTE_LOG(LTE_LOG_ALERT, "arccra platform init. ret %d", enRet);
                    return enRet;
            }
            LTE_LOG(LTE_LOG_INFO, "arccra platform init success");
        }
        break;
        case M_ARCCRA_SECOND:
        {
            enRet = arccra_init();
            if (LTE_OK != enRet)
            {
                    LTE_LOG(LTE_LOG_ALERT, "arccra 4 mode platform init. ret %d", enRet);
                    return enRet;
            }
            LTE_LOG(LTE_LOG_INFO, "arccra platform 4 mode init success");    
        }
        break;
#endif
#ifdef __THINKWILL_MI660__
              case M_THINKWILL_CDMA_MI660:
              {
                  enRet = thinkwill_cdma_mi660_init();
                  if (LTE_OK != enRet)
                  {
                          LTE_LOG(LTE_LOG_ALERT, "thinkwill cdma mi660 platform init. ret %d", enRet);
                          return enRet;
                  }
                  LTE_LOG(LTE_LOG_INFO, "thinkwill cdma mi660 platform init success");
              }
              break;
#endif
#ifdef __YUGA_CEM600__
                    case M_YUGA_CDMA_CEM600:
                    {
                        enRet = yuga_cdma_cem600_init();
                        if (LTE_OK != enRet)
                        {
                                LTE_LOG(LTE_LOG_ALERT, "yuga cdma cem600 platform init. ret %d", enRet);
                                return enRet;
                        }
                        LTE_LOG(LTE_LOG_INFO, "yuga cdma cem600 platform init success");
                    }
                    break;
#endif
#ifdef __ZTE_MICRO__
      case M_ZTE_MICRO:
      {
          enRet = zte_micro_B57_5_init();
          if (LTE_OK != enRet)
          {
                  LTE_LOG(LTE_LOG_ALERT, "ZTE microelectronics platform init. ret %d", enRet);
                  return enRet;
          }
          LTE_LOG(LTE_LOG_INFO, "ZTE microelectronics platform init success");
      }
      break;
#endif
#ifdef __HUAWEI__
            case M_HUAWEI:
            {
              enRet = huawei_init();
                  if (LTE_OK != enRet)
                  {
                          LTE_LOG(LTE_LOG_ALERT, "huawei platform init. ret %d", enRet);
                          return enRet;
                  }
              LTE_LOG(LTE_LOG_INFO, "huawei platform init success");
            }
            break;
#endif

#ifdef __YUGA__
            case M_YUGA:
            {
                enRet = yuga_init();
                    if (LTE_OK != enRet)
                    {
                            LTE_LOG(LTE_LOG_ALERT, "yuga platform init. ret %d", enRet);
                            return enRet;
                    }
                LTE_LOG(LTE_LOG_INFO, "yuga platform init success");
            }
            break;
#endif
      
#ifdef __MARVELL__
            case M_MARVELL:
            {
                enRet = marvell_init();
                    if (LTE_OK != enRet)
                    {
                            LTE_LOG(LTE_LOG_ALERT, "marvall platform init. ret %d", enRet);
                            return enRet;
                    }
                LTE_LOG(LTE_LOG_INFO, "marvall platform init success");
            }
            break;
#endif

#ifdef __HUAWEI4G__
          case M_HUAWEI4G_NETCARD:
          {
              enRet = huawei4G_init();
                  if (LTE_OK != enRet)
                  {
                          LTE_LOG(LTE_LOG_ALERT, "huawei4G platform init. ret %d", enRet);
                          return enRet;
                  }
              LTE_LOG(LTE_LOG_INFO, "huawei4G platform init success");
          }
          break;
#endif



	  default:
	  	LTE_LOG(LTE_LOG_ALERT, "unkown module platform ");
		return LTE_FAIL;
		break;
    }

    lte_init_set();
    return LTE_OK;

}


