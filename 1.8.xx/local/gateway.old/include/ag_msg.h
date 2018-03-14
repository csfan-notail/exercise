/*
 * ag_msg.h
 *
 ****************************************************************************
 */


#ifndef _AG_MSG_H_
#define _AG_MSG_H_

/*
*   消息编解码
*/
#define ENCODE_PUTC(P, V)                  \
    do {                                   \
      *(*P)     = (V) & 0xFF;              \
      (*P)++;                              \
    } while (0)

#define ENCODE_PUTW(P, V)                  \
    do {                                   \
      *(*P)     = ((V) >> 8) & 0xFF;      \
      *(*P + 1) = (V) & 0xFF;             \
      *P += 2;                             \
    } while (0)

#define ENCODE_PUTL(P, V)                  \
    do {                                   \
      *(*P)     = ((V) >> 24) & 0xFF;     \
      *(*P + 1) = ((V) >> 16) & 0xFF;     \
      *(*P + 2) = ((V) >> 8) & 0xFF;      \
      *(*P + 3) = (V) & 0xFF;             \
      *P += 4;                             \
    } while (0)

#define ENCODE_PUTLL(P,V)	\
	do{	\
	ENCODE_PUTL(P, V>>32);	\
	ENCODE_PUTL(P, (V<<32)>>32);	\
	}while(0)
	
#define ENCODE_PUT(P, B, L)                     \
    do {                                        \
      memcpy ((void *)(*P), (void *)(B), (L));  \
      *P += (L);                                \
    } while (0)

#define DECODE_GETC(P, V)                  \
		do {							   \
		  (V) = **P;					   \
		  (*P)++; 						   \
		} while (0)
	
#define DECODE_GETW(P, V)                  \
		do {						       \
		  (V) = ((*(*P)) << 8)            \
		      + (*(*P + 1)); 	           \
		  *P += 2;						   \
		} while (0)
	
#define DECODE_GETL(P, V)                  \
		do {							   \
		  (V) = ((*(*P))	   << 24)	   \
			  + ((*(*P + 1)) << 16)	   \
			  + ((*(*P + 2)) << 8)		   \
			  +  (*(*P + 3)); 			   \
		  *P += 4;						   \
		} while (0)

#define DECODE_GETLL(P, V)	\
		do{ \
			DECODE_GETL(P, V);	\
			V<<=32; \
			DECODE_GETL(P, V);	\
		}while

#define DECODE_GET(D, S, L)                \
		do {							   \
		  memcpy ((void *) (D), *S, (L));  \
		  *S += (L);					   \
		} while (0)

#define AG_NTOHL(x) (x)=ntohl(x)
#define AG_NTOHS(x)	(x)=ntohs(x)

#define AG_HTONL(x)	(x)=htonl(x)
#define AG_HTOSL(x)	(x)=htons(x)

/*
*   AP-GW通信相关
*/ 

/*
*  连接维护消息
*/
#define DEV_REGISTER_MSG			1
#define DEV_REGISTER_ACK_MSG		2
#define DEV_NAK_MSG					3
#define DEV_KEEPALIVE_MSG			4
#define DEV_KEEPALIVE_ACK_MSG		5
#define DEV_LOGOUT_MSG				6
#define DEV_LOGOUT_ACK_MSG			7


/*
*  终端消息
*/
#define STA_REGISTER_MSG			64
#define STA_REGISTER_ACK_MSG		65
#define STA_CONNECT_MSG				66
#define STA_CONNECT_ACK_MSG			67
#define STA_DISCONNECT_MSG			68
#define STA_DISCONNECT_ACK_MSG		69
#define STA_DETECT_MSG				70
#define STA_DETECT_ACK_MSG			71
#define STA_DETECT_TIME_MSG			72
#define STA_DETECT_ENABLE_MSG		73
#define STA_DETECT_DISABLE_MSG		74
#define STA_DETECT_RSSI_MSG			75


/*ap-gw消息头结构*/
struct ag_msg_ctrlhdr
{
	u16 msg_type;
	u16 msg_len; 
};


/*
*   Wifidog 通信相关
*/

#define UNIX_SOCK_PATH		"/tmp/gwctl.sock"
#define UNIX_SOCK_REQUEST	"aglink-request\n"
#ifdef _BOXLL_
#define UNIX_SOCK_PATH_BOXLL  "/tmp/boxctl.sock"
#endif


/*发送消息*/
#define WIFIDOG_AP_CNNT_MSG			1	
#define WIFIDOG_AP_DISCNNT_MSG		2
#define WIFIDOG_STA_CNNT_MSG		3	
#define WIFIDOG_STA_DISCNNT_MSG		4
#define WIFIDOG_STA_DCT_MSG			5

/*wifidog消息头结构*/
struct wifidog_msg_ctrlhdr
{
	u32 msg_type;
	u32 msg_len; 
};


/*接收消息*/
#define WIFIDOG_STADCT_ENABLE_MSG   		1
#define WIFIDOG_STADCT_DISABLE_MSG  		2
#define WIFIDOG_STADCT_RSSI_MSG   			3


#endif

