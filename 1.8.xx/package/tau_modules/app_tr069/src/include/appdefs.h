/*
 * Copyright (C) GBCOM ltd.,co 2015
 *
 * This is a part of project tr69cd, which implements TR069 protocal(aka
 * CWMP).
 * 
 * This is a PRIVATE software.
 * All rights reserved.
 */
 
extern  int g_debug;    //DEBUG 是否开启

/* Buffer sizes */

#define MAXWEBBUFSZ	132217728   /* 2**28 - Default web message/download max buffer size */
#define ATTRIBUTESAVESZ	2048     /* maximum size of notification attribute */
                                 /* storage in scratch pad */
/* compile time options */
/* xml debug output options */
//#define		DUMPSOAPOUT 		/* DUMP xml msg sent to ACS to stderr */
//#define		DUMPSOAPIN	 		/* DUMP xml msg from ACS to stderr */
/* xml formatting options */
/* #define 	OMIT_INDENT	  */  /* don't intend xml tag lines - smaller messages */

/* use openSSL lib to support https: */
/*altered by caibing xxx*/
#define    USE_SSL  
#define    WRITEPIDFILE

/* Allow Reboot and Factory Reset on ACS disconnect error */
#define		ALLOW_DISCONNECT_ERROR	/* acsDisconnect will call reboot and reset clean up*/
									/* even if there is an ACS disconnect error */
/* Authentication options */
#define		ALLOW_AUTH_RECONNECT	/* Some web servers such as Apache will close */
									/* the connection when sending a 401 status	  */
									/* This allows the CPE to close the current connection */
									/* and reconnect to the server with the Authorization */
									/* header in the first POST message. The CPE will */
									/* attempt this if the connection is closed following */
									/* the 401 status from the server */
									/* Undefining this will prohibit the CPE from sending */
									/* the Authorization on a new connection */

/* Generic compile time flags combination may be defined for specific ACS below */

#define GENERATE_SOAPACTION_HDR      /* generate SOAPAction header in POST reqs*/

/* TR-069 schema flags */
//#define SUPPRESS_SOAP_ARRAYTYPE		/* suppress generation of soap-env arraytype */
									/* such as
										<Event SOAP-END:arrayType="cwmp:EventStruct[x]">
									   SUPPRESS_SOAP_ARRAYTYPE generates
									    <Event>
									***/

/* ACS Connection Initiation */

/* 从WEB上读取，用全局变量uiTr69cPort代替 */
//#define ACSCONN_PORT         7547       /* PORT to listen for acs connections */
#define ACSREALM    "IgdAuthentication"
#define ACSDOMAIN   "/"
#define ACSCONNPATH "/"
#define CFMLISTENPORT		 TR69C_PORT		/* port to listen for signals from cfm*/
                                        /* 同util_udp.h中TR69C_PORT(7357),CGI向此进程发送消息使用此端口
                                           add by zhanghaozhong */
/* BEGIN: Added by zhanghaozhong, 2013/7/5   PN:add nat */
#define NATUDPCPEPORT		 8731
#define NATUDACSPPORT		 8731

/* END:   Added by zhanghaozhong, 2013/7/5   PN:add nat */
/* Timer valuse */
#define	LOCK_CHECK_INTERVAL	 1000		/* interval to attempt to lock local CFM */
										/* configuration data (ms)*/
										
#define ACSINFORMDELAY  500     /* initial delay (ms) after pgm start before */
                                /* sending inform to ACS */
#define CHECKWANINTERVAL    (60*1000) /* check wan up */
#define ACSRESPONSETIME    (60*1000) /* MAX Time to wait on ACS response */
#define CFMKICKDELAY		(3*1000) /* time to delay following a msg from the cfm*/
									/* before checking notifications. Allows the*/
									/* cfm to complete before starting */
						/* Retry intervals for ACS connect failures    */
						/* Retry time decays by 
						   CONN_DECAYTIME*<consecutivefailures> upto a
						   maximum of CONNECT_DECAYTIME*CONNDECAYMAX */ 
#define CONN_DECAYMAX		6	/* maximum number for decaying multiple */
#define CONN_DECAYTIME		10	/* decay time per multiple */
                                
 
/* Constants                           */

#define USER_AGENT_NAME    "GS_TR69_CPE-0.2"

#define TR69_DIR   "/var/tr69"

#define TR69_PID_FILE  "/var/tr69/tr69pid"

#define SHELL      "/bin/sh"

#define HTTP_PUT_BOUNDARY_KEY "------------------------02218836ef78434b" 
#define CONTET_TYPE_OTHER 	"application/octet-stream"
#define CONTET_TYPE_TEXT 	"text/plain"
//altered by caibing 
//#define USE_CERTIFICATES
/* ACS Server Certificate File path */
#define	CERT_FILE	"/var/cert/acscert.cacert"

#ifdef USE_CERTIFICATES
#define ACS_CIPHERS "RSA:DES:SHA+RSA:RC4:SAH+MEDIUM"
#else
//#define ACS_CIPHERS "EXPORT40:SSLv3"
#define ACS_CIPHERS "SSLv3"
#endif


/************************************************************/
/* compile time conditions for specific ACS                 */
/* Uncomment the required definition                        */ 
/************************************************************/
//#define SUPPORT_ACS_CISCO
//#define SUPPORT_ACS_GS
//#define SUPPORT_ACS_DIMARK
//#define SUPPORT_ACS_PIRELLI
//#define SUPPORT_ACS_2WIRE
//#define SUPPORT_ACS_SUPPORT_SOFT_TOT
 
#define GBCOM

/* set conditional compile flags based on ACS choice */

#ifdef SUPPORT_ACS_CISCO
#define    FORCE_NULL_AFTER_INFORM      /* Set timer to force null http after sending Inform*/
#define 	SUPPRESS_EMPTY_PARAM         /* Cisco doesn't handle <param></param> form */
/*#define	SUPPRESS_XML_NEWLINES */  /* replaces \n with space to avoid cisco tool parser problem*/                                        
#endif /* SUPPORT_ACS_CISCO */

/* #ifdef */ /* next ACS ????? */

/*#endif*/   /*        */
