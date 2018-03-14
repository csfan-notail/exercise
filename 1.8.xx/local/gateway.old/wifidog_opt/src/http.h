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

extern pthread_mutex_t first_page_mutex;
#define LOCK_FIRST_PAGE()		pthread_mutex_lock(&first_page_mutex)
#define UNLOCK_FIRST_PAGE()		pthread_mutex_unlock(&first_page_mutex)


typedef enum {
    AUTH_ERROR = -1, /**< An error occured during the validation process*/
    AUTH_DENIED = 0, /**< Client was denied by the auth server */
    AUTH_ALLOWED = 1, /**< Client was granted access by the auth server */
    AUTH_VALIDATION = 5, /**< A misnomer.  Client is in 15 min probation to validate his new account */
    AUTH_VALIDATION_FAILED = 6, /**< Client had X minutes to validate account by email and didn't = too late */
    AUTH_LOCKED = 254 /**< Account has been locked */
} t_authcode;

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
#define APPLE_SUCCESS "/etc/apple-success.html"
#endif

/** @brief Authenticate a single client against the central server */
void _authenticate_client(request *r, char *ip, char *mac, char *other_ip, char *other_mac);
void authenticate_client(request *r);

extern void process_http(httpd *webserver, request *r);

/**@brief Callback for libhttpd, main entry point for captive portal */
void http_callback_404(httpd *webserver, request *r);
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
void http_alipay_token_check_request(httpd *webserver, request *r,char *token);

void http_callback_roaming(httpd *webserver, request *r);
void http_callback_sta_get_itsmac(httpd *webserver, request *r);
void http_callback_sta_logout(httpd *webserver, request *r);

void http_wxl_redirect_callback(httpd *webserver, request *r);
void http_wxl_redirect2boa_callback(httpd *webserver, request *r);

void http_alipay_redirect_callback(httpd *webserver, request *r);


/** @brief Sends a HTML page to web browser */
void send_http_page(request *r, const char *title, const char* message);

void send_apple_success(request *r);

/** @brief Sends a redirect to the web browser */
void http_send_redirect(request *r, char *url, char *text);
/** @brief Convenience function to redirect the web browser to the authe server */
void http_send_redirect_to_auth(request *r, char *urlFragment, char *text);

/** @brief Convenience function to redirect the web browser to the local server */
void http_send_redirect_to_local(request *r, char *urlFragment, char *text);

void http_send_to_local(request *r, char *html_file, char *text);

t_authcode auth_server_request(request *r, const char *request_type, const char *ip, 
                                const char *mac, const char *token, unsigned long long int incoming, unsigned long long int outgoing);


int http_auth_sta(char *mac,int iAuthType,char *pBblink_userid);
int http_close_sta(char *mac);
                               
/*2015-3-12  ali auth*/
t_authcode ali_auth_server_request(request *r, const char *request_type, const char *ip, 
                                const char *mac,  char *token, unsigned long long int incoming, unsigned long long int outgoing);

#endif /* __HTTP_H__ */
