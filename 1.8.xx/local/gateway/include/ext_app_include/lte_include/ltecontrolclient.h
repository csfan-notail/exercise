#ifndef __LTECONTROLCLIENT_H__
#define __LTECONTROLCLIENT_H__

typedef enum{
	ALTAIR_NOSIM = 0,
	ALTAIR_SIMREADY,
	ALTAIR_PINLOCKED,
	ALTAIR_PUKLOCKED,
	ALTAIR_SIMULOCKED,
	ALTAIR_SIMULATOR
}ALTAIR_PIN_STATE_E;

typedef struct{
	ALTAIR_PIN_STATE_E enPinState;
}ALTAIR_PIN_STATE_T;

typedef enum{
	ALTAIR_PIN_CLOSE = 0,
	ALTAIR_PIN_OPEN
}ALTAIR_PIN_SW_STATE_E;

typedef struct{
	ALTAIR_PIN_SW_STATE_E enPinSwState;
}ALTAIR_PIN_SW_STATE_T;

typedef enum{
	ALTAIR_CONNECTED = 1,
	ALTAIR_CONNECTING,
	ALTAIR_DISCONNECTED,
	ALTAIR_AIRPLANEMODE
}ALTAIR_LINK_STATE_E;

typedef struct{
	ALTAIR_LINK_STATE_E enLinkState;
}ALTAIR_LINK_STATE_T;

typedef struct{
	char productName[32];
	char moduleId[32];
	char hardVersion[32];
	char softVersion[32];
	char imei[32];
	char imsi[32];
	char bands[32];
}ALTAIR_DEV_INFO_T;

typedef struct{
	char earfcn[32];
	char band[32];
	char bandWith[32];
	char cellId[32];
	char rsrp[32];
	char rsrq[32];
	char sinr[32];
}ALTAIR_LTE_INFO_T;

#define ALTAIR_SERVER "169.254.0.1"
#define ALTAIR_PORT "5566"
#define ALTAIR_DEFAULT_CLIENT_IP "169.254.0.10"

#endif /* __LTECONTROLCLIENT_H__ */

