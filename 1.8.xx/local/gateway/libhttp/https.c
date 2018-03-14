/*
 * OpenSSL SSL/TLS Https Client example
 * Only for Unix/Linux:
 *    cc -c https.c
 *    cc -o https https.c -lssl
 * OpenSSL library needed.
 *
 * ͬʱ֧����ͨ��socket�����Լ�������ͨsocket����֮�ϵ�ssl
 * ���ӡ���������е�socket�����޸���˵��ȽϷ��㣬������
 * ��ԭ���Ľṹ����̫��ĳ�ͻ.
 * Ҫע���һ�㣬�ƺ���ʹ��socket�׽���������ssl���ӵ�ʱ��,
 * ����׽����ǲ��÷�������ʽ�����Ļ����ᵼ��ssl�Ựʧ�ܣ���
 * ֪��Ϊʲô��������������ṩ��https���׽��ֲ�������ͨ��
 * connect����������
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#ifdef _OPENSSL_
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#else
#endif

#include "https.h"

#define BUF_LEN 1024
#define MAX_STRING_LEN  2048

//xnet_select x defines
#define READ_STATUS     0
#define WRITE_STATUS    1
#define EXCPT_STATUS    2

#define DEBUG 1  

/* flag to set request with ssl or not. */
static int bIsHttps = 1;

static int timeout_sec = 10;
static int timeout_microsec = 0;

void err_doit(int errnoflag, const char *fmt, va_list ap);
void err_quit(const char *fmt, ...);
int create_tcpsocket(const char *host, const unsigned short port);
int xnet_select(int s, int sec, int usec, short x);

/*******************************************************************************
* ��������:  get_ip
* ��    ��:  ������������ȡIP��ַ   
* ��    ��:
* ��������          ����/���         ����
*
* ��������:
*
* ˵    ��:
*******************************************************************************/
char * get_ip(char *hostname)
{
#ifdef WIN32  
    int ret;    
    struct hostent *host;    
    struct in_addr addr;    
    //����socket����    
    WSADATA wsaData;    
    ret = WSAStartup( 0x101 , &wsaData);    
    //��������ɹ� ������ֵΪ0    
    if( ret != 0 )    
    {        
    	printf("WSAStartup error:%d "  , WSAGetLastError() );        
    	return 1;    
    }    
    
    //��ȡ������Ϣ    
    host = gethostbyname( hostname );    
    if( host == NULL )    
    {        
    	printf("gethostbyname error:%d" , WSAGetLastError() );        
    	return 1;    
    }    
    
    addr.s_addr = *(unsigned long * )host->h_addr;    
    //printf("������  :%sn" , host->h_name);    
    //printf("������ַ: %sn " , inet_ntoa( addr ) );    
    WSACleanup();    

	return inet_ntoa( addr );

#else   
  
	struct addrinfo * res, *pt;
	struct sockaddr_in *sinp;
	const char *addr;   
	char abuf[INET_ADDRSTRLEN];  
	int succ=0,i=0;   
	
    static char  address[64];

	memset(address, 0, sizeof(address));

	succ = getaddrinfo(hostname, NULL, NULL, &res);   
	if(succ != 0)   
	{   
		printf("Can't get address info! error code = %d", succ);   
		return "";   
	}   

	for(pt=res, i=0; pt != NULL; pt=pt->ai_next, i++){   
		sinp = (struct sockaddr_in *)pt->ai_addr;   
		addr = (const char *)inet_ntop(AF_INET, &sinp->sin_addr, abuf, INET_ADDRSTRLEN);   
		printf("%2d. IP=%s\n", i, addr);
		sprintf(address, "%s", addr);
		break;
	}   

	return address;
#endif

}

/* create common tcp socket connection */
int create_tcpsocket(const char *host, const unsigned short port){
    int ret;

    char * transport = "tcp";
    struct hostent *phe; /* pointer to host information entry */
    struct protoent *ppe; /* pointer to protocol information entry */
    struct sockaddr_in sin; /* an Internet endpoint address */
    int s; /* socket descriptor and socket type */    

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;    

    if ((sin.sin_port = htons(port)) == 0)
        err_quit("invalid port \"%d\"\n", port);    

    /* Map host name to IP address, allowing for dotted decimal */
	phe = gethostbyname(host);
    if(NULL != phe)
    {
        memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
    }
	else if( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
        err_quit("can't get \"%s\" host entry\n", host);    

    /* Map transport protocol name to protocol number */
    if ( (ppe = getprotobyname(transport)) == 0)
        err_quit("can't get \"%s\" protocol entry\n", transport);    

    /* Allocate a common TCP socket */
    s = socket(PF_INET, SOCK_STREAM, ppe->p_proto);
    if (s < 0)
        err_quit("can't create socket: %s\n", strerror(errno));    

    if(bIsHttps != 1){
        /* Connect the socket with timeout */
        fcntl(s,F_SETFL, O_NONBLOCK);
        if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) == -1){
            if (errno == EINPROGRESS){// it is in the connect process 
                struct timeval tv; 
                fd_set writefds; 
                tv.tv_sec = timeout_sec; 
                tv.tv_usec = timeout_microsec; 

                FD_ZERO(&writefds); 
                FD_SET(s, &writefds); 

                if(select(s+1,NULL,&writefds,NULL,&tv)>0){ 
                    int len=sizeof(int); 

                    //�����һ��һ��Ҫ����Ҫ��Է���ǽ 
                    getsockopt(s, SOL_SOCKET, SO_ERROR, &errno, (socklen_t *)&len); 
                    if(errno != 0) 
                        ret = 1;
                    else
                        ret = 0;
                }
                else
                    ret = 2;//timeout or error happen 

            }
            else ret = 1; 
        }
        else{
            ret = 1;
        }
    }
    else{
        /* create common tcp socket.seems non-block type is not supported by ssl. */
        ret = connect(s, (struct sockaddr *)&sin, sizeof(sin));
    }

    if(ret != 0){
        close(s);
        err_quit("can't connect to %s:%d\n", host, port);
    }

    return s;
}

/*
s    - SOCKET
sec  - timeout seconds
usec - timeout microseconds
x    - select status
*/

int xnet_select(int s, int sec, int usec, short x){
    int st = errno;
    struct timeval to;
    fd_set fs;
    to.tv_sec = sec;
    to.tv_usec = usec;
    FD_ZERO(&fs);
    FD_SET(s, &fs);
	
    switch(x){
        case READ_STATUS:
        st = select(s+1, &fs, 0, 0, &to);
        break;
        case WRITE_STATUS:
        st = select(s+1, 0, &fs, 0, &to);
        break;
        case EXCPT_STATUS:
        st = select(s+1, 0, 0, &fs, &to);
        break;
    }
    return(st);
}

void err_doit(int errnoflag, const char *fmt, va_list ap){
    int errno_save;
    char buf[MAX_STRING_LEN];

    errno_save = errno; 
    vsprintf(buf, fmt, ap);
    if (errnoflag)
        sprintf(buf + strlen(buf), ": %s", strerror(errno_save));

    strcat(buf, "\n");
    fflush(stdout);
    fputs(buf, stderr);

    fflush(NULL);
    return;
}


/* Print a message and terminate. */
void err_quit(const char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    err_doit(0, fmt, ap);
    va_end(ap);

    exit(1);
}

/********************************************  
���ܣ������ַ����ұ���ĵ�һ��ƥ���ַ�  
********************************************/   
char *Rstrchr(char *s, char x)   
{   
    int i = strlen(s);   
    if (!(*s))   
        return 0;   
    while (s[i - 1])   
        if (strchr(s + (i - 1), x))   
            return (s + (i - 1));   
        else   
            i--;   
    return 0;   
}   
   
/**************************************************************  
���ܣ����ַ���src�з�������վ��ַ�Ͷ˿ڣ����õ��û�Ҫ���ص��ļ�  
***************************************************************/   
void GetHost(char *src, char *web, char *file, int *port)   
{   
    char *pA;   
    char *pB;   
    memset(web, 0, sizeof(web));   
    memset(file, 0, sizeof(file));   
    *port = 0;   
    if (!(*src))   
        return;   
    pA = src;   
    if (!strncmp(pA, "http://", strlen("http://")))   
        pA = src + strlen("http://");   
    else if (!strncmp(pA, "https://", strlen("https://")))   
        pA = src + strlen("https://");   
    pB = strchr(pA, '/');   
    if (pB) {   
        memcpy(web, pA, strlen(pA) - strlen(pB));   
        if (pB + 1) {   
            memcpy(file, pB + 1, strlen(pB) - 1);   
            file[strlen(pB) - 1] = 0;   
        }   
    } else   
        memcpy(web, pA, strlen(pA));   
    if (pB)   
        web[strlen(pA) - strlen(pB)] = 0;   
    else   
        web[strlen(pA)] = 0;   
    pA = strchr(web, ':');   
    if (pA)   
        *port = atoi(pA + 1);   
    else   
        *port = 443;   
}   
   
/************���ڱ��ĵ�********************************************  
*filename: https-client.c  
*purpose: ��ʾHTTPS�ͻ��˱�̷���  
*wrote by: zhoulifa(zhoulifa@163.com) ������(http://zhoulifa.bokee.com)  
Linux������ Linux֪ʶ������ SOHO�� ������ ���ó�C����  
*date time:2007-01-30 20:06  
*Note: �κ��˿������⸴�ƴ��벢������Щ�ĵ�����Ȼ���������ҵ��;  
* ������ѭGPL  
*Thanks to:Google  
*Hope:ϣ��Խ��Խ����˹����Լ���������Ϊ��ѧ������չ����  
* �Ƽ�վ�ھ��˵ļ���Ͻ������죡��л�п�Դǰ���Ĺ��ף�  
*********************************************************************/
#if 0
int main(int argc, char *argv[])   
{   
    int sockfd, ret;   
    char buffer[1024];   
    struct sockaddr_in server_addr;   
    struct hostent *host;   
    int portnumber, nbytes;   
    char host_addr[256];   
    char host_file[1024];   
    char local_file[256];   
    FILE *fp;   
    char request[1024];   
    int send, totalsend;   
    int i;   
    char *pt;   
    SSL *ssl;   
    SSL_CTX *ctx;   
   
    if (argc != 2) {   
        if (DEBUG)   
            fprintf(stderr, "Usage:%s webpage-address\a\n", argv[0]);   
        exit(1);   
    }   
    if (DEBUG)   
        printf("parameter.1 is: %s\n", argv[1]);   
   
    GetHost(argv[1], host_addr, host_file, &portnumber);        /*������ַ���˿ڡ��ļ����� */   
    if (DEBUG)   
        printf("webhost:%s\n", host_addr);   
    if (DEBUG)   
        printf("hostfile:%s\n", host_file);   
    if (DEBUG)   
        printf("portnumber:%d\n\n", portnumber);   
   
    if ((host = gethostbyname(host_addr)) == NULL) {        /*ȡ������IP��ַ */   
        if (DEBUG)   
            fprintf(stderr, "Gethostname error, %s\n", strerror(errno));   
        exit(1);   
    }   
   
    /* �ͻ�����ʼ���� sockfd������ */   
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {        /*����SOCKET���� */   
        if (DEBUG)   
            fprintf(stderr, "Socket Error:%s\a\n", strerror(errno));   
        exit(1);   
    }   
   
    /* �ͻ�����������˵����� */   
    bzero(&server_addr, sizeof(server_addr));   
    server_addr.sin_family = AF_INET;   
    server_addr.sin_port = htons(portnumber);   
    server_addr.sin_addr = *((struct in_addr *) host->h_addr);   
   
    /* �ͻ��������������� */   
    if (connect(sockfd, (struct sockaddr *) (&server_addr), sizeof(struct sockaddr)) == -1) {        /*������վ */   
        if (DEBUG)   
            fprintf(stderr, "Connect Error:%s\a\n", strerror(errno));   
        exit(1);   
    }   
   
    /* SSL��ʼ�� */   
    SSL_library_init();   
    SSL_load_error_strings();   
    ctx = SSL_CTX_new(SSLv23_client_method());   
    if (ctx == NULL) {   
        ERR_print_errors_fp(stderr);   
        exit(1);   
    }   
   
    ssl = SSL_new(ctx);   
    if (ssl == NULL) {   
        ERR_print_errors_fp(stderr);   
        exit(1);   
    }   
   
    /* ��socket��SSL���� */   
    ret = SSL_set_fd(ssl, sockfd);   
    if (ret == 0) {   
        ERR_print_errors_fp(stderr);   
        exit(1);   
    }   
   
    RAND_poll();   
    while (RAND_status() == 0) {   
        unsigned short rand_ret = rand() % 65536;   
        RAND_seed(&rand_ret, sizeof(rand_ret));   
    }   
   
    ret = SSL_connect(ssl);   
    if (ret != 1) {   
        ERR_print_errors_fp(stderr);   
        exit(1);   
    }   
   
    sprintf(request, "GET /%s HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\n\
		User-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\n\
		Host: %s:%d\r\nConnection: Close\r\n\r\n", host_file, host_addr,  
            portnumber);  
    if (DEBUG)  
        printf("%s", request);        /*׼��request����Ҫ���͸����� */   
   
    /*ȡ����ʵ���ļ��� */   
    if (host_file && *host_file)   
        pt = Rstrchr(host_file, '/');   
    else   
        pt = 0;   
   
    memset(local_file, 0, sizeof(local_file));   
    if (pt && *pt) {   
        if ((pt + 1) && *(pt + 1))   
            strcpy(local_file, pt + 1);   
        else   
            memcpy(local_file, host_file, strlen(host_file) - 1);   
    } else if (host_file && *host_file)   
        strcpy(local_file, host_file);   
    else   
        strcpy(local_file, "index.html");   
    if (DEBUG)   
        printf("local filename to write:%s\n\n", local_file);   
   
    /*����https����request */   
    send = 0;   
    totalsend = 0;   
    nbytes = strlen(request);   
    while (totalsend < nbytes) {   
        send = SSL_write(ssl, request + totalsend, nbytes - totalsend);   
        if (send == -1) {   
            if (DEBUG)   
                ERR_print_errors_fp(stderr);   
            exit(0);   
        }   
        totalsend += send;   
        if (DEBUG)   
            printf("%d bytes send OK!\n", totalsend);   
    }   
   
    fp = fopen(local_file, "a");   
    if (!fp) {   
        if (DEBUG)   
            printf("create file error! %s\n", strerror(errno));   
        return 0;   
    }   
    if (DEBUG)   
        printf("\nThe following is the response header:\n");   
    i = 0;   
    /* ���ӳɹ��ˣ�����https��Ӧ��response */   
    while ((nbytes = SSL_read(ssl, buffer, 1)) == 1) {   
        if (i < 4) {   
            if (buffer[0] == '\r' || buffer[0] == '\n')   
                i++;   
            else   
                i = 0;   
            if (DEBUG)   
                printf("%c", buffer[0]);        /*��httpsͷ��Ϣ��ӡ����Ļ�� */   
        } else {   
            fwrite(buffer, 1, 1, fp);        /*��https������Ϣд���ļ� */   
            i++;   
            if (i % 1024 == 0)   
                fflush(fp);        /*ÿ1Kʱ����һ�� */   
        }   
    }   
    fclose(fp);   
    /* ����ͨѶ */   
    ret = SSL_shutdown(ssl);   
    if (ret != 1) {   
        ERR_print_errors_fp(stderr);   
        exit(1);   
    }   
    close(sockfd);   
    SSL_free(ssl);   
    SSL_CTX_free(ctx);   
    ERR_free_strings();   
    exit(0);   
}   
#endif

/* openssl */
#ifdef _OPENSSL_
int https_download(char          *host_name, 
					   unsigned short port, 
					   char			  *requestpath, 
					   char 		  *post_data,
					   char           *cert_file,
					   char           *key_file,
					   char 		  *out_file_name)   
{   
    int sockfd, ret;   
    char buffer[1024]={0};   
    struct sockaddr_in server_addr;   
    struct hostent *host;   
    int portnumber, nbytes;   
    char host_addr[256];   
//    char host_file[1024];   
    char local_file[256];   
    FILE *fp;   
    char request[1024];   
    int send, totalsend;   
    int i;   
//    char *pt;   
    SSL *ssl;   
    SSL_CTX *ctx;   

//	int flag = 0;

   	#if 0
    if (argc != 2) {   
        if (DEBUG)   
            fprintf(stderr, "Usage:%s webpage-address\a\n", argv[0]);   
        exit(1);   
    }   
    if (DEBUG)   
        printf("parameter.1 is: %s\n", argv[1]);   
   
    GetHost(argv[1], host_addr, host_file, &portnumber);        /*������ַ���˿ڡ��ļ����� */  
	#endif
	char *header="jsonString=";
	sprintf(host_addr, "%s", host_name);
	portnumber = port;
	//host_file = ;
	sprintf(local_file, "%s", out_file_name);
	
    if (DEBUG)   
        printf("webhost:%s\n", host_addr);   
    //if (DEBUG)   
    //    printf("hostfile:%s\n", host_file);   
    if (DEBUG)   
        printf("portnumber:%d\n\n", portnumber);   
   
    if ((host = gethostbyname(host_addr)) == NULL) {        /*ȡ������IP��ַ */   
        if (DEBUG)   
            fprintf(stderr, "Gethostname error, %s\n", strerror(errno));   
        exit(1);   
    }   
   
    /* �ͻ�����ʼ���� sockfd������ */   
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {        /*����SOCKET���� */   
        if (DEBUG)   
            fprintf(stderr, "Socket Error:%s\a\n", strerror(errno));   
        exit(1);   
    }   
   
    /* �ͻ�����������˵����� */   
    bzero(&server_addr, sizeof(server_addr));   
    server_addr.sin_family = AF_INET;   
    server_addr.sin_port = htons(portnumber);   
    server_addr.sin_addr = *((struct in_addr *) host->h_addr);   
   
    /* �ͻ��������������� */   
    if (connect(sockfd, (struct sockaddr *) (&server_addr), sizeof(struct sockaddr)) == -1) {        /*������վ */   
        if (DEBUG)   
            fprintf(stderr, "Connect Error:%s\a\n", strerror(errno));   
        exit(1);   
    }   
   
    /* SSL��ʼ�� */   
    SSL_library_init();   
    SSL_load_error_strings();   
    ctx = SSL_CTX_new(SSLv23_client_method());   
    if (ctx == NULL) {   
        ERR_print_errors_fp(stderr);   
        exit(1);   
    }   
   
	/* set the local certificate from CertFile */
	SSL_CTX_use_certificate_file(ctx, cert_file, SSL_FILETYPE_PEM);
	/* set the private key from KeyFile */
	SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM);
	//SSL_CTX_set_default_passwd_cb_userdata(ctx, "llbox8");
	/* verify private key */
	if ( !SSL_CTX_check_private_key(ctx) ){
        printf("check SSL private key:%s\n",
                    ERR_reason_error_string(ERR_get_error()));
		return -1;
    }			
		
    ssl = SSL_new(ctx);   
    if (ssl == NULL) {   
        ERR_print_errors_fp(stderr);   
        exit(1);   
    }   
   
    /* ��socket��SSL���� */   
    ret = SSL_set_fd(ssl, sockfd);   
    if (ret == 0) {   
        ERR_print_errors_fp(stderr);   
        exit(1);   
    }   
   
    RAND_poll();   
    while (RAND_status() == 0) {   
        unsigned short rand_ret = rand() % 65536;   
        RAND_seed(&rand_ret, sizeof(rand_ret));   
    }   
   
    ret = SSL_connect(ssl);   
    if (ret != 1) {   
        ERR_print_errors_fp(stderr);   
        exit(1);   
    }   
   
    sprintf(request, "GET %s?%s%s HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\nHost: %s:%d\r\nConnection: Close\r\n\r\n", 
			requestpath, header, post_data, 
			host_addr,  
            portnumber);  
    if (DEBUG)  
        printf("%s", request);        /*׼��request����Ҫ���͸����� */   
   
    /*ȡ����ʵ���ļ��� */
	#if 0
    if (host_file && *host_file)   
        pt = Rstrchr(host_file, '/');   
    else   
        pt = 0;   
   
    memset(local_file, 0, sizeof(local_file));   
    if (pt && *pt) {   
        if ((pt + 1) && *(pt + 1))   
            strcpy(local_file, pt + 1);   
        else   
            memcpy(local_file, host_file, strlen(host_file) - 1);   
    } else if (host_file && *host_file)   
        strcpy(local_file, host_file);   
    else   
        strcpy(local_file, "index.html");   
    if (DEBUG)   
        printf("local filename to write:%s\n\n", local_file);   
	#endif
   
    /*����https����request */   
    send = 0;   
    totalsend = 0;   
    nbytes = strlen(request);   
    while (totalsend < nbytes) {   
        send = SSL_write(ssl, request + totalsend, nbytes - totalsend);   
        if (send == -1) {   
            if (DEBUG)   
                ERR_print_errors_fp(stderr);   
            exit(0);   
        }   
        totalsend += send;   
        if (DEBUG)   
            printf("%d bytes send OK!\n", totalsend);   
    }   
   
    fp = fopen(local_file, "ab+");   
    if (!fp) {   
        if (DEBUG)   
            printf("create file error! %s\n", strerror(errno));   
        return 0;   
    }   
    if (DEBUG)   
        printf("\nThe following is the response header:\n");   
    i = 0;   

	/* ���ӳɹ��ˣ�����https��Ӧ��response */   
	while ((nbytes = SSL_read(ssl, buffer, 1)) == 1) {   
        if (i < 4) {   
            if (buffer[0] == '\r' || buffer[0] == '\n')   
                i++;   
            else   
                i = 0;   

			//printf("i=%d\n", i);
			
            if (DEBUG)   
                printf("%c", buffer[0]);        /*��httpsͷ��Ϣ��ӡ����Ļ�� */   
        } else {
            fwrite(buffer, 1, 1, fp);        /*��https������Ϣд���ļ� */ 
			i++;   
            if (i % 1024 == 0)   
                fflush(fp);        /*ÿ1Kʱ����һ�� */
        }   
    }
	
    fclose(fp);   

	/* ����ͨѶ */   
    ret = SSL_shutdown(ssl);   
    if (ret != 1) {   
        ERR_print_errors_fp(stderr);   
        //exit(1);   
        return -1;
    }   
	
    close(sockfd);   
    SSL_free(ssl);   
    SSL_CTX_free(ctx);   
    ERR_free_strings();   
    //exit(0); 

	return 0;
}   


/*****************************************************************************
 �� �� ��  : 
 ��������  : ������
 �������  : 
 �������  : 
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ  :

*****************************************************************************/
int send_https_request(char			*host, 
							 unsigned short port, 
							 char           *requestpath, 
							 char 			*post_data,
					   		 char           *cert_file,
					   		 char           *key_file,
							 char 			*out_file_name,
							 int 			has_respone){

    int fd;
    
    SSL *ssl;
    SSL_CTX *ctx;

    int n=0;
	int ret = -1;

    char buf[BUF_LEN];

	char *header="jsonString=";

	FILE *fp = NULL;
	int flag = 0; /* flag */
	char *pc = NULL;

//	char szCmd[512] = {0};

    /* make connection to the cache server */
    fd = create_tcpsocket(host, port);

    /* http request. */
    sprintf(buf, "GET %s?%s%s HTTP/1.0\r\nHost: %s:%u\r\nContent-Length: %u\r\n\r\n",
    			requestpath, header,post_data, host, port, strlen(header)+strlen(post_data));

	printf("%s\n",buf);
	printf("%s\n",cert_file);
	printf("%s\n",key_file);

    SSL_load_error_strings();
    SSL_library_init();
    ctx = SSL_CTX_new(SSLv23_client_method());
    if ( ctx == NULL ){
        printf("init SSL CTX failed:%s\n",
                    ERR_reason_error_string(ERR_get_error()));
		return -1;
    }        

	/* set the local certificate from CertFile */
	SSL_CTX_use_certificate_file(ctx, cert_file, SSL_FILETYPE_PEM);
	/* set the private key from KeyFile */
	SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM);
	//SSL_CTX_set_default_passwd_cb_userdata(ctx, "llbox8");
	/* verify private key */
	if ( !SSL_CTX_check_private_key(ctx) ){
        printf("check SSL private key:%s\n",
                    ERR_reason_error_string(ERR_get_error()));
		return -1;
    }			

    ssl = SSL_new(ctx);
    if ( ssl == NULL ){
        printf("new SSL with created CTX failed:%s\n",
                    ERR_reason_error_string(ERR_get_error()));
		return -1;
    }

    ret = SSL_set_fd(ssl, fd);
    if ( ret == 0 ){
        printf("add SSL to tcp socket failed:%s\n",
                    ERR_reason_error_string(ERR_get_error()));
		return -1;
    }

    /* PRNG */
    RAND_poll();
    while ( RAND_status() == 0 ){
        unsigned short rand_ret = rand() % 65536;
        RAND_seed(&rand_ret, sizeof(rand_ret));
    }

    /* SSL Connect */
    ret = SSL_connect(ssl);
    if( ret != 1 ){
        printf("SSL connection failed:%s\n",
                    ERR_reason_error_string(ERR_get_error()));
		return -1;
    }

    // https socket write.
    SSL_write(ssl, buf, strlen(buf));

	/* if has response */
	if (has_respone)
	{
		printf("%s\n", out_file_name);

		/* write to file */
		fp = fopen(out_file_name, "a");
		if (NULL == fp)
    	{
			fprintf(stderr, "Can't open file: %s\n", out_file_name);
        	return -1;
		}
		
    	while((n = SSL_read(ssl, buf, BUF_LEN-1)) > 0){
	        buf[n] = '\0';

			printf("%u\n", n);
			
	        write(1, buf, n); // print to io
	        //���ǿ��ܷ��ص����ݱȽ϶࣬д���ļ��С������ļ�����
	        //"\r\n\r\n"
        	/* �����ַ��� */				
            if (0 == flag)
            {
            	pc = strstr(buf, "\r\n\r\n");
				if(pc != NULL)
				{
					flag = 1;
					pc += strlen("\r\n\r\n");
					
					if (1 != fwrite(pc, strlen(pc), 1, fp))
				    {
						fprintf(stderr, "Can't write file: %s\n", out_file_name);
				        fclose(fp);
				        return -1;
				    }					
				}				
            }
			else
			{
				if (1 != fwrite(buf, strlen(buf), 1, fp))
				{
					fprintf(stderr, "Can't write file: %s\n", out_file_name);
			        fclose(fp);
			        return -1;
			    }		
			} 
    	}

		/* close file */
		fclose(fp);
	}
	else
	{
		usleep(10000);
	}

    if(n != 0){
        printf("SSL read failed:%s\n",
                    ERR_reason_error_string(ERR_get_error()));
		return -1;
    }

	/**
	����ֵ��
	1.ִ�гɹ�
	0������ر�
	-1������ 
	*/
    // close ssl tunnel.
    ret = SSL_shutdown(ssl); 
    if( ret != 1 ){
		printf("ret=%d\n", ret);
        close(fd);
        printf("SSL shutdown failed:%s\n",
                    ERR_reason_error_string(ERR_get_error()));

		if (0 == ret)
		{
			return 0;
		}
		return -1;
    }
    
    // close the plain socket handler.
    close(fd);

    // clear ssl resource.
    SSL_free(ssl); 
    SSL_CTX_free(ctx);
    ERR_free_strings();

	return 0;
}
#else
int https_download(char          *host, 
					   unsigned short port, 
					   char			  *requestpath, 
					   char 		  *post_data,
					   char           *cert_file,
					   char           *key_file,
					   char 		  *out_file_name)  
{
	char acCmd[1024] = {0};

	char *header="jsonString="; 

#if (defined(GW_X86) || defined(GW_ARM) || defined(GW_AC))
	sprintf(acCmd, "/appfs/sbin/sslclient server_name=%s server_port=%u crt_file=%s key_file=%s request_page='%s?%s%s\' response_type=%d out_file=%s",
					host,
					port,
					cert_file,
					key_file,
					requestpath, header,post_data,
					HTTPS_RESPONSE_FILE,
					out_file_name);
#else
	sprintf(acCmd, "/usr/sbin/sslclient server_name=%s server_port=%u crt_file=%s key_file=%s request_page='%s?%s%s\' response_type=%d out_file=%s",
					host,
					port,
					cert_file,
					key_file,
					requestpath, header,post_data,
					HTTPS_RESPONSE_FILE,
					out_file_name);
#endif
	printf("%s\n", acCmd);

	system(acCmd);
	
    return 0;
}

int send_https_request(char			*host, 
							 unsigned short port, 
							 char           *requestpath, 
							 char 			*post_data,
					   		 char           *cert_file,
					   		 char           *key_file,
							 char 			*out_file_name,
							 int 			has_respone)
{
	char acCmd[1024] = {0};

	char *header="jsonString="; 

	int response_type = HTTPS_RESPONSE_NO;

	if (has_respone)
	{
		response_type = HTTPS_RESPONSE_STRING;
	}
	
#if (defined(GW_X86) || defined(GW_ARM) || defined(GW_AC))
	sprintf(acCmd, "/appfs/sbin/sslclient server_name=%s server_port=%u crt_file=%s key_file=%s request_page='%s?%s%s' response_type=%d out_file=%s",
					host,
					port,
					cert_file,
					key_file,
					requestpath, header,post_data,
					response_type,
					out_file_name);
#else
	sprintf(acCmd, "/usr/sbin/sslclient server_name=%s server_port=%u crt_file=%s key_file=%s request_page='%s?%s%s' response_type=%d out_file=%s",
					host,
					port,
					cert_file,
					key_file,
					requestpath, header,post_data,
					response_type,
					out_file_name);
#endif

	printf("%s\n", acCmd);

	system(acCmd);
	
    return 0;
}

#endif


#if 0
int main(int argc, char* argv[]){
    char* host = "127.0.0.1";
    unsigned short port = 80;

    int fd;
    
    SSL *ssl;
    SSL_CTX *ctx;

    int n,ret;

    char buf[BUF_LEN];
    char* requestpath = "/";

    if( argc == 5 ){
        host = argv[1];
        port = atoi(argv[2]);
        requestpath = argv[3];
        bIsHttps = atoi(argv[4]);
    }

    /* make connection to the cache server */
    fd = create_tcpsocket(host, port);

    /* http request. */
    sprintf(buf, "POST %s HTTP/1.1\r\nHost: %s:%u\r\n\r\n",requestpath, host, port);

    if(bIsHttps != 1){
        if(xnet_select(fd, timeout_sec, timeout_microsec, WRITE_STATUS)>0){
            /* send off the message */
            write(fd, buf, strlen(buf));
        }
        else{
            err_quit("Socket I/O Write Timeout %s:%d\n", host, port);
        }

        printf("Server response:\n");
        while (xnet_select(fd, timeout_sec, timeout_microsec, READ_STATUS)>0){
            if ((n = read(fd, buf, BUF_LEN-1)) > 0) {
                buf[n] = '\0';
                printf("%s", buf);
            }
            else{
                break;
            }
        }
		
        // close the plain socket handler.
        close(fd);

    }
    else{
        SSL_load_error_strings();
        SSL_library_init();
        ctx = SSL_CTX_new(SSLv23_client_method());
        if ( ctx == NULL ){
            err_quit("init SSL CTX failed:%s\n",
                        ERR_reason_error_string(ERR_get_error()));
        }        

		/* set the local certificate from CertFile */
		SSL_CTX_use_certificate_file(ctx, "/home/fengjing/work/lianlian/client.pem", SSL_FILETYPE_PEM);
		/* set the private key from KeyFile */
		SSL_CTX_use_PrivateKey_file(ctx, "/home/fengjing/work/lianlian/client.key", SSL_FILETYPE_PEM);
		//SSL_CTX_set_default_passwd_cb_userdata(ctx, "llbox8");
		/* verify private key */
		if ( !SSL_CTX_check_private_key(ctx) ){
            err_quit("check SSL private key:%s\n",
                        ERR_reason_error_string(ERR_get_error()));
        }			

        ssl = SSL_new(ctx);
        if ( ssl == NULL ){
            err_quit("new SSL with created CTX failed:%s\n",
                        ERR_reason_error_string(ERR_get_error()));
        }

        ret = SSL_set_fd(ssl, fd);
        if ( ret == 0 ){
            err_quit("add SSL to tcp socket failed:%s\n",
                        ERR_reason_error_string(ERR_get_error()));
        }

        /* PRNG */
        RAND_poll();
        while ( RAND_status() == 0 ){
            unsigned short rand_ret = rand() % 65536;
            RAND_seed(&rand_ret, sizeof(rand_ret));
        }

        /* SSL Connect */
        ret = SSL_connect(ssl);
        if( ret != 1 ){
            err_quit("SSL connection failed:%s\n",
                        ERR_reason_error_string(ERR_get_error()));
        }

        // https socket write.
        SSL_write(ssl, buf, strlen(buf));
        while((n = SSL_read(ssl, buf, BUF_LEN-1)) > 0){
            buf[n] = '\0';
            write(1, buf, n);            
        }

        if(n != 0){
            err_quit("SSL read failed:%s\n",
                        ERR_reason_error_string(ERR_get_error()));
        }

        // close ssl tunnel.
        ret = SSL_shutdown(ssl); 
        if( ret != 1 ){
            close(fd);
            err_quit("SSL shutdown failed:%s\n",
                        ERR_reason_error_string(ERR_get_error()));
        }
        
        // close the plain socket handler.
        close(fd);

        // clear ssl resource.
        SSL_free(ssl); 
        SSL_CTX_free(ctx);
        ERR_free_strings();
    }
}
#endif

