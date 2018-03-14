/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : http.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/11/27
  Last Modified :
  Description   : http.c header file
  Function List :
  History       :
  1.Date        : 2013/11/27
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#ifndef __HTTP_H__
#define __HTTP_H__

typedef enum {
    AUTH_ERROR = -1, /**< An error occured during the validation process*/
    AUTH_DENIED = 0, /**< Client was denied by the auth server */
    AUTH_ALLOWED = 1, /**< Client was granted access by the auth server */
    AUTH_VALIDATION = 5, /**< A misnomer.  Client is in 15 min probation to validate his new account */
    AUTH_VALIDATION_FAILED = 6, /**< Client had X minutes to validate account by email and didn't = too late */
    AUTH_LOCKED = 254 /**< Account has been locked */
} t_authcode;
#define ITOOLS   "www.itools.info" 
#define IBOOK    "www.ibook.info" 
#define IPCELL   "www.appleiphonecell.com" 
#define AIRPORT  "www.airport.us" 
#define CAPTIVE  "captive.apple.com" 
#define THINK    "www.thinkdifferent.us" 
#define APPLE    "www.apple.com" 
/** 
 * @brief This structure contains all the information returned by the  authentication server
 */
typedef struct _t_authresponse {
    t_authcode authcode; /**< Authentication code returned by the server */
    char *ext;
} t_authresponse;

#if (defined(GW_X86) || defined(GW_ARM) || defined(GW_AC))   //BY CJ
#define APPLE_SUCCESS "/appfs/etc/apple-success.html"
#else
#ifdef OPENWRT
#define APPLE_SUCCESS "/etc/wifidog/apple-success.html"
#else
#define APPLE_SUCCESS "/etc/apple-success.html"
#endif
#endif


#if (defined(GW_X86) || defined(GW_ARM) || defined(GW_AC))   //BY CJ
#define APPLE_NETWORK "/appfs/etc/apple-network.html"
#else
#ifdef OPENWRT
#define APPLE_NETWORK "/etc/wifidog/apple-network.html"
#else
#define APPLE_NETWORK "/etc/apple-network.html"
#endif
#endif

/** @brief Authenticate a single client against the central server */
void _authenticate_client(request *r, char *ip, char *mac, char *other_ip, char *other_mac);
void authenticate_client(request *r);

extern void process_http(httpd *webserver, request *r);

/**@brief Callback for libhttpd, main entry point for captive portal */
void http_callback_404(httpd *webserver, request *r, int check_auth);
void http_callback_404_8062(httpd *webserver, request *r);
/* portal 下沉增强方案 */
void http_callback_redirect2local(httpd *webserver, request *r);
/**@brief Callback for libhttpd */
void http_callback_wifidog(httpd *webserver, request *r);
/**@brief Callback for libhttpd */
void http_callback_about(httpd *webserver, request *r);
/**@brief Callback for libhttpd */
void http_callback_status(httpd *webserver, request *r);
/**@brief Callback for libhttpd, main entry point post login for auth confirmation */
void http_callback_auth(httpd *webserver, request *r);
httpVar * http_callback_auth_forced_attention(request *r);
void http_alipay_token_check_request(httpd *webserver, request *r,char *token);

void http_callback_roaming(httpd *webserver, request *r);
void http_callback_sta_get_itsmac(httpd *webserver, request *r);
void http_callback_sta_get_auth(httpd *webserver, request *r);
void http_callback_sta_logout(httpd *webserver, request *r);
void http_callback_sta_access(httpd *webserver, request *r);
void http_callback_thirdpartyap(httpd *webserver, request *r);

void http_wxl_redirect_callback(httpd *webserver, request *r);
void http_80_redirect2boa_callback(httpd *webserver, request *r);

void http_alipay_redirect_callback(httpd *webserver, request *r);
void http_jinjiang_auth_callback(httpd *webserver, request *r);

/** @brief Sends a HTML page to web browser */
void send_http_page(request *r, const char *title, const char* message);

void send_apple_success(request *r);
void send_apple_network(request *r);


/** @brief Sends a redirect to the web browser */
void http_send_redirect(request *r, char *url, char *text);
/** @brief Convenience function to redirect the web browser to the authe server */
void http_send_redirect_to_auth(request *r, char *urlFragment, char *text);

/** @brief Convenience function to redirect the web browser to the local server */
void http_send_redirect_to_local(request *r, char *urlFragment, char *text);

void http_send_to_local(request *r, char *html_file, char *text);

t_authcode auth_server_request(request *r, const char *request_type, const char *ip, 
                                const char *mac, const char *token, unsigned long long int incoming, unsigned long long int outgoing);



                               
/*2015-3-12  ali auth*/
t_authcode ali_auth_server_request(request *r, const char *request_type, const char *ip, 
                                const char *mac,  char *token, unsigned long long int incoming, unsigned long long int outgoing);


void http_send_thirdparty_redirect(httpd *webserver, request *r);
int jinjiang_auth_http_parse_json_data(request *r,char **jsdata);


/*begin:网间漫游 2015-8-26 */
void http_send_check_to_server(unsigned char *ip,unsigned char *pucStaMac);
void http_add_sta_to_server(t_client *client);
void http_del_sta_to_server(t_client *client,CLIENT_LOGOUT_TYPE_E eType);

void http_sta_onoff_to_server(char *pRequestType,char *pStrStaMac,char *pSsid,char *pStrApMac,char *pToken,CLIENT_LOGOUT_TYPE_E eType,request *r);


void thread_macro_hot(void);

/*end:网间漫游 2015-8-26 */


#endif /* __HTTP_H__ */
