/*
 * Copyright (C) GBCOM ltd.,co 2015
 *
 * This is a part of project tr69cd, which implements TR069 protocal(aka
 * CWMP).
 * 
 * This is a PRIVATE software.
 * All rights reserved.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
/*#ifdef linux */
#include <sys/select.h>
/*#endif  */
/* #ifdef __NetBSD__  */
#include <sys/types.h>
#include <unistd.h>
/* #endif   */
#include <sys/time.h>
#include <string.h>

/* BEGIN: Modified by zhanghaozhong, 2013/1/23   PN:gbcom add by zhz */
#define USE_SYSINFO
#ifndef linux
#define linux
#endif
/* END:   Modified by zhanghaozhong, 2013/1/23   PN:gbcom add by zhz */
#if defined(linux) && defined(USE_SYSINFO)
#include <sys/sysinfo.h>
#endif
#include <syslog.h>

#include "utils.h"
#include "event.h"

#ifdef DMALLOC
#include "dmalloc.h"
#endif
/*#define DEBUG*/
#undef DEBUG
/*#define DEBUG_TIMER */
/*#define DEBUG_LISTENER*/
/*#define DEBUG_IDLE*/
/*#define DEBUG_CALLBACK */
/*
#ifdef DEBUG
#ifndef DEBUG_TIMER
#define DEBUG_TIMER
#endif
#ifndef DEBUG_LISTENER
#define DEBUG_LISTENER
#endif
#ifndef DEBUG_CALLBACK
#define DEBUG_CALLBACK
#endif
#ifndef DEBUG_IDLE
#define DEBUG_IDLE
#endif
#endif
*/

extern int g_debug;
extern int g_log_level;


/**********************************************************************
 * internal data structures
 **********************************************************************/
typedef struct Timer {
  struct Timer* next;
  EventHandler func;
  void* handle;
  struct timeval expire;
} Timer;

typedef struct Listener {
  struct Listener* next;
  int fd;
  EventHandler func;
  void* handle;
  tListenerType type;
} Listener;

typedef struct Callback {
  struct Callback* next;
  int deleted;
  EventHandler func;
  void* handle;
} Callback;

typedef struct Target {
  struct Target* next;
  void *target_addr;
  Callback *callbacks;
} Target;

typedef struct ActiveTarget {
  struct ActiveTarget* next;
  void *target_addr;
} ActiveTarget;

/**********************************************************************
 * Globals
 *********************************************************************/

static Timer* timers;
static Listener* listeners;
static Target* targets;
static ActiveTarget* scheduled_targets;
static int callback_deleted;

/**********************************************************************
 * Utilities
 *********************************************************************/

#if defined(DEBUG_TIMER) || defined(DEBUG_LISTENER) || defined(DEBUG_CALLBACK) || defined(DEBUG_IDLE)
static struct timeval ts;

static void sub(struct timeval *t0, struct timeval *t1, struct timeval *res)
{
  res->tv_sec = t1->tv_sec - t0->tv_sec;
  res->tv_usec = t1->tv_usec - t0->tv_usec;
  if (res->tv_usec < 0) {
    res->tv_sec--;
    res->tv_usec += 1000000;
  }
}

static void stop_timer(const char *tag);

static void start_timer(const char *tag, void *address)
{
#ifdef DEBUG_IDLE
  if (tag)
    stop_timer(NULL);
#endif
  gettimeofday(&ts, NULL);
  if (address)
    fprintf(stderr, ">>>%s @%p ...\n", tag, (void *) address);
}

static void stop_timer(const char *tag)
{
  struct timeval res, te;
  
  gettimeofday(&te, NULL);
  sub(&ts, &te, &res);
  if (tag)
    fprintf(stderr, "<<<%s (%ld.%06ld)\n", tag, res.tv_sec, res.tv_usec);
  else if (res.tv_sec > 0 || res.tv_usec > 10000)
    fprintf(stderr, "===idle (%ld.%06ld)\n",  res.tv_sec, res.tv_usec);
#ifdef DEBUG_IDLE
  if (tag)
    start_timer(NULL, NULL);
#endif
}
#endif

static void gettime(struct timeval *tm)
{
#if defined(linux) && defined(USE_SYSINFO)
  struct sysinfo si;
#endif

  gettimeofday(tm,0);

#if defined(linux) && defined(USE_SYSINFO)
  /* NB: This little trick prevents the timer queue from breaking
   *     if someone does a settimeofday, for instance an ntpd.
   *     The worst case is that timers get displaced one second.
   */
  sysinfo(&si);
  tm->tv_sec=si.uptime;
#endif
}

#ifdef DEBUG_CALLBACK
/*--------------------*/
static void dump_callbacks(const char *tag, Target *t)
{
  if (t != NULL) {
    Callback *cb;

    fprintf(stderr, "DEBUG: %s ++++++++++ target=%p\n", tag, (void *) t);
    for (cb = t->callbacks; cb != NULL; cb = cb->next) {
      fprintf(stderr, "  %s %p (%p,%p)\n",
	      cb->deleted ? " " : "+",
	      (void *) cb, (void *) cb->func, (void *) cb->handle);
    }
    fprintf(stderr, "DEBUG: %s +++++++++++++++++++\n", tag);
  } else {
    fprintf(stderr, "DEBUG: %s ++++++++++ target=NULL\n", tag);
  }
}
#endif

/*--------------------*/
static void callCallbacks(void *target)
{
  Target *t = targets;

  while (t != NULL && t->target_addr != target) {
    t = t->next;
  }

#ifdef DEBUG_CALLBACK
  dump_callbacks("callCallbacks", t);
#endif
  
  if (t != NULL) {
    Callback *cb = t->callbacks;
    while (cb != NULL) {
      if (!cb->deleted) {
#ifdef DEBUG_CALLBACK
	start_timer("callback", (void *) cb->func);
#endif
	cb->func(cb->handle);
#ifdef DEBUG_CALLBACK
	stop_timer("callback");
#endif
      }
      cb = cb->next;
    }      
  }
}

/*--------------------*/
static void remove_deleted_callbacks(void) {
  Target *t, **th = &targets;

  while ((t = *th) != NULL) {
    Callback *cb, **cbh = &t->callbacks;
    while ((cb = *cbh) != NULL) {
      if (cb->deleted) {
	*cbh = cb->next;
	free(cb);
	cb=NULL;
      } else
	cbh = &cb->next;
    }
    if (t->callbacks == NULL) {
      *th = t->next;
      free(t);
	  t=NULL;
    } else
      th = &t->next;
  }
}

/*--------------------*/
static int checkCallbacks(ActiveTarget **ath)
{
  if (*ath == NULL)
    return 0;
  callback_deleted = FALSE;
  if ((*ath)->next != NULL)
    checkCallbacks(&((*ath)->next));
  callCallbacks((*ath)->target_addr);
  free(*ath);
  *ath = NULL;
  if (callback_deleted)
    remove_deleted_callbacks();
  return 1;
}

/**********************************************************************
 * Timers
 *********************************************************************/

/*--------------------*/
static void callbackTimer(void *handle)
{
  ActiveTarget *active_targets = scheduled_targets;
  scheduled_targets = NULL;
  checkCallbacks(&active_targets);
}

/**********************************************************************
 * Public functions
 *********************************************************************/

/*--------------------*/
void stepTime(struct timeval *tv) {
  Timer *t;
  for (t = timers; t; t = t->next) {
    t->expire.tv_sec += tv->tv_sec;
    t->expire.tv_usec += tv->tv_usec;
    if (t->expire.tv_usec > 1000000) {
      t->expire.tv_sec++;
      t->expire.tv_usec -= 1000000;
    }
  }
}

/*--------------------*/
void stopTimer(EventHandler func, void *handle)
{
   Timer* curr = NULL;
   Timer* prev = NULL;

   // find Timer that matches func and handle
   for ( prev = curr = timers;
         curr != NULL &&
         (curr->func != func || curr->handle != handle);
         curr = curr->next )
      prev = curr;

   if ( curr != NULL ) {
      // remove the match Timer
      prev->next = curr->next;
      /* move head pointer to the next one
         if head node is removed */
      if ( timers == curr )
         timers = curr->next;
      curr->func = NULL;
      curr->handle = NULL;
      curr->next = NULL;
      free(curr);
      curr = NULL;
   }
}
#if 0
void stopTimer(EventHandler func, void *handle)
{
  Timer* t;
  Timer** th = &timers;

  while ((t = *th)) {
    if (t->func == func && t->handle == handle) {
      *th = t->next;
      free(t);
    } else {
      th =& t->next;
    }
  }
}
#endif
/*--------------------*/
int checkTimer(EventHandler func, void *handle)
{
  Timer* t;

  for (t = timers; t; t = t->next) {
    if (t->func == func && t->handle == handle) {
      return 1;
    }
  }
  return 0;
}

/*--------------------*/
/* returns: 0 - unable to malloc timer structure */
/*          1 - timer queued */
int setTimer(EventHandler func, void *handle, int ms)
{
  Timer* t;
  Timer* t1;
  Timer** th = &timers;
  struct timeval tm;

  stopTimer(func, handle);

  gettime(&tm);
  addMs(&tm, &tm, ms);

  while ((t = *th)) {
    if (cmpTime(&tm, &t->expire) < 0) {
      break;
    } else {
      th = &t->next;
    }
  }

  if ( (t1 = malloc(sizeof *t1))==0)
      return 0;
  t1->func = func;
  t1->handle = handle;
  t1->expire.tv_sec = tm.tv_sec;
  t1->expire.tv_usec = tm.tv_usec;

  t1->next = t;
  *th = t1;
 
  return 1;
}

void stopListener(int fd)
{
   Listener* curr = NULL;
   Listener* prev = NULL;

   // find Listener that matches fd
   for ( prev = curr = listeners;
         curr != NULL &&
         curr->fd != fd;
         curr = curr->next )
      prev = curr;

   if ( curr != NULL ) {
      // remove the match Listener
      prev->next = curr->next;
      /* move head pointer to the next one
         if head node is removed */
      if ( listeners == curr )
         listeners = curr->next;
      // don't close fd since it's closed by caller
      // close(fd);         
      curr->func = NULL;
      curr->handle = NULL;
      curr->next = NULL;
      free(curr);
      curr = NULL;
   }
}

#if 0
void stopListener(int fd)
{
  Listener* l;
  Listener** lh=&listeners;

  while((l=*lh)) {
    if(l->fd==fd) {
      *lh=l->next;
      free(l);
    } else {
      lh=&l->next;
    }
  }
}
#endif
/*--------------------*/
void setListener(int fd, EventHandler func, void* handle)
{
  setListenerType(fd, func, handle, iListener_Read);
}

/*--------------------*/
void setListenerType(int fd, EventHandler func, void* handle, tListenerType type)
{
  Listener* l;

  stopListener(fd);

  l = malloc(sizeof *l);
  l->fd = fd;
  l->func = func;
  l->handle = handle;
  l->next = listeners;
  l->type = type;
  listeners = l;
}

/*--------------------*/
static Callback *lookup_callback(void *target, EventHandler func, void *handle)
{
  Target *t;

  for (t = targets; t != NULL; t = t->next) {
    if (t->target_addr == target) {
      Callback *cb;
      for (cb = t->callbacks; cb != NULL; cb = cb->next) {
	if (cb->func == func && cb->handle == handle) {
	  return cb;
	}
      }
    }    
  }
  return NULL;
}

/*--------------------*/
void stopCallback(void *target, EventHandler func, void *handle)
{
  Callback *cb;

  if ((cb = lookup_callback(target, func, handle)) != NULL) {
    cb->deleted = TRUE;
    callback_deleted = TRUE;
  }
}

/*--------------------*/
void setCallback(void *target, EventHandler func, void *handle)
{
  Target *t;
  Callback *cb;

  /* lookup callback */
  if ((cb = lookup_callback(target, func, handle)) != NULL) {
    /* callback already exists, ignore */
    slog(TR69C_LOG_DEBUG,"setCallback(), callback %p/%p aready exists (ignore).\n", (void *) func, handle);
    return;
  }
      
#ifdef DEBUG_CALLBACK
  fprintf(stderr, "DEBUG: +++ setCallback(%p, %p, %p)\n", target, (void *) func, handle);
#endif

  t = targets;
  cb = (Callback *) malloc(sizeof(Callback));
  cb->deleted = FALSE;
  cb->handle = handle;
  cb->func = func;
  while (t != NULL) {
    if (t->target_addr == target)
      break;
    t = t->next;
  }

#ifdef DEBUG_CALLBACK
  dump_callbacks("PRE setCallback", t);
#endif

  if (t == NULL) {
    t = (Target *)malloc(sizeof(Target));
    t->target_addr = target;
    t->callbacks = NULL;
    t->next = targets;  /* 把t插入到targets链表里 */
    targets = t;
  }
  cb->next = t->callbacks;  /* 把cb插入到t->calbacks链表里 */
  t->callbacks = cb;

#ifdef DEBUG_CALLBACK
  dump_callbacks("POST setCallback", t);
#endif
}

/*--------------------*/
void notifyCallbacks(void *target)
{
  ActiveTarget *at = (ActiveTarget *)malloc(sizeof(ActiveTarget));
  at->target_addr = target;
  at->next = scheduled_targets;
  scheduled_targets = at;
  if (!checkTimer(callbackTimer, NULL))
    setTimer(callbackTimer, NULL, 0);
}

void notifyCallbacksAndWait(void *target)
{
  ActiveTarget *at = (ActiveTarget *)malloc(sizeof(ActiveTarget));
  at->target_addr = target;
  at->next = NULL;
  checkCallbacks(&at);
}

/*--------------------*/
void eventLoop(void)
{
  slog(TR69C_LOG_DEBUG,"start eventLoop");

  while (timers || listeners) 
  {
			int i, n;
			int res;
			fd_set rfds, wfds, efds;
			struct timeval tm;
			Listener* l;

			n = 0;
			FD_ZERO(&rfds);
			FD_ZERO(&wfds);
			FD_ZERO(&efds);
			for (l = listeners; l; l = l->next) 
			{
					  switch (l->type) 
					  {
						  case iListener_Read:
							FD_SET(l->fd, &rfds);
							break;
						  case iListener_Write:
							FD_SET(l->fd, &wfds);
							break;
						  case iListener_ReadWrite:
							FD_SET(l->fd, &rfds);
							FD_SET(l->fd, &wfds);
							break;
						  case iListener_Except:
							FD_SET(l->fd, &efds);
							break;
						  default:
							fprintf(stderr, "Impossible error: eventLoop(): illegal listener type (%d)\n", l->type);
							break;
					  }
					  if (n <= l->fd) 
					  {
						  n = l->fd + 1;
					  }
			}
				/*change by duanguiyuan 20160320*/
				   if (timers) 
				   {
							  printf("select time ok!!!!!!\n");
							  // if (0) {
							  gettime(&tm);                 /* 获取当前时间 */
							  subTime(&tm, &timers->expire);    /* tm为时间间隔 */ /* 定时器的实现是采用时间差，addMS()来设置这个时间*/
							  if (tm.tv_sec < 0) 
							  {			/* if timer has expired then remove from timers queue*/
										Timer* t = timers;			/* and call t->func */
										EventHandler func = t->func;
										void* handle = t->handle;

										timers = t->next;
										free(t);
										t=NULL;
										#ifdef DEBUG_TIMER
										start_timer("timer", (void *) func);
										#endif
										func(handle);
										#ifdef DEBUG_TIMER
										stop_timer("timer");
										#endif
										continue;
							  }
							  res = select(n, &rfds, &wfds, &efds, &tm); /* 定时器的实现，采用select超时后执行fun(handle) */
					} 
					else 
					{
										 if (g_debug)
											fprintf(stderr, "select %d [\n",n); /* open by zhanghaozhong */
										  res = select(n, &rfds, &wfds, &efds, 0);
					}
					if (g_debug)
						fprintf(stderr, "select res=%d\n", res);/* open by zhanghaozhong */
					if (res < 0 && errno != EINTR) 
					{
					  slog(TR69C_LOG_CRIT, "Error in select (%d=%s)", errno, strerror(errno));
					  continue; 
					  /*return;*/
					}

					for (i = 0; i<res; ++i) 
					{ /* this is to keep trying until the number of fd returned from*/
								/* the select are processed */
						for (l = listeners; l; l = l->next) 
						{
								  if (FD_ISSET(l->fd, &rfds) || FD_ISSET(l->fd, &wfds) || FD_ISSET(l->fd, &efds)) 
								  {
											  /* Clear the fd in the fdset so we don't call the func again */
											FD_CLR(l->fd, &rfds); FD_CLR(l->fd,&wfds); FD_CLR(l->fd,&efds);

											#ifdef DEBUG_LISTENER
												start_timer("listener", (void *) l->func);
											#endif
											if (g_debug)
												fprintf(stderr, "***eventloop l->fd = %d\n", l->fd); /* open by zhanghaozhong */
											l->func(l->handle);    /* l is invalid following this call */
											#ifdef DEBUG_LISTENER
												stop_timer("listener");
											#endif
											break;
								  }
						}
					}
		  if (g_debug)
		     fprintf(stderr, "]\n"); /* open by zhanghaozhong */
  }
}

/*--------------------*/
void closeAllFds(void)
{
	Listener* l;
	for (l = listeners; l; l = l->next)
		close(l->fd);
}

