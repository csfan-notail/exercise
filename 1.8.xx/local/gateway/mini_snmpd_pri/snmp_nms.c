#include "snmp_nms.h"

#ifdef HAVE_SYSLOG
#include "syslog_public.h"
#include "syslog_common.h"
#include "syslogu.h"
#endif

// new add
#ifndef UINT8
#define UINT8 unsigned char
#define UINT16 unsigned short
#define UINT32 unsigned int
#endif

#ifdef WIN32
#ifdef HAVE_SYSLOG
#undef HAVE_SYSLOG
#endif
#endif

/* BEGIN: Added by zoucaihong, 2014/5/14 */
extern int g_udp_sockfd;
/* END: Added by zoucaihong, 2014/5/14 */

#if 0
#define CHECK_REQ_ID(stReqID, pucRspData) if(0)
#else
#define CHECK_REQ_ID(stReqID, pucRspData, ucTmp)  \
	if (stReqID.pucValue && \
        memcmp(pucRspData, stReqID.pucValue, ucTmp) != 0)
#endif

#define SET_LEN_VAR(x)			\
if (snmp_set_len_var(x) < 0)	\
{								\
	return -1;					\
}

#define TLV_LEN(x)	(1 + (x)->ucLenNum + (x)->ulLen)

#define INIT_VAR(x)										\
{														\
    if ((++ulCurrPDULen) > ulMaxReqPDULen) return -1;	\
	*pucValue++ = (x)->ucType;							\
	for (i=0; i<(x)->ucLenNum; i++)	   					\
	{													\
		if ((++ulCurrPDULen) >ulMaxReqPDULen) return -1;\
		*pucValue++ = (x)->aucLen[i];					\
	}													\
	if ((x)->pucValue)									\
	{													\
		ulCurrPDULen += (x)->ulLen;						\
		if (ulCurrPDULen > ulMaxReqPDULen) return -1;	\
		memcpy(pucValue, (x)->pucValue, (x)->ulLen);	\
		pucValue += (x)->ulLen;							\
	}													\
}

#define DATA_DUP(x, data, len)      \
{                                   \
    (x)->pucValue = (UINT8*)malloc(len); \
    if (!(x)->pucValue)             \
    {                               \
        return -1;                  \
    }                               \
    memcpy((x)->pucValue, data, len);\
    (x)->ucNeedFree = 1;            \
}

#define FREE_TLV(x)         \
if (snmp_free_tlv(x) < 0)   \
{                           \
    return -1;              \
}

#if 0
void snmp_free_value(SNMP_VALUE_T *pstValue)
{
	for(; pstValue != NULL; pstValue = pstValue->pstNext)
	{
		if (pstValue->pucValue)
		{
			free(pstValue->pucValue);
			pstValue->pucValue = NULL;
		}
	}
}
#endif

/*************************************************************************
* 函数名称: snmp_free_tlv
* 功    能: 释放PDU中的动态申请的内存
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_free_tlv(SNMP_TLV_T *pstVar)
{
    if (!pstVar)
    {
        return 0;
    }

    if (pstVar->ucNeedFree && pstVar->pucValue)
    {                           
        free(pstVar->pucValue);
        pstVar->pucValue = NULL;
        pstVar->ucNeedFree = 0;
    }

    return 0;
}
#if 0
void nms_print_tlv(SNMP_TLV_T *pstTlv)
{
	unsigned long i=0;
	if (NULL == pstTlv)
	{
		return;
	}
	
	printf("ucType:%u\n", pstTlv->ucType);	
	printf("ucLenNum:%u\n", pstTlv->ucLenNum);
	printf("aucLen:");
	for(i=0; i<8; i++)
	{
		printf("%02x ", pstTlv->aucLen[i]);
	}
	printf("\n");	
	printf("ulLen:%u\n", pstTlv->ulLen);
	printf("ucNeedFree:%u\n", pstTlv->ucNeedFree);

	if (pstTlv->pucValue != NULL)
	{
		printf("pucValue:\n");	
		for(i=0; i<pstTlv->ulLen; i++)
		{
			printf("%02x ", pstTlv->pucValue[i]);
			if ((i+1)%10 == 0)
			{
				printf("\n");
			}
		}
		printf("\n");
	}
}

void nms_print_binary(unsigned char   *pucRsp, unsigned int ulRspDataLen)
{
	unsigned long i=0;
	
	printf("RspData:\n");
	printf("ulRspDataLen:%u\n", ulRspDataLen);

	for (i=0;i<ulRspDataLen; i++)
	{
		printf("%02x ", pucRsp[i]);
		if ((i+1)%10 == 0)
		{
			printf("\n");
		}
	}

	printf("\n");
	
	printf("\n");
}

void nms_print_table_pdu(SNMP_GET_TABLE_PDU_T *pstPDU)
{
//	unsigned long i=0;

	printf("ReqData:\n");
	
	printf("stPDU:\n");
	nms_print_tlv(&pstPDU->stPDU);
	
	printf("stVer:\n");
	nms_print_tlv(&pstPDU->stVer);
	
	printf("stCommunity:\n");
	nms_print_tlv(&pstPDU->stCommunity);
	
	printf("stSnmpPDU:\n");	
	nms_print_tlv(&pstPDU->stSnmpPDU);
	
	printf("stReqID:\n");
	nms_print_tlv(&pstPDU->stReqID);
	
	printf("stErrStatus:\n");
	nms_print_tlv(&pstPDU->stErrStatus);
	
	printf("stErrIndex:\n");
	nms_print_tlv(&pstPDU->stErrIndex);
	
	printf("stParas:\n");
	nms_print_tlv(&pstPDU->stParas);
	
	printf("stPara1:\n");
	nms_print_tlv(&pstPDU->stPara1);
	
	printf("stOID:\n");
	nms_print_tlv(&pstPDU->stOID);	
	
	printf("ucOIDValue:%u\n", pstPDU->ucOIDValue);
	printf("ucEndMark:%u\n", pstPDU->ucEndMark);		
	
	
}
#endif
/*************************************************************************
* 函数名称: snmp_create_oid_var
* 功    能: 生成OID的BER报文
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
#ifndef HAVE_SYSLOG
void nms_log(int iLogLevel, char *szLogFormat, ...)
{
#ifndef LMT_MODE
    char    acMsg[1024];
    va_list vaMsg;
	char	*szLogLevel;

    va_start(vaMsg, szLogFormat);
    vsprintf(acMsg, szLogFormat, vaMsg);
    va_end(vaMsg);

	     if (iLogLevel == LOG_EMERG)	szLogLevel = "Emerg";
	else if (iLogLevel == LOG_ALERT)	szLogLevel = "Alert";
	else if (iLogLevel == LOG_CRIT)		szLogLevel = "Crit";
	else if (iLogLevel == LOG_ERR)		szLogLevel = "Error";
	else if (iLogLevel == LOG_WARNING)	szLogLevel = "Warn";
	else if (iLogLevel == LOG_NOTICE)	szLogLevel = "Notice";
	else if (iLogLevel == LOG_INFO)		szLogLevel = "Info";
	else if (iLogLevel == LOG_DEBUG)	szLogLevel = "Debug";
	else szLogLevel = "";

	printf("%s: %s\n", 
			szLogLevel,
            acMsg);

    return;    
#endif
}
#else
#define nms_log logu
#endif

/*************************************************************************
* 函数名称: snmp_create_oid_var
* 功    能: 生成OID的BER报文
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_create_oid_var(SNMP_OID *pOID, unsigned int ulOIDSize, 
						unsigned char *pucVarData, unsigned char *pucVarLen)
{
    unsigned int    i, j;
    unsigned int    ulOID;
    unsigned char   aucOID[5];
    unsigned char   ucOIDSize;
    unsigned char   *pucVar = pucVarData;

    //第1、2个字节合并为40*x + y ; 需要考虑超过128，目前固定为 43 = 40*1+3
    *pucVar++ = (unsigned char)(40*pOID[0] + pOID[1]);

    for (i=2; i<ulOIDSize; i++)
    {
        ulOID = pOID[i];

        ucOIDSize = 5;
        for (j=0; j<5; j++)
        {
            aucOID[j] = (unsigned char)(ulOID & 0x7F);
            ulOID = (ulOID >> 7);

            if (ulOID == 0)
            {
                ucOIDSize = (unsigned char)(j+1);
                break;
            }
        }

        //  0   1   2   3   4
        //  33  40  1   0   0
        // 28723 = (0x01*0x80 + 0x40) * 0x80 + 0x33 (81 e0 33)
        for (j=1; j<ucOIDSize; j++)
        {
            *pucVar++ = (unsigned char)(aucOID[ucOIDSize-j] + 0x80);
        }

        *pucVar++ = aucOID[0];
    }

    *pucVarLen = (unsigned char)(pucVar - pucVarData);

    return 0;
}
#if 0
/*************************************************************************
* 函数名称: snmp_parse_oid_var
* 功    能: 根据BER格式的OID生成标准OID
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_parse_oid_var(unsigned char *pucVarData, unsigned char ucVarLen, 
					   SNMP_OID *pOID, unsigned int *pulOIDSize)
{
    unsigned int    i;
    unsigned int    ulOID;
	unsigned int	ulOIDSize = 0;
	int				iVarLen = 0;
    unsigned char   *pucVar = pucVarData;

	*pulOIDSize = 0;
    
	//第1、2个字节合并为40*x + y ; 需要考虑超过128，目前固定为 43 = 40*1+3
	pOID[0] = pucVar[0] / 40;
	pOID[1] = pucVar[0] % 40;
	
	ulOIDSize = 2;
	iVarLen = 0;

	while(1)
	{
		/* 获取一个OID */
		ulOID = 0;
		for (i=0; ;i++)
		{
			iVarLen ++;
			if (iVarLen >= ucVarLen)
			{
				return -1;
			}			

			if (pucVar[iVarLen] < 0x80)
			{
				ulOID = (ulOID << 7) + pucVar[iVarLen];
				break;
			}

			ulOID = (ulOID << 7) + pucVar[iVarLen] - 0x80;
		}

		pOID[ulOIDSize++] = ulOID;

		if ((iVarLen+1) == ucVarLen)
		{
			break;
		}
	}

	*pulOIDSize = ulOIDSize;
  
    return 0;
}
#endif
/*************************************************************************
* 函数名称: snmp_get_req_id
* 功    能: 获取snmp PDU报文中的req id
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
unsigned short snmp_get_req_id(void)
{
    /*static unsigned short usReqID = 256;

    if (usReqID < 256)
    {
        usReqID = 256;
    }*/

    static unsigned short usReqID = 1;

    usReqID ++;
    if (usReqID > 0xff)
    {
        usReqID = 1;
    }

    return (usReqID <<8) + usReqID;
    
    //return ++usReqID;
}

/*************************************************************************
* 函数名称: snmp_get_int_value
* 功    能: 根据报文中整数参数转换成整数
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
unsigned int snmp_get_int_value(unsigned char *pucValue, unsigned char ucSize)
{
    unsigned int ulValue = 0;
    int i;

    for (i=0; i<ucSize; i++)
    {
        ulValue = (ulValue << 8) + pucValue[i];
    }

    return ulValue;
}

/*************************************************************************
* 函数名称: snmp_set_len_var
* 功    能: 生成报文中的整数字段
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_set_len_var(SNMP_TLV_T *pVar)
{
	unsigned char	aucLen[4];

	if (pVar->ulLen <= 0x7f)
	{
		pVar->ucLenNum = 1;
		pVar->aucLen[0] = pVar->ulLen;
	}
	else
	{
		pVar->ucLenNum = 4;
		*((int*)aucLen) = htonl(pVar->ulLen);
		if (aucLen[0] == 0)	
		{
			pVar->ucLenNum --;

			if (aucLen[1] == 0)	
			{
				pVar->ucLenNum --;
				if (aucLen[2] == 0)	
				{
					pVar->ucLenNum --;
				}
			}
		}
		
		memcpy(&pVar->aucLen[1], &aucLen[4-pVar->ucLenNum], pVar->ucLenNum);
		pVar->aucLen[0] = 0x80 + pVar->ucLenNum;
		pVar->ucLenNum++;
	}

	return 0;
}

/*************************************************************************
* 函数名称: snmp_init_var
* 功    能: 生成tlv报文
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_init_var(SNMP_VAR_T *pstVar, SNMP_OID *pOID, unsigned int ulOIDSize, 
				  unsigned char ucReqDataType, unsigned char *pucReqData, 
				  unsigned int ulReqDataLen)
{
    SNMP_TLV_T			*pstTLV;
    //unsigned int        ulTmp;
    int                 iTmp;
    unsigned long long  ulLongTmp;
    unsigned int        ulLoop = 0;
	unsigned char		ucOIDLen = 0;
	unsigned char		aucOID[64];

    /* value */
	pstTLV = &pstVar->stValue;
    pstTLV->ucType = ucReqDataType;
    pstTLV->pucValue = pucReqData;
    pstTLV->ulLen  = ulReqDataLen;
    pstTLV->ucNeedFree = 0;
    /* BEGIN: Modified by zoucaihong, 2013/4/18 */
    #if 0
	if (ucReqDataType == ASN_GAUGE      ||
        //ucReqDataType == ASN_INTEGER    ||
        ucReqDataType == ASN_TIMETICKS  )
	{
		if( ulReqDataLen != 4)
		{
			return -1;
		}

        DATA_DUP(pstTLV, pucReqData, 4);
		ulTmp = *((unsigned int*)pstTLV->pucValue);
		if (ulTmp <= 0xff)
		{
			pstTLV->pucValue[0] = (unsigned char)ulTmp;
			pstTLV->ulLen = 1;
		}
		else if (ulTmp <= 0xffff)
		{
			pstTLV->pucValue[0] = (unsigned char)(ulTmp>>8);
			pstTLV->pucValue[1] = (unsigned char)(ulTmp);
			pstTLV->ulLen = 2;
		}
		else if (ulTmp <= 0xffffff)
		{
			pstTLV->pucValue[0] = (unsigned char)(ulTmp>>16);
			pstTLV->pucValue[1] = (unsigned char)(ulTmp>>8);
			pstTLV->pucValue[2] = (unsigned char)(ulTmp);
			pstTLV->ulLen = 3;
		}
		else
		{
			pstTLV->pucValue[0] = (unsigned char)(ulTmp>>24);
			pstTLV->pucValue[1] = (unsigned char)(ulTmp>>16);
			pstTLV->pucValue[2] = (unsigned char)(ulTmp>>8);
			pstTLV->pucValue[3] = (unsigned char)(ulTmp);
			pstTLV->ulLen = 4;
		}
	}
    /* BEGIN: Added by 李文, 2013/1/7   PN:bug#1531 */
    else if(ucReqDataType == ASN_INTEGER)
    {
        if( ulReqDataLen != 4)
        {
            return -1;
        }
 
        DATA_DUP(pstTLV, pucReqData, 4);
        iTmp = *((int*)pstTLV->pucValue);
        if(iTmp < -8388608 || iTmp > 8388607)
        {
            pstTLV->ulLen = 4;
        }
        else if(iTmp < -32768 || iTmp > 32767)
        {
            pstTLV->ulLen = 3;
        }
        else if(iTmp < -128 || iTmp > 127)
        {
            pstTLV->ulLen = 2;
        }
        else
        {
            pstTLV->ulLen = 1;
        }
        while (ulLoop < pstTLV->ulLen) {
            pstTLV->pucValue[ulLoop] = ((unsigned int)iTmp >> (8 * (pstTLV->ulLen-ulLoop-1))) & 0xFF;
            ulLoop++;
        }
    }
    /* END:   Added by 李文, 2013/1/7   PN:bug#1531 */
    #else
	if (ucReqDataType == ASN_GAUGE      ||
        ucReqDataType == ASN_INTEGER    ||
        ucReqDataType == ASN_TIMETICKS  )
	{
        if( ulReqDataLen != 4)
        {
            return -1;
        }
 
        DATA_DUP(pstTLV, pucReqData, 4);
        iTmp = *((int*)pstTLV->pucValue);
        if(iTmp < -8388608 || iTmp > 8388607)
        {
            pstTLV->ulLen = 4;
        }
        else if(iTmp < -32768 || iTmp > 32767)
        {
            pstTLV->ulLen = 3;
        }
        else if(iTmp < -128 || iTmp > 127)
        {
            pstTLV->ulLen = 2;
        }
        else
        {
            pstTLV->ulLen = 1;
        }
        while (ulLoop < pstTLV->ulLen) {
            pstTLV->pucValue[ulLoop] = ((unsigned int)iTmp >> (8 * (pstTLV->ulLen-ulLoop-1))) & 0xFF;
            ulLoop++;
        }
	}
    else if ( ucReqDataType == ASN_COUNTER64 )
    {
        if( ulReqDataLen != 8)
        {
            return -1;
        }
 
        DATA_DUP(pstTLV, pucReqData, 8);
        ulLongTmp = *((unsigned long long *)pstTLV->pucValue);
        #ifdef MGSOFT
        if ( ulLongTmp & 0x8000000000000000LL )
        {
            ulLongTmp &=~0x8000000000000000LL;//首位1去掉
        }
        if ( ulLongTmp > 0x007FFFFFFFFFFFFFLL ) { 
            pstTLV->ulLen = 8;
        }else if ( ulLongTmp > 0x00007FFFFFFFFFFFLL ) {
            pstTLV->ulLen = 7;
    	} else if (ulLongTmp > 0x0000007FFFFFFFFFLL) {
    		pstTLV->ulLen = 6;
    	} else if (ulLongTmp > 0x000000007FFFFFFFLL) {
    		pstTLV->ulLen = 5;
    	} else if (ulLongTmp > 0x00000000007FFFFFLL) {
    		pstTLV->ulLen = 4;
    	} else if (ulLongTmp > 0x0000000000007FFFLL) {
    		pstTLV->ulLen = 3;
    	} else if (ulLongTmp > 0x000000000000007FLL) {
    		pstTLV->ulLen = 2;
    	} else {
    		pstTLV->ulLen = 1;
    	}
        #else
    	if (ulLongTmp & 0xFF00000000000000LL) {
    		pstTLV->ulLen = 8;
    	} else if (ulLongTmp & 0x00FF000000000000LL) {
    		pstTLV->ulLen = 7;
    	} else if (ulLongTmp & 0x0000FF0000000000LL) {
    		pstTLV->ulLen = 6;
    	} else if (ulLongTmp & 0x000000FF00000000LL) {
    		pstTLV->ulLen = 5;
    	} else if (ulLongTmp & 0x00000000FF000000LL) {
    		pstTLV->ulLen = 4;
    	} else if (ulLongTmp & 0x0000000000FF0000LL) {
    		pstTLV->ulLen = 3;
    	} else if (ulLongTmp & 0x000000000000FF00LL) {
    		pstTLV->ulLen = 2;
    	} else {
    		pstTLV->ulLen = 1;
    	}
        #endif
        while (ulLoop < pstTLV->ulLen) {
            pstTLV->pucValue[ulLoop] = ((unsigned long long)ulLongTmp >> (8 * (pstTLV->ulLen-ulLoop-1))) & 0xFF;
            ulLoop++;
        }
    }
    #endif
    /* END: Modified by zoucaihong, 2013/4/18 */
    else if (ucReqDataType == ASN_OBJECT_ID)
    {
        if (snmp_create_oid_var((SNMP_OID*)pucReqData, ulReqDataLen/sizeof(SNMP_OID), &aucOID[0], &ucOIDLen) < 0)
        {
            nms_log(LOG_ERR, "snmp_init_var: create oid var failed");
            return -1;
        }
	    pstTLV->ulLen = ucOIDLen;
	    DATA_DUP(pstTLV, aucOID, ucOIDLen);
    }
	SET_LEN_VAR(pstTLV);

    /* oid */
	pstTLV = &pstVar->stOID;
    pstTLV->ucType = ASN_OBJECT_ID;
    if (snmp_create_oid_var(pOID, ulOIDSize, &aucOID[0], &ucOIDLen) < 0)
    {
        nms_log(LOG_ERR, "snmp_init_var: create oid var failed");
        return -1;
    }
	pstTLV->ulLen = ucOIDLen;
	DATA_DUP(pstTLV, aucOID, ucOIDLen);
	SET_LEN_VAR(pstTLV);

	/* para */
	pstTLV = &pstVar->stPara;
    pstTLV->ucType = SNMP_ID_SEQUENCE;
    pstTLV->ulLen = TLV_LEN(&pstVar->stOID) + 
		            TLV_LEN(&pstVar->stValue);
	SET_LEN_VAR(pstTLV);

    return 0;
}
#if 0
/*************************************************************************
* 函数名称: snmp_init_get_info
* 功    能: 生成get操作的PDU
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_init_get_info(char *szCommunity, SNMP_OID *pOID, unsigned int ulOIDSize, 
					   //unsigned char *pucValue, unsigned int *pulValueLen,
                       SNMP_GET_PDU_T *pstPDU)
{
    unsigned short      usReqIDValue;

    /* oid */
    pstPDU->ucOIDType = ASN_OBJECT_ID;
    if (snmp_create_oid_var(pOID, ulOIDSize, &pstPDU->aucOID[0], &pstPDU->ucOIDSize) < 0)
    {
        return -1;
    }
    pstPDU->ucOIDValue = ASN_NULL;

    pstPDU->ucPara1Len = (1+1+pstPDU->ucOIDSize) +
                         (1+1);   //null & endmark
    pstPDU->ucPara1Type = SNMP_ID_SEQUENCE;

    pstPDU->ucParasLen = (1+1+pstPDU->ucPara1Len);
    pstPDU->ucParasType = SNMP_ID_SEQUENCE;

    /* resv */
    pstPDU->ucResv1Type = ASN_INTEGER;
    pstPDU->ucResv1Len  = 1;
    pstPDU->ucResv1Value= 0;

    pstPDU->ucResv2Type = ASN_INTEGER;
    pstPDU->ucResv2Len  = 1;
    pstPDU->ucResv2Value= 0;

    /* req id */
    pstPDU->ucReqIDType = ASN_INTEGER;

    usReqIDValue = snmp_get_req_id();
    if (usReqIDValue <= 0xff)
    {
        pstPDU->aucReqIDValue[0] = (unsigned char)usReqIDValue;

        pstPDU->ucReqIDLen = 1;
    }
    else
    {
        pstPDU->aucReqIDValue[1] = (unsigned char)usReqIDValue;
        pstPDU->aucReqIDValue[0] = (unsigned char)(usReqIDValue >> 8);

        pstPDU->ucReqIDLen = 2;
    }
	pstPDU->usReqID = usReqIDValue;
    
    pstPDU->ucSnmpPDULen = (1+1+pstPDU->ucReqIDLen) + // req id
                           (1+1+1) +  //resv1
                           (1+1+1) +  //resv2
                           (1+1+pstPDU->ucParasLen);
    pstPDU->ucSnmpPDUType = SNMP_MSG_GET;

    //community
    pstPDU->ucCommunityType = ASN_OCTET_STR;
    pstPDU->ucCommunityLen = strlen(szCommunity);
    if (pstPDU->ucCommunityLen > sizeof(pstPDU->aucCommunity))
    {
        return -1;
    }
    memcpy(pstPDU->aucCommunity, szCommunity, pstPDU->ucCommunityLen);

    //ver
    pstPDU->ucVerType = ASN_INTEGER;
    pstPDU->ucVerLen  = 1;
    pstPDU->ucVerValue= SNMP_GET_VER;

    pstPDU->ucPDUType = SNMP_ID_SEQUENCE;
    pstPDU->ucPDULen  = (1+1+1) + //ver
                        (1+1+pstPDU->ucCommunityLen) + 
                        (1+1+pstPDU->ucSnmpPDULen);

    return 0;
}

/*************************************************************************
* 函数名称: snmp_create_get_pdu
* 功    能: 生成get操作的PDU
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_create_get_pdu1(SNMP_GET_PDU_T *pstPDU, unsigned char *pucValue, unsigned int *pulValueLen)
{
    //unsigned short      usReqIDValue;
    unsigned int        i;

     /* 根据结构生成PDU */
    *pucValue++ = pstPDU->ucPDUType;
    *pucValue++ = pstPDU->ucPDULen;
    
    *pucValue++ = pstPDU->ucVerType;
    *pucValue++ = pstPDU->ucVerLen;
    *pucValue++ = pstPDU->ucVerValue;

    *pucValue++ = pstPDU->ucCommunityType;
    *pucValue++ = pstPDU->ucCommunityLen;
    memcpy(pucValue, pstPDU->aucCommunity, pstPDU->ucCommunityLen);
    pucValue += pstPDU->ucCommunityLen;

    *pucValue++ = pstPDU->ucSnmpPDUType;
    *pucValue++ = pstPDU->ucSnmpPDULen;

    *pucValue++ = pstPDU->ucReqIDType;
    *pucValue++ = pstPDU->ucReqIDLen;
    for(i=0; i<pstPDU->ucReqIDLen; i++)
    {
        *pucValue++ = pstPDU->aucReqIDValue[i];
    }

    *pucValue++ = pstPDU->ucResv1Type;
    *pucValue++ = pstPDU->ucResv1Len;
    *pucValue++ = pstPDU->ucResv1Value;

    *pucValue++ = pstPDU->ucResv2Type;
    *pucValue++ = pstPDU->ucResv2Len;
    *pucValue++ = pstPDU->ucResv2Value;

    *pucValue++ = pstPDU->ucParasType;
    *pucValue++ = pstPDU->ucParasLen;

    *pucValue++ = pstPDU->ucPara1Type;
    *pucValue++ = pstPDU->ucPara1Len;

    *pucValue++ = pstPDU->ucOIDType;
    *pucValue++ = pstPDU->ucOIDSize;
    memcpy(pucValue, pstPDU->aucOID, pstPDU->ucOIDSize);
    pucValue += pstPDU->ucOIDSize;

    *pucValue++ = pstPDU->ucOIDValue;
    *pucValue++ = pstPDU->ucEndMark;

    *pulValueLen = (1+1+pstPDU->ucPDULen);

    return 0;
}

/*************************************************************************
* 函数名称: snmp_create_get_pdu
* 功    能: 生成get操作报文
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_init_get_pdu(char *szCommunity, SNMP_OID *pOID, unsigned int ulOIDSize, 
					  unsigned char ucGetType, SNMP_GET_SCALAR_PDU_T *pstPDU)
{
	SNMP_TLV_T			*pstVar;
    unsigned short      usReqIDValue;
	unsigned char		ucOIDLen = 0;
	unsigned char		aucOID[64];
	unsigned char		aucReqID[2];
	unsigned char		ucErrorStatus = 0;
	unsigned char		ucErrorIndex  = 0;
	unsigned char		ucVersion     = SNMP_GET_VER;

	/* end mark */
	pstPDU->ucEndMark = 0;

	/* oid value */
	pstPDU->ucOIDValue = ASN_NULL;

    /* oid */
	pstVar = &pstPDU->stOID;
    pstVar->ucType = ASN_OBJECT_ID;
    if (snmp_create_oid_var(pOID, ulOIDSize, &aucOID[0], &ucOIDLen) < 0)
    {
        nms_log(LOG_ERR, "snmp_init_set_pdu: create oid var failed");
        return -1;
    }
	pstVar->ulLen = ucOIDLen;	
	DATA_DUP(pstVar, aucOID, ucOIDLen);
	SET_LEN_VAR(pstVar);

	/* para1 */
	pstVar = &pstPDU->stPara1;
    pstVar->ucType = SNMP_ID_SEQUENCE;
    pstVar->ulLen = TLV_LEN(&pstPDU->stOID) + 
					sizeof(pstPDU->ucOIDValue)+
				    sizeof(pstPDU->ucEndMark);
	SET_LEN_VAR(pstVar);

	/* paras */
	pstVar = &pstPDU->stParas;
    pstVar->ucType = SNMP_ID_SEQUENCE;
    pstVar->ulLen = TLV_LEN(&pstPDU->stPara1);
	SET_LEN_VAR(pstVar);

    /* error index */
	pstVar = &pstPDU->stErrIndex;
    pstVar->ucType = ASN_INTEGER;
    pstVar->ulLen = 1;
    DATA_DUP(pstVar, &ucErrorIndex, sizeof(ucErrorIndex));
	SET_LEN_VAR(pstVar);

    /* error status */
	pstVar = &pstPDU->stErrStatus;
    pstVar->ucType = ASN_INTEGER;
    pstVar->ulLen = 1;
    DATA_DUP(pstVar, &ucErrorStatus, sizeof(ucErrorStatus));
	SET_LEN_VAR(pstVar);

    /* req id */
	pstVar = &pstPDU->stReqID;
    pstVar->ucType = ASN_INTEGER;
    usReqIDValue = snmp_get_req_id();
    if (usReqIDValue <= 0xff)
    {
        aucReqID[0] = (unsigned char)usReqIDValue;
        pstVar->ulLen = 1;
    }
    else
    {
        aucReqID[0] = (unsigned char)usReqIDValue;
        aucReqID[1] = (unsigned char)(usReqIDValue >> 8);
		pstVar->ulLen = 2;
    }
    DATA_DUP(pstVar, aucReqID, pstVar->ulLen);
    SET_LEN_VAR(pstVar);
	//pstPDU->usReqID = usReqIDValue;

	/* snmp pdu */
	pstVar = &pstPDU->stSnmpPDU;
    pstVar->ucType = ucGetType; //SNMP_MSG_GET;
	pstVar->ulLen = TLV_LEN(&pstPDU->stReqID) +
					TLV_LEN(&pstPDU->stErrStatus) +
					TLV_LEN(&pstPDU->stErrIndex) +
					TLV_LEN(&pstPDU->stParas);
	SET_LEN_VAR(pstVar);

    //community
	pstVar = &pstPDU->stCommunity;
    pstVar->ucType = ASN_OCTET_STR;
    pstVar->ulLen = strlen(szCommunity);
    if (pstVar->ulLen > SNMP_MAX_COMMUNITY_LEN)
    {
        return -1;
    }
	pstVar->pucValue = (unsigned char*)szCommunity;
    pstVar->ucNeedFree = 0;
    SET_LEN_VAR(pstVar);

    //version
	pstVar = &pstPDU->stVer;
    pstVar->ucType = ASN_INTEGER;
    pstVar->ulLen  = 1;
    DATA_DUP(pstVar, &ucVersion, sizeof(ucVersion));
	SET_LEN_VAR(pstVar);

	//PDU
	pstVar = &pstPDU->stPDU;
    pstVar->ucType = SNMP_ID_SEQUENCE;
	pstVar->ulLen  = TLV_LEN(&pstPDU->stVer) +
					 TLV_LEN(&pstPDU->stCommunity) +
					 TLV_LEN(&pstPDU->stSnmpPDU);
	SET_LEN_VAR(pstVar);

    return 0;
}

/*************************************************************************
* 函数名称: snmp_create_getbulk_pdu
* 功    能: 生成getbulk操作报文
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_init_getbulk_pdu(char *szCommunity, unsigned int ulOIDNum, SNMP_OID_T *pstOID, 
						  unsigned char ucGetType, SNMP_GET_BULK_PDU_T *pstPDU)
{
	SNMP_TLV_T			*pstVar;
	unsigned int		i;
	unsigned int		ulParaLen = 0;
    unsigned short      usReqIDValue;
	unsigned char		ucOIDLen = 0;
	unsigned char		aucOID[64];
	unsigned char		aucReqID[2];
	unsigned char		ucErrorStatus = 0;
	unsigned char		ucErrorIndex  = 0;
	unsigned char		ucVersion     = SNMP_GET_VER;

	/* end mark */
	pstPDU->ucEndMark = 0;

	/* oid value */
	pstPDU->ucOIDValue = ASN_NULL;

	/* oid & para */
	pstPDU->ulOIDNum = ulOIDNum;
	pstPDU->pstPara = (SNMP_TLV_T*)malloc(2*ulOIDNum*sizeof(SNMP_TLV_T));
	if (NULL == pstPDU->pstPara)
	{
		nms_log(LOG_ERR, "snmp_init_getbulk_pdu: malloc failed");
		return -1;
	}

	memset(pstPDU->pstPara, 0, 2*ulOIDNum*sizeof(SNMP_TLV_T));

	pstPDU->pstOID = pstPDU->pstPara + ulOIDNum;
    
	for (i=0; i<ulOIDNum; i++)
	{
		pstVar = pstPDU->pstOID+i;
		pstVar->ucType = ASN_OBJECT_ID;
		if (snmp_create_oid_var((pstOID+i)->astOID, (pstOID+i)->ulOIDSize, &aucOID[0], &ucOIDLen) < 0)
		{
			nms_log(LOG_ERR, "snmp_init_getbulk_pdu: create oid var failed");
			return -1;
		}
		pstVar->ulLen = ucOIDLen;	
		DATA_DUP(pstVar, aucOID, ucOIDLen);
		SET_LEN_VAR(pstVar);

		/* para */
		pstVar = pstPDU->pstPara+i;
		pstVar->ucType = SNMP_ID_SEQUENCE;
		pstVar->ulLen = TLV_LEN(pstPDU->pstOID+i) + 
						sizeof(pstPDU->ucOIDValue)+
						sizeof(pstPDU->ucEndMark);
		SET_LEN_VAR(pstVar);

		ulParaLen += TLV_LEN(pstVar);
	}

	/* paras */
	pstVar = &pstPDU->stParas;
    pstVar->ucType = SNMP_ID_SEQUENCE;
    pstVar->ulLen = ulParaLen;
	SET_LEN_VAR(pstVar);

    /* error index */
	pstVar = &pstPDU->stErrIndex;
    pstVar->ucType = ASN_INTEGER;
    pstVar->ulLen = 1;
    DATA_DUP(pstVar, &ucErrorIndex, sizeof(ucErrorIndex));
	SET_LEN_VAR(pstVar);

    /* error status */
	pstVar = &pstPDU->stErrStatus;
    pstVar->ucType = ASN_INTEGER;
    pstVar->ulLen = 1;
    DATA_DUP(pstVar, &ucErrorStatus, sizeof(ucErrorStatus));
	SET_LEN_VAR(pstVar);

    /* req id */
	pstVar = &pstPDU->stReqID;
    pstVar->ucType = ASN_INTEGER;
    usReqIDValue = snmp_get_req_id();
    if (usReqIDValue <= 0xff)
    {
        aucReqID[0] = (unsigned char)usReqIDValue;
        pstVar->ulLen = 1;
    }
    else
    {
        aucReqID[0] = (unsigned char)usReqIDValue;
        aucReqID[1] = (unsigned char)(usReqIDValue >> 8);
		pstVar->ulLen = 2;
    }
    DATA_DUP(pstVar, aucReqID, pstVar->ulLen);
    SET_LEN_VAR(pstVar);
	//pstPDU->usReqID = usReqIDValue;

	/* snmp pdu */
	pstVar = &pstPDU->stSnmpPDU;
    pstVar->ucType = ucGetType; //SNMP_MSG_GET;
	pstVar->ulLen = TLV_LEN(&pstPDU->stReqID) +
					TLV_LEN(&pstPDU->stErrStatus) +
					TLV_LEN(&pstPDU->stErrIndex) +
					TLV_LEN(&pstPDU->stParas);
	SET_LEN_VAR(pstVar);

    //community
	pstVar = &pstPDU->stCommunity;
    pstVar->ucType = ASN_OCTET_STR;
    pstVar->ulLen = strlen(szCommunity);
    if (pstVar->ulLen > SNMP_MAX_COMMUNITY_LEN)
    {
        return -1;
    }
	pstVar->pucValue = (unsigned char*)szCommunity;
    pstVar->ucNeedFree = 0;
    SET_LEN_VAR(pstVar);

    //version
	pstVar = &pstPDU->stVer;
    pstVar->ucType = ASN_INTEGER;
    pstVar->ulLen  = 1;
    DATA_DUP(pstVar, &ucVersion, sizeof(ucVersion));
	SET_LEN_VAR(pstVar);

	//PDU
	pstVar = &pstPDU->stPDU;
    pstVar->ucType = SNMP_ID_SEQUENCE;
	pstVar->ulLen  = TLV_LEN(&pstPDU->stVer) +
					 TLV_LEN(&pstPDU->stCommunity) +
					 TLV_LEN(&pstPDU->stSnmpPDU);
	SET_LEN_VAR(pstVar);

    return 0;
}

/*************************************************************************
* 函数名称: snmp_create_get_pdu
* 功    能: 生成get操作报文
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_create_get_pdu(SNMP_GET_SCALAR_PDU_T *pstPDU, unsigned char *pucReqPDU, 
                        unsigned int ulMaxReqPDULen, unsigned int *pulReqPDULen)
{
	unsigned char		*pucValue = pucReqPDU;
    unsigned int        i;
    unsigned int        ulCurrPDULen = 0;

    /* 根据结构生成PDU */
	INIT_VAR(&pstPDU->stPDU);
	INIT_VAR(&pstPDU->stVer);
	INIT_VAR(&pstPDU->stCommunity);
	INIT_VAR(&pstPDU->stSnmpPDU);
	INIT_VAR(&pstPDU->stReqID);
	INIT_VAR(&pstPDU->stErrStatus);
	INIT_VAR(&pstPDU->stErrIndex);
	INIT_VAR(&pstPDU->stParas);
	INIT_VAR(&pstPDU->stPara1);
	INIT_VAR(&pstPDU->stOID);	
	
	*pucValue++ = pstPDU->ucOIDValue;
	*pucValue++ = pstPDU->ucEndMark;

    *pulReqPDULen = 1+TLV_LEN(&pstPDU->stPDU);

    return 0;
}

/*************************************************************************
* 函数名称: snmp_create_get_pdu
* 功    能: 生成get操作报文
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_create_getbulk_pdu(SNMP_GET_BULK_PDU_T *pstPDU, unsigned char *pucReqPDU, 
                            unsigned int ulMaxReqPDULen, unsigned int *pulReqPDULen)
{
	unsigned char		*pucValue = pucReqPDU;
    unsigned int        i;
    unsigned int        ulCurrPDULen = 0;

    /* 根据结构生成PDU */
	INIT_VAR(&pstPDU->stPDU);
	INIT_VAR(&pstPDU->stVer);
	INIT_VAR(&pstPDU->stCommunity);
	INIT_VAR(&pstPDU->stSnmpPDU);
	INIT_VAR(&pstPDU->stReqID);
	INIT_VAR(&pstPDU->stErrStatus);
	INIT_VAR(&pstPDU->stErrIndex);
	INIT_VAR(&pstPDU->stParas);
	
	for (i=0; i<pstPDU->ulOIDNum; i++)
	{
		INIT_VAR((pstPDU->pstPara+i));
		INIT_VAR((pstPDU->pstOID+i));

		*pucValue++ = pstPDU->ucOIDValue;
		*pucValue++ = pstPDU->ucEndMark;
	}
	
    *pulReqPDULen = 1+TLV_LEN(&pstPDU->stPDU);

    return 0;
}

/*************************************************************************
* 函数名称: snmp_create_set_pdu
* 功    能: 生成set操作报文
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_init_set_pdu(char *szCommunity, SNMP_OID *pOID, unsigned int ulOIDSize, 
					   unsigned char *pucReqData, unsigned char ucReqDataType, 
					   unsigned int ulReqDataLen,SNMP_SET_PDU_T *pstPDU)
{
	SNMP_TLV_T			*pstVar;
    unsigned short      usReqIDValue;
	unsigned char		ucOIDLen = 0;
	unsigned char		aucOID[64];
	unsigned char		aucReqID[2];
	unsigned char		ucErrorStatus = 0;
	unsigned char		ucErrorIndex  = 0;
	unsigned char		ucVersion     = SNMP_GET_VER;
    unsigned int        ulTmp;

    /* value */
	pstVar = &pstPDU->stValue;
    pstVar->ucType = ucReqDataType;
    pstVar->pucValue = pucReqData;
    pstVar->ulLen  = ulReqDataLen;
    pstVar->ucNeedFree = 0;
	if (ucReqDataType == ASN_GAUGE
		|| ucReqDataType == ASN_INTEGER)
	{
		// modify
		#if 0
		if( ulReqDataLen != 4)
		{
			return -1;
		}

        DATA_DUP(pstVar, pucReqData, 4);
        
        ulTmp = *((unsigned int*)pstVar->pucValue);
		if (ulTmp <= 0xff)
		{
			pstVar->pucValue[0] = (unsigned char)ulTmp;
			pstVar->ulLen = 1;
		}
		else if (ulTmp <= 0xffff)
		{
			pstVar->pucValue[0] = (unsigned char)(ulTmp>>8);
			pstVar->pucValue[1] = (unsigned char)(ulTmp);
			pstVar->ulLen = 2;
		}
		else if (ulTmp <= 0xffffff)
		{
			pstVar->pucValue[0] = (unsigned char)(ulTmp>>16);
			pstVar->pucValue[1] = (unsigned char)(ulTmp>>8);
			pstVar->pucValue[2] = (unsigned char)(ulTmp);
			pstVar->ulLen = 3;
		}
		else
		{
			pstVar->pucValue[0] = (unsigned char)(ulTmp>>24);
			pstVar->pucValue[1] = (unsigned char)(ulTmp>>16);
			pstVar->pucValue[2] = (unsigned char)(ulTmp>>8);
			pstVar->pucValue[3] = (unsigned char)(ulTmp);
			pstVar->ulLen = 4;
		}
		#endif

		DATA_DUP(pstVar, pucReqData, ulReqDataLen);
		if (ulReqDataLen == sizeof(UINT8))
		{
		}
		else if (ulReqDataLen == sizeof(UINT16))
		{
		}
		else if (ulReqDataLen == sizeof(UINT32))
		{
			ulTmp = *((unsigned int*)pstVar->pucValue);
			#if 0
            if (ulTmp <= 0xff)
			{
				pstVar->pucValue[0] = (unsigned char)ulTmp;
				pstVar->ulLen = 1;
			}
			else if (ulTmp <= 0xffff)
			{
				pstVar->pucValue[0] = (unsigned char)(ulTmp>>8);
				pstVar->pucValue[1] = (unsigned char)(ulTmp);
				pstVar->ulLen = 2;
			}
			else if (ulTmp <= 0xffffff)
			{
				pstVar->pucValue[0] = (unsigned char)(ulTmp>>16);
				pstVar->pucValue[1] = (unsigned char)(ulTmp>>8);
				pstVar->pucValue[2] = (unsigned char)(ulTmp);
				pstVar->ulLen = 3;
			}
			else
			{
				pstVar->pucValue[0] = (unsigned char)(ulTmp>>24);
				pstVar->pucValue[1] = (unsigned char)(ulTmp>>16);
				pstVar->pucValue[2] = (unsigned char)(ulTmp>>8);
				pstVar->pucValue[3] = (unsigned char)(ulTmp);
				pstVar->ulLen = 4;
			}
            #else
				pstVar->pucValue[0] = (unsigned char)(ulTmp>>24);
				pstVar->pucValue[1] = (unsigned char)(ulTmp>>16);
				pstVar->pucValue[2] = (unsigned char)(ulTmp>>8);
				pstVar->pucValue[3] = (unsigned char)(ulTmp);
				pstVar->ulLen = 4;
            #endif
		}
		
	}
	SET_LEN_VAR(pstVar);

    /* oid */
	pstVar = &pstPDU->stOID;
    pstVar->ucType = ASN_OBJECT_ID;
    if (snmp_create_oid_var(pOID, ulOIDSize, &aucOID[0], &ucOIDLen) < 0)
    {
        nms_log(LOG_ERR, "snmp_init_set_pdu: create oid var failed");
        return -1;
    }
	pstVar->ulLen = ucOIDLen;
	DATA_DUP(pstVar, aucOID, ucOIDLen);
	SET_LEN_VAR(pstVar);

	/* para1 */
	pstVar = &pstPDU->stPara1;
    pstVar->ucType = SNMP_ID_SEQUENCE;
    pstVar->ulLen = TLV_LEN(&pstPDU->stOID) + 
		            TLV_LEN(&pstPDU->stValue);
	SET_LEN_VAR(pstVar);

	/* paras */
	pstVar = &pstPDU->stParas;
    pstVar->ucType = SNMP_ID_SEQUENCE;
    pstVar->ulLen = TLV_LEN(&pstPDU->stPara1);
	SET_LEN_VAR(pstVar);

    /* error index */
	pstVar = &pstPDU->stErrIndex;
    pstVar->ucType = ASN_INTEGER;
    pstVar->ulLen = 1;
    DATA_DUP(pstVar, &ucErrorIndex, sizeof(ucErrorIndex));
	SET_LEN_VAR(pstVar);

    /* error status */
	pstVar = &pstPDU->stErrStatus;
    pstVar->ucType = ASN_INTEGER;
    pstVar->ulLen = 1;
    DATA_DUP(pstVar, &ucErrorStatus, sizeof(ucErrorStatus));
	SET_LEN_VAR(pstVar);

    /* req id */
	pstVar = &pstPDU->stReqID;
    pstVar->ucType = ASN_INTEGER;
    usReqIDValue = snmp_get_req_id();
    if (usReqIDValue <= 0xff)
    {
        aucReqID[0] = (unsigned char)usReqIDValue;
        pstVar->ulLen = 1;
    }
    else
    {
        aucReqID[0] = (unsigned char)usReqIDValue;
        aucReqID[1] = (unsigned char)(usReqIDValue >> 8);
		pstVar->ulLen = 2;
    }
    DATA_DUP(pstVar, aucReqID, pstVar->ulLen);
    SET_LEN_VAR(pstVar);
	//pstPDU->usReqID = usReqIDValue;

	/* snmp pdu */
	pstVar = &pstPDU->stSnmpPDU;
    pstVar->ucType = SNMP_MSG_SET;
	pstVar->ulLen = TLV_LEN(&pstPDU->stReqID) +
					TLV_LEN(&pstPDU->stErrStatus) +
					TLV_LEN(&pstPDU->stErrIndex) +
					TLV_LEN(&pstPDU->stParas);
	SET_LEN_VAR(pstVar);

    //community
	pstVar = &pstPDU->stCommunity;
    pstVar->ucType = ASN_OCTET_STR;
    pstVar->ulLen = strlen(szCommunity);
    if (pstVar->ulLen > SNMP_MAX_COMMUNITY_LEN)
    {
        return -1;
    }
	pstVar->pucValue = (unsigned char*)szCommunity;
    pstVar->ucNeedFree = 0;
    SET_LEN_VAR(pstVar);

    //version
	pstVar = &pstPDU->stVer;
    pstVar->ucType = ASN_INTEGER;
    pstVar->ulLen  = 1;
    DATA_DUP(pstVar, &ucVersion, sizeof(ucVersion));
	SET_LEN_VAR(pstVar);

	//PDU
	pstVar = &pstPDU->stPDU;
    pstVar->ucType = SNMP_ID_SEQUENCE;
	pstVar->ulLen  = TLV_LEN(&pstPDU->stVer) +
					 TLV_LEN(&pstPDU->stCommunity) +
					 TLV_LEN(&pstPDU->stSnmpPDU);
	SET_LEN_VAR(pstVar);

    return 0;
}

/*************************************************************************
* 函数名称: snmp_create_set_pdu
* 功    能: 生成set操作报文
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_create_set_pdu(SNMP_SET_PDU_T *pstPDU, unsigned char *pucReqPDU, 
                        unsigned int ulMaxReqPDULen, unsigned int *pulReqPDULen)
{
	unsigned char		*pucValue = pucReqPDU;
    unsigned int        i;
    unsigned int        ulCurrPDULen = 0;

    /* 根据结构生成PDU */
	INIT_VAR(&pstPDU->stPDU);
	INIT_VAR(&pstPDU->stVer);
	INIT_VAR(&pstPDU->stCommunity);
	INIT_VAR(&pstPDU->stSnmpPDU);
	INIT_VAR(&pstPDU->stReqID);
	INIT_VAR(&pstPDU->stErrStatus);
	INIT_VAR(&pstPDU->stErrIndex);
	INIT_VAR(&pstPDU->stParas);
	INIT_VAR(&pstPDU->stPara1);
	INIT_VAR(&pstPDU->stOID);
	INIT_VAR(&pstPDU->stValue);

    *pulReqPDULen = TLV_LEN(&pstPDU->stPDU);

    return 0;
}
#endif
/*************************************************************************
* 函数名称: snmp_create_trap2_pdu
* 功    能: 生成trap2操作报文
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_create_trap2_pdu(SNMP_TRAP2_PDU_T *pstPDU, unsigned char *pucReqPDU, 
                          unsigned int ulMaxReqPDULen, unsigned int *pulReqPDULen)
{
	unsigned char		*pucValue = pucReqPDU;
    unsigned int        i;
    unsigned int        ulCurrPDULen = 0;
    SNMP_VAR_T			*pstVar;

    /* 根据结构生成PDU */
	INIT_VAR(&pstPDU->stPDU);
	INIT_VAR(&pstPDU->stVer);
	INIT_VAR(&pstPDU->stCommunity);
	INIT_VAR(&pstPDU->stSnmpPDU);
	INIT_VAR(&pstPDU->stReqID);
	INIT_VAR(&pstPDU->stErrStatus);
	INIT_VAR(&pstPDU->stErrIndex);
	INIT_VAR(&pstPDU->stParas);

    INIT_VAR(&pstPDU->stTimestamp.stPara);
	INIT_VAR(&pstPDU->stTimestamp.stOID);
	INIT_VAR(&pstPDU->stTimestamp.stValue);

    for(pstVar=pstPDU->pstVarList; pstVar; pstVar=pstVar->pstNext)
    {
        INIT_VAR(&pstVar->stPara);
	    INIT_VAR(&pstVar->stOID);
	    INIT_VAR(&pstVar->stValue);
    }

    *pulReqPDULen = TLV_LEN(&pstPDU->stPDU);

    return 0;
}

/******************************************************************************
* 方法名称: snmp_get_uptime_10ms
* 功    能: 获取上电时间(单位: 10ms)
* 访问属性:
* 参    数:
* 参数名称            类型                  输入/输出        描述
* 函数返回:
* 说    明:
******************************************************************************/
unsigned int snmp_get_uptime_10ms(void)
{
#ifdef WIN32
    return GetTickCount()/10;
#else
    static int              iInit = 0;
    static struct timespec  stStartTime = {0};
    struct timespec         stCurrTime;
    int             sec;
    int             i10ms;      /* 毫秒(=1000微秒)*/
    int	            nsec;	    /* 纳秒*/

#if 1
    if (!iInit)
    {
        long ret = clock_gettime(CLOCK_REALTIME, &stStartTime);
        if (ret < 0)
        {
            if (EINVAL == ret)  printf("\n error EINVAL \n");
            if (EFAULT == ret)  printf("\n error EFAULT \n");

            return 0;
        }
        iInit = 1;
        return 0;
    }
#endif

    clock_gettime(CLOCK_REALTIME, &stCurrTime);
    sec  = stCurrTime.tv_sec - stStartTime.tv_sec;
    nsec = stCurrTime.tv_nsec - stStartTime.tv_nsec;

    i10ms = sec * 100;
    i10ms += (nsec / 10000000);

    return (unsigned int)i10ms;
#endif
}

/*****************************************************************************
 Prototype    : snmp_get_dev_uptime
 Description  : 获取设备上电时间
 Input        : void  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2013/4/19
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
UINT32 snmp_get_dev_uptime()
{
    struct sysinfo info     = {0};
    int iRet                = -1;

    iRet = sysinfo(&info);

    if (0 == iRet)
    {
        return info.uptime;
    }
    else
    {
        return 0;
    }
    
}/* end app_get_uptime */

/*************************************************************************
* 函数名称: snmp_init_trap2_pdu
* 功    能: 生成trap2操作报文
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_init_trap2_pdu(char *szCommunity, /*oid *pOID, unsigned int ulOIDSize, 
				   	    unsigned char *pucReqData, unsigned char ucReqDataType, 
					    unsigned int ulReqDataLen, */SNMP_TRAP2_PDU_T *pstPDU)
{
	SNMP_TLV_T			*pstTLV;
    unsigned short      usReqIDValue;
//	unsigned char		ucOIDLen = 0;
	unsigned char		aucReqID[2];
	unsigned char		ucErrorStatus = 0;
	unsigned char		ucErrorIndex  = 0;
	unsigned char		ucVersion     = SNMP_TRAP_VER;
    SNMP_VAR_T          *pstVar;
    unsigned int        ulParaLen = 0;
    SNMP_OID            astTimestampOID[] = {SNMP_TIMESTAMP_OID};
    /* BEGIN: Added by zoucaihong, 2013/4/19 */
    #if 0
    unsigned int        ulTimeStamp = snmp_get_uptime_10ms();
    #else
    unsigned int        ulTimeStamp = snmp_get_dev_uptime()*100;
    #endif
    /* END: Added by zoucaihong, 2013/4/19 */

    for(pstVar=pstPDU->pstVarList; pstVar; pstVar=pstVar->pstNext)
    {
        ulParaLen += TLV_LEN(&pstVar->stPara);
    }

    if (ulParaLen == 0)
    {
        return -1;
    }

    if (snmp_init_var(&pstPDU->stTimestamp, astTimestampOID, sizeof(astTimestampOID)/sizeof(SNMP_OID), 
                      ASN_TIMETICKS, (unsigned char*)&ulTimeStamp, sizeof(ulTimeStamp)) < 0)
    {
        nms_log(LOG_ERR, "snmp_init_trap2_pdu: snmp_init_var of timestamp failed");
        return -1;
    }
  
	/* paras */
	pstTLV = &pstPDU->stParas;
    pstTLV->ucType = SNMP_ID_SEQUENCE;
    pstTLV->ulLen = ulParaLen + TLV_LEN(&pstPDU->stTimestamp.stPara);
	SET_LEN_VAR(pstTLV);

    /* error index */
	pstTLV = &pstPDU->stErrIndex;
    pstTLV->ucType = ASN_INTEGER;
    pstTLV->ulLen = 1;
    DATA_DUP(pstTLV, &ucErrorIndex, sizeof(ucErrorIndex));
	SET_LEN_VAR(pstTLV);

    /* error status */
	pstTLV = &pstPDU->stErrStatus;
    pstTLV->ucType = ASN_INTEGER;
    pstTLV->ulLen = 1;
    DATA_DUP(pstTLV, &ucErrorStatus, sizeof(ucErrorStatus));
	SET_LEN_VAR(pstTLV);

    /* req id */
	pstTLV = &pstPDU->stReqID;
    pstTLV->ucType = ASN_INTEGER;
    usReqIDValue = snmp_get_req_id();
    if (usReqIDValue <= 0xff)
    {
        aucReqID[0] = (unsigned char)usReqIDValue;
        pstTLV->ulLen = 1;
    }
    else
    {
        aucReqID[0] = (unsigned char)usReqIDValue;
        aucReqID[1] = (unsigned char)(usReqIDValue >> 8);
		pstTLV->ulLen = 2;
    }
    DATA_DUP(pstTLV, aucReqID, pstTLV->ulLen);
    SET_LEN_VAR(pstTLV);
	//pstPDU->usReqID = usReqIDValue;

	/* snmp pdu */
	pstTLV = &pstPDU->stSnmpPDU;
    pstTLV->ucType = SNMP_MSG_TRAP2;//SNMP_MSG_INFORM;
	pstTLV->ulLen = TLV_LEN(&pstPDU->stReqID) +
					TLV_LEN(&pstPDU->stErrStatus) +
					TLV_LEN(&pstPDU->stErrIndex) +
					TLV_LEN(&pstPDU->stParas);
	SET_LEN_VAR(pstTLV);

    //community
	pstTLV = &pstPDU->stCommunity;
    pstTLV->ucType = ASN_OCTET_STR;
    pstTLV->ulLen = strlen(szCommunity);
    if (pstTLV->ulLen > SNMP_MAX_COMMUNITY_LEN)
    {
        return -1;
    }
	pstTLV->pucValue = (unsigned char*)szCommunity;
    pstTLV->ucNeedFree = 0;
    SET_LEN_VAR(pstTLV);

    //version
	pstTLV = &pstPDU->stVer;
    pstTLV->ucType = ASN_INTEGER;
    pstTLV->ulLen  = 1;
    DATA_DUP(pstTLV, &ucVersion, sizeof(ucVersion));
	SET_LEN_VAR(pstTLV);

	//PDU
	pstTLV = &pstPDU->stPDU;
    pstTLV->ucType = SNMP_ID_SEQUENCE;
	pstTLV->ulLen  = TLV_LEN(&pstPDU->stVer) +
					 TLV_LEN(&pstPDU->stCommunity) +
					 TLV_LEN(&pstPDU->stSnmpPDU);
	SET_LEN_VAR(pstTLV);

    return 0;
}
/*****************************************************************************
 Prototype    : snmp_init_inform_pdu
 Description  : 生成inform类型trap操作报文
 Input        : char *szCommunity         
                SNMP_TRAP2_PDU_T *pstPDU  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/8/25
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
int snmp_init_inform_pdu(char *szCommunity, /*oid *pOID, unsigned int ulOIDSize, 
				   	    unsigned char *pucReqData, unsigned char ucReqDataType, 
					    unsigned int ulReqDataLen, */SNMP_TRAP2_PDU_T *pstPDU)
{
	SNMP_TLV_T			*pstTLV;
    unsigned short      usReqIDValue;
//	unsigned char		ucOIDLen = 0;
	unsigned char		aucReqID[2];
	unsigned char		ucErrorStatus = 0;
	unsigned char		ucErrorIndex  = 0;
	unsigned char		ucVersion     = SNMP_TRAP_VER;
    SNMP_VAR_T          *pstVar;
    unsigned int        ulParaLen = 0;
    SNMP_OID            astTimestampOID[] = {SNMP_TIMESTAMP_OID};
    /* BEGIN: Added by zoucaihong, 2013/4/19 */
    #if 0
    unsigned int        ulTimeStamp = snmp_get_uptime_10ms();
    #else
    unsigned int        ulTimeStamp = snmp_get_dev_uptime()*100;
    #endif
    /* END: Added by zoucaihong, 2013/4/19 */

    for(pstVar=pstPDU->pstVarList; pstVar; pstVar=pstVar->pstNext)
    {
        ulParaLen += TLV_LEN(&pstVar->stPara);
    }

    if (ulParaLen == 0)
    {
        return -1;
    }

    if (snmp_init_var(&pstPDU->stTimestamp, astTimestampOID, sizeof(astTimestampOID)/sizeof(SNMP_OID), 
                      ASN_TIMETICKS, (unsigned char*)&ulTimeStamp, sizeof(ulTimeStamp)) < 0)
    {
        nms_log(LOG_ERR, "snmp_init_trap2_pdu: snmp_init_var of timestamp failed");
        return -1;
    }
  
	/* paras */
	pstTLV = &pstPDU->stParas;
    pstTLV->ucType = SNMP_ID_SEQUENCE;
    pstTLV->ulLen = ulParaLen + TLV_LEN(&pstPDU->stTimestamp.stPara);
	SET_LEN_VAR(pstTLV);

    /* error index */
	pstTLV = &pstPDU->stErrIndex;
    pstTLV->ucType = ASN_INTEGER;
    pstTLV->ulLen = 1;
    DATA_DUP(pstTLV, &ucErrorIndex, sizeof(ucErrorIndex));
	SET_LEN_VAR(pstTLV);

    /* error status */
	pstTLV = &pstPDU->stErrStatus;
    pstTLV->ucType = ASN_INTEGER;
    pstTLV->ulLen = 1;
    DATA_DUP(pstTLV, &ucErrorStatus, sizeof(ucErrorStatus));
	SET_LEN_VAR(pstTLV);

    /* req id */
	pstTLV = &pstPDU->stReqID;
    pstTLV->ucType = ASN_INTEGER;
    usReqIDValue = snmp_get_req_id();
    if (usReqIDValue <= 0xff)
    {
        aucReqID[0] = (unsigned char)usReqIDValue;
        pstTLV->ulLen = 1;
    }
    else
    {
        aucReqID[0] = (unsigned char)usReqIDValue;
        aucReqID[1] = (unsigned char)(usReqIDValue >> 8);
		pstTLV->ulLen = 2;
    }
    DATA_DUP(pstTLV, aucReqID, pstTLV->ulLen);
    SET_LEN_VAR(pstTLV);
	//pstPDU->usReqID = usReqIDValue;

	/* snmp pdu */
	pstTLV = &pstPDU->stSnmpPDU;
    pstTLV->ucType = SNMP_MSG_INFORM;
	pstTLV->ulLen = TLV_LEN(&pstPDU->stReqID) +
					TLV_LEN(&pstPDU->stErrStatus) +
					TLV_LEN(&pstPDU->stErrIndex) +
					TLV_LEN(&pstPDU->stParas);
	SET_LEN_VAR(pstTLV);

    //community
	pstTLV = &pstPDU->stCommunity;
    pstTLV->ucType = ASN_OCTET_STR;
    pstTLV->ulLen = strlen(szCommunity);
    if (pstTLV->ulLen > SNMP_MAX_COMMUNITY_LEN)
    {
        return -1;
    }
	pstTLV->pucValue = (unsigned char*)szCommunity;
    pstTLV->ucNeedFree = 0;
    SET_LEN_VAR(pstTLV);

    //version
	pstTLV = &pstPDU->stVer;
    pstTLV->ucType = ASN_INTEGER;
    pstTLV->ulLen  = 1;
    DATA_DUP(pstTLV, &ucVersion, sizeof(ucVersion));
	SET_LEN_VAR(pstTLV);

	//PDU
	pstTLV = &pstPDU->stPDU;
    pstTLV->ucType = SNMP_ID_SEQUENCE;
	pstTLV->ulLen  = TLV_LEN(&pstPDU->stVer) +
					 TLV_LEN(&pstPDU->stCommunity) +
					 TLV_LEN(&pstPDU->stSnmpPDU);
	SET_LEN_VAR(pstTLV);

    return 0;
}
/*************************************************************************
* 函数名称: snmp_free_varlist
* 功    能: 释放varlist
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
void snmp_free_varlist(SNMP_VAR_T *pstVarList)
{
    SNMP_VAR_T  *pstVar = pstVarList;
    SNMP_VAR_T  *pstNextVar;

    while(pstVar)
    {
        pstNextVar = pstVar->pstNext;

        snmp_free_tlv(&pstVar->stPara);
        snmp_free_tlv(&pstVar->stOID);
        snmp_free_tlv(&pstVar->stValue);
        free(pstVar);
        pstVar = NULL;

        pstVar = pstNextVar;
    }
}


/*************************************************************************
* 函数名称: snmp_free_trap2_pdu
* 功    能: 释放PDU中的动态申请的内存
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_free_trap2_pdu(SNMP_TRAP2_PDU_T *pstPDU)
{
	FREE_TLV(&pstPDU->stPDU);
	FREE_TLV(&pstPDU->stVer);
	FREE_TLV(&pstPDU->stCommunity);
	FREE_TLV(&pstPDU->stSnmpPDU);
	FREE_TLV(&pstPDU->stReqID);
	FREE_TLV(&pstPDU->stErrStatus);
	FREE_TLV(&pstPDU->stErrIndex);
	FREE_TLV(&pstPDU->stParas);

    FREE_TLV(&pstPDU->stTimestamp.stPara);
    FREE_TLV(&pstPDU->stTimestamp.stOID);
    FREE_TLV(&pstPDU->stTimestamp.stValue);

    snmp_free_varlist(pstPDU->pstVarList);

    return 0;
}

/*************************************************************************
* 函数名称: snmp_init_trap2_pdu
* 功    能: 生成trap2操作报文
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_varlist_add_var(SNMP_VAR_T **pstVarList, SNMP_OID *pOID, unsigned int ulOIDSize, 
				   	     unsigned char ucReqDataType, unsigned char *pucReqData, 
					     unsigned int ulReqDataLen)
{
    SNMP_VAR_T  *pstVar;

    if (pstVarList == NULL)
    {
        nms_log(LOG_ERR, "snmp_varlist_add_var: pstVarList is null");
        return -1;
    }

    if (*pstVarList == NULL)
    {
        *pstVarList = (SNMP_VAR_T*)malloc(sizeof(SNMP_VAR_T));
        if ((*pstVarList) == NULL)
        {
            nms_log(LOG_ERR, "snmp_varlist_add_var: alloc pstVarList failed");
            goto fail;
        }

        pstVar = *pstVarList;
        memset(pstVar, 0, sizeof(SNMP_VAR_T));
    }
    else
    {
        for (pstVar=*pstVarList; pstVar->pstNext; pstVar=pstVar->pstNext);

        pstVar->pstNext = (SNMP_VAR_T*)malloc(sizeof(SNMP_VAR_T));
        if (pstVar->pstNext == NULL)
        {
            nms_log(LOG_ERR, "snmp_varlist_add_var: alloc pstVar failed");
            goto fail;
        }

        pstVar = pstVar->pstNext;
        memset(pstVar, 0, sizeof(SNMP_VAR_T));
    }

    if (snmp_init_var(pstVar, pOID, ulOIDSize, ucReqDataType, pucReqData, ulReqDataLen) < 0)
    {
        goto fail;
    }

    return 0;

fail:
    snmp_free_varlist(*pstVarList);

    return -1;
}

/*************************************************************************
* 函数名称: snmp_parse_get_rsp
* 功    能: 解析get应答报文
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
#if 0
int snmp_parse_get_rsp1(SNMP_GET_PDU_T *pstPDU, unsigned char *pucRspData, 
                       unsigned int ulRspDataLen, unsigned char *pucValue,
                       unsigned int ulMaxValueLen, unsigned int *pulValueLen)
{
    unsigned int    ulPDULen;
    unsigned int    ulSnmpPDULen;
    unsigned int    ulParasLen;
    unsigned int    ulPara1Len;
    unsigned int    ulDataLen;
    unsigned short  usReqID;
    unsigned char   ucOIDSize;
    unsigned char   ucDataType;
    unsigned char   ucTmp;
    unsigned int    *pulTmp;
    unsigned int    i;

    if ((*pucRspData++) != SNMP_ID_SEQUENCE)
    {
        return -1;
    }

    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulPDULen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;

        if (ulRspDataLen != (1+1+ucTmp+ulPDULen))
        {
            return -1;
        }
    }
    else
    {
        ulPDULen = pucRspData[0];
        pucRspData ++;

        if (ulRspDataLen != (1+1+ulPDULen))
        {
            return -1;
        }
    }    

    //ver check
    if (pucRspData[0] != ASN_INTEGER ||
        pucRspData[1] != 1 ||
        pucRspData[2] != SNMP_GET_VER)
    {
        return -1;
    }
    pucRspData += (1+1+1);

    //community
    if (pucRspData[0] != ASN_OCTET_STR ||
        pucRspData[1] == 0)
    {
        return -1;
    }
    pucRspData += (1+1+pucRspData[1]);

    //snmp pdu type : get response 
    if ((*pucRspData++) != SNMP_MSG_RESPONSE)
    {
        return -1;
    }

    //snmp pdu len
    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulSnmpPDULen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;
    }
    else
    {
        ulSnmpPDULen = pucRspData[0];
        pucRspData ++;
    }

    //req id
    if (pucRspData[0] != ASN_INTEGER ||
        pucRspData[1] == 0)
    {
        return -1;
    }

    ucTmp = pucRspData[1];
    pucRspData += 2;
	usReqID = snmp_get_int_value(pucRspData, ucTmp);
	if (usReqID != pstPDU->usReqID)
	{
		return -1;
	}
    pucRspData += ucTmp;

    //error status
    if (pucRspData[0] != ASN_INTEGER ||
        pucRspData[1] != 1)
    {
        return -1;
    }

    pucRspData += 2;

    if ((*pucRspData++) != SNMP_ERR_NOERROR)
    {
        return -2;
    }

    //error index
    pucRspData += (1+1+1);

    //paras
    if ((*pucRspData++) != SNMP_ID_SEQUENCE)
    {
        return -1;
    }

    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulParasLen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;
    }
    else
    {
        ulParasLen = pucRspData[0];
        pucRspData ++;
    }

    //para1
    if ((*pucRspData++) != SNMP_ID_SEQUENCE)
    {
        return -1;
    }

    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulPara1Len = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;
    }
    else
    {
        ulPara1Len = pucRspData[0];
        pucRspData ++;
    }

    //oid type
    if (pucRspData[0] != ASN_OBJECT_ID ||
        pucRspData[1] == 0)
    {
        return -1;
    }
    
    ucOIDSize = pucRspData[1];
    pucRspData += 2;

    //check oid
    pucRspData += ucOIDSize;

    //octet string type
    ucDataType = *pucRspData++;
    //if (ucDataType != ASN_OCTET_STR)
    {
        //return -1;
    }

    //octet string len
    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulDataLen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;
    }
    else
    {
        ulDataLen = pucRspData[0];
        pucRspData ++;
    }

    //check len
    if (ulDataLen > ulMaxValueLen)
    {
        return -1;
    }

    *pulValueLen = ulDataLen;
	if (ucDataType == ASN_GAUGE)
	{
		pulTmp = (unsigned int*)pucValue;
		pulTmp[0] = 0;
		for (i=0; i<ulDataLen; i++)
		{
			pulTmp[0] = (pulTmp[0]<<8) + pucRspData[i];
		}		
	}
	else
    {
        memcpy(pucValue, pucRspData, ulDataLen);
	}

    return 0;
}
#endif
#if 0
int snmp_parse_get_rsp(SNMP_GET_SCALAR_PDU_T *pstPDU, unsigned char *pucRspData, 
                       unsigned int ulRspDataLen, unsigned char *pucValue,
                       unsigned int ulMaxValueLen, unsigned int *pulValueLen)
{
    unsigned int    ulPDULen;
    unsigned int    ulSnmpPDULen;
    unsigned int    ulParasLen;
    unsigned int    ulPara1Len;
    unsigned int    ulDataLen;
    unsigned char   ucOIDSize;
    unsigned char   ucDataType;
    unsigned char   ucTmp;
    unsigned int    *pulTmp;
    unsigned int    i;

	unsigned char   *pucRsp=NULL;
	pucRsp = pucRspData;
	
    if ((*pucRspData++) != SNMP_ID_SEQUENCE)
    {
        return -1;
    }

    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulPDULen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;

        if (ulRspDataLen != (1+1+ucTmp+ulPDULen))
        {
            return -1;
        }
    }
    else
    {
        ulPDULen = pucRspData[0];
        pucRspData ++;

        if (ulRspDataLen != (1+1+ulPDULen))
        {
            return -1;
        }
    }    

    //ver check
    if (pucRspData[0] != ASN_INTEGER ||
        pucRspData[1] != 1 ||
        pucRspData[2] != SNMP_GET_VER)
    {
        return -1;
    }
    pucRspData += (1+1+1);

    //community
    if (pucRspData[0] != ASN_OCTET_STR ||
        pucRspData[1] == 0)
    {
        return -1;
    }
    pucRspData += (1+1+pucRspData[1]);

    //snmp pdu type : get response 
    if ((*pucRspData++) != SNMP_MSG_RESPONSE)
    {
        return -1;
    }

    //snmp pdu len
    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulSnmpPDULen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;
    }
    else
    {
        ulSnmpPDULen = pucRspData[0];
        pucRspData ++;
    }

    //req id
    if (pucRspData[0] != ASN_INTEGER ||
        pucRspData[1] == 0)
    {
    	nms_log(LOG_ERR, "snmp_parse_get_rsp: pucRspData[0]:%u,pucRspData[1]:%u.",
			pucRspData[0],
			pucRspData[1]);
		nms_log(LOG_ERR, "snmp_parse_get_rsp: check reqid failed");
        return -1;
    }

    ucTmp = pucRspData[1];
    pucRspData += 2;

	CHECK_REQ_ID(pstPDU->stReqID, pucRspData, ucTmp)
	{
		/*nms_log(LOG_ERR, "snmp_parse_get_rsp: pucRspData:%02x %02x,pstPDU->stReqID.pucValue:%02x %02x.",
			pucRspData[0], pucRspData[1],
			pstPDU->stReqID.pucValue[0],
			pstPDU->stReqID.pucValue[1]);*/
		
		// add print
		nms_print_binary(pucRsp, ulRspDataLen);
		//nms_print_table_pdu(pstPDU);
		
		nms_log(LOG_ERR, "snmp_parse_get_rsp: check reqid failed");
		return -1;
	}

	pucRspData += ucTmp;
    
	//error status
    if (pucRspData[0] != ASN_INTEGER ||
        pucRspData[1] != 1)
    {
        return -1;
    }

    pucRspData += 2;

    if ((*pucRspData++) != SNMP_ERR_NOERROR)
    {
        return -2;
    }

    //error index
    pucRspData += (1+1+1);

    //paras
    if ((*pucRspData++) != SNMP_ID_SEQUENCE)
    {
        return -1;
    }

    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulParasLen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;
    }
    else
    {
        ulParasLen = pucRspData[0];
        pucRspData ++;
    }

    //para1
    if ((*pucRspData++) != SNMP_ID_SEQUENCE)
    {
        return -1;
    }

    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulPara1Len = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;
    }
    else
    {
        ulPara1Len = pucRspData[0];
        pucRspData ++;
    }

    //oid type
    if (pucRspData[0] != ASN_OBJECT_ID ||
        pucRspData[1] == 0)
    {
        return -1;
    }
    
    ucOIDSize = pucRspData[1];
    pucRspData += 2;

    //check oid
    pucRspData += ucOIDSize;

    //octet string type
    ucDataType = *pucRspData++;
    //if (ucDataType != ASN_OCTET_STR)
    {
        //return -1;
    }

    //octet string len
    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulDataLen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;
    }
    else
    {
        ulDataLen = pucRspData[0];
        pucRspData ++;
    }

    //check len
    if (ulDataLen > ulMaxValueLen)
    {
        if (ucDataType == ASN_GAUGE && ulDataLen == (ulMaxValueLen+1))
        {
            pucRspData ++;
            ulDataLen --;
        }
        else
        {
            return -1;
        }
    }

    *pulValueLen = ulDataLen;
	
	if (ucDataType == ASN_GAUGE ||
	    ucDataType == ASN_TIMETICKS ||
	    ucDataType == ASN_INTEGER ||
	    ucDataType == ASN_COUNTER ||
	    ucDataType == ASN_COUNTER64)
	{
		if (ulDataLen>4)
		{
			for(i=0;i<ulDataLen;i++)
			{
				memcpy(pucValue+ulDataLen-(i+1), pucRspData+i, 1);
			}
		}
		else
		{
			pulTmp = (unsigned int*)pucValue;
			pulTmp[0] = 0;
			for (i=0; i<ulDataLen; i++)
			{
				pulTmp[0] = (pulTmp[0]<<8) + pucRspData[i];
			}
		}
	}
	else
       {
        memcpy(pucValue, pucRspData, ulDataLen);
	}

    return 0;
}

int snmp_parse_get_next_rsp(SNMP_GET_TABLE_PDU_T *pstPDU, unsigned char *pucRspData, 
						    unsigned int ulRspDataLen, SNMP_OID *pNextOID, unsigned int *pulNextOIDSize, 
							SNMP_VALUE_T *pstValue)
{
    unsigned int    ulPDULen;
    unsigned int    ulSnmpPDULen;
    unsigned int    ulParasLen;
    unsigned int    ulPara1Len;
    unsigned int    ulDataLen;
    unsigned char   ucOIDSize;
    unsigned char   ucDataType;
    unsigned char   ucTmp;
    unsigned int    *pulTmp;
    unsigned int    i;

	unsigned char   *pucRsp=NULL;
	pucRsp = pucRspData;
	
    if ((*pucRspData++) != SNMP_ID_SEQUENCE)
    {
        return -1;
    }

    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulPDULen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;

        if (ulRspDataLen != (1+1+ucTmp+ulPDULen))
        {
            return -1;
        }
    }
    else
    {
        ulPDULen = pucRspData[0];
        pucRspData ++;

        if (ulRspDataLen != (1+1+ulPDULen))
        {
            return -1;
        }
    }    

    //ver check
    if (pucRspData[0] != ASN_INTEGER ||
        pucRspData[1] != 1 ||
        pucRspData[2] != SNMP_GET_VER)
    {
        return -1;
    }
    pucRspData += (1+1+1);

    //community
    if (pucRspData[0] != ASN_OCTET_STR ||
        pucRspData[1] == 0)
    {
        return -1;
    }
    pucRspData += (1+1+pucRspData[1]);

    //snmp pdu type : get response 
    if ((*pucRspData++) != SNMP_MSG_RESPONSE)
    {
        return -1;
    }

    //snmp pdu len
    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulSnmpPDULen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;
    }
    else
    {
        ulSnmpPDULen = pucRspData[0];
        pucRspData ++;
    }

    //req id
    if (pucRspData[0] != ASN_INTEGER ||
        pucRspData[1] == 0)
    {
		nms_log(LOG_ERR, "snmp_parse_get_next_rsp: pucRspData[0]:%u,pucRspData[1]:%u.",
			pucRspData[0],
			pucRspData[1]);
		nms_log(LOG_ERR, "snmp_parse_get_next_rsp: check reqid failed");
        return -1;
    }

    ucTmp = pucRspData[1];
    pucRspData += 2;
	
       CHECK_REQ_ID(pstPDU->stReqID, pucRspData, ucTmp)
	{
        	/*nms_log(LOG_ERR, "snmp_parse_get_next_rsp: pucRspData:%02x %02x,pstPDU->stReqID.pucValue:%02x %02x.",
			pucRspData[0], pucRspData[1],
			pstPDU->stReqID.pucValue[0],
			pstPDU->stReqID.pucValue[1]);*/
		
		// add print
		nms_print_binary(pucRsp, ulRspDataLen);
		//nms_print_table_pdu(pstPDU);
		
		nms_log(LOG_ERR, "snmp_parse_get_next_rsp: check reqid failed");
		return -1;
	}
	pucRspData += ucTmp;
    
	//error status
    if (pucRspData[0] != ASN_INTEGER ||
        pucRspData[1] != 1)
    {
        return -1;
    }

    pucRspData += 2;

    if ((*pucRspData++) != SNMP_ERR_NOERROR)
    {
        return -2;
    }

    //error index
    pucRspData += (1+1+1);

    //paras
    if ((*pucRspData++) != SNMP_ID_SEQUENCE)
    {
        return -1;
    }

    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulParasLen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;
    }
    else
    {
        ulParasLen = pucRspData[0];
        pucRspData ++;
    }

    //para1
    if ((*pucRspData++) != SNMP_ID_SEQUENCE)
    {
        return -1;
    }

    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulPara1Len = snmp_get_int_value(pucRspData, ucTmp);	
        pucRspData += ucTmp;
    }
    else
    {
        ulPara1Len = pucRspData[0];
        pucRspData ++;
    }

    //oid type
    if (pucRspData[0] != ASN_OBJECT_ID ||
        pucRspData[1] == 0)
    {
        return -1;
    }
    
    ucOIDSize = pucRspData[1];
    pucRspData += 2;

    //check oid
	if (snmp_parse_oid_var(pucRspData, ucOIDSize, pNextOID, pulNextOIDSize) < 0)
	{
		return -1;
	}
    pucRspData += ucOIDSize;

    //value type
    ucDataType = *pucRspData++;
    //if (ucDataType != ASN_OCTET_STR)
    {
        //return -1;
    }

    //octet string len
    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulDataLen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;
    }
    else
    {
        ulDataLen = pucRspData[0];
        pucRspData ++;
    }

	pstValue->ucType = ucDataType;
	pstValue->ulLen  = ulDataLen;
	if (ulDataLen == 0)
	{
		pstValue->pucValue = NULL;
	}
	else
	{
		if (ucDataType == ASN_GAUGE ||
	        ucDataType == ASN_TIMETICKS ||
	        ucDataType == ASN_INTEGER   ||
	        ucDataType == ASN_COUNTER   ||
	        ucDataType == ASN_COUNTER64 )
		{
			if (ulDataLen>4)  
			{
				pstValue->pucValue = (UINT8*)malloc(ulDataLen);
				if (pstValue->pucValue == NULL)
				{
					return -1;
				}
				for(i=0;i<ulDataLen;i++)                                  
    			{                                                         
    				memcpy(pstValue->pucValue +ulDataLen-(i+1), pucRspData+i, 1);
    			}   
			}
			else
			{
				pstValue->pucValue = (UINT8*)malloc(sizeof(unsigned int));
				if (pstValue->pucValue == NULL)
				{
					return -1;
				}

				if (ulDataLen == sizeof(unsigned int)+1)
				{
				    pucRspData ++;
				    pstValue->ulLen --;
				}
				
				pulTmp = (unsigned int*)pstValue->pucValue;
				pulTmp[0] = 0;
				for (i=0; i<pstValue->ulLen; i++)
				{
					pulTmp[0] = (pulTmp[0]<<8) + pucRspData[i];
				}
			}
		}
		else
		{	
			pstValue->pucValue = (UINT8*)malloc(ulDataLen);
			if (pstValue->pucValue == NULL)
			{
				return -1;
			}
			memcpy(pstValue->pucValue, pucRspData, ulDataLen);
		}
	}

    return 0;
}

int snmp_parse_get_table_rsp(SNMP_GET_BULK_PDU_T *pstPDU, unsigned char *pucRspData, 
						    unsigned int ulRspDataLen, 
							SNMP_TABLE_VALUE_T *pstTableValue)
{
    unsigned int    ulPDULen;
    unsigned int    ulSnmpPDULen;
    unsigned int    ulParasLen;
    unsigned int    ulPara1Len;
    unsigned int    ulDataLen;
    unsigned char   ucOIDSize;
    unsigned char   ucDataType;
    unsigned char   ucTmp;
    unsigned int    *pulTmp;
	unsigned char   *pucOriRspData = pucRspData;
    unsigned int    i;
	SNMP_OID_T		*pstOID;
	SNMP_VALUE_T	*pstValue;

	unsigned char   *pucRsp=NULL;
	pucRsp = pucRspData;

    if ((*pucRspData++) != SNMP_ID_SEQUENCE)
    {
        return -1;
    }

    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulPDULen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;

        if (ulRspDataLen != (1+1+ucTmp+ulPDULen))
        {
            return -1;
        }
    }
    else
    {
        ulPDULen = pucRspData[0];
        pucRspData ++;

        if (ulRspDataLen != (1+1+ulPDULen))
        {
            return -1;
        }
    }    

    //ver check
    if (pucRspData[0] != ASN_INTEGER ||
        pucRspData[1] != 1 ||
        pucRspData[2] != SNMP_GET_VER)
    {
        return -1;
    }
    pucRspData += (1+1+1);

    //community
    if (pucRspData[0] != ASN_OCTET_STR ||
        pucRspData[1] == 0)
    {
        return -1;
    }
    pucRspData += (1+1+pucRspData[1]);

    //snmp pdu type : get response 
    if ((*pucRspData++) != SNMP_MSG_RESPONSE)
    {
        return -1;
    }

    //snmp pdu len
    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulSnmpPDULen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;
    }
    else
    {
        ulSnmpPDULen = pucRspData[0];
        pucRspData ++;
    }

    //req id
    if (pucRspData[0] != ASN_INTEGER ||
        pucRspData[1] == 0)
    {
		nms_log(LOG_ERR, "snmp_parse_get_table_rsp: pucRspData[0]:%u,pucRspData[1]:%u.",
			pucRspData[0],
			pucRspData[1]);
		nms_log(LOG_ERR, "snmp_parse_get_table_rsp: check reqid failed");
        return -1;
    }

    ucTmp = pucRspData[1];
    pucRspData += 2;
	
    CHECK_REQ_ID(pstPDU->stReqID, pucRspData)
	{
        nms_log(LOG_ERR, "snmp_parse_get_table_rsp: pucRspData:%02x %02x,pstPDU->stReqID.pucValue:%02x %02x.",
			pucRspData[0], pucRspData[1],
			pstPDU->stReqID.pucValue[0],
			pstPDU->stReqID.pucValue[1]);
		
        nms_log(LOG_ERR, "snmp_parse_get_table_rsp: check reqid failed");
		return -1;
	}
	pucRspData += ucTmp;
    
	//error status
    if (pucRspData[0] != ASN_INTEGER ||
        pucRspData[1] != 1)
    {
        return -1;
    }

    pucRspData += 2;

    if ((*pucRspData++) != SNMP_ERR_NOERROR)
    {
        return -2;
    }

    //error index
    pucRspData += (1+1+1);

    //paras
    if ((*pucRspData++) != SNMP_ID_SEQUENCE)
    {
        return -1;
    }

    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulParasLen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;
    }
    else
    {
        ulParasLen = pucRspData[0];
        pucRspData ++;
    }

   	while(pucRspData < pucOriRspData+ulRspDataLen)
	{
		if (pstTableValue->pstOID == NULL)
		{
			pstTableValue->pstOID   = (SNMP_OID_T*)malloc(sizeof(SNMP_OID_T));
			pstTableValue->pstValue = (SNMP_VALUE_T*)malloc(sizeof(SNMP_VALUE_T));

			pstOID   = pstTableValue->pstOID;
			pstValue = pstTableValue->pstValue;
		}
		else
		{
			pstOID->pstNext = (SNMP_OID_T*)malloc(sizeof(SNMP_OID_T));
			pstValue->pstNext = (SNMP_VALUE_T*)malloc(sizeof(SNMP_VALUE_T));

			pstOID   = pstOID->pstNext;
			pstValue = pstValue->pstNext;
		}

		if (pstOID == NULL || pstValue == NULL)
		{
			if (pstOID)
			{
				free(pstOID);
			}

			if (pstValue)
			{
				free(pstValue);
			}

			return -1;
		}

		memset(pstOID, 0, sizeof(SNMP_OID_T));
		memset(pstValue, 0, sizeof(SNMP_VALUE_T));
		pstTableValue->ulColumnNum ++;

		//para
		if ((*pucRspData++) != SNMP_ID_SEQUENCE)
		{
			return -1;
		}

		if (pucRspData[0] > 0x80)
		{
			ucTmp = pucRspData[0] - 0x80;
			pucRspData ++;

			ulPara1Len = snmp_get_int_value(pucRspData, ucTmp);
			pucRspData += ucTmp;
		}
		else
		{
			ulPara1Len = pucRspData[0];
			pucRspData ++;
		}

		//oid type
		if (pucRspData[0] != ASN_OBJECT_ID ||
			pucRspData[1] == 0)
		{
			return -1;
		}
    
		ucOIDSize = pucRspData[1];
		pucRspData += 2;

		//check oid
		if (snmp_parse_oid_var(pucRspData, ucOIDSize, pstOID->astOID, &pstOID->ulOIDSize) < 0)
		{
			return -1;
		}
		pucRspData += ucOIDSize;

		//value type
		ucDataType = *pucRspData++;
		//if (ucDataType != ASN_OCTET_STR)
		{
			//return -1;
		}

		//value len
		if (pucRspData[0] > 0x80)
		{
			ucTmp = pucRspData[0] - 0x80;
			pucRspData ++;

			ulDataLen = snmp_get_int_value(pucRspData, ucTmp);
			pucRspData += ucTmp;
		}
		else
		{
			ulDataLen = pucRspData[0];
			pucRspData ++;
		}

		pstValue->ucType = ucDataType;
		pstValue->ulLen  = ulDataLen;
		if (ulDataLen == 0)
		{
			pstValue->pucValue = NULL;
		}
		else
		{
			if (ucDataType == ASN_GAUGE)
			{
                if (pstValue->ulLen == sizeof(unsigned int)+1)
                {
                    pucRspData ++;
                    pstValue->ulLen --;                    
                }
				pstValue->pucValue = (UINT8*)malloc(sizeof(unsigned int));
				if (pstValue->pucValue == NULL)
				{
					return -1;
				}

				pulTmp = (unsigned int*)pstValue->pucValue;
				pulTmp[0] = 0;
				for (i=0; i<pstValue->ulLen; i++)
				{
					pulTmp[0] = (pulTmp[0]<<8) + pucRspData[i];
				}		
			}
			else
			{
				pstValue->pucValue = (UINT8*)malloc(ulDataLen);
				if (pstValue->pucValue == NULL)
				{
					return -1;
				}
				memcpy(pstValue->pucValue, pucRspData, ulDataLen);
			}

			pucRspData += ulDataLen;
		}	
	}

    return 0;
}
#endif
#if 0
int snmp_parse_trap2_pdu(unsigned char *pucRspData,
						unsigned int ulRspDataLen,
						SNMP_TRAP2_PDU_T *pstPDU)
{
    unsigned int    ulPDULen;
    unsigned int    ulSnmpPDULen;
    unsigned int    ulParasLen;
    unsigned int    ulPara1Len;
    unsigned int    ulDataLen;
    unsigned char   ucOIDSize;
    unsigned char   ucDataType;
    unsigned char   ucTmp;
    unsigned int    *pulTmp;
	unsigned char	*pucOriRspData = pucRspData;
    unsigned int    i;
	//SNMP_OID_T		*pstOID;
	//SNMP_VALUE_T	*pstValue;

    if ((*pucRspData++) != SNMP_ID_SEQUENCE)
    {
        return -1;
    }

    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulPDULen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;

        if (ulRspDataLen != (1+1+ucTmp+ulPDULen))
        {
            return -1;
        }
    }
    else
    {
        ulPDULen = pucRspData[0];
        pucRspData ++;

        if (ulRspDataLen != (1+1+ulPDULen))
        {
            return -1;
        }
    }

    //ver check
    if (pucRspData[0] != ASN_INTEGER ||
        pucRspData[1] != 1 ||
        pucRspData[2] != SNMP_TRAP_VER)
    {
        return -1;
    }
    pucRspData += (1+1+1);

    //community
    if (pucRspData[0] != ASN_OCTET_STR ||
        pucRspData[1] == 0)
    {
        return -1;
    }
    pucRspData += (1+1+pucRspData[1]);

    //snmp pdu type : get response
    if ((*pucRspData++) != SNMP_MSG_TRAP2)
    {
        return -1;
    }

    //snmp pdu len
    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulSnmpPDULen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;
    }
    else
    {
        ulSnmpPDULen = pucRspData[0];
        pucRspData ++;
    }

    //req id
    if (pucRspData[0] != ASN_INTEGER ||
        pucRspData[1] == 0)
    {
		nms_log(LOG_ERR, "snmp_parse_get_rsp: check reqid failed");
        return -1;
    }

    ucTmp = pucRspData[1];
    pucRspData += 2;
	CHECK_REQ_ID(pstPDU->stReqID, pucRspData, ucTmp)
	{
        nms_log(LOG_ERR, "snmp_parse_get_rsp: check reqid failed");
		return -1;
	}
	pucRspData += ucTmp;

	//error status
    if (pucRspData[0] != ASN_INTEGER ||
        pucRspData[1] != 1)
    {
        return -1;
    }

    pucRspData += 2;

    if ((*pucRspData++) != SNMP_ERR_NOERROR)
    {
        return -2;
    }

    //error index
    pucRspData += (1+1+1);

    //paras
    if ((*pucRspData++) != SNMP_ID_SEQUENCE)
    {
        return -1;
    }

    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulParasLen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;
    }
    else
    {
        ulParasLen = pucRspData[0];
        pucRspData ++;
    }

	pstPDU->pstVarList = NULL;
   	while(pucRspData < pucOriRspData+ulRspDataLen)
	{
		SNMP_OID_T	stOID;
		SNMP_OID_T	*pstOID = &stOID;
		SNMP_VALUE_T stValue = {0};
		SNMP_VALUE_T *pstValue = &stValue;
		/*
		//pstOID   = (SNMP_OID_T*)malloc(sizeof(SNMP_OID_T));
		//pstValue = (SNMP_VALUE_T*)malloc(sizeof(SNMP_VALUE_T));

		if (pstOID == NULL)
		{
			if (pstOID)
			{
				free(pstOID);
			}

			return -1;
		}
		*/

		//memset(pstOID, 0, sizeof(SNMP_OID_T));
		//memset(pstValue, 0, sizeof(SNMP_VALUE_T));
		//pstTableValue->ulColumnNum ++;

		//para
		if ((*pucRspData++) != SNMP_ID_SEQUENCE)
		{
			return -1;
		}

		if (pucRspData[0] > 0x80)
		{
			ucTmp = pucRspData[0] - 0x80;
			pucRspData ++;

			ulPara1Len = snmp_get_int_value(pucRspData, ucTmp);
			pucRspData += ucTmp;
		}
		else
		{
			ulPara1Len = pucRspData[0];
			pucRspData ++;
		}

		//oid type
		if (pucRspData[0] != ASN_OBJECT_ID ||
			pucRspData[1] == 0)
		{
			return -1;
		}

		ucOIDSize = pucRspData[1];
		pucRspData += 2;

		//check oid
		if (snmp_parse_oid_var(pucRspData, ucOIDSize, pstOID->astOID, &pstOID->ulOIDSize) < 0)
		{
			return -1;
		}
		pucRspData += ucOIDSize;

		//value type
		ucDataType = *pucRspData++;
		//if (ucDataType != ASN_OCTET_STR)
		{
			//return -1;
		}

		//value len
		if (pucRspData[0] > 0x80)
		{
			ucTmp = pucRspData[0] - 0x80;
			pucRspData ++;

			ulDataLen = snmp_get_int_value(pucRspData, ucTmp);
			pucRspData += ucTmp;
		}
		else
		{
			ulDataLen = pucRspData[0];
			pucRspData ++;
		}

		pstValue->ucType = ucDataType;
		pstValue->ulLen  = ulDataLen;
		if (ulDataLen == 0)
		{
			pstValue->pucValue = NULL;
		}
		else
		{
			if (ucDataType == ASN_GAUGE ||
				ucDataType == ASN_INTEGER ||
				ucDataType == ASN_TIMETICKS )
			{
                if (pstValue->ulLen == sizeof(unsigned int)+1)
                {
                    pucRspData ++;
                    pstValue->ulLen --;
                }
				pstValue->pucValue = (unsigned char*)malloc(sizeof(unsigned int));
				if (pstValue->pucValue == NULL)
				{
					return -1;
				}

                if (pstValue->ulLen > sizeof(unsigned int))
                {
                    return -1;
                }
				pulTmp = (unsigned int*)pstValue->pucValue;
				pulTmp[0] = 0;
				for (i=0; i<pstValue->ulLen; i++)
				{
					pulTmp[0] = (pulTmp[0]<<8) + pucRspData[i];
				}

				pstValue->ulLen = sizeof(unsigned int);
			}
			else
			{
				pstValue->pucValue = (unsigned char*)malloc(ulDataLen);
				if (pstValue->pucValue == NULL)
				{
					return -1;
				}
				memcpy(pstValue->pucValue, pucRspData, ulDataLen);
			}

			pucRspData += ulDataLen;
		}

		if (snmp_varlist_add_var(&pstPDU->pstVarList, pstOID->astOID, pstOID->ulOIDSize,
			                     pstValue->ucType,
							     pstValue->pucValue, pstValue->ulLen) < 0)
		{
			return -1;
		}
	}

    return 0;
}

/*************************************************************************
* 函数名称: snmp_parse_set_rsp
* 功    能: 解析set应答报文
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_parse_set_rsp(SNMP_SET_PDU_T *pstPDU,
                       unsigned char *pucRspData, unsigned int ulRspDataLen)
{
    unsigned int    ulPDULen;
    unsigned int    ulSnmpPDULen;
    unsigned int    ulParasLen;
    unsigned int    ulPara1Len;
    unsigned char   ucOIDSize;
    unsigned char   ucErrorStatus;
    unsigned char   ucTmp;
    unsigned char   ucRspDataType;
    unsigned int    ulDataLen;

    if ((*pucRspData++) != SNMP_ID_SEQUENCE)
    {
        nms_log(LOG_ERR, "snmp_parse_set_rsp: check snmp pdu failed");
        return -1;
    }

    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulPDULen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;

        if (ulRspDataLen != (1+1+ucTmp+ulPDULen))
        {
            nms_log(LOG_ERR, "snmp_parse_set_rsp: check snmp pdu len failed");
            return -1;
        }
    }
    else
    {
        ulPDULen = pucRspData[0];
        pucRspData ++;

        if (ulRspDataLen != (1+1+ulPDULen))
        {
            nms_log(LOG_ERR, "snmp_parse_set_rsp: check snmp pdu len failed");
            return -1;
        }
    }    

    //ver check
    if (pucRspData[0] != ASN_INTEGER ||
        pucRspData[1] != 1 ||
        pucRspData[2] != SNMP_GET_VER)
    {
        nms_log(LOG_ERR, "snmp_parse_set_rsp: check ver failed");
        return -1;
    }
    pucRspData += (1+1+1);

    //community
    if (pucRspData[0] != ASN_OCTET_STR ||
        pucRspData[1] == 0)
    {
        nms_log(LOG_ERR, "snmp_parse_set_rsp: check community failed");
        return -1;
    }
    pucRspData += (1+1+pucRspData[1]);

    //snmp pdu type : get response 
    if ((*pucRspData++) != SNMP_MSG_RESPONSE)
    {
        nms_log(LOG_ERR, "snmp_parse_set_rsp: check snmp pdu type failed");
        return -1;
    }

    //snmp pdu len
    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulSnmpPDULen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;
    }
    else
    {
        ulSnmpPDULen = pucRspData[0];
        pucRspData ++;
    }

    //req id
    if (pucRspData[0] != ASN_INTEGER ||
        pucRspData[1] == 0)
    {
        nms_log(LOG_ERR, "snmp_parse_set_rsp: check reqid failed");
        return -1;
    }

    ucTmp = pucRspData[1];
    pucRspData += 2;
    if (pstPDU->stReqID.pucValue &&
        memcmp(pucRspData, pstPDU->stReqID.pucValue, 2) != 0)
	{
        nms_log(LOG_ERR, "snmp_parse_set_rsp: check reqid failed");
		return -1;
	}
    pucRspData += ucTmp;

    //error status
    if (pucRspData[0] != ASN_INTEGER ||
        pucRspData[1] != 1)
    {
        nms_log(LOG_ERR, "snmp_parse_set_rsp: check error status failed");
        return -1;
    }

    pucRspData += 2;
    ucErrorStatus = *pucRspData++;
    if (ucErrorStatus != SNMP_ERR_NOERROR)
    {
        nms_log(LOG_ERR, "snmp_parse_set_rsp: snmp set failed, error status is %d", ucErrorStatus);
        return -2;
    }

    //error index
    pucRspData += (1+1+1);

    //paras
    if ((*pucRspData++) != SNMP_ID_SEQUENCE)
    {
        nms_log(LOG_ERR, "snmp_parse_set_rsp: check paras failed");
        return -1;
    }

    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulParasLen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;
    }
    else
    {
        ulParasLen = pucRspData[0];
        pucRspData ++;
    }

    //para1
    if ((*pucRspData++) != SNMP_ID_SEQUENCE)
    {
        nms_log(LOG_ERR, "snmp_parse_set_rsp: check para failed");
        return -1;
    }

    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulPara1Len = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;
    }
    else
    {
        ulPara1Len = pucRspData[0];
        pucRspData ++;
    }

    //oid type
    if (pucRspData[0] != ASN_OBJECT_ID ||
        pucRspData[1] == 0)
    {
        nms_log(LOG_ERR, "snmp_parse_set_rsp: check oid failed");
        return -1;
    }
    
    ucOIDSize = pucRspData[1];
    pucRspData += 2;
    if (ucOIDSize != pstPDU->stOID.ulLen)
    {
        nms_log(LOG_ERR, "snmp_parse_set_rsp: check oid size failed");
        return -1;
    }

    if (pstPDU->stOID.pucValue &&
        memcmp(pucRspData, pstPDU->stOID.pucValue, ucOIDSize) != 0)
    {
        nms_log(LOG_ERR, "snmp_parse_set_rsp: check oid value failed");
        return -1;
    }

    pucRspData += ucOIDSize;

    //value type
    ucRspDataType = *pucRspData++;
    if (ucRspDataType != pstPDU->stValue.ucType)
    {
        nms_log(LOG_ERR, "snmp_parse_set_rsp: check set data type(%d <-> %d) failed", 
                     ucRspDataType, pstPDU->stValue.ucType);
        return -1; 
    }

    //octet string len
    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulDataLen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;
    }
    else
    {
        ulDataLen = pucRspData[0];
        pucRspData ++;
    }

    return 0;
}
#endif
/*************************************************************************
* 函数名称: snmp_get_socket_err
* 功    能: 获取socket错误原因
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
char* snmp_get_socket_err(void)
{
    int iErrCode;
//    static char acErrInfo[32];

#ifdef WIN32
    iErrCode = WSAGetLastError();
#else
    iErrCode = errno;
#endif

#ifdef HAVE_TYPEDEF
    sprintf(acErrInfo, "Socket error: %d", iErrCode);
    return acErrInfo;
#else

    switch (iErrCode)
    {
    case SEDESTADDRREQ:
        return "Socket error: Destination address required!";

    case SEPROTOTYPE:
        return "Socket error: Protocol wrong type for socket!";

    case SENOPROTOOPT:
        return "Socket error: Protocol not available!";

    case SEPROTONOSUPPORT:
        return "Socket error: Protocol not supported!";

    case SESOCKTNOSUPPORT:
        return "Socket error: Socket type not supported!";

    case SEOPNOTSUPP:
        return "Socket error: Operation not supported on socket!";

    case SEPFNOSUPPORT:
        return "Socket error: Protocol family not supported!";

    case SEAFNOSUPPORT:
        return "Socket error: Addr family not supported!";

    case SEADDRINUSE:
        return "Socket error: Port already in use!";

    case SEADDRNOTAVAIL:
        return "Socket error: Can't assign requested address!";

    case SENOTSOCK:
        return "Socket error: Socket operation on non-socket!";

    case SENETUNREACH:
        return "Socket error: Network is unreachable!";

    case SENETRESET:
        return "Socket error: Network dropped connection on reset!";

    case SECONNABORTED:
        return "Socket error: Software caused connection abort!";

    case SECONNRESET:
        return "Socket error: Connection reset by peer!";

    case SENOBUFS:
        return "Socket error: No buffer space available!";

    case SEISCONN:
        return "Socket error: Socket is already connected!";

    case SENOTCONN:
        return "Socket error: Socket is not connected!";

    case SETOOMANYREFS:
        return "Socket error: Too many references: can't splice!";

    case SESHUTDOWN:
        return "Socket error: Can't send after socket shutdown!";

    case SETIMEDOUT:
        return "Socket error: Connection timed out!";

    case SECONNREFUSED:
        return "Socket error: Connection refused!";

    case SENETDOWN:
        return "Socket error: Network is down!";

    case SELOOP:
        return "Socket error: Too many levels of symbolic links!";

    case SEHOSTUNREACH:
        return "Socket error: No route to host!";

    case SEHOSTDOWN:
        return "Socket error: Peer is down!";

    case SEINPROGRESS:
        return "Socket error: Operation now in progress!";

    case SEALREADY:
        return "Socket error: Operation already in progress!";

    case SEWOULDBLOCK:
        return "Socket error: Operation would block!";

    case SENOTINITIALISED:
        return "Socket error: Not initialized!";

    case SEFAULT:
        return "Socket error: Bad ip address!";

    case SEINVAL:
        return "Socket error: Invalid parameter!";

    default:
        return "Scoket error: Unknown error!";
    }
#endif
}

/*************************************************************************
* 函数名称: snmp_init_socket
* 功    能: 初始化socket
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_init_socket(void)
{
#ifdef WIN32
    WORD        wVersionRequested;
    WSADATA     wsaData;

    wVersionRequested = MAKEWORD (2, 2);
    if (WSAStartup (wVersionRequested, &wsaData) != 0)
    {
        return -1;
    }

    if (LOBYTE (wsaData.wVersion) != 2 ||
        HIBYTE (wsaData.wVersion) != 2 )
    {
        WSACleanup ();
        return -1;
    }

    return 0;
#else
    return 0;
#endif
}

/*************************************************************************
* 函数名称: snmp_recv_from
* 功    能: 接受socket消息（允许超时）
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_recv_from(SOCKET stSock, void *pMsgBuf, unsigned int ulMsgBufLen, unsigned int ulTimeout)
{
    fd_set fds;
    struct timeval timeout = {0};
    int iRet;
    int iRcvLen;

    /* 同步调用的超时时间必定大于0，异步调用不需要recv */
    if (ulTimeout == 0)
    {
        //nms_log(LOG_ERR, "snmp_recv_from: timeout(%u) must be large than 0", ulTimeout);
        //return -1;

        iRcvLen = recv(stSock, (char*)pMsgBuf, ulMsgBufLen, 0);
        return iRcvLen;
    }

again:
    timeout.tv_sec = ulTimeout;
    FD_ZERO(&fds);
    FD_SET(stSock, &fds);

    iRet = select(stSock + 1, &fds, NULL, NULL, &timeout);
    if (iRet == 0)
    {
        nms_log(LOG_ERR, "snmp_recv_from: select timeout(%u)", ulTimeout);
        return 0;
    }
    
    if (iRet < 0)
    {
        if (errno == EINTR)
            goto again;

        nms_log(LOG_ERR, "snmp_recv_from: select() failed(%d)", iRet);
    	return -1;
	}

    if (FD_ISSET(stSock, &fds))
    {
        iRcvLen = recv(stSock, (char*)pMsgBuf, ulMsgBufLen, 0);
        if (iRcvLen <= 0)
        {
            nms_log(LOG_ERR, "snmp_recv_from: recv() failed, recv Len is %d", iRcvLen);
        }

        FD_CLR(stSock, &fds);

        return iRcvLen;
    }

    nms_log(LOG_ERR, "snmp_recv_from: select failed, unknow socket");

    FD_CLR(stSock, &fds);
    return -1;
}

/*************************************************************************
* 函数名称: snmp_check_rsp
* 功    能: 发送UDP消息，并接受socket消息（允许超时）
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_check_rsp(unsigned char *pucData, 
						 unsigned int   ulDataLen,
						 SNMP_TLV_T     *pstReqID)
{
	UINT8 ucTmp = 0;
	UINT32 ulPDULen = 0;
	UINT32 ulSnmpPDULen = 0;
	UINT8 *pucRspData = pucData;
	UINT32 ulRspDataLen = ulDataLen;

	if ((*pucRspData++) != SNMP_ID_SEQUENCE)
    {
        return -1;
    }

    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulPDULen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;

        if (ulRspDataLen != (1+1+ucTmp+ulPDULen))
        {
		nms_log(LOG_ERR, "snmp_check_rsp: check ulRspDataLen=%u,ulShouldBe=%u failed",ulRspDataLen, 
			(1+1+ucTmp+ulPDULen));	
            return -1;
        }
    }
    else
    {
        ulPDULen = pucRspData[0];
        pucRspData ++;

        if (ulRspDataLen != (1+1+ulPDULen))
        {
		nms_log(LOG_ERR, "snmp_check_rsp: check ulRspDataLen=%u,ulShouldBe=%u failed",ulRspDataLen, 
			(1+1+ulPDULen));	
            return -1;
        }
    }    

    //ver check
    if (pucRspData[0] != ASN_INTEGER ||
        pucRspData[1] != 1 ||
        pucRspData[2] != SNMP_GET_VER)
    {
		nms_log(LOG_ERR, "snmp_check_rsp: check pucRspData[0]=%u,[1]=%u,[2]=%u failed",pucRspData[0] , 
			pucRspData[1], pucRspData[2]);	
        return -1;
    }
    pucRspData += (1+1+1);

    //community
    if (pucRspData[0] != ASN_OCTET_STR ||
        pucRspData[1] == 0)
    {
		nms_log(LOG_ERR, "snmp_check_rsp: check pucRspData[0]=%u,[1]=%u failed",pucRspData[0] , 
			pucRspData[1]);	
        return -1;
    }
    pucRspData += (1+1+pucRspData[1]);

    //snmp pdu type : get response 
    if ((*pucRspData++) != SNMP_MSG_RESPONSE)
    {
		nms_log(LOG_ERR, "snmp_check_rsp: check (*pucRspData++) failed", (*pucRspData++));	
        return -1;
    }

    //snmp pdu len
    if (pucRspData[0] > 0x80)
    {
        ucTmp = pucRspData[0] - 0x80;
        pucRspData ++;

        ulSnmpPDULen = snmp_get_int_value(pucRspData, ucTmp);
        pucRspData += ucTmp;
    }
    else
    {
        ulSnmpPDULen = pucRspData[0];
        pucRspData ++;
    }

    //req id
    if (pucRspData[0] != ASN_INTEGER ||
        pucRspData[1] == 0)
    {
		nms_log(LOG_ERR, "snmp_check_rsp: pucRspData[0]:%u,pucRspData[1]:%u.",
			pucRspData[0],
			pucRspData[1]);
		nms_log(LOG_ERR, "snmp_check_rsp: check reqid failed");
        return -1;
    }

    ucTmp = pucRspData[1];
    pucRspData += 2;

       CHECK_REQ_ID((*pstReqID), pucRspData, ucTmp)
	{		
		return -1;
	}

	return 0;
}


/*************************************************************************
* 函数名称: snmp_udp_send
* 功    能: 发送UDP消息，并接受socket消息（允许超时）
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_udp_send(unsigned char *pucServerIP, unsigned short usServerPort, unsigned int ulTimeout,
                  void *pstReqMsg, unsigned int ulReqMsgLen,
                  void *pRcvMsg,   unsigned int ulMaxRcvMsgLen, unsigned int *pulRcvMsgLen,
                  SNMP_TLV_T	*pstReqID)
#if 0                  
{
	SOCKET  stSock;
    int     iRet = 0;
    SOCKADDR_IN stSockAddr = {0};
    SOCKADDR *pstAddr = (SOCKADDR *)&stSockAddr;
    int iAddrLen = sizeof(SOCKADDR);
    unsigned int  ulServerIP;
    int iRcvSize;

	unsigned int i=0;
	unsigned int timeout = 10;

    memcpy(&ulServerIP, pucServerIP, 4);
	
    for (i=0; i<(ulTimeout/timeout)+1; i++)
    {    
	    stSockAddr.sin_family = AF_INET;
	    stSockAddr.sin_port   = htons(usServerPort);
	    stSockAddr.sin_addr.s_addr = ulServerIP;

	    stSock = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	    if (stSock == INVALID_SOCKET)
	    {
	        nms_log(LOG_ERR, "snmp_udp_send: socket() failed");
	        return -1;
	    }

		//nms_print_binary(pstReqMsg, ulReqMsgLen);

	    iRet = sendto(stSock, (const char*)pstReqMsg, ulReqMsgLen, 0, pstAddr, iAddrLen);
	    if (iRet < 0)
	    {
	        nms_log(LOG_ERR, "snmp_udp_send: sendto() failed");
	        closesocket(stSock);
	        return -1;
	    }

	    if (pRcvMsg && pulRcvMsgLen)
	    {
	        iRcvSize = snmp_recv_from(stSock, (char*)pRcvMsg, ulMaxRcvMsgLen, timeout);
	        if (iRcvSize <= 0)
	        {
	            nms_log(LOG_ERR, "snmp_udp_send: snmp_recv_from() failed(%s)", snmp_get_socket_err());
	            closesocket(stSock);
	            return -1;
	        }

	        if ((unsigned int)iRcvSize > ulMaxRcvMsgLen)
	        {
	            nms_log(LOG_ERR, "snmp_udp_send: recv size(%d) is too big", iRcvSize);

	            closesocket(stSock);
	            return -1;
	        }			

	        *pulRcvMsgLen = iRcvSize;
	    }
	    closesocket(stSock);	

		if (pRcvMsg && pulRcvMsgLen)
		{
			if (-1 == snmp_check_rsp(pRcvMsg, *pulRcvMsgLen, pstReqID))
			{
				//nms_log(LOG_ERR, "snmp_check_rsp: check failed");
				continue;
			}	
		}

		// add temp
		//nms_print_binary(pRcvMsg, *pulRcvMsgLen);
			
  	  	return 0;
    }

	nms_log(LOG_ERR, "snmp_check_rsp: check %u times failed",(ulTimeout/5)+1);	
	return -1;				
}
#else
{
	/* SOCKET  stSock; 2014/4/21 added by zhangping snmp穿越nat */
    int     iRet = 0;
    SOCKADDR_IN stSockAddr = {0};
    SOCKADDR *pstAddr = (SOCKADDR *)&stSockAddr;
    int iAddrLen = sizeof(SOCKADDR);
    unsigned int  ulServerIP;
    int iRcvSize;

	unsigned int i=0;
	unsigned int timeout = 10;

    memcpy(&ulServerIP, pucServerIP, 4);
	
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port   = htons(usServerPort);
    stSockAddr.sin_addr.s_addr = ulServerIP;

    #if 0 /* 2014/4/21 added by zhangping snmp穿越nat */
    stSock = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (stSock == INVALID_SOCKET)
    {
        agent_printf(LOG_ERR, "snmp_udp_send: socket() failed");
        return -1;
    }
    #endif
    for (i=0; i<(ulTimeout/timeout)+1; i++)
    {    

		//nms_print_binary(pstReqMsg, ulReqMsgLen);
        iRet = 0;
        /* 2014/4/21 added by zhangping snmp穿越nat */
	    iRet = sendto(g_udp_sockfd, (const char*)pstReqMsg, ulReqMsgLen, 0, pstAddr, iAddrLen);
	    if (iRet < 0)
	    {
	        nms_log(LOG_ERR, "snmp_udp_send: sendto() failed");
	        /* closesocket(stSock);  2014/4/21 added by zhangping snmp穿越nat */
	        return -1;
	    }

	    if (NULL != pRcvMsg && NULL != pulRcvMsgLen)
	    {
	        iRcvSize = snmp_recv_from(g_udp_sockfd, (char*)pRcvMsg, ulMaxRcvMsgLen, timeout);
	        if (iRcvSize < 0)
	        {
	            nms_log(LOG_ERR, "snmp_udp_send: snmp_recv_from() failed(%s)", snmp_get_socket_err());
	            /* closesocket(stSock); 2014/4/21 added by zhangping snmp穿越nat */
	            return -1;
	        }
	        else if(0 == iRcvSize)
	        {
	            continue;
	        }

	        if ((unsigned int)iRcvSize > ulMaxRcvMsgLen)
	        {
	            nms_log(LOG_ERR, "snmp_udp_send: recv size(%d) is too big", iRcvSize);

	            /* closesocket(stSock); 2014/4/21 added by zhangping snmp穿越nat */
	            return -1;
	        }			

	        *pulRcvMsgLen = iRcvSize;
	        
			if (-1 == snmp_check_rsp(pRcvMsg, *pulRcvMsgLen, pstReqID))
			{
				nms_log(LOG_ERR, "snmp_check_rsp: check failed");
				continue;
			}	
	    }
		// add temp
		//nms_print_binary(pRcvMsg, *pulRcvMsgLen);
	    /* closesocket(stSock); 2014/4/21 added by zhangping snmp穿越nat */
  	  	return 0;
    }
   /* closesocket(stSock); 2014/4/21 added by zhangping snmp穿越nat */
	nms_log(LOG_ERR, "snmp_check_rsp: check %u times failed",i);	
	return -1;				
}
#endif

/*************************************************************************
* 函数名称: snmp_udp_send
* 功    能: 发送UDP消息，并接受socket消息（允许超时）
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
#if 0
int snmp_udp_send(unsigned char *pucServerIP, unsigned short usServerPort, unsigned int ulTimeout,
                  void *pstReqMsg, unsigned int ulReqMsgLen,
                  void *pRcvMsg,   unsigned int ulMaxRcvMsgLen, unsigned int *pulRcvMsgLen)
{
    SOCKET  stSock;
    int     iRet = 0;
    SOCKADDR_IN stSockAddr = {0};
    SOCKADDR *pstAddr = (SOCKADDR *)&stSockAddr;
    int iAddrLen = sizeof(SOCKADDR);
    unsigned int  ulServerIP;
    int iRcvSize;

	unsigned int i=0;

    memcpy(&ulServerIP, pucServerIP, 4);

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port   = htons(usServerPort);
    stSockAddr.sin_addr.s_addr = ulServerIP;

    stSock = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (stSock == INVALID_SOCKET)
    {
        nms_log(LOG_ERR, "snmp_udp_send: socket() failed");
        return -1;
    }

    iRet = sendto(stSock, (const char*)pstReqMsg, ulReqMsgLen, 0, pstAddr, iAddrLen);
    if (iRet < 0)
    {
        nms_log(LOG_ERR, "snmp_udp_send: sendto() failed");
        closesocket(stSock);
        return -1;
    }

    if (pRcvMsg && pulRcvMsgLen)
    {
        iRcvSize = snmp_recv_from(stSock, (char*)pRcvMsg, ulMaxRcvMsgLen, ulTimeout);
        if (iRcvSize <= 0)
        {
            nms_log(LOG_ERR, "snmp_udp_send: snmp_recv_from() failed(%s)", snmp_get_socket_err());
            closesocket(stSock);
            return -1;
        }

        if ((unsigned int)iRcvSize > ulMaxRcvMsgLen)
        {
            nms_log(LOG_ERR, "snmp_udp_send: recv size(%d) is too big", iRcvSize);

            closesocket(stSock);
            return -1;
        }			

        *pulRcvMsgLen = iRcvSize;
    }
    closesocket(stSock);		
				
    return 0;
}
#endif
#if 0
/*************************************************************************
* 函数名称: snmp_init_session
* 功    能: 创建session（对于每个Agent，不需要重复创建）
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
SNMP_SESSION_T* snmp_init_session(unsigned char *pucAgengIP,
								  unsigned short usAgentPort,
								  char  *szCommunity, 
								  unsigned int ulTimeout)
{
    SNMP_SESSION_T  *pstSession = NULL;

	if (strlen(szCommunity) >= sizeof(pstSession->acCommunity))
	{
		return NULL;
	}

	if (pstSession == NULL)
	{
		pstSession = (SNMP_SESSION_T*)malloc(sizeof(SNMP_SESSION_T));
	}

	if (pstSession)
	{
		memset(pstSession, 0, sizeof(SNMP_SESSION_T));

		memcpy(pstSession->aucAgentIP, pucAgengIP, sizeof(pstSession->aucAgentIP));
		pstSession->usAgentPort = usAgentPort;
		pstSession->ulTimeout = ulTimeout;

		strcpy(pstSession->acCommunity, szCommunity);
	}

	return pstSession;
}

/*************************************************************************
* 函数名称: snmp_get_data
* 功    能: 实现snmp get操作
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明: 
*************************************************************************/
int snmp_get_data(SNMP_SESSION_T *pstSession, SNMP_OID *pOID, unsigned int ulOIDSize, unsigned char ucType, 
                  unsigned int ulMaxDataLen, void *pData, unsigned int *pulDataLen)
{
    int             iRet;
    SNMP_GET_PDU_T  stPDU = {0};
    unsigned char   aucReqMsg[256];
    unsigned int    ulReqMsgLen = 0;
    unsigned int    ulRecvMsgLen;
    unsigned int    ulDataLen = 0;
    unsigned char   *pBuf = NULL;
    unsigned int    ulBufLen = 64*1024;

	if (!pstSession)
	{
		return -1;
	}

    iRet = snmp_init_get_info(pstSession->acCommunity, pOID, ulOIDSize, &stPDU);
    if (iRet < 0)
    {
        return -1;
    }

    iRet = snmp_create_get_pdu1(&stPDU, aucReqMsg, &ulReqMsgLen);
    if (iRet < 0)
    {
        return -1;
    }

    if (snmp_init_socket() < 0)
    {
        return -1;
    }

    pBuf = malloc(ulBufLen);
    if (NULL == pBuf)
    {
        return -1;
    }

    iRet = snmp_udp_send(pstSession->aucAgentIP, pstSession->usAgentPort, pstSession->ulTimeout, 
						 aucReqMsg, ulReqMsgLen,
						 pBuf, ulBufLen, &ulRecvMsgLen);
    if (iRet < 0)
    {
        free(pBuf);
        return -1;
    }

    iRet = snmp_parse_get_rsp1(&stPDU, pBuf, ulRecvMsgLen, pData, ulMaxDataLen, &ulDataLen);
    if (iRet < 0)
    {
        free(pBuf);
        return iRet;
    }

    if (pulDataLen)
    {
        *pulDataLen = ulDataLen;
    }
    
    free(pBuf);
    return 0;
}
#endif
#if 0
/*************************************************************************
* 函数名称: snmp_free_set_pdu
* 功    能: 释放PDU中的动态申请的内存
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明:
*************************************************************************/
int snmp_free_set_pdu(SNMP_SET_PDU_T *pstPDU)
{
	FREE_TLV(&pstPDU->stPDU);
	FREE_TLV(&pstPDU->stVer);
	FREE_TLV(&pstPDU->stCommunity);
	FREE_TLV(&pstPDU->stSnmpPDU);
	FREE_TLV(&pstPDU->stReqID);
	FREE_TLV(&pstPDU->stErrStatus);
	FREE_TLV(&pstPDU->stErrIndex);
	FREE_TLV(&pstPDU->stParas);
	FREE_TLV(&pstPDU->stPara1);
	FREE_TLV(&pstPDU->stOID);
	FREE_TLV(&pstPDU->stValue);

    return 0;
}

/*************************************************************************
* 函数名称: snmp_get_scalar
* 功    能: 实现snmp get scalar操作
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明: 
*************************************************************************/
int snmp_get_scalar(SNMP_SESSION_T *pstSession, SNMP_OID *pOID, unsigned int ulOIDSize, unsigned char ucType, 
                    unsigned int ulMaxDataLen, void *pData, unsigned int *pulDataLen)
{
    int						iRet = -1;
    SNMP_GET_SCALAR_PDU_T	stPDU;// = {0};
    unsigned char   aucReqMsg[1024];
    unsigned int    ulReqMsgLen = 0;
    unsigned int    ulRecvMsgLen;
    unsigned int    ulDataLen = 0;
    unsigned char   *pBuf = NULL;
    unsigned int    ulBufLen = 64*1024;

	if (!pstSession)
	{
		return -1;
	}

    iRet = snmp_init_get_pdu(pstSession->acCommunity, pOID, ulOIDSize, SNMP_MSG_GET, &stPDU);
    if (iRet < 0)
    {
        return -1;
    }

    iRet = snmp_create_get_pdu(&stPDU, aucReqMsg, sizeof(aucReqMsg)-1, &ulReqMsgLen);
    if (iRet < 0)
    {
        return -1;
    }

    if (snmp_init_socket() < 0)
    {
        return -1;
    }

    pBuf = (unsigned char*)malloc(ulBufLen);
    if (NULL == pBuf)
    {
        return -1;
    }

    iRet = snmp_udp_send(pstSession->aucAgentIP, pstSession->usAgentPort, pstSession->ulTimeout, 
						 aucReqMsg, ulReqMsgLen,
						 pBuf, ulBufLen, &ulRecvMsgLen,
						 &(stPDU.stReqID));
    if (iRet < 0)
    {
        free(pBuf);
        return -1;
    }

    iRet = snmp_parse_get_rsp(&stPDU, pBuf, ulRecvMsgLen, (unsigned char*)pData, ulMaxDataLen, &ulDataLen);
    if (iRet < 0)
    {
        free(pBuf);
        return iRet;
    }

    if (pulDataLen)
    {
        *pulDataLen = ulDataLen;
    }
    
    free(pBuf);
    return 0;
}
#endif
#if 0
/*************************************************************************
* 函数名称: snmp_get_next
* 功    能: 实现snmp get next操作
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明: 
*************************************************************************/
int snmp_get_next(SNMP_SESSION_T *pstSession, SNMP_OID *pOID, unsigned int ulOIDSize, 
				  SNMP_OID *pNextOID, unsigned int *pulNextOIDSize, SNMP_VALUE_T *pstValue)
{
    int						iRet = -1;
    SNMP_GET_TABLE_PDU_T	stPDU;// = {0};
    unsigned char   aucReqMsg[1024];
    unsigned int    ulReqMsgLen = 0;
    unsigned int    ulRecvMsgLen;
//    unsigned int    ulDataLen = 0;
    unsigned char   *pBuf = NULL;
    unsigned int    ulBufLen = 64*1024;

	if (!pstSession)
	{
		return -1;
	}

    iRet = snmp_init_get_pdu(pstSession->acCommunity, pOID, ulOIDSize, SNMP_MSG_GETNEXT, &stPDU);
    if (iRet < 0)
    {
        return -1;
    }

    iRet = snmp_create_get_pdu(&stPDU, aucReqMsg, sizeof(aucReqMsg)-1, &ulReqMsgLen);
    if (iRet < 0)
    {
        return -1;
    }

    if (snmp_init_socket() < 0)
    {
        return -1;
    }

    pBuf = (unsigned char*)malloc(ulBufLen);
    if (NULL == pBuf)
    {
        return -1;
    }

    iRet = snmp_udp_send(pstSession->aucAgentIP, pstSession->usAgentPort, pstSession->ulTimeout, 
						 aucReqMsg, ulReqMsgLen,
						 pBuf, ulBufLen, &ulRecvMsgLen,
						 &(stPDU.stReqID));
    if (iRet < 0)
    {
        free(pBuf);
        return -1;
    }

    iRet = snmp_parse_get_next_rsp(&stPDU, pBuf, ulRecvMsgLen, pNextOID, pulNextOIDSize, pstValue);
    if (iRet < 0)
    {
        free(pBuf);
        return iRet;
    }

    free(pBuf);
    return 0;
}

void snmp_free_getbulk_pdu(SNMP_GET_BULK_PDU_T *pstPDU)
{
	unsigned int	i;
	SNMP_TLV_T		*pstVar;

	if (pstPDU->pstPara)
	{
		for (i=0; i<pstPDU->ulOIDNum; i++)
		{
			pstVar = pstPDU->pstOID + i;
			snmp_free_tlv(pstVar);
				
			pstVar = pstPDU->pstPara + i;
			snmp_free_tlv(pstVar);
		}
		
		free(pstPDU->pstPara);
		pstPDU->pstPara = NULL;
	}
	
	snmp_free_tlv(&pstPDU->stPDU);
	snmp_free_tlv(&pstPDU->stVer);
	snmp_free_tlv(&pstPDU->stCommunity);
	snmp_free_tlv(&pstPDU->stSnmpPDU);
	snmp_free_tlv(&pstPDU->stReqID);
	snmp_free_tlv(&pstPDU->stErrStatus);
	snmp_free_tlv(&pstPDU->stErrIndex);
	snmp_free_tlv(&pstPDU->stParas);
}

void snmp_free_table_value(SNMP_TABLE_VALUE_T *pstTableValue)
{
	SNMP_OID_T			*pstOID, *pstNextOID;
	SNMP_VALUE_T		*pstValue, *pstNextValue;

	for(pstOID = pstTableValue->pstOID; pstOID != NULL; )
	{
		pstNextOID = pstOID->pstNext;
		free(pstOID);
		pstOID = pstNextOID;
	}

	for(pstValue = pstTableValue->pstValue; pstValue != NULL; )
	{
		pstNextValue = pstValue->pstNext;
		free(pstValue);
		pstValue = pstNextValue;
	}
}
/*************************************************************************
* 函数名称: snmp_get_table
* 功    能: 实现snmp get table操作
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明: 
*************************************************************************/

int snmp_get_table(SNMP_SESSION_T *pstSession, SNMP_OID *pTableOID, unsigned int ulOIDSize, 
				   unsigned int ulColumnNum, SNMP_TABLE_VALUE_T *pstValue)
{
    int				iRet;
	unsigned int    i;
    unsigned int    ulReqMsgLen = 0;
    unsigned int    ulRecvMsgLen;
    unsigned int    ulDataLen = 0;
    unsigned char   *pBuf = NULL;
	unsigned char   *pucReqMsg;
    unsigned int    ulBufLen = 64*1024;
	SNMP_OID_T		*pstOID = NULL;
	SNMP_GET_BULK_PDU_T	stPDU = {0};

	if (!pstSession)
	{
		return -1;
	}

    if (snmp_init_socket() < 0)
    {
        return -1;
    }

	memset(&stPDU, 0, sizeof(stPDU));

	pstOID = malloc(ulColumnNum * sizeof(SNMP_OID_T));
	if (NULL == pstOID)
	{
		goto fail;
	}

	for (i=0; i<ulColumnNum; i++)
	{
		memcpy((pstOID+i)->astOID, pTableOID, sizeof(SNMP_OID)*ulOIDSize);
		(pstOID+i)->astOID[ulOIDSize] = i+1;
		(pstOID+i)->ulOIDSize = ulOIDSize+1;
	}

    iRet = snmp_init_getbulk_pdu(pstSession->acCommunity, ulColumnNum, pstOID, SNMP_MSG_GETNEXT, &stPDU);
    if (iRet < 0)
    {
        goto fail;
    }

    pBuf = malloc(ulBufLen*2);
    if (NULL == pBuf)
    {
        goto fail;
    }
	pucReqMsg = pBuf + ulBufLen;

    iRet = snmp_create_getbulk_pdu(&stPDU, pucReqMsg, ulBufLen-1, &ulReqMsgLen);
    if (iRet < 0)
    {
		goto fail;
    }

    iRet = snmp_udp_send(pstSession->aucAgentIP, pstSession->usAgentPort, pstSession->ulTimeout, 
						 pucReqMsg, ulReqMsgLen,
						 pBuf, ulBufLen, &ulRecvMsgLen);
    if (iRet < 0)
    {
        goto fail;
    }

    iRet = snmp_parse_get_table_rsp(&stPDU, pBuf, ulRecvMsgLen, pstValue);
    if (iRet < 0)
    {
        goto fail;
    }

    goto end;

fail:
	iRet = -1;

end:

	if (pstOID)
	{
		free(pstOID);
	}

	if (pBuf)
	{
		free(pBuf);
	}

	snmp_free_getbulk_pdu(&stPDU);

	return iRet;
}
#endif
#if 0
/*************************************************************************
* 函数名称: snmp_set_data
* 功    能: 实现snmp set操作
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明: 目前仅支持一个OID的set操作
*************************************************************************/
int snmp_set_data(SNMP_SESSION_T *pstSession, SNMP_OID *pOID, 
                  unsigned int ulOIDSize, unsigned char ucDataType, 
                  unsigned int ulDataLen, void *pData)
{
    int             iRet;
    SNMP_SET_PDU_T  stPDU;
    unsigned int    ulRecvMsgLen;
    unsigned char   *pBuf = NULL;
    unsigned int    ulBufLen = 64*1024;
	unsigned int	ulReqPDULen;
	unsigned char   *pucReqPDU;
    unsigned char   *pucRspData = NULL;

	if (!pstSession)
	{
        nms_log(LOG_ERR, "snmp_set_data: pstSession is null");
		return -1;
	}

    pBuf = (unsigned char *)malloc(ulBufLen);
    if (NULL == pBuf)
    {
        nms_log(LOG_ERR, "snmp_set_data: malloc failed");
        return -1;
    }

    memset(&stPDU, 0, sizeof(stPDU));
    iRet = snmp_init_set_pdu(pstSession->acCommunity, pOID, ulOIDSize, 
							 (unsigned char *)pData, ucDataType, ulDataLen, &stPDU);
    if (iRet < 0)
    {
        nms_log(LOG_ERR, "snmp_set_data: snmp_init_set_pdu failed");
        goto fail;
    }

    pucReqPDU = pBuf;
    iRet = snmp_create_set_pdu(&stPDU, pucReqPDU, ulBufLen, &ulReqPDULen);
    if (iRet < 0)
    {
        nms_log(LOG_ERR, "snmp_set_data: snmp_create_set_pdu failed");
		goto fail;
    }

    if (snmp_init_socket() < 0)
    {
        nms_log(LOG_ERR, "snmp_set_data: snmp_init_socket failed");
		goto fail;
    }

    if (pstSession->ulTimeout == 0)
    {
        pucRspData = NULL;
    }
    else
    {
        pucRspData = pBuf;
    }
    iRet = snmp_udp_send(pstSession->aucAgentIP, pstSession->usAgentPort, pstSession->ulTimeout, 
						 pucReqPDU, ulReqPDULen,
						 pucRspData, ulBufLen, &ulRecvMsgLen,
						 &(stPDU.stReqID));
    if (iRet < 0)
    {
        nms_log(LOG_ERR, "snmp_set_data: snmp_udp_send failed");
        goto fail;
    }

    if (pstSession->ulTimeout > 0)
    {
        /* 根据reqID和OID进行检查 */
        iRet = snmp_parse_set_rsp(&stPDU, pBuf, ulRecvMsgLen);
        if (iRet < 0)
        {
            nms_log(LOG_ERR, "snmp_set_data: snmp_parse_set_rsp failed");
            goto fail;
        }
    }

    free(pBuf);
    snmp_free_set_pdu(&stPDU);

    return 0;

fail:
    if (pBuf)
    {
        free(pBuf);
    }

    snmp_free_set_pdu(&stPDU);

    return -1;
}
#endif

/*************************************************************************
* 函数名称: snmp_send_trap2
* 功    能: 实现snmp inform操作
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明: 
*************************************************************************/
int snmp_send_trap2(SNMP_SESSION_T *pstSession, SNMP_VAR_T *pstVarList)
{
    int                 iRet;
    SNMP_TRAP2_PDU_T    stPDU;
    unsigned int        ulRecvMsgLen;
    unsigned char       *pBuf = NULL;
    unsigned int        ulBufLen = 64*1024;
	unsigned int	    ulReqPDULen;
	unsigned char       *pucReqPDU;
    unsigned char       *pucRspData = NULL;

	if (!pstSession)
	{
        nms_log(LOG_ERR, "snmp_send_trap2: pstSession is null");
        snmp_free_varlist(pstVarList);
		return -1;
	}

    pBuf = (unsigned char *)malloc(ulBufLen);
    if (NULL == pBuf)
    {
        nms_log(LOG_ERR, "snmp_send_trap2: malloc failed");
        snmp_free_varlist(pstVarList);
        return -1;
    }

    memset(&stPDU, 0, sizeof(stPDU));
    stPDU.pstVarList = pstVarList;
    iRet = snmp_init_trap2_pdu(pstSession->acCommunity, /*pOID, ulOIDSize, 
						       pData, ucDataType, ulDataLen, */ &stPDU);
    if (iRet < 0)
    {
        nms_log(LOG_ERR, "snmp_send_trap2: snmp_init_set_pdu failed");
        goto fail;
    }

    pucReqPDU = pBuf;
    iRet = snmp_create_trap2_pdu(&stPDU, pucReqPDU, ulBufLen, &ulReqPDULen);
    if (iRet < 0)
    {
        nms_log(LOG_ERR, "snmp_send_trap2: snmp_create_trap2_pdu failed");
		goto fail;
    }

    if (snmp_init_socket() < 0)
    {
        nms_log(LOG_ERR, "snmp_send_trap2: snmp_init_socket failed");
		goto fail;
    }

    pstSession->ulTimeout = 0;
    if (pstSession->ulTimeout == 0)
    {
        pucRspData = NULL;
    }
    else
    {
        pucRspData = pBuf;
    }

    iRet = snmp_udp_send(pstSession->aucAgentIP, pstSession->usAgentPort, pstSession->ulTimeout, 
						 pucReqPDU, ulReqPDULen,
						 pucRspData, ulBufLen, &ulRecvMsgLen,
						 &(stPDU.stReqID));
    if (iRet < 0)
    {
        nms_log(LOG_ERR, "snmp_send_trap2: snmp_udp_send failed");
        goto fail;
    }

    free(pBuf);
    snmp_free_trap2_pdu(&stPDU);

    return 0;

fail:
    if (pBuf)
    {
        free(pBuf);
    }

    snmp_free_trap2_pdu(&stPDU);

    return -1;
}
/*****************************************************************************
 Prototype    : snmp_send_inform
 Description  : 发送inform类型trap
 Input        : SNMP_SESSION_T *pstSession  
                SNMP_VAR_T *pstVarList      
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/8/25
    Author       : zoucaihong
    Modification : Created function

*****************************************************************************/
int snmp_send_inform(SNMP_SESSION_T *pstSession, SNMP_VAR_T *pstVarList)
{
    int                 iRet;
    SNMP_TRAP2_PDU_T    stPDU;
    unsigned int        ulRecvMsgLen;
    unsigned char       *pBuf = NULL;
    unsigned int        ulBufLen = 64*1024;
	unsigned int	    ulReqPDULen;
	unsigned char       *pucReqPDU;
    unsigned char       *pucRspData = NULL;

	if (!pstSession)
	{
        nms_log(LOG_ERR, "Func:%s: pstSession is null", __func__);
        snmp_free_varlist(pstVarList);
		return -1;
	}

    pBuf = (unsigned char *)malloc(ulBufLen);
    if (NULL == pBuf)
    {
        nms_log(LOG_ERR, "Func:%s: malloc failed", __func__);
        snmp_free_varlist(pstVarList);
        return -1;
    }

    memset(&stPDU, 0, sizeof(stPDU));
    stPDU.pstVarList = pstVarList;
    iRet = snmp_init_inform_pdu(pstSession->acCommunity, &stPDU);
    if (iRet < 0)
    {
        nms_log(LOG_ERR, "Func:%s: snmp_init_set_pdu failed", __func__);
        goto fail;
    }

    pucReqPDU = pBuf;
    iRet = snmp_create_trap2_pdu(&stPDU, pucReqPDU, ulBufLen, &ulReqPDULen);
    if (iRet < 0)
    {
        nms_log(LOG_ERR, "Func:%s: snmp_create_trap2_pdu failed", __func__);
		goto fail;
    }

    if (snmp_init_socket() < 0)
    {
        nms_log(LOG_ERR, "Func:%s: snmp_init_socket failed", __func__);
		goto fail;
    }

    pstSession->ulTimeout = 0;
    if (pstSession->ulTimeout == 0)
    {
        pucRspData = NULL;
    }
    else
    {
        pucRspData = pBuf;
    }

    iRet = snmp_udp_send(pstSession->aucAgentIP, pstSession->usAgentPort, pstSession->ulTimeout, 
						 pucReqPDU, ulReqPDULen,
						 pucRspData, ulBufLen, &ulRecvMsgLen,
						 &(stPDU.stReqID));
    if (iRet < 0)
    {
        nms_log(LOG_ERR, "Func:%s: snmp_udp_send failed", __func__);
        goto fail;
    }

    free(pBuf);
    snmp_free_trap2_pdu(&stPDU);

    return 0;

fail:
    if (pBuf)
    {
        free(pBuf);
    }

    snmp_free_trap2_pdu(&stPDU);

    return -1;
}

