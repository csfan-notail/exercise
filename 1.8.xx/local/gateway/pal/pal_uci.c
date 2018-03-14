
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

#include "ap_types.h"
#include "pal_log.h"
#include "pal_uci.h"


int factory_env_get(char *value, char *name)
{
	FILE *pf = NULL;
	char cmd[64]= {0};
	char buf[128] = {0};
	char *vl=NULL;
	
	sprintf(cmd,"factory_env get %s",name);
	pf = popen(cmd,"r");
	if(NULL == pf){
		log_info(LOG_ERR,"popen failed. ");
	}
	else{
		if(fgets(buf, sizeof(buf), pf) != NULL){
			vl = strstr(buf, "=");
			if(vl != NULL){
				strncpy(value, vl+1, strlen(vl));
				pclose(pf);
				log_info(LOG_INFO,"%s %d value %s   name :%s \n",__FUNCTION__,__LINE__,value,name);
				return strlen(value);
			}
		}
		pclose(pf);
	}
	return -1;
}


/*convert nvram ops  to uci*/
int nvram_set(char *name ,char *value)
{
    return uci_config_set(name,value);
}

int nvram_unset(char *name)
{
    return uci_config_delete(name);
}

int nvram_get(char *value,char *name, int len)
{
    return uci_config_get(value,name);
}


int nvram_commit(void)
{
    return uci_config_commit();
}

int nvram_set_and_commit(char *name ,char *value)
{
    return uci_config_set_and_commit(name,value);
}




/*uci ops*/

int uci_config_add(char *name, char *value)
{
	char cmd[128];
	sprintf(cmd, "uci add %s '%s'",name, value);
	gw_log(LOG_INFO,"cmd %s",cmd);
	system(cmd);
	return 0;
}

int uci_config_set(char *name, char *value)
{
	char cmd[128];
	sprintf(cmd, "uci set %s='%s'",name, value);

	gw_log(LOG_INFO,"cmd %s",cmd);
	system(cmd);
	return 0;
}

int uci_config_delete(char *name)
{
	char cmd[128];
	sprintf(cmd, "uci delete %s",name);
	system(cmd);
	return 0;
}


int uci_config_get(char *value,char *name)
{
    FILE *pf = NULL;
    char cmd[128];
    char buf[128] = {0};
    char *vl=NULL;
    memset(value,0,strlen(value));
    sprintf(cmd, "uci get %s",name);
    gw_log(LOG_INFO,"%s %d cmd %s \n",__FUNCTION__,__LINE__,cmd);
    if((pf = popen(cmd, "r")) == NULL){
        return -1;
    }
    else{
        if(fgets(buf, sizeof(buf),pf) != NULL)
        {
            vl = strstr(buf, "Entry not found");
            if(vl){
			syslog(0,"%s %d not find the entry!\n",__FUNCTION__,__LINE__);		
                pclose(pf);
                return -1;
            }
            vl = strstr(buf,"\n");		
            if(vl){
                *vl='\0';
            }
            strncpy(value,buf, strlen(buf));
            value[strlen(buf)]='\0';
            gw_log(LOG_INFO,"%s %d  find the entry!value (%s) buf (%s) len is %d \n",__FUNCTION__,__LINE__,value,buf,strlen(buf));
            
            pclose(pf);
            return 0;
        }
        pclose(pf);
    }
    return -1;
}


int uci_config_commit(void)
{
    char cmd[128];
    sprintf(cmd, "uci commit");
    system(cmd);
    return 0;
}


int uci_config_set_and_commit(char *name ,char *value)
{
    char cmd[128];
    sprintf(cmd, "uci set %s='%s'",name, value);
    system(cmd);
    sprintf(cmd, "uci commit");
    system(cmd);
    return 0;
}


