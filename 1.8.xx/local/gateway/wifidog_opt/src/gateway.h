#ifndef __GATEWAY_H__
#define __GATEWAY_H__
#include "common.h"
extern request client_request[CLIENT_TCP_8060_NUM];
extern pthread_mutex_t g_client_request_8060;
extern request *get_invalid_sockfd(request *r, int max);

#endif

