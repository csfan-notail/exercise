/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : auth_server.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/11/26
  Last Modified :
  Description   : auth_server.c header file
  Function List :
  History       :
  1.Date        : 2013/11/26
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#ifndef __AUTH_SERVER_H__
#define __AUTH_SERVER_H__

#define ON_LINE     1
#define OFF_LINE    0

/** @brief Ask the central server to login a client */
#define REQUEST_TYPE_LOGIN     "login"
/** @brief Notify the the central server of a client logout */
#define REQUEST_TYPE_LOGOUT    "logout"
/** @brief Update the central server's traffic counters */
#define REQUEST_TYPE_COUNTERS  "counters"

/** @brief Sent when the user's token is denied by the central server */
#define GATEWAY_MESSAGE_DENIED     "denied"
/** @brief Sent when the user's token is accepted, but user is on probation  */
#define GATEWAY_MESSAGE_ACTIVATE_ACCOUNT     "activate"
/** @brief  Sent when the user's token is denied by the central server because the probation period is over */
#define GATEWAY_MESSAGE_ACCOUNT_VALIDATION_FAILED     "failed_validation"
/** @brief Sent after the user performed a manual log-out on the gateway  */
#define GATEWAY_MESSAGE_ACCOUNT_LOGGED_OUT     "logged-out"

extern int connect_auth_server(void);

#endif /* __AUTH_SERVER_H__ */

