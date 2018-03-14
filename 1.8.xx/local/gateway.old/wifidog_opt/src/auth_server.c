/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : auth_server.c
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/11/26
  Last Modified :
  Description   : 
  Function List :
  History       :
  1.Date        : 2013/11/26
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <stdarg.h>
#include "cJSON.h"
#include "config.h"
#include "conf.h"
#include "util.h"
#include "log.h"
#include "common.h"
#include "httpd.h"
#include "client_list.h"
#include "firewall.h"
#include "auth_server.h"

int _connect_auth_server()
{
    s_config *config;
    t_auth_serv *auth_server = NULL;
	struct in_addr *h_addr;
	char *hostname;
	char *ip;
	struct sockaddr_in their_addr;
	int sockfd;

    config = config_get_config();

    auth_server = config->auth_servers;
    hostname = auth_server->authserv_hostname;
    h_addr = wd_gethostbyname(hostname);
    if(!h_addr)
    {
        //TODO ... 无法解析平台服务器流程处理
        log_info(__LOG_ERROR__, "Marking auth server [%s] as bad", hostname);
        return SOCKET_INVALID;
    }
    else
    {
        ip = safe_strdup(inet_ntoa(*h_addr));
        log_info(__LOG_DEBUG__, "Resolving auth server [%s] succeeded = [%s]", hostname, ip);
        log_info(__LOG_DEBUG__, "Connecting to auth server %s:%d", hostname, auth_server->authserv_http_port);

        /** @每次更新平台地址 */
        if(auth_server->last_ip) free(auth_server->last_ip);
        auth_server->last_ip = ip;
        
		their_addr.sin_family = AF_INET;
		their_addr.sin_port = htons(auth_server->authserv_http_port);
		their_addr.sin_addr = *h_addr;
		memset(&(their_addr.sin_zero), '\0', sizeof(their_addr.sin_zero));
		free (h_addr);

		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			log_info(__LOG_ERROR__, "Failed to create a new SOCK_STREAM socket: %s", strerror(errno));
			return SOCKET_INVALID;
		}

		if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) {
			/*
			 * Failed to connect
			 * Mark the server as bad and try the next one
			 */
			log_info(__LOG_DEBUG__, "Failed to connect to auth server %s:%d (%s). Marking it as bad and trying next if possible", hostname, auth_server->authserv_http_port, strerror(errno));
			close(sockfd);
			return SOCKET_INVALID;
		}
		else {
			/*
			 * We have successfully connected
			 */
			log_info(__LOG_DEBUG__, "Successfully connected to auth server %s:%d", hostname, auth_server->authserv_http_port);
			return sockfd;
		}
    }
    
}

int connect_auth_server(void)
{
    int sockfd;

    sockfd = _connect_auth_server();
    if(SOCKET_INVALID == sockfd)
    {
        log_info(__LOG_ERROR__, "Failed to connect to auth servers");
        return SOCKET_INVALID;
    }
    else
    {
        log_info(__LOG_DEBUG__, "Connect to auth servers");
    }
    return sockfd;    
}


