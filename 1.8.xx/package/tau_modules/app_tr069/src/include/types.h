/*
 * Copyright (C) GBCOM ltd.,co 2015
 *
 * This is a part of project tr69cd, which implements TR069 protocal(aka
 * CWMP).
 * 
 * This is a PRIVATE software.
 * All rights reserved.
 */
 
//#if 0
#ifndef _MAIN_TYPES_H_
#define _MAIN_TYPES_H_

#if 0
#define GC_DEBUG
#include "/usr/local/include/gc.h"
#if 1
#define malloc(n) GC_MALLOC(n)
#define calloc(m, n) GC_MALLOC((m)*(n))
#define free(p) GC_FREE(p)
#define realloc(p,n) GC_REALLOC((p),(n))
#endif
#define CHECK_LEAKS() GC_gcollect()
//#else
#define CHECK_LEAKS() 
//#endif
#endif

/*----------------------------------------------------------------------
 * types
 */

typedef unsigned int tIpAddr;

#define LOG_ERROR LOG_ERR

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#endif
