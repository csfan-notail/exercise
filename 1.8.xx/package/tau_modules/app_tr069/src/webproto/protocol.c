/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : protocol.c
  版 本 号   : 初稿
  作    者   : zhanghaozhong
  生成日期   : 2013年5月16日
  最近修改   :
  功能描述   : protocol
  函数列表   :
              addCookieHdr
              getticks
              postShutdownCleanUp
              proto_FreeCtx
              proto_FreeHttpHdrs
              proto_Init
              proto_NewCtx
              proto_NewHttpHdrs
              proto_ParseHdrs
              proto_ParsePost
              proto_ParseRequest
              proto_ParseResponse
              proto_Printline
              proto_Readline
              proto_Readn
              proto_ReadWait
              proto_SendCookie
              proto_SendEndHeaders
              proto_SendError
              proto_SendHeader
              proto_SendHeaders
              proto_SendRaw
              proto_SendRedirect
              proto_SendRedirectProtoHost
              proto_SendRedirectViaRefresh
              proto_SendRequest
              proto_SetSslCtx
              proto_Skip
              proto_SSL_IO
              proto_Writen
              server_wait_for_ssl
              showSocketStatus
              SSL_io_handler
              verify_callback
              wait_for_sslShutdown
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
extern int g_log_level;
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
#include <unistd.h>
#include <ctype.h>
#include <malloc.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/poll.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "appdefs.h"
#include "utils.h"

//#ifdef USE_SSL 
    #include <openssl/ssl.h>
    #include <openssl/err.h>
//#endif

#include "event.h"

#include "protocol.h"
#include "www.h"

#ifdef USE_DMALLOC
    #include <dmalloc.h>
#endif

#define SERVER_NAME "milli_httpd"
#define PROTOCOL "HTTP/1.1"
#define RFC1123FMT "%a, %d %b %Y %H:%M:%S GMT"


#define DEBUG 1  /* general debug log */
#define WRITETRACE  /* prints everything written to socket */
#define READTRACE  /* prints everything read to socket */
/*altered by caibing*/
#define DEBUGSSL  /* log ssl io and connections */
/*Define DBGSSLC(X) to debug SSL connection and SSL_shutdown*/
//#define DBGSSLC(X) fprintf X
#define DBGSSLC(X)
//#define DEBUGHDRS /* log html headers */

#ifdef DEBUGSSL 
    #define mkstr(S) # S
    #define setListener(A,B,C) {fprintf(stderr,mkstr(%s setListener B fd=%d\n), getticks(), A);\
setListener( A,B,C);}

    #define setListenerType(A,B,C,E) {fprintf(stderr,mkstr(%s setListenerType B-E fd=%d\n), getticks(), A);\
setListenerType( A,B,C,E);}

    #define stopListener(A) {fprintf(stderr,"%s stopListener fd=%d\n", getticks(), A);\
stopListener( A );}
#endif
//#ifdef DEBUGSSL
char timestr[40];
char *getticks()
{
    struct timeval now;
    gettimeofday( &now,NULL);
    sprintf(timestr,"%04ld.%06ld", now.tv_sec%1000, now.tv_usec);
    return timestr;
}

//#endif

#define BUF_SIZE_MAX 4096

#ifdef USE_SSL
static SSL_CTX *ssl_ctx = NULL;
#endif

#ifdef DEBUGSSL
static void showSocketStatus( unsigned fd)
{

    struct pollfd fdl[1];
    int e;

    fdl[0].fd = fd;
    fdl[0].events = 0xff;
    if ( (e=poll(fdl,1,0))<0)
        fprintf(stderr,"*poll() error\n");
    else
        fprintf(stderr, "poll=%0x\n",fdl[0].revents);
}
#endif

#ifdef USE_SSL
/*
* verify server certificate	 see SSL_CTS_set_verify()
 * Returns: 0 - verification failed: stop verifying in failed state.
 *          1 - continue  verifying.
*/
static int verify_callback(int ok, X509_STORE_CTX *store)
{
	return ok;
}
#endif

/*======================================================================*
 * Init
 *======================================================================*/
void proto_Init()
{
#ifdef USE_SSL
#ifdef USE_DMALLOC
   CRYPTO_malloc_debug_init();
   CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ON);
   CRYPTO_set_mem_debug_options(V_CRYPTO_MDEBUG_ALL);
#endif

	SSL_library_init();
   	SSL_load_error_strings();

   ssl_ctx = SSL_CTX_new(TLSv1_2_client_method());
   if (ssl_ctx == NULL) {
      slog(LOG_ERROR,"Could not create SSL context");
      exit(1);
   }
   /* 注释 by duanguiyuan 20161111 */
   /*
   if (! SSL_CTX_set_cipher_list(ssl_ctx, ACS_CIPHERS) )
      slog(LOG_ERROR, "Could not set cipher list for SSL");
	*/
#ifdef USE_CERTIFICATES
   {
      struct stat fstat;
      if ( lstat(CERT_FILE, &fstat)==0) {
	      /* if CERT_FILE is present then setup cert verification */
	      if (! SSL_CTX_use_certificate_chain_file(ssl_ctx, CERT_FILE)) {
		      slog(LOG_ERROR,"Error loading certificate chain" );
	      }
	
	      if (! SSL_CTX_load_verify_locations(ssl_ctx, CERT_FILE, NULL))
		      slog(LOG_ERROR, "Could not load verify locations");
	      SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_PEER, verify_callback);
      }
   }
#endif

   /*SSL_CTX_set_mode(ssl_ctx, SSL_MODE_AUTO_RETRY); */
  /* 注释 by duanguiyuan 20161111 */
   /*
   ERR_print_errors_fp(stderr);
   SSL_CTX_set_session_cache_mode(ssl_ctx, SSL_SESS_CACHE_OFF);
   */
#endif
}  /* End of proto_Init() */

/*======================================================================*
 * Ctx
 *======================================================================*/
/*----------------------------------------------------------------------*/
tProtoCtx *proto_NewCtx(int fd)
{
   tProtoCtx *pc;

   pc = (tProtoCtx *) malloc(sizeof(tProtoCtx));
   memset(pc, 0, sizeof(tProtoCtx));
   /*altered by caibing */
   pc->type = iNormal;
   pc->fd = fd;
   return pc;

}  /* End of proto_NewCtx() */

#ifdef USE_SSL
/*----------------------------------------------------------------------*/
static void server_wait_for_ssl(void *handle)
{
   tProtoCtx *pc = handle;

#ifdef DEBUGSSL
   fprintf(stderr,"%s server_wait_for_ssl() SSL connect fd=%d ", getticks(), pc->fd);
   showSocketStatus(pc->fd);
#endif
   /* stopListener(pc->fd); */
   if ((pc->sslConn = SSL_connect(pc->ssl)) <= 0) {
      int sslres;
      sslres = SSL_get_error(pc->ssl, pc->sslConn);
      if (sslres == SSL_ERROR_WANT_READ) {
#ifdef DEBUGSSL
         fprintf(stderr,"%s SSL connection wants to read fd=%d\n", getticks(), pc->fd);
#endif
         setListener(pc->fd, server_wait_for_ssl, pc);
      } else if (sslres == SSL_ERROR_WANT_WRITE) {
#ifdef DEBUGSSL
         fprintf(stderr,"%s SSL connection wants to write fd=%d\n",getticks(), pc->fd);
#endif
         setListenerType(pc->fd, server_wait_for_ssl, pc, iListener_Write);      
      } else {
         char errstr[256];
         ERR_error_string_n(sslres,errstr,256);
         slog(TR69C_LOG_DEBUG,
              "Error connecting to server(Possible certificate error):(res=%d, sslres=%d) %s",
              pc->sslConn, sslres, errstr );
         (*(pc->cb))(pc->data, PROTO_ERROR_SSL);
         /* Note: pc may have been freed by the callback */
         return;
      }
   } else {
#ifdef DEBUGSSL
      fprintf(stderr,"%s SSL server_wait_for_ssl callback fd=%d\n",getticks(), pc->fd);
#endif
	printf ("SSL connection using %s\n", SSL_get_cipher (pc->ssl));

      (*(pc->cb))(pc->data, PROTO_OK);
   }
}  /* End of server_wait_for_ssl() */

/*----------------------------------------------------------------------*
 * callback errorcodes
 *  PROTO_OK          all ok
 *  PROTO_ERROR       generic error
 *  PROTO_ERROR_SSL   ssl error
 */
void proto_SetSslCtx(tProtoCtx *pc, tProtoHandler cb, void *data)
{
   pc->type = iSsl;

   if (pc->ssl == NULL) {
      pc->ssl = SSL_new(ssl_ctx);
   }

   if (pc->ssl != NULL) {
      DBGSSLC((stderr, "%s proto_SetSslCtx() ssl_ctx=%p ssl=%p fd=%d\n",
               getticks(), ssl_ctx, pc->ssl, pc->fd));
      if (SSL_set_fd(pc->ssl, pc->fd) > 0) {
         pc->cb = cb;
         pc->data = data;
         /* TBD: add a timeout */
         server_wait_for_ssl(pc);
      } else {
         slog(LOG_ERROR, "SSL_set_fd failed");
      }
   } else {
      slog(LOG_ERROR,"SSL_new failed");
   }
}  /* End of proto_SetSslCtx() */

static void postShutdownCleanUp( void *handle)
{ 
	tProtoCtx	*pc = (tProtoCtx *)handle;

	DBGSSLC( (stderr,"%s  postShutdownCleanUp() ssl=%p fd=%d\n", getticks(), pc->ssl, pc->fd) );
	stopTimer( postShutdownCleanUp, (void*)pc );
	stopListener(pc->fd);
	close(pc->fd);
	if (pc->ssl)
		SSL_free(pc->ssl);
   pc->sslConn = 0;
	free(pc);
	pc=NULL;
	return;
}  /* End of postShutdownCleanUp() */

static void wait_for_sslShutdown( void *handle)
{
	tProtoCtx	*pc = (tProtoCtx *)handle;
	int	r;

	DBGSSLC((stderr, "%s  wait_for_sslShutdown()fd=%d\n", getticks(), pc->fd));
	r= SSL_shutdown(pc->ssl);
	DBGSSLC((stderr, "%s  SSL_shutdown= %d ssl=%p fd=%d\n", getticks(), r, pc->ssl, pc->fd ));
	if (r == 0) {
		/* started shutdown -- now call again */
		r= SSL_shutdown(pc->ssl);
		DBGSSLC((stderr, "%s  2nd SSL_shutdown= %d ssl=%p fd=%d\n", getticks(), r, pc->ssl, pc->fd) );
	}
	if( r == 1 ) {
		postShutdownCleanUp(pc);
	} else if ( r == -1 ) {
		int sslres;
		sslres = SSL_get_error(pc->ssl, r );
		if (sslres == SSL_ERROR_WANT_READ) {
			DBGSSLC((stderr,"%s SSL_shutdown wants to read fd=%d\n", getticks(), pc->fd));
			setListener(pc->fd, wait_for_sslShutdown, pc);
		} else if (sslres == SSL_ERROR_WANT_WRITE) {
			DBGSSLC((stderr,"%s SSL_shutdown wants to write fd=%d\n",getticks(), pc->fd));
			setListenerType(pc->fd, wait_for_sslShutdown, pc, iListener_Write);      
		} else {
			char errstr[256];
			ERR_error_string_n(sslres,errstr,256);
			slog(LOG_ERROR, "SSL_shutdown server: (r=%d,sslres=%d) %s",
				r, sslres, errstr );
			postShutdownCleanUp( pc );
			return;
		}
	} else {
		DBGSSLC((stderr, "%s  SSL_shutdown state error ssl=%p fd=%d\n", getticks(), pc->ssl, pc->fd));
		postShutdownCleanUp(pc);
	}
	return;
}  /* End of wait_for_sslShutdown() */ 
#endif

/*----------------------------------------------------------------------*/
/* For iNormal protoCtx stopListener and close the fd					*/
/* For iSSL start shutdown												*/

void proto_FreeCtx(tProtoCtx *pc)
{
   switch (pc->type) {
      case iNormal:
         stopListener(pc->fd);
         close(pc->fd);
         free (pc);
		 pc=NULL;
         break;
#ifdef USE_SSL
      case iSsl:
         stopListener(pc->fd);
         DBGSSLC((stderr, "%s   proto_FreeCtx()ssl=%p fd=%d\n",getticks(),pc->ssl,pc->fd));
         setTimer(postShutdownCleanUp, (void *)pc, 3000);
         if (pc->ssl) {
            /* not completed */
            wait_for_sslShutdown(pc);
         } else {
            postShutdownCleanUp(pc);
         }
         break;
#endif
      default:
         slog(LOG_ERROR,"Impossible error; proto_FreeCtx() illegal ProtoCtx type (%d)", pc->type);
         free(pc);
		 pc=NULL;
         break;
   }
}  /* End of proto_FreeCtx() */

/*======================================================================*
 * Util
 *======================================================================*/
#ifdef USE_SSL
typedef struct {
    tProtoCtx *pc;
    tSSLIO    iofunc;
    char *ptr;
    int nbytes;
    tProtoHandler cb;
    void *userdata;
} SSL_io_ctx;
/*
* The following SSL io routines ensure that the parameters are saved
 * and restored in the subsequent call to SSL_read/write whenever the 
 * functions return a -1 indicating non-blocking inprogress io.
 */
static void SSL_io_handler(void *handle) {
    SSL_io_ctx *rc = handle;
    int nresult;
    int sslres;
    if (rc->iofunc == sslRead ) {
        nresult = SSL_read(rc->pc->ssl, (void *)rc->ptr, rc->nbytes);
#ifdef DEBUGSSL
        fprintf(stderr, "%s SSL_io_handler read ssl=%x socket=%d nresult=%d\n", getticks(), rc->pc->ssl, rc->pc->fd, nresult);
#endif
    } else {
        nresult = SSL_write(rc->pc->ssl, (void *)rc->ptr, rc->nbytes);
#ifdef DEBUGSSL
        fprintf(stderr, "%s SSL_io_handler write ssl=%x socket=%d nresult=%d\n", getticks(),rc->pc->ssl, rc->pc->fd, nresult);
#endif
    }
    if (nresult < 0) {
        sslres = SSL_get_error(rc->pc->ssl, nresult);
        if (sslres == SSL_ERROR_WANT_READ) {
#ifdef DEBUGSSL
            fprintf(stderr, "%s SSL connection listen to read fd=%d\n", getticks(), rc->pc->fd);
#endif
            setListener(rc->pc->fd, SSL_io_handler, rc);
            return;
        } else if (sslres == SSL_ERROR_WANT_WRITE) {
#ifdef DEBUGSSL
            fprintf(stderr, "%s SSL connection listen to write fd=%d\n",getticks(), rc->pc->fd);
#endif
            setListenerType(rc->pc->fd, SSL_io_handler, rc, iListener_Write);
            return;
        }
        slog(TR69C_LOG_DEBUG,"SSL_io_handler %s error fd=%d errcode=%d",
             rc->iofunc==sslRead? "read": "write", rc->pc->fd, sslres);
        return;
    }
    /* If we get here, we're done */

    stopListener(rc->pc->fd);
    (*(rc->cb))((void*)rc->userdata, nresult);
    free(rc);
	rc->pc=NULL;  
}

/*----------------------------------------------------------------------*/
int proto_SSL_IO(tSSLIO func, tProtoCtx *pc, char *ptr, int nbytes, tProtoHandler cb, void *data) {

    SSL_io_ctx *rc;
    int nresult;

    if (func == sslRead) {
        nresult = SSL_read(pc->ssl, ptr, nbytes);
#ifdef DEBUGSSL
        fprintf(stderr, "%s proto_SSL_io read fd=%d nresult=%d\n", getticks(), pc->fd,nresult);
#endif
    } else if (func == sslWrite) {
        nresult = SSL_write(pc->ssl, ptr, nbytes);
#ifdef DEBUGSSL
        fprintf(stderr, "%s proto_SSL_io write fd=%d nresult=%d\n",getticks(), pc->fd,nresult);
#endif
    }
    if (nresult < 0) {
        int sslres = SSL_get_error(pc->ssl, nresult);
        rc = (SSL_io_ctx *)malloc(sizeof(SSL_io_ctx));
        if (!rc)
            return -2;
        memset(rc, 0, sizeof(SSL_io_ctx));
        rc->iofunc = func;
        rc->pc = pc;
        rc->ptr = ptr;
        rc->nbytes = nbytes;
        rc->cb = cb;
        rc->userdata = data;
        if (sslres == SSL_ERROR_WANT_READ) {
#ifdef DEBUGSSL
            fprintf(stderr, "%s SSL_IO connection listen to read fd=%d\n", getticks(),rc->pc->fd);
#endif
            setListenerType(rc->pc->fd, SSL_io_handler, rc,iListener_ReadWrite);
        } else if (sslres == SSL_ERROR_WANT_WRITE) {
#ifdef DEBUGSSL
            fprintf(stderr, "%s SSL_IO connection listen to write fd=%d\n", getticks(),rc->pc->fd);
#endif
            setListenerType(rc->pc->fd, SSL_io_handler, rc, iListener_Write);
        } else {
#ifdef DEBUGSSL
            fprintf(stderr, "%s SSL_IO fd=%d error=%d\n", getticks(),rc->pc->fd, sslres);
#endif
            free (rc);
			rc=NULL;

        }
    }
    return nresult;
}
#endif
/*----------------------------------------------------------------------*/
/* blocking read */
int proto_ReadWait(tProtoCtx *pc, char *ptr, int nbytes)
{
    int nread=0;
    int flags, bflags;

    /* turn on synchroneous I/O, this call will block. */
    {
        flags = (long) fcntl(pc->fd, F_GETFL);
        bflags = flags & ~O_NONBLOCK; /* clear non-block flag, i.e. block */
        fcntl(pc->fd, F_SETFL, bflags);
    }

    errno = 0;
	printf("[%s:%d] pc->type= %d\n",__FUNCTION__,__LINE__,pc->type);	
    switch (pc->type) {
    case iNormal:
        nread = read(pc->fd, ptr, nbytes);
        break;
#ifdef USE_SSL
    case iSsl:
#ifdef DEBUGSSL
      //  fprintf(stderr, "%s read_SSL(%d, lth=%d)", getticks(),pc->fd, nbytes);
        nread = SSL_read(pc->ssl, (void *) ptr, nbytes);
      //  fprintf(stderr, " result=%d\n", nread);
#else
        nread = SSL_read(pc->ssl, (void *) ptr, nbytes);
#endif
        break;
#endif
    default:
        slog(LOG_ERROR, "Impossible error; readn() illegal ProtoCtx type (%d)", pc->type);
        break;
    }
    if (nread > nbytes) {
        slog(LOG_ERROR, "proto_READ of %d returned %d", nbytes, nread);
    }

    fcntl(pc->fd, F_SETFL, flags); /* remove blocking flags */
    //#ifdef READTRACE
/*
	if (g_debug)
    {   int i;
        for (i=0; i<nread; ++i)
            fprintf(stderr, "%c", *(ptr+i));
        fprintf(stderr, "\n\n");
    }
  */  //#endif
    return nread;
}
/* */
/*----------------------------------------------------------------------*/
int proto_Readn(tProtoCtx *pc, char *ptr, int nbytes)
{
    int nleft, nread=0;
    int   errnoval;

    nleft = nbytes;
    while (nleft > 0) {
        errno =0;
        switch (pc->type) {
        case iNormal:
            nread = read(pc->fd, ptr, nleft);
            break;
#ifdef USE_SSL
        case iSsl:
#ifdef DEBUGSSL
		//altered by caibing
         //   fprintf(stderr, "------%s\n)",ptr);
            nread = SSL_read(pc->ssl, (void *) ptr, nleft);
       //     fprintf(stderr, " result=%d\n", nread);
#else
            nread = SSL_read(pc->ssl, (void *) ptr, nleft);
#endif
            break;
#endif
        default:
            slog(LOG_ERROR, "Impossible error; readn() illegal ProtoCtx type (%d)", pc->type);
            break;
        }

        if (nread < 0) {                            /* This function will read until the byte cnt*/
            errnoval=errno;                         /* is reached or the return is <0. In the case*/
            if (errnoval==EAGAIN )                  /* of non-blocking reads this may happen after*/
                return nbytes-nleft;                /* some bytes have been retrieved. The EAGAIN*/
            else                                    /* status indicates that more are coming */
                                                    /* Other possibilites are ECONNRESET indicating*/
                /* that the tcp connection is broken */
                fprintf(stderr,"!!!!!!!! read(fd=%d) error=%d\n",
                        pc->fd, errnoval);
            return nread; /* error, return < 0 */

        } else if (nread == 0) {
            break; /* EOF */
        }

        nleft -= nread;
        ptr += nread;
    }

    return nbytes - nleft; /* return >= 0 */
}
/*
 * Return number of bytes written or -1.
 * If -1 check for errno for EAGAIN and recall.
 *----------------------------------------------------------------------*/
int proto_Writen(tProtoCtx *pc, const char *ptr, int nbytes)
{
   int  nwritten=0;

   errno = 0;
   switch (pc->type) {
      case iNormal:
	  	
#ifdef DUMPSOAPOUT
   fprintf(stderr, "%s", ptr); 
#endif
         nwritten = write(pc->fd, ptr, nbytes);
         break;
/*altered by caibing xxx*/
#ifdef USE_SSL
      case iSsl:
#ifdef DEBUGSSL
		//altered by caibing 
      //   fprintf(stderr, "--------------%s SSL_write(%d, lth=%d),content:%s\n", getticks(),pc->fd, nbytes,ptr);
         nwritten = SSL_write(pc->ssl, ptr, nbytes);
    //     fprintf(stderr, "result=%d\n", nwritten);
#else
         nwritten = SSL_write(pc->ssl, ptr, nbytes);
#endif
         break;
#endif
      default:
         slog(LOG_ERROR, "Impossible error; writen() illegal ProtoCtx type (%d)", pc->type);
         break;
   }

//#ifdef WRITETRACE
/* 此处将要发送给ACS的内容打印出来 */
#if 0
    if (g_debug)
    {
    	//fprintf(stderr,"proto_Writen:");
       if ( nwritten>0 ) {
          int  i;
          for (i=0;i<nwritten;++i)
             fprintf(stderr,"%c", *(ptr+i));
       }
    }
#endif
//#endif

#if 0
   if ( nwritten>0 ) {
      int  i;
      for (i=0;i<nwritten;++i)
         fprintf(stderr,"%c", *(ptr+i));
   }
#endif

   if (nwritten <= 0) {
      if (errno!=EAGAIN)
/*            fprintf(stderr,"proto_Writen() status = %d Error%s(%d)\n",nwritten,strerror(errno),errno);  */
         return nwritten;
		/*
		else
            fprintf(stderr,"proto_Writen() status = %d Error%s(%d)\n",nwritten,strerror(errno),errno);
	  	*/
   }
	/*
    if (nwritten != nbytes) {
        fprintf(stderr,"proto_Writen() short write rlth=%d actual=%d\n", nbytes, nwritten);
    }
	*/
   return nwritten;

}  /* End of proto_Writen() */

/*----------------------------------------------------------------------*
 * Read a line from a descriptor. Read the line one byte at a time,
 * looking for the newline. We store the newline in the buffer,
 * then follow it with a \0 (the same as fgets).
 * We return the number of characters up to, but not including,
 * the \0 (the same as strlen).
 */

int proto_Readline(tProtoCtx *pc, char *buf, int maxlen)
{
    int n, rc;
    char   *ptr = buf;
    char c;
    int flags, bflags;

    /* turn on synchroneous I/O, this call will block. */
    {
        flags = (long) fcntl(pc->fd, F_GETFL);
        bflags = flags & ~O_NONBLOCK; /* clear non-block flag, i.e. block */
        fcntl(pc->fd, F_SETFL, bflags);
    }

    for (n = 1; n < maxlen; n++) {
        rc = proto_Readn(pc, &c, 1);
        if (rc == 1) {
            *ptr++ = c;
            if (c == '\n')
                break;
        } else if (rc == 0) {
            if (n == 1) {
                fcntl(pc->fd, F_SETFL, flags); /* TBD: fix part2, remove blocking flags */
                return 0; /* EOF, no data read */
            } else
                break;    /* EOF, some data was read */
        } else {
if (g_debug)
            fprintf(stderr, "ERROR: proto_Readline fd=%d (%d)\n", pc->fd, errno);
            fcntl(pc->fd, F_SETFL, flags); /* remove blocking flags */
            return -1; /* ERROR */
        }
    }

    *ptr = '\0';
    //#ifdef READTRACE
    /*altered by caibing*/
    //if (g_debug)
    fprintf(stderr, "---------------%s\n", buf);
    //#endif
    fcntl(pc->fd, F_SETFL, flags); /* remove blocking flags */
    return n;
}


/*----------------------------------------------------------------------*/
void proto_Printline(tProtoCtx *pc, const char *fmt, ...)
{
    char *p;
    va_list ap;
    int n;
    int size;

    size = 1024;
    if ((p = malloc(size)) == NULL) {
        slog(LOG_ERROR, "failed to malloc(%d)", size);
        return;
    }

    while (1) {
        /* try to print in the allocated space */
        va_start(ap, fmt);
        n = vsnprintf(p, size, fmt, ap);
        va_end(ap);

        if (n < 0) {
            slog(LOG_WARNING, "fdprintf() vsnprintf failed *%d): %s (%d) fmt=\"%s\"\n", n, strerror(errno), errno, fmt);
#if DEBUG
            fprintf(stderr, "vsnprintf failed (%d): %s (%d) fmt=\"%s\"\n", n, strerror(errno), errno, fmt);
#endif
            return;
        } else if (n >= 0 && n < size) {
            /* print succeeded, let's write it on outstream */
            proto_Writen(pc, p, n);
            free(p);
			p=NULL;
            return;
        } else {
#if DEBUG
            fprintf(stderr, "vsnprintf, only wrote %d bytes, retrying: fmt=\"%s\" strlen(fmt)=%d size=%d\n",
                    n, fmt, strlen(fmt), size);
#endif
            size *= 2;
            if ((p = realloc(p, size)) == NULL) {
                slog(LOG_ERROR, "failed to realloc(%d)", size);
                return;
            }
        }
    }/*end while(1)*/
}

/*======================================================================*
 * Data
 *======================================================================*/
tHttpHdrs *proto_NewHttpHdrs()
{
    tHttpHdrs *p;

    p = (tHttpHdrs *) malloc(sizeof(tHttpHdrs));
    if (p == NULL) {
        return NULL;
    }
    memset(p, 0, sizeof(tHttpHdrs));
    return p;
}

/*----------------------------------------------------------------------*/
void proto_FreeHttpHdrs(tHttpHdrs *p)
{
    CookieHdr	*cp, *last;
    free(p->content_type);
	p->content_type=NULL;
    free(p->protocol);
	p->protocol=NULL;
    free(p->wwwAuthenticate);
	p->wwwAuthenticate=NULL;
    free(p->Authorization);
	p->Authorization=NULL;
    free(p->TransferEncoding);
	p->TransferEncoding=NULL;
    free(p->Connection);
	p->Connection=NULL;
    free(p->method);
	p->method=NULL;
    free(p->path);
	p->path=NULL;
    free(p->host);
	p->host=NULL;
    cp = p->setCookies;
    while (cp) {
        last = cp->next;
        free(cp->name);
		cp->name=NULL;
        free(cp->value);
		cp->value=NULL;
        free(cp);
		cp=NULL;
        cp = last;
    }
    free(p->message);
	p->message=NULL;
    free(p->locationHdr);
	p->locationHdr=NULL;
    free(p->filename);
	p->filename=NULL;
    free(p->arg);
	p->arg=NULL;
    free(p);
	p=NULL;
}

/*======================================================================*
 * Sending
 *======================================================================*/
/*----------------------------------------------------------------------*/
void proto_SendRequest(tProtoCtx *pc, const char *method, const char *url)
{
    char buf[BUF_SIZE_MAX];
    int len;

  //  len = snprintf(buf,BUF_SIZE_MAX, "%s %smm.txt HTTP/1.1\r\n", method, url);
   len = snprintf(buf,BUF_SIZE_MAX, "%s %s HTTP/1.1\r\n", method, url);
    if (len != proto_Writen(pc, buf, len)) {
        /* error in sending */
        ;
    }
//if (g_debug)
  //  fprintf(stderr, "*****proto_SendRequest(%s)\n", buf);
}

/*----------------------------------------------------------------------*/
void proto_SendCookie(tProtoCtx *pc, CookieHdr *c)
{
    char buf[BUF_SIZE_MAX];
    int len;

    len = snprintf(buf,BUF_SIZE_MAX, "Cookie: %s=%s\r\n", c->name, c->value);
    if (len != proto_Writen(pc, buf, len)) {
        /* error in sending */
        ;
    }
/*
#ifdef DEBUGHDRS
    fprintf(stderr, "proto_SendCookie -> %s", buf);
#endif
*/
}
/*----------------------------------------------------------------------*/
void proto_SendHeader(tProtoCtx *pc,  const char *header, const char *value)
{
    char buf[BUF_SIZE_MAX];
    int len;

    if ( header == NULL || value == NULL ) return;
    
    len = snprintf(buf,BUF_SIZE_MAX, "%s: %s\r\n", header, value);
    if (len != proto_Writen(pc, buf, len)) {
        /* error in sending */
        ;
    }
/*
#ifdef DEBUGHDRS
    fprintf(stderr, "proto_SendHeader -> %s", buf);
#endif
*/
}

/*----------------------------------------------------------------------*/
void proto_SendRaw(tProtoCtx *pc, const char *arg, int len)
{
   int	wlth;
   int totWlth = 0;
#ifdef DEBUGHDRS
   if (*arg!='<' && *arg!= '\r') {
      /* debuggging*/
      fprintf(stderr, "!!%10.10s!!\n", arg);
   }
#endif
   
   while ( totWlth<len ) {
      if ( (wlth = proto_Writen(pc, arg+totWlth, len-totWlth)) >= 0) {
         /* some or all data sent*/
		// fprintf(stderr, "proto_Writen wlth = %d\n", wlth); 
         totWlth += wlth;
         continue;
      } else {
         if (wlth < 0 && errno == EAGAIN ) {
            /* can't send is all keep trying -- busy wait on writes */
            continue; 
         }
         /* data send error */
         break;
      }
   }
}  /* End of proto_SendRaw() */

/*----------------------------------------------------------------------*/
void proto_SendEndHeaders(tProtoCtx *pc)
{
#ifdef DEBUGHDRS
    fprintf(stderr, "proto_SendEndHeaders()\n");
#endif
    proto_SendRaw(pc, "\r\n", 2);
}

/*----------------------------------------------------------------------*/
void proto_SendHeaders(tProtoCtx *pc, int status, const char* title, const char* extra_header, const char* content_type)
{
    time_t now;
    char timebuf[100];

    proto_Printline(pc, "%s %d %s\r\n", PROTOCOL, status, title);
    now = time((time_t*) 0);
    (void) strftime(timebuf, sizeof(timebuf), RFC1123FMT, gmtime(&now));
    proto_Printline(pc, "Date: %s\r\n", timebuf);
    proto_Printline(pc, "MIME-Version: 1.0\r\n");
    proto_Printline(pc, "Server: %s\r\n", SERVER_NAME);
    proto_Printline(pc, "Connection: Close\r\n" );
    if (extra_header != NULL)
        proto_Printline(pc, "%s\r\n", extra_header );
    if (content_type != NULL)
        proto_Printline(pc, "Content-Type: %s\r\n", content_type);
    proto_Printline(pc, "\r\n" );
}

/*----------------------------------------------------------------------*/
void proto_SendRedirect(tProtoCtx *pc, const char *host, const char* location)
{
    char header[BUF_SIZE_MAX];
    char slash[2];

    if (location[0] == '/')
        strcpy(slash, "");
    else
        strcpy(slash, "/");
if (g_debug)
    fprintf(stderr, "web: proto_SEndRedirect: host=%s location=%s\n",host, location);
    (void) snprintf(header, sizeof(header), "Location: http://%s%s%s", host, slash, location);
    proto_SendHeaders(pc, 307, "Redirect", header, "text/html");
if (g_debug)
    fprintf(stderr, "web: proto_SEndRedirect: %s\n", header);
}


/*----------------------------------------------------------------------*/
void proto_SendRedirectProtoHost(tProtoCtx *pc, const char *protohost, const char* location)
{
    char header[BUF_SIZE_MAX];

    (void) snprintf(header, sizeof(header), "Location: %s%s", protohost, location);
    proto_SendHeaders(pc, 307, "Redirect", header, "text/html");
if (g_debug)
    fprintf(stderr, "web: proto_SendRedirectProtoHost: %s\n", header);
}

/*----------------------------------------------------------------------*/
void proto_SendRedirectViaRefresh(tProtoCtx *pc, const char *host, const char* location)
{
    char slash[2];

    if (location[0] == '/')
        strcpy(slash, "");
    else
        strcpy(slash, "/");

    proto_SendHeaders(pc, 200, "Ok", NULL, "text/html");
    proto_Printline(pc, "<HTML><HEAD><TITLE>Redirecting to requested site...</TITLE>\n");
    proto_Printline(pc, "<meta http-equiv=\"refresh\" content=\"0;URL=http://%s%s%s\"></HEAD>\n",
                    host, slash, location);
}

/*----------------------------------------------------------------------*/
void proto_SendError(tProtoCtx *pc, int status, const char* title, const char* extra_header, const char* text)
{
    proto_SendHeaders(pc, status, title, extra_header, "text/html");
    proto_Printline(pc, "<HTML><HEAD><TITLE>%d %s</TITLE></HEAD>\n", status, title);
    proto_Printline(pc, "<BODY BGCOLOR=\"#cc9999\"><H4>%d %s</H4>\n", status, title);
    proto_Printline(pc, "%s\n", text );
    proto_Printline(pc, "</BODY></HTML>\n");
}

/*======================================================================*
 * Receiving
 *======================================================================*/
/*----------------------------------------------------------------------*
 * return
 *   0 if ok
 *  -1 on failure
 */
int proto_ParseRequest(tProtoCtx *pc, tHttpHdrs *hdrs)
{
    char buf[BUF_SIZE_MAX];
    char method[BUF_SIZE_MAX];
    char path[BUF_SIZE_MAX];
    char protocol[BUF_SIZE_MAX];

    /* Parse the first line of the request. */
    if (proto_Readline(pc, buf, BUF_SIZE_MAX) <= 0) {
if (g_debug)
        fprintf(stderr, "DEBUG: error =%d proto_ParseRequest() proto_Readline() rtns empty\n",
                errno);
return -1;
    }
    if (sscanf(buf, "%[^ ] %[^ ] %[^ ]", method, path, protocol) != 3) {
if (g_debug)
        fprintf(stderr,"sscanf error on >>%s<<\n",buf);
        return -1;
    }

    www_StripTail(method);
    www_StripTail(path);
    www_StripTail(protocol);
    free(hdrs->method);
	hdrs->method=NULL;
    hdrs->method = strdup(method);
    free(hdrs->path);
	hdrs->path=NULL;
    hdrs->path = strdup(path);
    free(hdrs->protocol);
	hdrs->protocol=NULL;
    hdrs->protocol = strdup(protocol);
#ifdef DEBUGHDRS
    fprintf(stderr, "proto_ParseRequest method=\"%s\" path=\"%s\" protocol=\"%s\"\n",
         hdrs->method, hdrs->path, hdrs->protocol);
#endif    
    return 0; /* OK */
}

/*----------------------------------------------------------------------*
 * return
 *   0 if ok
 *  -1 on failure
 */
int proto_ParseResponse(tProtoCtx *pc, tHttpHdrs *hdrs)
{
    char buf[BUF_SIZE_MAX];
    char protocol[BUF_SIZE_MAX];
    char status[BUF_SIZE_MAX];
    char message[BUF_SIZE_MAX];

#ifdef DEBUGSSL
    fprintf(stderr, "%s proto_ParseResponse()\n", getticks());
#endif
    /* Parse the first line of the request. */
    if (proto_Readline(pc, buf, BUF_SIZE_MAX) <= 0) {
        return -1;
    }

    if (sscanf(buf, "%[^ ] %[^ ] %[^\r]", protocol, status, message ) != 3) {
if (g_debug)
        fprintf(stderr,"sscanf error on >>%s<<\n",buf);

        return -1;
    }

    www_StripTail(protocol);
    www_StripTail(status);
    www_StripTail(message);
    free(hdrs->protocol);
	hdrs->protocol=NULL;
    hdrs->protocol = strdup(protocol);
    hdrs->status_code = atoi(status); /* TBD: add sanity check */
    free(hdrs->message);
	hdrs->protocol=NULL;
    hdrs->message = strdup(message);
if (g_debug)
    fprintf(stderr, "proto_ParseResponse(protocol=\"%s\", status=%d message=\"%s\")\n",
            hdrs->protocol, hdrs->status_code, hdrs->message);

    return 0; /* OK */
}

static char HostStr[]="Host:";
static char ConnectionStr[]="Connection:";
static char SetCookieStr[]="Set-Cookie:";
static char SetCookieStr2[]="Set-Cookie2:";
static char ContentLthStr[]="Content-Length:";
static char ContentTypeStr[]="Content-Type:";
static char WWWAuthenticateStr[]="WWW-Authenticate:";
static char AuthorizationStr[]="Authorization:";
static char TransferEncoding[]="Transfer-Encoding:";
static char LocationStr[]="Location:";

extern char *strndup(const char *s, size_t n);

static void addCookieHdr( CookieHdr **p, char *c)
{
	CookieHdr	*newCookie = (CookieHdr*) malloc(sizeof (CookieHdr));
	char	*cp;
		 
	if ( (cp = strchr(c,'='))){
		newCookie->next = *p;
		newCookie->name =  (char *)strndup(c,cp-c);
		newCookie->value = strdup(cp+1);
		*p = newCookie;
	} else
		{
			free(newCookie);
			newCookie=NULL;
		}
		
}
/*----------------------------------------------------------------------*
 * hdrs->type needs to be initiated
 * Only read headers according to type
 * Reads all headers until an empty '\r\n" is found.
 */
void proto_ParseHdrs(tProtoCtx *pc, tHttpHdrs *hdrs)
{
    char buf[BUF_SIZE_MAX];
    char *cp;
    int n;

#ifdef DEBUGHDRS
    fprintf(stderr, "DEBUG: proto_ParseHdrs() pc=%p pc->type=%d\n", pc, pc->type);
#endif

    /* Parse the rest of the request headers. */
    while ((n = proto_Readline(pc, buf, BUF_SIZE_MAX)) > 0) {
        www_StripTail(buf);
#ifdef DEBUGHDRS
        fprintf(stderr, "  DEBUG: read \"%s\"\n", buf);
#endif
        if (strcmp(buf, "") == 0) {
            break;
        } else if (strncasecmp(buf, HostStr,sizeof(HostStr)-1) == 0) {
            cp = &buf[sizeof(HostStr)-1];
            cp += strspn(cp, " \t");
            free(hdrs->host);
			hdrs->host=NULL;
            hdrs->host = strdup(cp);
        } else if (strncasecmp(buf, ContentLthStr,sizeof(ContentLthStr)-1) == 0) {
            cp = &buf[sizeof(ContentLthStr)-1];
            cp += strspn(cp, " \t");
            hdrs->content_length = atoi(cp);
        } else if (strncasecmp(buf, ContentTypeStr,sizeof(ContentTypeStr)-1) == 0) {
            cp = &buf[sizeof(ContentTypeStr)-1];
            cp += strspn(cp, " \t");
            free(hdrs->content_type);
			hdrs->content_type=NULL;
            hdrs->content_type = strdup(cp);
        } else if (strncasecmp(buf, ConnectionStr,sizeof(ConnectionStr)-1) == 0) {
            cp = &buf[sizeof(ConnectionStr)-1];
            cp += strspn(cp, " \t");
            free(hdrs->Connection);
			hdrs->Connection=NULL;
            hdrs->Connection = strdup(cp);
        } else if (strncasecmp(buf, WWWAuthenticateStr, sizeof(WWWAuthenticateStr)-1)==0) {
            cp =&buf[sizeof(WWWAuthenticateStr)-1];
            cp += strspn(cp, " \t");
            free(hdrs->wwwAuthenticate);
			hdrs->wwwAuthenticate=NULL;
            hdrs->wwwAuthenticate = strdup(cp);
        } else if (strncasecmp(buf, AuthorizationStr, sizeof(AuthorizationStr)-1)==0) {
            cp =&buf[sizeof(AuthorizationStr)-1];
            cp += strspn(cp, " \t");
            free(hdrs->Authorization);
			hdrs->Authorization=NULL;
            hdrs->Authorization = strdup(cp);
        } else if (strncasecmp(buf, TransferEncoding, sizeof(TransferEncoding)-1)==0) {
            cp =&buf[sizeof(TransferEncoding)-1];
            cp += strspn(cp, " \t");
            free(hdrs->TransferEncoding);
			hdrs->Authorization=NULL;
            hdrs->TransferEncoding = strdup(cp);
        } else if (strncasecmp(buf, LocationStr, sizeof(LocationStr)-1)==0) {
            cp =&buf[sizeof(LocationStr)-1];
            cp += strspn(cp, " \t");
            free(hdrs->locationHdr);
			hdrs->locationHdr=NULL;
            hdrs->locationHdr = strdup(cp);
        }  else if ( (strncasecmp(buf, SetCookieStr, sizeof(SetCookieStr)-1)==0)
			      || (strncasecmp(buf, SetCookieStr2, sizeof(SetCookieStr2)-1)==0) ) {
            char *c;
            cp =&buf[sizeof(SetCookieStr)-1];
            cp += strspn(cp, " \t:");   /* colon is added to skip : in SetCookieStr2 str*/
            /* don't need anything after ";" if it exists */
            if ( (c = strstr(cp,";")))
                *c = '\0';
            addCookieHdr( &hdrs->setCookies, cp );
        }


    }

#ifdef DEBUGHDRS
    fprintf(stderr, "DEBUG: proto_ParseHdrs done.\n");
#endif
}

/*----------------------------------------------------------------------*/
void proto_ParsePost(tProtoCtx *pc, tHttpHdrs *hdrs)
{
    char *data;
    size_t n;
    int len;

if (g_debug)
    fprintf(stderr, "DEBUG: proto_ParsePost() to read %d bytes\n", hdrs->content_length);
    len = hdrs->content_length;
    data = (char *) malloc(len + 1); /* make room for terminating '\0' */
    n = proto_Readn(pc, data, len);
    if (n>0)
        data[n] = '\0';
    else
        data[0] = '\n';
if (g_debug)
    fprintf(stderr, "DEBUG: proto_ParsePost() read %d bytes \"%s\"\n", n, data);

    free(hdrs->arg);
	hdrs->arg=NULL;
    hdrs->arg = data;

    proto_Skip(pc);
}

/*----------------------------------------------------------------------*
 * discard all there is to read on the in buffer
 * This is used since some stupid browsers (e.g. IE) sends more data
 * than specified in the content-lenght header
 * Returns result of last read():
 *  	0 - eof
 *     -1 - connection error.
 *      1 - no data, possibly more.
 */
int proto_Skip(tProtoCtx *pc)
{
    char c;
    int nread = 0;
    int ret = 0;
    long flags, nbflags;

if (g_debug)
    fprintf(stderr, "DEBUG: proto_Skip() read all from fd and ignore\n");


    flags = (long) fcntl(pc->fd, F_GETFL);
    nbflags = flags | O_NONBLOCK;
    fcntl(pc->fd, F_SETFL, nbflags);

    do {
		switch (pc->type) {
		case iNormal:
			nread = read(pc->fd, &c, 1);
			break;
	#ifdef USE_SSL
		case iSsl:
			nread = SSL_read(pc->ssl, &c, 1);
			break;
	#endif
		default:
			slog(LOG_ERROR, "Impossible error; illegal ProtoCtx type (%d)", pc->type);
			break;
		}
		if (nread<0) {
			ret = errno == EAGAIN? 1: -1;
			break;
			}
	} while (nread>0);
    fcntl(pc->fd, F_SETFL, flags);

if (g_debug)
    fprintf(stderr, "DEBUG: proto_Skip() done.ret=%d\n", ret);

	return ret;
}
