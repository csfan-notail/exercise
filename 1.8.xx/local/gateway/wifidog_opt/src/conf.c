/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : conf.c
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/11/25
  Last Modified :
  Description   : 
  Function List :
  History       :
  1.Date        : 2013/11/25
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdarg.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <netdb.h>

#if defined(__NetBSD__)
#include <sys/socket.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <util.h>
#endif

#ifdef __linux__
#include <netinet/in.h>
#include <net/if.h>
#endif
#include <time.h>

#include "includes.h"
#include "log.h"
#include "common.h"
#include "util.h"
#include "conf.h"


static s_config config;
#ifdef OPENWRT
#include "pal_uci.h"
#include "pal_sys.h"

static wifidog_conf swifidog_conf;
char wifi_conf[][32]={	
		{"GatewayID"}, 
		{"Hostname"}, 
		{"Path"}, 
		{"Model"}, 
		{"WanInterface"},
		{"AuthCtrl"},
		{"ccsserver"},
		{"snmpserver"},
		{"servermode"},
		{"wxl_ssid"},
		{"wxl_bssid"},
		{"GatewayInterface"}
	};


#define HOTSPOT_NAME GATEWAY"device_hotspot_name"
#define HOTSPOT_URL GATEWAY"device_hotspot_url"
#define HOTSPOT_PATH GATEWAY"device_hotspot_path"
#define AUTHCTRL GATEWAY"auth_ctrl"
#define CCS_SERVER GATEWAY"ccsserver"
#define SNMP_SERVER GATEWAY"snmpserver"
#define SERVER_MODE GATEWAY"wifidog_switch"
#define WXL_SSID GATEWAY"wxl_ssid"
#define WXL_BSSID GATEWAY"wxl_bssid"
#define GATEWAY_INTERFACE GATEWAY"gateway_interface"



#define IOS_AUTO_PORTAL         GATEWAY"ios_auto_portal"
#define IOS_REDIRECT_MODE       GATEWAY"ios_redirect_mode"
#define UCWEB_REDIRECT_MODE     GATEWAY"ucweb_redirect_mode"

#endif

char *g_pstAppleSuccess = NULL;

char *g_pstExtDebugLevel = NULL;

int g_iIosAutoPortal = 1;	  /*0:禁止1:允许2:仅第三方禁止*/
int g_iIosRedirectMode = 0;   /*0:http307  1:js*/
int g_iUcwebRedirectMode = 0; /*0:http307  1:js*/

int g_ext_dbg_level = 0;

/** @internal
 The different configuration options */
typedef enum {
	oBadOption,
	oDaemon,
	oDebugLevel,
	oExternalInterface,
	oGatewayID,
	oGatewayInterface,
	oGatewayAddress,
	oGatewayPort,
	oGatewayRedirectPort,
	oAuthServer,
	oAuthServHostname,
	oAuthServSSLAvailable,
	oAuthServSSLPort,
	oAuthServHTTPPort,
	oAuthServPath,
	oAuthServLoginScriptPathFragment,
	oAuthServPortalScriptPathFragment,
	oAuthServMsgScriptPathFragment,
	oAuthServPingScriptPathFragment,
	oAuthServAuthScriptPathFragment,
	oAuthServConfScriptPathFragment,
	oAuthServCmdScriptPathFragment,
	oCCSServHostname,
	oSNMPServHostname,
	oHTTPDMaxConn,
	oHTTPDName,
	oHTTPDRealm,
        oHTTPDUsername,
        oHTTPDPassword,
	oClientTimeout,
	oCheckInterval,
	oPingTimeouts,
	oWdctlSocket,
	oSyslogFacility,
	oFirewallRule,
	oFirewallRuleSet,
	oTrustedMACList,
	oTrustedIPList,
	oTrustedWANHOSTList,
  oHtmlMessageFile,
  oConfVer,
  oCmdVer,
  oQosEnable,
  oSuccessACL,
  oAllowHost,
  oDenyHost,
  oModel,
  oWanInterface,
  oExtDebugLevel,
  oAuthCtrl,
  oGatewayFirstpageHandlePort,
  oWanFlowReportSwitch,
  oWanFlowReportInterval,
  oRedirectThreadnum,
  oServerMode,
  oWxlSsid,
  oWxlBSsid,
  oRttpadswitch,
  oHttpadwldura,
  oHttpadbldura,
  oHttprdmaxcnt,
  oHttpckIntvl,
  oHttpckStaLimit
} OpCodes;

/** @internal
 The config file keywords for the different configuration options */
static const struct {
	const char *name;
	OpCodes opcode;
} keywords[] = {
	{ "daemon",             	oDaemon },
	{ "debuglevel",         	oDebugLevel },
	{ "externalinterface",  	oExternalInterface },
	{ "gatewayid",          	oGatewayID },
	{ "gatewayinterface",   	oGatewayInterface },
	{ "gatewayaddress",     	oGatewayAddress },
	{ "gatewayport",        	oGatewayPort },
	{ "gatewayredirectport",    oGatewayRedirectPort},
	{ "authserver",         	oAuthServer },
	{ "httpdmaxconn",       	oHTTPDMaxConn },
	{ "httpdname",          	oHTTPDName },
	{ "httpdrealm",			    oHTTPDRealm },
	{ "httpdusername",		    oHTTPDUsername },
	{ "httpdpassword",		    oHTTPDPassword },
	{ "clienttimeout",      	oClientTimeout },
	{ "checkinterval",      	oCheckInterval },
	{ "PingTimeouts",           oPingTimeouts  },
	{ "syslogfacility", 		oSyslogFacility },
	{ "wdctlsocket",		    oWdctlSocket },
	{ "hostname",			    oAuthServHostname },
	{ "sslavailable",		    oAuthServSSLAvailable },
	{ "sslport",			    oAuthServSSLPort },
	{ "httpport",			    oAuthServHTTPPort },
	{ "path",			        oAuthServPath },
	{ "loginscriptpathfragment",	oAuthServLoginScriptPathFragment },
	{ "portalscriptpathfragment",	oAuthServPortalScriptPathFragment },
	{ "msgscriptpathfragment",	oAuthServMsgScriptPathFragment },
	{ "pingscriptpathfragment",	oAuthServPingScriptPathFragment },
	{ "authscriptpathfragment",	oAuthServAuthScriptPathFragment },
	{ "confscriptpathfragment",	oAuthServConfScriptPathFragment },
	{ "cmdscriptpathfragment",	oAuthServCmdScriptPathFragment },
	{ "ccsserver", oCCSServHostname},
	{ "snmpserver", oSNMPServHostname},
	{ "firewallruleset",		oFirewallRuleSet },
	{ "firewallrule",		oFirewallRule },
	{ "trustedmaclist",		oTrustedMACList },
	{ "trustediplist",		oTrustedIPList },
	{ "trustedwanhostlist",		oTrustedWANHOSTList },
    { "htmlmessagefile",	    oHtmlMessageFile },
    { "confver",	oConfVer },
    { "cmdver",		oCmdVer },
    { "qosenable",		oQosEnable },
    { "SuccessACL",     oSuccessACL },
    { "AllowHost",     oAllowHost },
    { "DenyHost",     oDenyHost },
    { "Model",          oModel },
    { "WanInterface",         oWanInterface },
    { "ExtDebugLevel",         oExtDebugLevel },
    { "AuthCtrl",         oAuthCtrl },
	{ "GatewayFirstpageHandlePort", oGatewayFirstpageHandlePort},
	{ "WanFlowReportSwitch", oWanFlowReportSwitch},
	{ "WanFlowReportInterval", oWanFlowReportInterval},
	{ "redirectThreadNum", 		oRedirectThreadnum },
	{ "servermode", 		oServerMode },
	{ "wxl_ssid", 		oWxlSsid },
	{ "wxl_bssid", 		oWxlBSsid },
	{ "httpadswitch", 		oRttpadswitch },
	{ "httpadwldura", 		oHttpadwldura },
	{ "httpadbldura", 		oHttpadbldura },
	{ "httprdmaxcnt", 		oHttprdmaxcnt },
	{ "httpckIntvl", 		oHttpckIntvl },
	{ "httpckStalimit", 	oHttpckStaLimit },
	{ NULL,				oBadOption },
};

static int config_notnull(const void *parm, const char *parmname);
static int parse_boolean_value(char *);
static void parse_auth_server(FILE *, const char *, int *);
static int _parse_firewall_rule(const char *ruleset, char *leftover);
static void parse_firewall_ruleset(const char *, FILE *, const char *, int *);

static OpCodes config_parse_token(const char *cp, const char *filename, int linenum);


/** @internal
Parses a boolean value from the config file
*/
static int
parse_boolean_value(char *line)
{
	if (strcasecmp(line, "yes") == 0) {
		return 1;
	}
	if (strcasecmp(line, "no") == 0) {
		return 0;
	}
	if (strcmp(line, "1") == 0) {
		return 1;
	}
	if (strcmp(line, "0") == 0) {
		return 0;
	}

	return -1;
}

/** @internal
 *Parses a single token from the config file
 */
static OpCodes config_parse_token(const char *cp, const char *filename, int linenum)
{
	int i;

	for (i = 0; keywords[i].name; i++)
	{
		if (strcasecmp(cp, keywords[i].name) == 0)
		{
			return keywords[i].opcode;
	    }
	}

	log_info(__LOG_FAILED__, "%s: line %d: Bad configuration option: %s", 
			filename, linenum, cp);
	return oBadOption;
}

/** @internal
Parses auth server information
*/
static void
parse_auth_server(FILE *file, const char *filename, int *linenum)
{
	char	*host = NULL,
			*path = NULL,
			*loginscriptpathfragment = NULL,
			*portalscriptpathfragment = NULL,
			*msgscriptpathfragment = NULL,
			*pingscriptpathfragment = NULL,
			*authscriptpathfragment = NULL,
			*confscriptpathfragment = NULL,
			*cmdscriptpathfragment = NULL,
			*clientscriptpathfragment = NULL,
			*wanflowreportscriptpathfragment = NULL, /*add@2014.8.26*/
			line[MAX_BUF],
			*p1,
			*p2;
	int		http_port,
			ssl_port,
			ssl_available,
			opcode;
	t_auth_serv	*new,
			*tmp;

	
	char tmp_host[128] = {0};
	int iLoop = 0;
	int iFlag = 0; /*host中有无:*/

	/* Defaults */
	path = safe_strdup(DEFAULT_AUTHSERVPATH);
	loginscriptpathfragment = safe_strdup(DEFAULT_AUTHSERVLOGINPATHFRAGMENT);
	portalscriptpathfragment = safe_strdup(DEFAULT_AUTHSERVPORTALPATHFRAGMENT);
	msgscriptpathfragment = safe_strdup(DEFAULT_AUTHSERVMSGPATHFRAGMENT);
	pingscriptpathfragment = safe_strdup(DEFAULT_AUTHSERVPINGPATHFRAGMENT);
	authscriptpathfragment = safe_strdup(DEFAULT_AUTHSERVAUTHPATHFRAGMENT);
	confscriptpathfragment = safe_strdup(DEFAULT_CONFSERVAUTHPATHFRAGMENT);
	cmdscriptpathfragment = safe_strdup(DEFAULT_CMDSERVAUTHPATHFRAGMENT);
	clientscriptpathfragment = safe_strdup(DEFAUTL_CLIENTSERVAUTHPATHFRAGMENT);
	/*add@2014.8.26 for reporting flow of wan*/
	wanflowreportscriptpathfragment = safe_strdup(DEFAUTL_WANFLOWREPORTPATHFRAGMENT);
	http_port = DEFAULT_AUTHSERVPORT;
	ssl_port = DEFAULT_AUTHSERVSSLPORT;
	ssl_available = DEFAULT_AUTHSERVSSLAVAILABLE;
	
	/* Read first line */	
	memset(line, 0, MAX_BUF);
	fgets(line, MAX_BUF - 1, file);
	(*linenum)++; /* increment line counter. */

	/* Parsing loop */
	while ((line[0] != '\0') && (strchr(line, '}') == NULL)) {
		/* skip leading blank spaces */
		for (p1 = line; isspace(*p1); p1++);

		/* End at end of line */
		if ((p2 = strchr(p1, '#')) != NULL) {
			*p2 = '\0';
		} else if ((p2 = strchr(p1, '\r')) != NULL) {
			*p2 = '\0';
		} else if ((p2 = strchr(p1, '\n')) != NULL) {
			*p2 = '\0';
		}

		/* next, we coopt the parsing of the regular config */
		if (strlen(p1) > 0) {
			p2 = p1;
			/* keep going until word boundary is found. */
			while ((*p2 != '\0') && (!isspace(*p2)))
				p2++;

			/* Terminate first word. */
			*p2 = '\0';
			p2++;

			/* skip all further blanks. */
			while (isspace(*p2))
				p2++;
			
			/* Get opcode */
			opcode = config_parse_token(p1, filename, *linenum);
			
			switch (opcode) {
				case oAuthServHostname:
					/*modified: add@2014.12.18 for http port begin*/
					for (iLoop = 0; iLoop < strlen(p2); iLoop++){
						if (p2[iLoop] == ':'){
							iFlag = 1;
						}
					}
					if (iFlag){
						sscanf(p2, "%[^: ]:%d", tmp_host, &http_port);
						host = safe_strdup(tmp_host);
						log_info(__LOG_DEBUG__, "tmp_host: %s, host: %s, port: %d", tmp_host, host, http_port);
					}else{
						host = safe_strdup(p2);
					}
					//host = safe_strdup(p2);
					/*modified: add@2014.12.18 for http port end*/
#ifdef GW_AC
					if(!strcmp(host, "127.0.0.1")){
						if((host = get_iface_ip(config.wan_interface)) != NULL)
							log_info(__LOG_DEBUG__, "Convert auth server IP to br2 IP %s", host);
						else
							log_info(__LOG_ERROR__, "Get auth server IP failed!");
					}
#endif
					break;
				case oAuthServPath:
					free(path);
					path = safe_strdup(p2);
					break;
				case oAuthServLoginScriptPathFragment:
					free(loginscriptpathfragment);
					loginscriptpathfragment = safe_strdup(p2);
					break;					
				case oAuthServPortalScriptPathFragment:
					free(portalscriptpathfragment);
					portalscriptpathfragment = safe_strdup(p2);
					break;
				case oAuthServMsgScriptPathFragment:
					free(msgscriptpathfragment);
					msgscriptpathfragment = safe_strdup(p2);
					break;
				case oAuthServPingScriptPathFragment:
					free(pingscriptpathfragment);
					pingscriptpathfragment = safe_strdup(p2);
					break;					
				case oAuthServAuthScriptPathFragment:
					free(authscriptpathfragment);
					authscriptpathfragment = safe_strdup(p2);
					break;
				case oAuthServConfScriptPathFragment:
					free(confscriptpathfragment);
					confscriptpathfragment = safe_strdup(p2);
					break;
				case oAuthServCmdScriptPathFragment:
					free(cmdscriptpathfragment);
					cmdscriptpathfragment = safe_strdup(p2);
					break;
				case oAuthServSSLPort:
					ssl_port = atoi(p2);
					break;
				case oAuthServHTTPPort:
					http_port = atoi(p2);
					break;
				case oAuthServSSLAvailable:
					ssl_available = parse_boolean_value(p2);
					if (ssl_available < 0)
						ssl_available = 0;
					break;
				case oBadOption:
				default:
				{
					log_info(__LOG_FAILED__, "Bad option on line %d "
							"in %s.", *linenum,
							filename);
					log_info(__LOG_FAILED__, "Exiting...");
					sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at parse_auth_server(). opcode = %d", opcode);
					exit(-1);
				}
					break;
			}
		}

		/* Read next line */
		memset(line, 0, MAX_BUF);
		fgets(line, MAX_BUF - 1, file);
		(*linenum)++; /* increment line counter. */
	}

	/* only proceed if we have an host and a path */
	if (host == NULL)
		return;
	
	log_info(__LOG_DEBUG__, "Adding %s:%d (SSL: %d) %s to the auth server list",
			host, http_port, ssl_port, path);

	/* Allocate memory */
	new = safe_malloc(sizeof(t_auth_serv));
	
	/* Fill in struct */
	memset(new, 0, sizeof(t_auth_serv)); /*< Fill all with NULL */
	new->authserv_hostname = host;
	new->authserv_use_ssl = ssl_available;
	new->authserv_path = path;
	new->authserv_login_script_path_fragment = loginscriptpathfragment;
	new->authserv_portal_script_path_fragment = portalscriptpathfragment;
	new->authserv_msg_script_path_fragment = msgscriptpathfragment;    
	new->authserv_ping_script_path_fragment = pingscriptpathfragment;  
	new->authserv_auth_script_path_fragment = authscriptpathfragment;  
	new->authserv_conf_script_path_fragment = confscriptpathfragment; 
	new->authserv_cmd_script_path_fragment = cmdscriptpathfragment; 
	new->authserv_client_script_path_fragment = clientscriptpathfragment;
	new->authserv_wan_flow_report_script_path_fragment = wanflowreportscriptpathfragment;
	new->authserv_http_port = http_port;
	new->authserv_ssl_port = ssl_port;
	
	/* If it's the first, add to config, else append to last server */
	if (config.auth_servers == NULL) {
		config.auth_servers = new;
	} else {
		for (tmp = config.auth_servers; tmp->next != NULL;
				tmp = tmp->next);
		tmp->next = new;
	}
	
	log_info(__LOG_DEBUG__, "Auth server added");
}

/**
Advance to the next word
@param s string to parse, this is the next_word pointer, the value of s
	 when the macro is called is the current word, after the macro
	 completes, s contains the beginning of the NEXT word, so you
	 need to save s to something else before doing TO_NEXT_WORD
@param e should be 0 when calling TO_NEXT_WORD(), it'll be changed to 1
	 if the end of the string is reached.
*/
#define TO_NEXT_WORD(s, e) do { \
	while (*s != '\0' && !isspace(*s)) { \
		s++; \
	} \
	if (*s != '\0') { \
		*s = '\0'; \
		s++; \
		while (isspace(*s)) \
			s++; \
	} else { \
		e = 1; \
	} \
} while (0)

/** @internal
Parses firewall rule set information
*/
static void parse_firewall_ruleset(const char *ruleset, FILE *file, const char *filename, int *linenum)
{
	char		line[MAX_BUF],
			*p1,
			*p2;
	int		opcode;

	log_info(__LOG_DEBUG__, "Adding Firewall Rule Set %s", ruleset);
	
	/* Read first line */	
	memset(line, 0, MAX_BUF);
	fgets(line, MAX_BUF - 1, file);
	(*linenum)++; /* increment line counter. */

	/* Parsing loop */
	while ((line[0] != '\0') && (strchr(line, '}') == NULL)) {
		/* skip leading blank spaces */
		for (p1 = line; isspace(*p1); p1++);

		/* End at end of line */
		if ((p2 = strchr(p1, '#')) != NULL) {
			*p2 = '\0';
		} else if ((p2 = strchr(p1, '\r')) != NULL) {
			*p2 = '\0';
		} else if ((p2 = strchr(p1, '\n')) != NULL) {
			*p2 = '\0';
		}

		/* next, we coopt the parsing of the regular config */
		if (strlen(p1) > 0) {
			p2 = p1;
			/* keep going until word boundary is found. */
			while ((*p2 != '\0') && (!isspace(*p2)))
				p2++;

			/* Terminate first word. */
			*p2 = '\0';
			p2++;

			/* skip all further blanks. */
			while (isspace(*p2))
				p2++;
			
			/* Get opcode */
			opcode = config_parse_token(p1, filename, *linenum);

			log_info(__LOG_DEBUG__, "p1 = [%s]; p2 = [%s]", p1, p2);
			
			switch (opcode) {
				case oFirewallRule:
					_parse_firewall_rule(ruleset, p2);
					break;

				case oBadOption:
				default:
				{
					log_info(__LOG_ERROR__, "Bad option on line %d "
							"in %s.", *linenum,
							filename);
					log_info(__LOG_ERROR__, "Exiting...");
					sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at parse_firewall_ruleset() opcode = %d.", opcode);
					exit(-1);
				}
					break;
			}
		}

		/* Read next line */
		memset(line, 0, MAX_BUF);
		fgets(line, MAX_BUF - 1, file);
		(*linenum)++; /* increment line counter. */
	}

	log_info(__LOG_DEBUG__, "Firewall Rule Set %s added.", ruleset);
}

/** @internal
Helper for parse_firewall_ruleset.  Parses a single rule in a ruleset
*/
static int
_parse_firewall_rule(const char *ruleset, char *leftover)
{
	int i;
	int block_allow = 0; /**< 0 == block, 1 == allow */
	int all_nums = 1; /**< If 0, port contained non-numerics */
	int finished = 0; /**< reached end of line */
	char *token = NULL; /**< First word */
	char *port = NULL; /**< port to open/block */
	char *protocol = NULL; /**< protocol to block, tcp/udp/icmp */
	char *mask = NULL; /**< Netmask */
	char *other_kw = NULL; /**< other key word */
	t_firewall_ruleset *tmpr;
	t_firewall_ruleset *tmpr2;
	t_firewall_rule *tmp;
	t_firewall_rule *tmp2;

	log_info(__LOG_DEBUG__, "leftover: %s", leftover);

	/* lower case */
	for (i = 0; *(leftover + i) != '\0'
			&& (*(leftover + i) = tolower((unsigned char)*(leftover + i))); i++);
	
	token = leftover;
	TO_NEXT_WORD(leftover, finished);
	
	/* Parse token */
	if (!strcasecmp(token, "block") || finished) {
		block_allow = 0;
	} else if (!strcasecmp(token, "allow")) {
		block_allow = 1;
	} else {
		log_info(__LOG_ERROR__, "Invalid rule type %s, expecting "
				"\"block\" or \"allow\"", token);
		return -1;
	}

	/* Parse the remainder */
	/* Get the protocol */
	if (strncmp(leftover, "tcp", 3) == 0
			|| strncmp(leftover, "udp", 3) == 0
			|| strncmp(leftover, "icmp", 4) == 0) {
		protocol = leftover;
		TO_NEXT_WORD(leftover, finished);
	}

	/* should be exactly "port" */
	if (strncmp(leftover, "port", 4) == 0) {
		TO_NEXT_WORD(leftover, finished);
		/* Get port now */
		port = leftover;
		TO_NEXT_WORD(leftover, finished);
		for (i = 0; *(port + i) != '\0'; i++)
			if (!isdigit((unsigned char)*(port + i)))
				all_nums = 0; /*< No longer only digits */
		if (!all_nums) {
			log_info(__LOG_ERROR__, "Invalid port %s", port);
			return -3; /*< Fail */
		}
	}

	/* Now, further stuff is optional */
	if (!finished) {
		/* should be exactly "to" */
		other_kw = leftover;
		TO_NEXT_WORD(leftover, finished);
		if (strcmp(other_kw, "to") || finished) {
			log_info(__LOG_ERROR__, "Invalid or unexpected keyword %s, "
					"expecting \"to\"", other_kw);
			return -4; /*< Fail */
		}

		/* Get port now */
		mask = leftover;
		TO_NEXT_WORD(leftover, finished);
		all_nums = 1;
		for (i = 0; *(mask + i) != '\0'; i++)
			if (!isdigit((unsigned char)*(mask + i)) && (*(mask + i) != '.')
					&& (*(mask + i) != '/'))
				all_nums = 0; /*< No longer only digits */
		if (!all_nums) {
			log_info(__LOG_ERROR__, "Invalid mask %s", mask);
			return -3; /*< Fail */
		}
	}

	/* Generate rule record */
	tmp = safe_malloc(sizeof(t_firewall_rule));
	memset((void *)tmp, 0, sizeof(t_firewall_rule));
	tmp->block_allow = block_allow;
	if (protocol != NULL)
		tmp->protocol = safe_strdup(protocol);
	if (port != NULL)
		tmp->port = safe_strdup(port);
	if (mask == NULL)
		tmp->mask = safe_strdup("0.0.0.0/0");
	else
		tmp->mask = safe_strdup(mask);

	log_info(__LOG_DEBUG__, "Adding Firewall Rule %s %s port %s to %s", token, tmp->protocol, tmp->port, tmp->mask);
	
	/* Append the rule record */
	if (config.rulesets == NULL) {
		config.rulesets = safe_malloc(sizeof(t_firewall_ruleset));
		memset(config.rulesets, 0, sizeof(t_firewall_ruleset));
		config.rulesets->name = safe_strdup(ruleset);
		tmpr = config.rulesets;
	} else {
		tmpr2 = tmpr = config.rulesets;
		while (tmpr != NULL && (strcmp(tmpr->name, ruleset) != 0)) {
			tmpr2 = tmpr;
			tmpr = tmpr->next;
		}
		if (tmpr == NULL) {
			/* Rule did not exist */
			tmpr = safe_malloc(sizeof(t_firewall_ruleset));
			memset(tmpr, 0, sizeof(t_firewall_ruleset));
			tmpr->name = safe_strdup(ruleset);
			tmpr2->next = tmpr;
		}
	}

	/* At this point, tmpr == current ruleset */
	if (tmpr->rules == NULL) {
		/* No rules... */
		tmpr->rules = tmp;
	} else {
		tmp2 = tmpr->rules;
		while (tmp2->next != NULL)
			tmp2 = tmp2->next;
		tmp2->next = tmp;
	}
	
	return 1;
}

void parse_trusted_mac_list(char *ptr) {
	char *ptrcopy = NULL;
	char *possiblemac = NULL;
	char *mac = NULL;
	t_trusted_mac *p = NULL;

	log_info(__LOG_DEBUG__, "Parsing string [%s] for trusted MAC addresses", ptr);

	mac = safe_malloc(18);

	/* strsep modifies original, so let's make a copy */
	ptrcopy = safe_strdup(ptr);

	while ((possiblemac = strsep(&ptrcopy, ", "))) {
		if (sscanf(possiblemac, " %17[A-Fa-f0-9:]", mac) == 1) {
			/* Copy mac to the list */
			log_info(__LOG_DEBUG__, "Adding MAC address [%s] to trusted list", mac);
			if (config.trustedmaclist == NULL) {
				config.trustedmaclist = safe_malloc(sizeof(t_trusted_mac));
				config.trustedmaclist->mac = safe_strdup(mac);
				config.trustedmaclist->next = NULL;
			}
			else {
				/* Advance to the last entry */
				for (p = config.trustedmaclist; p->next != NULL; p = p->next);
				p->next = safe_malloc(sizeof(t_trusted_mac));
				p = p->next;
				p->mac = safe_strdup(mac);
				p->next = NULL;
			}

		}
	}
	free(ptrcopy);
	free(mac);
}

void parse_trusted_ip_list(char *ptr) {
	char *ptrcopy = NULL;
	char *possibleip = NULL;
	char *ip = NULL;
	t_trusted_ip *p = NULL;

	log_info(__LOG_DEBUG__, "Parsing string [%s] for trusted IP addresses", ptr);

	ip = safe_malloc(16);

	/* strsep modifies original, so let's make a copy */
	ptrcopy = safe_strdup(ptr);

	while ((possibleip = strsep(&ptrcopy, ", "))) {
		if (sscanf(possibleip, " %15[0-9.]", ip) == 1) {
			/* Copy mac to the list */
			log_info(__LOG_DEBUG__, "Adding IP address [%s] to trusted list", ip);
			if (config.trustediplist == NULL) {
				config.trustediplist = safe_malloc(sizeof(t_trusted_ip));
				config.trustediplist->ip = safe_strdup(ip);
				config.trustediplist->next = NULL;
			}
			else {
				/* Advance to the last entry */
				for (p = config.trustediplist; p->next != NULL; p = p->next);
				p->next = safe_malloc(sizeof(t_trusted_ip));
				p = p->next;
				p->ip = safe_strdup(ip);
				p->next = NULL;
			}
		}
	}
	free(ptrcopy);
	free(ip);
}

void parse_trusted_wanhost_list(char *ptr) {
	char *ptrcopy = NULL;
	char *possiblewanhost = NULL;
	char *ip = NULL;
	t_trusted_wanhost *p = NULL;
	struct hostent *he;
	struct in_addr *in_addr_temp;

	log_info(__LOG_DEBUG__, "Parsing string [%s] for trusted WANHOST addresses", ptr);
	/* strsep modifies original, so let's make a copy */
	ptrcopy = safe_strdup(ptr);
	
	while ((possiblewanhost = strsep(&ptrcopy, ", "))) {
		he = gethostbyname(possiblewanhost);
		if (he == NULL) 
		{
			continue ;
		}
		else
		{
			in_addr_temp = (struct in_addr *)he->h_addr_list[0];
			ip = safe_strdup((const char*)inet_ntoa(*in_addr_temp));
			/* Copy mac to the list */

			log_info(__LOG_DEBUG__, "Adding WANHOST address [%s] to trusted list", ip);

			if (config.trustedwanhostlist == NULL) 
			{
				config.trustedwanhostlist = safe_malloc(sizeof(t_trusted_wanhost));
				config.trustedwanhostlist->host = safe_strdup(possiblewanhost);
				config.trustedwanhostlist->ip = ip;
				config.trustedwanhostlist->next = NULL;
			}
			else 
			{
				/* Advance to the last entry */
				for (p = config.trustedwanhostlist; p->next != NULL; p = p->next);
				p->next = safe_malloc(sizeof(t_trusted_wanhost));
				p = p->next;
				p->host = safe_strdup(possiblewanhost);
				p->ip = ip;
				p->next = NULL;
			}
		}
	}
	free(ptrcopy);
}

/** Verifies if the configuration is complete and valid.  Terminates the program if it isn't */
void config_validate(void)
{
	if ((!config_notnull(config.gw_interface, "GatewayInterface"))
	    ||(!config_notnull(config.auth_servers, "AuthServer"))) 
	{
		log_info(__LOG_ERROR__, "Configuration is not complete, exiting...");
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at config_validate()");
		exit(-1);
	}
	return;
}

/** @internal
    Verifies that a required parameter is not a null pointer
*/
static int config_notnull(const void *parm, const char *parmname)
{
	if (parm == NULL) {
		log_info(__LOG_ERROR__, "%s is not set", parmname);
		return 0;
	}
	return 1;
}

/** @get auth_servers */
t_auth_serv *get_auth_server(void)
{
    return config.auth_servers;
}

/** @get config */
s_config *config_get_config(void)
{
    return &config;
}

/** @设置缺省config配置 */
void config_init(void)
{
	char value[128];
	
	log_info(__LOG_DEBUG__, "Setting default config parameters");
	#ifdef OPENWRT
	strncpy(config.configfile, CURRENT_CONFIGFILE, sizeof(config.configfile));
	#else
	strncpy(config.configfile, DEFAULT_CONFIGFILE, sizeof(config.configfile));
	#endif
	config.htmlmsgfile = safe_strdup(DEFAULT_HTMLMSGFILE);
	config.debuglevel = DEFAULT_DEBUGLEVEL;
	config.httpdmaxconn = DEFAULT_HTTPDMAXCONN;
	config.external_interface = NULL;
	config.gw_id = DEFAULT_GATEWAYID;
	config.gw_interface = NULL;
	config.gw_address = NULL;
	config.gw_port = DEFAULT_GATEWAYPORT;
	config.gw_redirect_port = DEFAULT_GATEWAYREDIRECTPORT;
	/*add@ 2014.8.13 for store tmp config version*/
	config.gw_first_page_handle_port = DEFAULT_GATEWAYFIRSTPAGEHANDLEPORT;
	
	config.wan_interface = NULL;
	config.wan_mac = NULL;
	config.auth_servers = NULL;
	config.ccs_server = NULL;
	config.snmp_server = NULL;
	config.httpdname = NULL;
	config.httpdrealm = DEFAULT_HTTPDNAME;
	config.httpdusername = NULL;
	config.httpdpassword = NULL;
	config.clienttimeout = DEFAULT_CLIENTTIMEOUT;
	config.checkinterval = DEFAULT_CHECKINTERVAL;
	config.syslog_facility = DEFAULT_SYSLOG_FACILITY;
	config.daemon = -1;
	config.log_syslog = DEFAULT_LOG_SYSLOG;
	config.wdctl_sock = safe_strdup(DEFAULT_WDCTL_SOCK);
	config.internal_sock = safe_strdup(DEFAULT_INTERNAL_SOCK);
	config.rulesets = NULL;
	config.trustedmaclist = NULL;
	config.conf_ver = 0;
	/*add@ 2014.8.13 for store tmp config version*/
	config.config_ver_tmp = 0;
	
	config.cmd_ver = 0;
	config.qos_enable = 0;
	config.tcp_timeout = DEFAULT_TCP_TIMEOUT;
	config.max_client = DEFAULT_CLINET_MAX_ACCESS_NUM;
	
	sys_config_get(value, "device_type", 128);
	if(strstr(value, "VSS-WG-180") != NULL){
		config.dev_max_client = 1000;
	}
	else if(strstr(value, "VSS-WG-200") != NULL){
		config.dev_max_client = 2000;
	}
	else
		config.dev_max_client = DEFAULT_DEV_CLINET_MAX_ACCESS_NUM;
	config.ext_debuglevel = 0;

	config.bw_up = 0;
	config.bw_down = 0;
	config.forcetimeout = 86400; /*second*/
	config.idletimeout = 900;    /*second*/
	config.wan_flow_report_switch = 0;
	config.wan_flow_report_interval = 30; /*minutes*/
	memset(config.config_change_flag, 0, sizeof(config.config_change_flag));
    config.iUser_ver = 0; 
	/*add@2014.8.16 */
	#ifdef GW_X86_DEL
	config.redirect_thread_num = 4;
	#else
	#endif
	config.child_process = 0;
	config.server_mode = SERVER_MODE_WIFIDOG; /*0:禁用1: wifidog接口server 2: 第三方server*/
	config.wxl_ssid = NULL; /*微信连SSID*/
	config.wxl_bssid = NULL; /*微信连BSSID*/

	pthread_mutex_init(&config.freeauth_maclist_mutex, NULL);

	INIT_LIST_HEAD(&config.stLanInfo);
	pthread_mutex_init(&config.laninfo_list_mutex, NULL);
	config.uiOption43 = 0;
}

#ifdef OPENWRT
int get_wifidog_conf(void)
{
	wifidog_conf *p_wifidog_conf;
	char wan_proto[16];
	char wan_name[16];
	char ios_auto_portal[32] = {0};
	char ios_auto_portal_redirect_mode[32] = {0};
	char ucweb_portal_redirect_mode[32] = {0};
	char auth_ctrl[32] = {0};
	
	p_wifidog_conf = &swifidog_conf;
	memset(p_wifidog_conf,0,sizeof(wifidog_conf));

	/*get  wifidog config from uci*/
	sys_config_get(p_wifidog_conf->hotspot_name,HOTSPOT_NAME,0);
	sys_config_get(p_wifidog_conf->hotspot_url,HOTSPOT_URL,0);
	sys_config_get(p_wifidog_conf->hotspot_path,HOTSPOT_PATH,0);


	if(-1 == sys_config_get(ios_auto_portal, IOS_AUTO_PORTAL, 32))
	{
		sprintf(ios_auto_portal, "1");
		sys_config_set(IOS_AUTO_PORTAL, "1");
		uci_config_commit();
	}
	if(-1 == sys_config_get(ios_auto_portal_redirect_mode, IOS_REDIRECT_MODE, 32))
	{
		sprintf(ios_auto_portal_redirect_mode, "0");
		sys_config_set(IOS_REDIRECT_MODE, "0");
		uci_config_commit();
	}
	if(-1 == sys_config_get(ucweb_portal_redirect_mode, UCWEB_REDIRECT_MODE, 32))
	{
		sprintf(ucweb_portal_redirect_mode, "0");
		sys_config_set(UCWEB_REDIRECT_MODE, "0");
		uci_config_commit();
	}
	snprintf(p_wifidog_conf->auth_ctrl, sizeof(auth_ctrl), "%s,%s,%s", ios_auto_portal, ios_auto_portal_redirect_mode, 
	ucweb_portal_redirect_mode);
    
	sys_config_get(p_wifidog_conf->ccs_server,CCS_SERVER,0);
	sys_config_get(p_wifidog_conf->snmp_server,SNMP_SERVER,0);
	sys_config_get(p_wifidog_conf->servermode,SERVER_MODE,0);
	
	//sys_config_get(p_wifidog_conf->wxl_ssid,WXL_SSID,0);
	//sys_config_get(p_wifidog_conf->wxl_bssid,WXL_BSSID,0);
	

	if(-1 == sys_config_get(p_wifidog_conf->gateway_interface, GATEWAY_INTERFACE, 32))
	{
		sprintf(p_wifidog_conf->gateway_interface, "br-lan");
		sys_config_set(GATEWAY_INTERFACE, "br-lan");
		uci_config_commit();
	}

	sys_config_get(wan_proto,UCI_NETWORK_WAN_PROTO,0);
	if(!strncmp(wan_proto,"pppoe",strlen("pppoe")))
		strncpy(p_wifidog_conf->WanInterface,"pppoe-wan",sizeof("pppoe-wan"));
	else{
		if(sys_config_get(wan_name,UCI_NETWORK_WAN_IFNAME,0) >0)
			strncpy(p_wifidog_conf->WanInterface,wan_name,strlen(wan_name));
		else
			strncpy(p_wifidog_conf->WanInterface,"eth0",sizeof("eth0"));
	}

	factory_env_get(p_wifidog_conf->Model,"Innermodel");
	return 1;
}



int set_wifidog_conf(void)
{
    FILE *pf;
    char *buff;
    char *start;
    char temp[256];
    wifidog_conf *p_wifidog_conf;
    
    int conf_num = 12;
    int loop;
    p_wifidog_conf = &swifidog_conf;

    buff = NULL;
    buff = (char *)malloc(2048);
    if(NULL == buff)
    {
        log_info(__LOG_MSG__,  "malloc buff failed.");
        return -1;
    }
    memset(buff, 0, 2048);
    if(NULL != (pf = fopen(DEFAULT_CONFIGFILE, "r")))
    {
        memset(temp, 0, sizeof(temp));
        while(NULL != fgets(temp, sizeof(temp), pf))
        {
            for(loop = 0; loop < conf_num; loop++)
            {
                start = strstr(temp, wifi_conf[loop]);
                if(NULL == start)
                {
                    continue;
                }  
                memset(temp, 0, sizeof(temp));
                switch(loop)
                {
                    case 0:
                        sprintf(temp, "%s %s\n", wifi_conf[loop], p_wifidog_conf->hotspot_name);
                        break;
                    case 1:
                        sprintf(temp, "%s %s\n", wifi_conf[loop], p_wifidog_conf->hotspot_url);
                        break;
                    case 2:
                        sprintf(temp, "%s %s\n", wifi_conf[loop], p_wifidog_conf->hotspot_path);
                        break;
                    case 3:
                        sprintf(temp, "%s %s\n", wifi_conf[loop], p_wifidog_conf->Model);
                        break;
                    case 4:
                        sprintf(temp, "%s %s\n", wifi_conf[loop], p_wifidog_conf->WanInterface);
                        break;
			 case 5:
                        sprintf(temp, "%s %s\n", wifi_conf[loop], p_wifidog_conf->auth_ctrl);
                        break;
			 case 6:
                        sprintf(temp, "%s %s\n", wifi_conf[loop], p_wifidog_conf->ccs_server);
                        break;
			 case 7:
                        sprintf(temp, "%s %s\n", wifi_conf[loop], p_wifidog_conf->snmp_server);
                        break;
			 case 8:
                        sprintf(temp, "%s %s\n", wifi_conf[loop], p_wifidog_conf->servermode);
                        break;
			 case 9:
                        sprintf(temp, "%s %s\n", wifi_conf[loop], p_wifidog_conf->wxl_ssid);
                        break;
			 case 10:
                        sprintf(temp, "%s %s\n", wifi_conf[loop], p_wifidog_conf->wxl_bssid);
                        break;
			 case 11:
                        sprintf(temp, "%s %s\n", wifi_conf[loop], p_wifidog_conf->gateway_interface);
                        break;
                    default:
                        break;
                }
            }
            strcat(buff, temp);
            memset(temp, 0, sizeof(temp));
        }
    
        fclose(pf);
        if(NULL != (pf = fopen(CURRENT_CONFIGFILE, "w")))
        {
            fwrite(buff, 2048, 1, pf);
            fclose(pf);
        }
    }
    free(buff);
    
    return 1;
}

#endif

/**
 *@读取配置文件
 */
extern int g_debug_level;
extern int g_daemon;
void config_read(const char *filename)
{
    FILE *fd;
	char line[MAX_BUF], *s, *p1, *p2;
	int linenum = 0, opcode, value;

	log_info(__LOG_MSG__, "Reading configuration file '%s'", filename);

	if (!(fd = fopen(filename, "r"))) {
		log_info(__LOG_FAILED__, "Could not open configuration file '%s', "
				"exiting...", filename);
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at config_read()");
		exit(1);
	}

	while (!feof(fd) && fgets(line, MAX_BUF, fd)) {
		linenum++;
		s = line;

		if (s[strlen(s) - 1] == '\n')
			s[strlen(s) - 1] = '\0';

		if ((p1 = strchr(s, ' '))) {
			p1[0] = '\0';
		} else if ((p1 = strchr(s, '\t'))) {
			p1[0] = '\0';
		}

		if (p1) {
			p1++;

			if ((p2 = strchr(p1, ' '))) {
				p2[0] = '\0';
			} else if ((p2 = strstr(p1, "\r\n"))) {
				p2[0] = '\0';
			} else if ((p2 = strchr(p1, '\n'))) {
				p2[0] = '\0';
			}
		}

		if (p1 && p1[0] != '\0') {
			/* Strip trailing spaces */

			if ((strncmp(s, "#", 1)) != 0) {
				log_info(__LOG_DEBUG__, "Parsing token: %s, "
						"value: %s", s, p1);
				opcode = config_parse_token(s, filename, linenum);

				switch(opcode) {
				case oDaemon:
					if (config.daemon == -1 && ((value = parse_boolean_value(p1)) != -1)) 
					{
						config.daemon = value;
						g_daemon = value;
					}
					break;
				case oExternalInterface:
					config.external_interface = safe_strdup(p1);
					break;
				case oGatewayID:
					if(-1 != config.conf_ver)
					{
						config.gw_id = safe_strdup(p1);
					}
					break;
				case oGatewayInterface:
					config.gw_interface = safe_strdup(p1);
					break;
				case oGatewayAddress:
					config.gw_address = safe_strdup(p1);
					break;
				case oGatewayPort:
					sscanf(p1, "%d", &config.gw_port);
					break;
			    case oGatewayRedirectPort:
			        sscanf(p1, "%d", &config.gw_redirect_port);
			        break;
				case oAuthServer:
					parse_auth_server(fd, filename,
							&linenum);
					break;
			    case oCCSServHostname:
                    config.ccs_server = safe_strdup(p1);
#ifdef GW_AC
					if(!strcmp(config.ccs_server, "127.0.0.1")){
						if((config.ccs_server = get_iface_ip(config.wan_interface)) != NULL)
							log_info(__LOG_DEBUG__, "Convert ccs server IP to br2 IP %s", config.ccs_server);
						else
							log_info(__LOG_ERROR__, "Get ccs server IP failed!");
					}
#endif
			        break;
				case oSNMPServHostname:
					config.snmp_server = safe_strdup(p1);
			        break;					
				case oFirewallRuleSet:
					parse_firewall_ruleset(p1, fd, filename, &linenum);
					break;
				case oTrustedMACList:
					parse_trusted_mac_list(p1);
					break;
				case oTrustedIPList:
					parse_trusted_ip_list(p1);
					break;
				case oTrustedWANHOSTList:
					parse_trusted_wanhost_list(p1);
					break;
				case oHTTPDName:
					config.httpdname = safe_strdup(p1);
					break;
				case oHTTPDMaxConn:
					sscanf(p1, "%d", &config.httpdmaxconn);
					break;
				case oHTTPDRealm:
					config.httpdrealm = safe_strdup(p1);
					break;
				case oHTTPDUsername:
					config.httpdusername = safe_strdup(p1);
					break;
				case oHTTPDPassword:
					config.httpdpassword = safe_strdup(p1);
					break;
				case oCheckInterval:
					sscanf(p1, "%d", &config.checkinterval);
					break;
			    case oPingTimeouts:
			        sscanf(p1, "%d", &config.pingtimeouts);
			        break;
				case oWdctlSocket:
					free(config.wdctl_sock);
					config.wdctl_sock = safe_strdup(p1);
					break;
				case oClientTimeout:
					sscanf(p1, "%d", &config.clienttimeout);
					break;
				case oSyslogFacility:
					sscanf(p1, "%d", &config.syslog_facility);
					break;
				case oHtmlMessageFile:
					config.htmlmsgfile = safe_strdup(p1);
					break;
				case oConfVer:
					if(-1 != config.conf_ver)
						sscanf(p1, "%lld", &config.conf_ver);
					break;
				case oCmdVer:
					sscanf(p1, "%lld", &config.cmd_ver);
					break;
				case oQosEnable:
					config.qos_enable = parse_boolean_value(p1);
					break;
			    case oSuccessACL:
			        g_pstAppleSuccess = safe_strdup(p1);
			        break;
			    case oModel:
			        config.model = safe_strdup(p1);
			        break;
			    case oWanInterface:
			        config.wan_interface = safe_strdup(p1);
			        break;
				case oExtDebugLevel:
					g_pstExtDebugLevel = safe_strdup(p1);
					break;
				case oAuthCtrl:
					sscanf(p1, "%d,%d,%d", &g_iIosAutoPortal, &g_iIosRedirectMode, 
						&g_iUcwebRedirectMode);
					break;
				/*begin: add@2014.8.18*/
				case oGatewayFirstpageHandlePort:
			        sscanf(p1, "%d", &config.gw_first_page_handle_port);
			        break;
				case oWanFlowReportSwitch:
					sscanf(p1, "%d", &config.wan_flow_report_switch);
			        break;
				case oWanFlowReportInterval:
					sscanf(p1, "%d", &config.wan_flow_report_interval);
			        break;
		#ifdef GW_X86_DEL
			    case oRedirectThreadnum:
			        sscanf(p1, "%d", &config.redirect_thread_num);
			        break;
		#else
		#endif
				/*end: add@2014.8.18*/
				case oServerMode:
			        sscanf(p1, "%d", &config.server_mode);
			        break;
				case oWxlSsid:
					config.wxl_ssid = safe_strdup(p1);
			        break;
				case oWxlBSsid:
					config.wxl_bssid = safe_strdup(p1);
			        break;
				case oRttpadswitch:
					sscanf(p1, "%d", &config.uiHttpadswitch);
					break;
				case oHttpadwldura:
					sscanf(p1, "%d", &config.uiHttpadwldura);
					break;
				case oHttpadbldura:
					sscanf(p1, "%d", &config.uiHttpadbldura);
					break;
				case oHttprdmaxcnt:
					sscanf(p1, "%d", &config.uiHttprdmaxcnt);
					break;	
				case oHttpckIntvl:
					sscanf(p1, "%d", &config.uiHttpckIntvl);
					break;
				case oHttpckStaLimit:
					sscanf(p1, "%d", &config.uiHttpckStaLimit);
					break;
			    case oBadOption:
					log_info(__LOG_ERROR__, "Bad option on line %d "
							"in %s.", linenum,
							filename);
					log_info(__LOG_ERROR__, "Exiting...");
					sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at config_read() oBadOption");
					exit(-1);
					break;
				}
			}
		}
	}

	if (config.httpdusername && !config.httpdpassword) {
		log_info(__LOG_ERROR__, "HTTPDUserName requires a HTTPDPassword to be set.");
		sys_log_set(WIFIDOG_LOG_FILE, "<<<<<<<<<<<< wifidog exit at config_read() usr-passwd wrong");
		exit(-1);
	}
	fclose(fd);
}


/*系统配置相关*/

int sys_config_set(char *name, char *value)
{
	#ifdef OPENWRT
	return uci_config_set(name,value);
	#else
	char cmd[512];

	sprintf(cmd, "nvramcli set %s=%s", name, value);
	system(cmd);
	sprintf(cmd, "nvramcli commit");
	system(cmd);

	return 0;
	#endif
}

int sys_config_get(char *value, char *name, int len)
{
	#ifdef OPENWRT
	return uci_config_get(value,name);
	#else
	FILE *pf = NULL;
	char cmd[128];
	char buf[128] = {0};
	char *vl, *v2;

	sprintf(cmd, "nvramcli get %s", name);

    if((pf = popen(cmd, "r")) == NULL)
        return -1;
	else{
	    if(fgets(buf, sizeof(buf), pf) != NULL){
	        vl = strstr(buf, "=");
			if(vl != NULL){
				v2 = strstr(buf, "\n");
				if(v2 != NULL)
					*v2 = '\0';
		        strncpy(value, vl+1, len);
				pclose(pf);
				return strlen(value);
			}
	    }
		pclose(pf);
	}
	#endif
	return -1;
}

#define LOG_BUF_SIZE  2048
pthread_mutex_t g_sys_log_mutex = PTHREAD_MUTEX_INITIALIZER;
char g_sys_log_buf[LOG_BUF_SIZE*4];

int sys_log_set(char *name, const char *format, ...)
{
	FILE *fp = NULL;
	time_t   now;
	struct tm *timenow;
	unsigned long filesize = -1;
	va_list args;
	int len;
	int ret = 0;

	pthread_mutex_lock(&g_sys_log_mutex);

	if((NULL == name) || (NULL == format)){
		ret = -1;
		goto sys_log_end;
	}
	if((fp = fopen(name, "a+")) == NULL){
		ret = -1;
		goto sys_log_end;
	}

	fseek(fp, 0L, SEEK_END);
	filesize = ftell(fp);

#ifdef GW_X86
	if(filesize > 2621440)
#else
#ifdef MEM_128M
	if(filesize > 262144)
#else
	if(filesize > 131072)
#endif
#endif
	{
		fclose(fp);
		remove(name);
		if((fp = fopen(name, "a+")) == NULL){
			ret = -1;
			goto sys_log_end;	
		}	
	}

	g_sys_log_buf[0] = '\0';

	time(&now);
	timenow = localtime(&now);
	strftime(g_sys_log_buf, LOG_BUF_SIZE, "[%Y-%m-%d %H:%M:%S]  ", timenow);

	va_start(args, format);
	len = strlen(g_sys_log_buf);
	vsnprintf((g_sys_log_buf+len), (LOG_BUF_SIZE - 3 - len), format, args);
	va_end(args);
	
	strcat(g_sys_log_buf, "\n\n");

	fwrite(g_sys_log_buf, strlen(g_sys_log_buf), 1, fp);
	
	fclose(fp);

sys_log_end:
	pthread_mutex_unlock(&g_sys_log_mutex);
	return ret;
}

int debug_log_set(char *name, int id, const char *format, ...)
{
	FILE *fp = NULL;
	time_t   now;
	struct tm *timenow;
	unsigned long filesize = -1;
	va_list args;
	int len;
	char *log_buf = NULL;
	int ret = 0;
	char filename[128] = {0};

	if((NULL == name) || (NULL == format)){
		ret = -1;
		goto debug_log_end;
	}
	snprintf(filename, sizeof(filename), "%s%d", name, id);
	if((fp = fopen(filename, "a+")) == NULL){
		ret = -1;
		goto debug_log_end;
	}

	fseek(fp, 0L, SEEK_END);
	filesize = ftell(fp);

#ifdef GW_X86
	if(filesize > 2621440)
#else
#ifdef MEM_128M
	if(filesize > 262144)
#else
	if(filesize > 131072)
#endif
#endif
	{
		fclose(fp);
		remove(filename);
		if((fp = fopen(filename, "a+")) == NULL){
			ret = -1;
			goto debug_log_end;	
		}
	}

	if((log_buf = (char *)malloc(LOG_BUF_SIZE)) == NULL){
		fclose(fp);
		ret = -1;
		goto debug_log_end;
	}

	time(&now);
	timenow = localtime(&now);
	strftime(log_buf, LOG_BUF_SIZE, "[%Y-%m-%d %H:%M:%S]  ", timenow);

	va_start(args, format);
	len = strlen(log_buf);
	vsnprintf((log_buf+len), (LOG_BUF_SIZE - 3 - len), format, args);
	va_end(args);
	
	strcat(log_buf, "\n\n");

	fwrite(log_buf, strlen(log_buf), 1, fp);
	
	fclose(fp);
	free(log_buf);

debug_log_end:
	return ret;
}

int strmac_to_mac(char *str_mac, unsigned char *mac)
{
	int i;
	unsigned char val1 = 0, val2 = 0;

	if((NULL == str_mac) || (NULL == mac))
		return -1;
	
	/*str_mac: xx:xx:xx:xx:xx:xx*/
	for(i=0; i<17; ++i)
	{
		switch(i%3)
		{
		case 0:	
			if((str_mac[i] >= 'a') && (str_mac[i] <= 'f')){
				val2 = str_mac[i] - 'a' + 0xA;
			}
			else if((str_mac[i] >= 'A') && (str_mac[i] <= 'F')){
				val2 = str_mac[i] - 'A' + 0xA;
			}
			else if((str_mac[i] >= '0') && (str_mac[i] <= '9')){
				val2 = str_mac[i] - '0';
			}
			else
				return -1;
			break;
		case 1:
			if((str_mac[i] >= 'a') && (str_mac[i] <= 'f')){
				val1 = str_mac[i] - 'a' + 0xA;
			}
			else if((str_mac[i] >= 'A') && (str_mac[i] <= 'F')){
				val1 = str_mac[i] - 'A' + 0xA;
			}
			else if((str_mac[i] >= '0') && (str_mac[i] <= '9')){
				val1 = str_mac[i] - '0';
			}
			else
				return -1;
			if(i<2)
				mac[0] = val1 + val2*16;
			else if(i<5)
				mac[1] = val1 + val2*16;
			else if(i<8)
				mac[2] = val1 + val2*16;
			else if(i<11)
				mac[3] = val1 + val2*16;
			else if(i<14)
				mac[4] = val1 + val2*16;
			else
				mac[5] = val1 + val2*16;
			break;
		case 2:
			if(str_mac[i] != ':')
				return -1;
			break;
		default:
			break;
		}
	}

	return 0; 
}


int strip_to_intip(char *srcip, char *dstip)
{
    int iRet;
    int loop;
    int ip[4];

    if(NULL == srcip || NULL == dstip)
    {
        return -1;
    }
    iRet = 0;
    iRet = sscanf(srcip, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);
    if(4 != iRet)
    {
        return 1;
    }
    for(loop = 0; loop < 4; loop++)
    {
        dstip[loop] = ip[loop];
    }

    return 0;
}




int addr_to_ip_string(const char *addr, char *ip, int len)
{
	struct hostent *he;
	struct in_addr h_addr, *in_addr_temp;
	
	if((he = gethostbyname(addr)) == NULL)
		return -1;
	
	in_addr_temp = (struct in_addr *)he->h_addr_list[0];
	h_addr.s_addr = in_addr_temp->s_addr;

	strncpy(ip, inet_ntoa(h_addr), len);
	
	return 0;
}


int strncat_format(char *str, int len, const char *format, ...)
{
	va_list args;
	int s_len;

	if(NULL == str)
		return -1;

	s_len = strlen(str);
	if(s_len > len)
		return -1;

	va_start(args, format);
	vsnprintf((str + s_len), (len - s_len), format, args);
	va_end(args);

	return 0;
}

