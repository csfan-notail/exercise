/*
 * Copyright (C) GBCOM ltd.,co 2015
 *
 * This is a part of project tr69cd, which implements TR069 protocal(aka
 * CWMP).
 * 
 * This is a PRIVATE software.
 * All rights reserved.
 */
 
#ifndef HTTPPROTO_H
#define HTTPPROTO_H

typedef enum {
   sIdle,
   sAuthenticating,
   sAuthenticated,
   sAuthFailed,
   sShutdown
} AuthState;
 
typedef enum {
	eClosed,    /* connection is closed */
	eClose,		/* set Connection: close on next send */
	eStart,		/* connection is connecting */
	eConnected,	/* connection has completed */
} HttpState;

typedef void (*CallBack)(AcsStatus);

typedef struct HttpTask {
   /* current posted msg dope */
   HttpState      eHttpState;
   AuthState      eAuthState;
   tWgetInternal  *wio;
   char           *postMsg;
   int            postLth;
   int            content_len;
   AcsStatus      xfrStatus;
   CallBack       callback;
   char           *authHdr;		
} HttpTask;

#endif /*HTTPPROTO_H*/
