/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : www.c
  版 本 号   : 初稿
  作    者   : zhanghaozhong
  生成日期   : 2013年5月16日
  最近修改   :
  功能描述   : www
  函数列表   :
              hexToDec
              main
              unescapeUrl
              www_EstablishConnection
              www_ParseUrl
              www_StripTail
              www_UrlDecode
              www_UrlEncode
  修改历史   :
  1.日    期   : 2013年5月16日
    作    者   : zhanghaozhong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "types.h"
#include "www.h"

#ifdef DMALLOC
#include "dmalloc.h"
#endif
/* #define DEBUG 1   */

/*----------------------------------------------------------------------*
 * Converts hexadecimal to decimal (character):
 */
static char hexToDec(char *what)
{
   char digit;

   digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A')+10 : (what[0] - '0'));
   digit *= 16;
   digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A')+10 : (what[1] - '0'));

   return (digit);
}

/*----------------------------------------------------------------------*
 * Unescapes "%"-escaped characters in a query:
 */
static void unescapeUrl(char *url)
{
   int x, y, len;

   len = strlen(url);
   for (x = 0, y = 0; url[y]; x++, y++) {
      if ((url[x] = url[y]) == '%' && y < (len - 2)) {
	  url[x] = hexToDec(&url[y+1]);
	  y += 2;
      }
   }
   url[x] = '\0';
}

/*----------------------------------------------------------------------*/
void www_UrlDecode(char *s)
{
   char *pstr = s;

   /* convert plus (+) to space (' ') */
   for (pstr = s; pstr != NULL && *pstr != '\0'; pstr++) {
       if (*pstr == '+')
	   *pstr = ' ';
   }
   unescapeUrl(s);
}

/*----------------------------------------------------------------------*/
void www_UrlEncode(const char *s, char *t)
{
  while (*s) {
    if (*s == ' ') {
      *t++ = '+';
    } else if (isalnum(*s)) {
      *t++ = *s;
    } else {
      /* hex it */
      *t++ = '%';
      sprintf(t, "%2x", *s);
      t += 2;
    }
    s++;
  }
  *t = '\0';
}

/*----------------------------------------------------------------------*
 * parse url on form:
 *  "<proto>://<host>[:<port>][<uri>]"
 *  returns
 *    0 if parse ok
 *   -1 if parse failed
 *  port sets to 0 if no port is specified in URL
 *  uri is set to "" if no URI is specified
 */
int www_ParseUrl(const char *url, char *proto, char *host, int *port, char *uri)
{
  int n;
  char *p;

  *port = 0;
  strcpy(uri, "");

  /* proto */
  p = (char *) url;
  if ((p = strchr(url, ':')) == NULL) {
    return -1;
  }
  n = p - url;
  strncpy(proto, url, n);
  proto[n] = '\0';

  /* skip "://" */
  if (*p++ != ':') return -1;
  if (*p++ != '/') return -1;
  if (*p++ != '/') return -1;

  /* host */
  {
    char *hp = host;
    
    while (*p && *p != ':' && *p != '/') {
      *hp++ = *p++;
    }
    *hp = '\0';
  }
  if (strlen(host) == 0)
    return -1;

  /* end */
  if (*p == '\0') {
    *port = 0;
    strcpy(uri, "");
    return 0;
  }

  /* port */
  if (*p == ':') {
    char buf[10];
    char *pp = buf;

    p++;
    while (isdigit(*p)) {
      *pp++ = *p++;
    }
    *pp = '\0';
    if (strlen(buf) == 0)
      return -1;
    *port = atoi(buf);
  }
  
  /* uri */
  if (*p == '/') {
    char *up = uri;
    while ((*up++ = *p++));
  }

  return 0;
}

/*----------------------------------------------------------------------*
 * returns
 *  0   if ok  (fd contains descriptor for connection)
 *  -1  if socket couldn't be created
 *  -2  if connection function could not be started.
 *
 * sock_fd will hold the socket.
 * The caller of www_Establishconnection must wait until write is possible
 * i.e. setListenerType(sockfd, ..., iListener_Write)
 * this to avoid blocking.
 */
 /* 建立http连接 */
int www_EstablishConnection(tIpAddr host_addr, int port, int *sock_fd)
{
  int fd;
  struct sockaddr_in sa;
  long flags;
  int res;

  memset(&sa, 0, sizeof(sa));
  sa.sin_family       = AF_INET;
  sa.sin_addr.s_addr  = htonl(host_addr);
  sa.sin_port         = htons(port);

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    return -1;
  }

  /* set non-blocking */
  flags = (long) fcntl(fd, F_GETFL);
  flags |= O_NONBLOCK;
  res = fcntl(fd, F_SETFL, flags);

  errno = 0;
  if (connect(fd, (struct sockaddr *) &sa, sizeof(sa)) < 0) {       /* 尝试连接 */
    if (errno != EINPROGRESS) {
      /* connect failed */
      close(fd);
      return -2;
    }
  }

  *sock_fd = fd;
  return 0;
}

/*----------------------------------------------------------------------
 * removes any trailing whitespaces, \r and \n
 * it destroys its argument...
 */
void www_StripTail(char *s)
{
  if (*s != '\0') {
    while(*s) s++;
    s--;
    while(*s == '\r' || *s == '\n' || *s == ' ' || *s == '\t') {
      *s = '\0';
      s--;
    }
  }
}


/*======================================================================*
 * module test code
 *======================================================================*/

#ifdef TEST
int main(int argc, char **argv)
{
  int res;
  char proto[256];
  char host[256];
  int port;
  char uri[256];

  strcpy(proto, "");

  res = www_ParseUrl(argv[1], proto, host, &port, uri);

  printf("result=%d\n", res);
  printf("proto= \"%s\"\n", proto);
  printf("host=  \"%s\"\n", host);
  printf("port=  %d\n", port);
  printf("uri=   \"%s\"\n", uri);
  return 0;
}
#endif
