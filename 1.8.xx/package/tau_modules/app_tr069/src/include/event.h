/*
 * Copyright (C) GBCOM ltd.,co 2015
 *
 * This is a part of project tr69cd, which implements TR069 protocal(aka
 * CWMP).
 * 
 * This is a PRIVATE software.
 * All rights reserved.
 */
 
#ifndef EVENT_H
#define EVENT_H

/**********************************************************************
 * typedefs
 *********************************************************************/

/*----------------------------------------------------------------------
 * type of listener, see select() for more information
 */
typedef enum {
  iListener_Read,
  iListener_Write,
  iListener_Except,
  iListener_ReadWrite
} tListenerType;


/*
 * listeners are all on form
 *    void f(void *) {}
 */
typedef void (*EventHandler)(void*);

/**********************************************************************
 * public functions
 *********************************************************************/

extern void stepTime(struct timeval *tv);

extern void stopTimer (EventHandler func, void *handle);
extern int  checkTimer(EventHandler func, void *handle);
extern int  setTimer  (EventHandler func, void *handle, int ms);

extern void stopListener(int fd);
extern void setListener (int fd, EventHandler func, void* handle);
extern void setListenerType(int fd, EventHandler func, void* handle, tListenerType type);

extern void stopCallback(void *target, EventHandler func, void *handle);
extern void setCallback(void *target, EventHandler func, void *handle);
extern void notifyCallbacks(void *target);
extern void notifyCallbacksAndWait(void *target);

extern void eventLoop(void);

extern void closeAllFds(void);


#endif
