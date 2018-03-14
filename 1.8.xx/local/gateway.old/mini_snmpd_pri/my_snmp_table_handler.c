/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* ��    ��: 
* �޸���ʷ: 
* 2012-9-10     �뾲              �½�
******************************************************************************/
#include <syslog.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "mini_snmpd.h"

#include "my_snmp_def.h"
#include "my_snmp_data.h"
//#include "my_snmp_oid.h"
#include "my_snmp_handle.h"
#include "my_snmp_util.h"

/*************************************************************************/
extern OSTOKEN gtSemConfig;	/* ���û����ź��� */
extern OSTOKEN gtSemStat;      /* ͳ�ƻ����ź��� */

/*************************************************************************
* ��������: snmp_set_table_status
* ��    ��: ��ȡ����޸�״̬
* ��    ��: 
* ��������			����/���				����
* ��������: 
* ˵    ��: 
*************************************************************************/
UINT32 snmp_set_table_status(SNMP_TABLE_T *pstTable)
{
	UINT8  *pValue = NULL;
	UINT32 ulLen = 0;
	CHAR *szRecValue = NULL;
	CHAR *szColumnValue = NULL;
	UINT8  ucValue = 0;
	UINT16 usValue = 0;
	INT32  lValue = 0;

	UINT32 i=0;
	
	SNMP_COLUMN_T *pstColumn = NULL;

#if 0
	for (i=0; i<pstTable->ulKeyColumnNum; i++)
	{
		pstColumn = pstTable->apstKeyColumns[i];
		if (pstTable->aulColumnState[pstColumn->usColumnID] == SNMP_COLUMN_STATE_NULL)
		{
			/* ���������δ���� */
			/* ������ */
		}
		else
		{
			/* ������ */
		}
	}	
#endif
	
	szRecValue = (CHAR*)pstTable->pNewTableData;
	for (i=0; i<pstTable->ulColumnNum; i++)
	{
		pstColumn = &(pstTable->astColumns[i]);
		if (pstTable->aulColumnState[pstColumn->usColumnID] != SNMP_COLUMN_STATE_NULL)
		{
			continue;
		}

		szColumnValue = szRecValue+pstColumn->usOffSet;		
		pValue = pstColumn->pucDefValue;

		/* ����Ƿ����ȱʡֵ */
		/* �������ȱʡֵ����ȱʡֵ���ã�
		������0ֵ��ֵ*/	
		switch(pstColumn->ucColumnType)
		{
			case TYPE_CHAR:
				if (NULL != pValue)
				{
					ulLen = strlen((CHAR *)pValue)+1;
					strcpy(szColumnValue, (CHAR *)pValue);
				}
				else
				{
				}
				break;
			case TYPE_BYTE:
				ulLen = sizeof(UINT8);
				if (NULL != pValue)
				{
					memcpy(szColumnValue, (UINT8*)(pValue), ulLen);
				}
				else
				{
					ucValue = (UINT8)pstColumn->ulDefValue;
					memcpy(szColumnValue, (UINT8*)(&ucValue), ulLen);
				}
				break;
			case TYPE_SHORT:
				ulLen = sizeof(UINT16);
				if (NULL != pValue)
				{
					memcpy(szColumnValue, (UINT8*)(pValue), ulLen);
				}
				else
				{
					usValue = (UINT16)pstColumn->ulDefValue;
					memcpy(szColumnValue, (UINT8*)(&usValue), ulLen);
				}
				break;
			case TYPE_LONG:
				ulLen = sizeof(UINT32);
				if (NULL != pValue)
				{
					memcpy(szColumnValue, (UINT8*)(pValue), ulLen);
				}
				else
				{
					memcpy(szColumnValue, (UINT8*)(&pstColumn->ulDefValue), ulLen);
				}
				break;
			case TYPE_BOOL:
				ulLen = sizeof(UINT8);
				if (NULL != pValue)
				{
					memcpy(szColumnValue, (UINT8*)(pValue), ulLen);
				}
				else
				{
					ucValue = (UINT8)pstColumn->ulDefValue;
					memcpy(szColumnValue, (UINT8*)(&ucValue), ulLen);
				}
				break;
			case TYPE_INT:
				ulLen = sizeof(INT32);
				if (NULL != pValue)
				{
					memcpy(szColumnValue, (UINT8*)(pValue), ulLen);
				}
				else
				{
					lValue = (INT32)pstColumn->lDefValue;
					memcpy(szColumnValue, (UINT8*)(&lValue), ulLen);
				}
				break;
			case TYPE_BIN:
				// no
				break;
			case TYPE_IP:
				if (NULL != pValue)
				{
					ulLen = 4;
					memcpy(szColumnValue, pValue, ulLen);
				}
				else
				{
					//
				}
				break;
			case TYPE_MAC:
				if (NULL != pValue)
				{
					ulLen = 6;
					memcpy(szColumnValue, pValue, ulLen);
				}
				else
				{
					//
				}
				break;
			case TYPE_TIME:
				ulLen = sizeof(UINT32);
				if (NULL != pValue)
				{
					memcpy(szColumnValue, (UINT8*)(pValue), ulLen);
				}
				else
				{
					memcpy(szColumnValue, (UINT8*)(&pstColumn->ulDefValue), ulLen);
				}
				break;
			case TYPE_IPV6:
				if (NULL != pValue)
				{
					ulLen = 16;
					memcpy(szColumnValue, pValue, ulLen);
				}
				else
				{
					//
				}
				break;
			case TYPE_OID:
				// no
				break;
			case TYPE_ROWSTATUS:
				// no
				break;
			case TYPE_TADDR:
				if (NULL != pValue)
				{
					ulLen = 6;
					memcpy(szColumnValue, pValue, ulLen);
				}
				else
				{
					//
				}
				break;
			case TYPE_LONGLONG:
				// no
				break;
		}
		
		pstTable->aulColumnState[pstColumn->usColumnID] = SNMP_COLUMN_STATE_SET;
		
	}	

	return 0;
}

/*************************************************************************
* ��������: snmp_get_table_status
* ��    ��: ��ȡ����޸�״̬
* ��    ��: 
* ��������			����/���				����
* ��������: 
* ˵    ��: 
*************************************************************************/
UINT32 snmp_get_table_status(SNMP_TABLE_T *pstTable)
{
    UINT32  i;
    UINT32  ulSetColumnNum = 0;

	UINT32 ulStatus = 0;

//	UINT8  *pValue = NULL;
//	UINT32 ulLen = 0;
	CHAR *szRecValue = NULL;
	CHAR *szColumnValue = NULL;
//	UINT8  ucValue = 0;
//	UINT16 usValue = 0;
//	INT32  lValue = 0;
	SNMP_COLUMN_T *pstColumn = NULL;
	UINT32 ulRowStatus = 0;

    for (i=0; i<pstTable->ulColumnNum; i++)
    {
        if (pstTable->aulColumnState[i] != SNMP_COLUMN_STATE_NULL)
        {
            ulSetColumnNum ++;
        }
    }

    if (ulSetColumnNum == 0)
    {
        ulStatus = SNMP_TABLE_STATE_NULL;
    }
    else if (ulSetColumnNum < pstTable->ulColumnNum)
    {
        ulStatus = SNMP_TABLE_STATE_SET;
    }
    else
    {
        ulStatus = SNMP_TABLE_STATE_SETALL;
    }

	if (SNMP_TABLE_STATE_SET == ulStatus)
	{	
		/* �����ж��Ƿ���CreateAndWait����� */
		pstColumn = &(pstTable->astColumns[pstTable->ulColumnNum-1]);
		if (pstColumn->ucColumnType != TYPE_ROWSTATUS)
		{
			return ulStatus;
		}
		
		szRecValue = (CHAR*)pstTable->pNewTableData;
		if (NULL == szRecValue)
		{
			return ulStatus;
		}
		
		szColumnValue = szRecValue+pstColumn->usOffSet;
		ulRowStatus = *(UINT32 *)(szColumnValue);
		if (ENUM_ROWSTATUS_CREATEANDWAIT == ulRowStatus)
		{
			ulStatus = SNMP_TABLE_STATE_WAIT;
		}
		/* 2011-06-27 fengjing add */
		else if (ENUM_ROWSTATUS_CREATEANDGO == ulRowStatus)
		{
			/* ��Ҫ�����Щδ���õ��� */
			/* ����������У�����ֵ��OID�л�ȡ */
			/* ����Ƿ������У�����Ƿ������ȱʡֵ��
			������ڣ�������ȱʡֵ�����򣬱���*/	
			snmp_set_table_status(pstTable);
			ulStatus = SNMP_TABLE_STATE_SETALL;
		}
	}

	return ulStatus;
}

/*************************************************************************
* ��������: snmp_init_table_add_status
* ��    ��: ��ʼ�����add״̬
* ��    ��: 
* ��������			����/���				����
* ��������: 
* ˵    ��: 
*************************************************************************/
VOID snmp_init_table_add_status(SNMP_TABLE_T *pstTable)
{
    UINT32  i;

    pstTable->ulSetStartTime = 0;

    for (i=0; i<pstTable->ulColumnNum; i++)
    {
        pstTable->aulColumnState[i] = SNMP_COLUMN_STATE_NULL;
    }
}

/*************************************************************************
* ��������: snmp_table_reset_set_status
* ��    ��: set/add������ض�̬������λ
* ��    ��: 
* ��������			����/���				����
* ��������: 
* ˵    ��: 
*************************************************************************/
BOOL snmp_table_reset_set_status(SNMP_TABLE_T *pstTable)
{
    snmp_init_table_add_status(pstTable);

    pstTable->ulTableRecNum = 0;

    return TRUE;
}

/*************************************************************************
* ��������: snmp_get_table_data
* ��    ��: ��ȡ���¼����
* ��    ��: 
* ��������			����/���				����
* ��������: 
* ˵    ��: 
*************************************************************************/
BOOL snmp_get_table_data(SNMP_TABLE_T *pstTable)
{
	UINT32  ulTableStatus;
	UINT32  i;

	UINT32  ulRecMaxNum = 0;
	UINT32  ulRecLen = 0;
//	UINT32  ulRecNum = 0;
	UINT8   *pucData = NULL;
	BOOL    bRet = FALSE;

//	UINT32  ulLoopNo = 0;

	ulTableStatus = snmp_get_table_status(pstTable);
	if (SNMP_TABLE_STATE_NULL != ulTableStatus)
	{
		agent_printf(LOG_INFO, "ulTableStatus is not null\n");
		return TRUE;
	}

	/* �������� */
	if ((pstTable->ulTableID>=TABLE_ID_AP_CONFIG_BEGIN &&
		pstTable->ulTableID<=TABLE_ID_AP_CONFIG_END))
	{		
		if (!OsSemaphoreP(gtSemConfig, WAIT_FOREVER, 1))
    	{
			agent_printf(LOG_ERR,
			"File:%s, Line:%d: gtSemConfig OsSemaphoreP fail\n",
			__FILE__, __LINE__);
        	return FALSE;
    	}
	}
	else if ((pstTable->ulTableID>=TABLE_ID_AP_STAT_BEGIN &&
		pstTable->ulTableID<=TABLE_ID_AP_STAT_END))
	{		
		if (!OsSemaphoreP(gtSemStat, WAIT_FOREVER, 1))
    	{
			agent_printf(LOG_ERR,
			"File:%s, Line:%d: gtSemStat OsSemaphoreP fail\n",
			__FILE__, __LINE__);
        	return FALSE;
    	}
	}
	
	// new add
	bRet = snmp_get_table_reget_status(pstTable);
	if (!bRet)
	{
		agent_printf(LOG_INFO, "snmp_get_table_reget_status() not need reget\n");
		bRet = TRUE;
		goto end;
	}

	ulRecLen = pstTable->ulTableRecLen;
	ulRecMaxNum = pstTable->ulMaxTableRecNum;

	agent_printf(LOG_INFO, "%s: snmp_get_table_data table:%u %s\n", SNMPAGENT_CONTROL_NAME,
		pstTable->ulTableID,
		pstTable->acTableName);
	
	pucData = snmp_get_table_record(pstTable);
	if (pucData == NULL)
	{
		agent_printf(LOG_ERR, "%s:pstTable->acTblName:%s, pucData is null\n", SNMPAGENT_CONTROL_NAME, pstTable->acTableName);
		bRet = FALSE;
		goto end;
	}
	
	if ((pstTable->ulTableID>=TABLE_ID_AP_CONFIG_BEGIN &&
		pstTable->ulTableID <= TABLE_ID_AP_CONFIG_END))
	{
		if (pstTable->pTableData == NULL)
		{				
			pstTable->pTableData = SNMP_AGENT_MALLOC(ulRecLen*ulRecMaxNum);

			if (pstTable->bIsTable)
			{
				if(NULL == pstTable->pNewTableData)
				{
					pstTable->pNewTableData = SNMP_AGENT_MALLOC(pstTable->ulTableRecLen);
				}
				memset(pstTable->pNewTableData, 0, pstTable->ulTableRecLen);
			}
		}
	}
	else if ((pstTable->ulTableID>=TABLE_ID_AP_STAT_BEGIN &&
		      pstTable->ulTableID <= TABLE_ID_AP_STAT_END))
	{			
		if (NULL == pstTable->pTableData)
		{
			pstTable->pTableData = pucData;
		}
	}

	if (pstTable->pTableData == NULL)
	{
		agent_printf(LOG_ERR, "%s:pstTable->pTableData is null\n", SNMPAGENT_CONTROL_NAME);
		bRet = FALSE;
		goto end;
	}

    /* ��ȡ���¼ */
	if ((pstTable->ulTableID>=TABLE_ID_AP_CONFIG_BEGIN &&
		pstTable->ulTableID <= TABLE_ID_AP_CONFIG_END))
	{
		memset(pstTable->pTableData, 0, ulRecLen*ulRecMaxNum);
		pstTable->pOriTableData = pucData;
	}
	else if ((pstTable->ulTableID>=TABLE_ID_AP_STAT_BEGIN &&
		      pstTable->ulTableID <= TABLE_ID_AP_STAT_END))
	{
	}
	
	pstTable->ulTableRecNum = 0;
	for (i=0; i<ulRecMaxNum; i++)
	{
	    agent_printf(LOG_INFO, "%s:table %s pucData %p ulRecLen:%u  key:%u  key addr %p i:%u\n", SNMPAGENT_CONTROL_NAME, 
		                pstTable->acTableName,pucData,ulRecLen,*(UINT32 *)(UINT8 *)(pucData+i*ulRecLen),pucData+i*ulRecLen,
			   i);
        
		if (pstTable->pfKeyValidFunc(pstTable->ulTableID, pstTable->ulTableRecLen, pucData+i*ulRecLen))
		{
			if ((pstTable->ulTableID>=TABLE_ID_AP_CONFIG_BEGIN &&
				pstTable->ulTableID <= TABLE_ID_AP_CONFIG_END))
			{
				memcpy(pstTable->pTableData+pstTable->ulTableRecNum*(pstTable->ulTableRecLen), 
					pucData+i*ulRecLen, ulRecLen);
			}
		    pstTable->ulTableRecNum ++;				
		}
		else
		{
			agent_printf(LOG_INFO, "%s:table %s pfKeyValidFunc failed:%u\n", SNMPAGENT_CONTROL_NAME, 
		                pstTable->acTableName,
			   i);
			break;
		}
	}

	snmp_set_table_reget_status(pstTable);
	bRet = TRUE;

end:
	if ((pstTable->ulTableID>=TABLE_ID_AP_CONFIG_BEGIN &&
		pstTable->ulTableID<=TABLE_ID_AP_CONFIG_END))
	{	
		OsSemaphoreV(gtSemConfig, 1);
	}
	else if ((pstTable->ulTableID>=TABLE_ID_AP_STAT_BEGIN &&
		pstTable->ulTableID<=TABLE_ID_AP_STAT_END))
	{		
		OsSemaphoreV(gtSemStat, 1);
	}
	
	return bRet;
}

/*************************************************************************
* ��������: snmp_table_set_commit
* ��    ��: ��set������Ч
* ��    ��: 
* ��������			����/���				����
* ��������: 
* ˵    ��: 
*************************************************************************/
BOOL snmp_table_set_commit(SNMP_TABLE_T *pstTable, char *szOperator)
{
	CHAR    *szRecDate = NULL;
	CHAR    *szOriRecDate = NULL;
		
	szRecDate = pstTable->pTableData;
	szOriRecDate = pstTable->pOriTableData;


	if (pstTable->pfTableValidFunc != NULL)
	{
		if (!pstTable->pfTableValidFunc(szRecDate, NULL))
		{
			agent_printf(LOG_ERR, "%s:snmp_table_set_commit pfTableValidFunc failed\n",
				SNMPAGENT_CONTROL_NAME);			
			return FALSE;
		}
	}
	
	snmp_reset_table_reget_status(pstTable->ulTableID);
	
    return TRUE;
}


/*************************************************************************
* ��������: snmp_table_add_commit
* ��    ��: ��add������Ч
* ��    ��: 
* ��������			����/���				����
* ��������: 
* ˵    ��: 
*************************************************************************/
BOOL snmp_table_add_commit(SNMP_TABLE_T *pstTable, char *szOperator)
{
	CHAR    *szRecDate = NULL;
	
	szRecDate = pstTable->pNewTableData;
	if (pstTable->pfKeyValidFunc(pstTable->ulTableID, pstTable->ulTableRecLen, szRecDate))
	{
		if (pstTable->pfTableValidFunc != NULL)
		{
			if(!pstTable->pfTableValidFunc(szRecDate, pstTable->pOriTableData))
			{
				agent_printf(LOG_ERR, "%s:snmp_table_add_commit pfTableValidFunc failed\n",
					SNMPAGENT_CONTROL_NAME);
				return FALSE;
			}
		}
	}

	snmp_reset_table_reget_status(pstTable->ulTableID);
	
	return TRUE;
}


/*************************************************************************
* ��������: snmp_table_add_wait
* ��    ��: ��add�����ȴ�
* ��    ��: 
* ��������			����/���				����
* ��������: 
* ˵    ��: 
*************************************************************************/
BOOL snmp_table_add_wait(SNMP_TABLE_T *pstTable, char *szOperator)
{
	return TRUE;
}

/*************************************************************************
* ��������: snmp_is_table_add_timeout
* ��    ��: add�����Ƿ�ʱ
* ��    ��: 
* ��������			����/���				����
* ��������: 
* ˵    ��: 
*************************************************************************/
BOOL snmp_is_table_add_timeout(SNMP_TABLE_T *pstTable)
{
    UINT32  ulTableStatus;
    UINT32  ulCurrTime;

    ulTableStatus = snmp_get_table_status(pstTable);
    if (SNMP_TABLE_STATE_SET != ulTableStatus)
    {
        return FALSE;
    }

    ulCurrTime = snmp_get_systime();
    if (ulCurrTime < pstTable->ulSetStartTime + SNMP_TIMEOUT)
    {
        return FALSE;
    }

    return TRUE;
}

/*************************************************************************
* ��������: snmp_compare_column_value
* ��    ��: SNMP���޸ļ�¼��
* ��    ��: 
* ��������			����/���				����
* ��������: 
* ˵    ��: 
*************************************************************************/
BOOL snmp_compare_column_value(UINT32     ulColumnType,
                     UINT32     ulColumnLen,
                     VOID       *pInValue,
                     UINT32     ulInLen,
                     VOID       *pOutValue)
{	
    BOOL    bRet = FALSE;

    if (NULL == pInValue || NULL == pOutValue)
    {
        agent_printf(LOG_ERR, "snmp_compare_column_value: pInValue or pOutValue is null");
        return bRet;
    }
	
	switch(ulColumnType)
    {
        case TYPE_CHAR:
			if (ulInLen > ulColumnLen)
			{
			    agent_printf(LOG_WARNING, "snmp_set_column: ulInLen(%u) is too big(%u)", ulInLen, ulColumnLen);
			}
			if (memcmp(pInValue, pOutValue, (ulInLen>ulColumnLen? ulColumnLen : ulInLen)) == 0)
			{
				bRet = TRUE;
			}		
			break;

        case TYPE_BYTE:
            /* BEGIN: Modified by Zhou Shouya, 2013/8/9   PN:VLAN */
			//if (ulInLen == sizeof(UINT32) && ulColumnLen == sizeof(UINT8))
            if (ulInLen <= sizeof(UINT8) && ulColumnLen == sizeof(UINT8))
            /* END:   Modified by Zhou Shouya, 2013/8/9   PN:VLAN */
			{
				if (memcmp(pInValue, pOutValue, (ulInLen>ulColumnLen? ulColumnLen : ulInLen)) == 0)
				{
					bRet = TRUE;
				}
			}
            break;

        case TYPE_SHORT:
            /* BEGIN: Modified by Zhou Shouya, 2013/8/9   PN:VLAN */
            if(ulInLen < sizeof(UINT16))
            {
                *((UINT16 *)pInValue)>>=((sizeof(UINT16)-ulInLen)*BYTE_BITNUM);
            }
			//if (ulInLen == sizeof(UINT32) && ulColumnLen == sizeof(UINT16))
            if (ulInLen <= sizeof(UINT16) && ulColumnLen == sizeof(UINT16))
            /* END:   Modified by Zhou Shouya, 2013/8/9   PN:VLAN */
			{
				if (memcmp(pInValue, pOutValue, (ulInLen>ulColumnLen? ulColumnLen : ulInLen)) == 0)
				{
					bRet = TRUE;
				}
			}
            break;

        case TYPE_LONG:
            /* BEGIN: Modified by Zhou Shouya, 2013/8/9   PN:VLAN */
            if(ulInLen < sizeof(UINT32))
            {
                *((UINT32 *)pInValue)>>=((sizeof(UINT32)-ulInLen)*BYTE_BITNUM);
            }
			//if (ulInLen == sizeof(UINT32) && ulColumnLen == sizeof(UINT32))
            if (ulInLen <= sizeof(UINT32) && ulColumnLen == sizeof(UINT32))
            /* END:   Modified by Zhou Shouya, 2013/8/9   PN:VLAN */
			{
				if (memcmp(pInValue, pOutValue, (ulInLen>ulColumnLen? ulColumnLen : ulInLen)) == 0)
				{
					bRet = TRUE;
				}
			}
            break;

		case TYPE_LONGLONG:
            /* BEGIN: Modified by Zhou Shouya, 2013/8/9   PN:VLAN */
            if(ulInLen < sizeof(UINT64))
            {
                *((UINT64 *)pInValue)>>=((sizeof(UINT64)-ulInLen)*BYTE_BITNUM);
            }
			//if (ulInLen == sizeof(UINT64) && ulColumnLen == sizeof(UINT64))
			if (ulInLen <= sizeof(UINT64) && ulColumnLen == sizeof(UINT64))
            /* END:   Modified by Zhou Shouya, 2013/8/9   PN:VLAN */
			{
				if (memcmp(pInValue, pOutValue, (ulInLen>ulColumnLen? ulColumnLen : ulInLen)) == 0)
				{
					bRet = TRUE;
				}
			}
			break;

		/* 2010/07/02 fengjing cmcc test */
		case TYPE_INT:
		    /* BEGIN: Modified by Zhou Shouya, 2013/8/9   PN:VLAN */
            if(ulInLen < sizeof(INT32))
            {
                *((INT32 *)pInValue)>>=((sizeof(INT32)-ulInLen)*BYTE_BITNUM);
            }
			//if (ulInLen == sizeof(INT32) && ulColumnLen == sizeof(INT32))
			if (ulInLen <= sizeof(INT32) && ulColumnLen == sizeof(INT32))
		    /* END:   Modified by Zhou Shouya, 2013/8/9   PN:VLAN */
			{
				if (memcmp(pInValue, pOutValue, (ulInLen>ulColumnLen? ulColumnLen : ulInLen)) == 0)
				{
					bRet = TRUE;
				}
			}
			break;

        case TYPE_BIN:
			if (ulInLen == ulColumnLen)
			{
				if (memcmp(pInValue, pOutValue, (ulInLen>ulColumnLen? ulColumnLen : ulInLen)) == 0)
				{
					bRet = TRUE;
				}
			}
            break;

        case TYPE_IP:
			if (ulInLen == ulColumnLen && ulColumnLen == 4)
			{
				if (memcmp(pInValue, pOutValue, (ulInLen>ulColumnLen? ulColumnLen : ulInLen)) == 0)
				{
					bRet = TRUE;
				}
			}
            break;
		case TYPE_IPV6:
			if (ulInLen == ulColumnLen && ulColumnLen == 16)
			{
				if (memcmp(pInValue, pOutValue, (ulInLen>ulColumnLen? ulColumnLen : ulInLen)) == 0)
				{
					bRet = TRUE;
				}
			}
			break;

        case TYPE_MAC:
			if (ulInLen == ulColumnLen && ulColumnLen == 6)
			{
				if (memcmp(pInValue, pOutValue, (ulInLen>ulColumnLen? ulColumnLen : ulInLen)) == 0)
				{
					bRet = TRUE;
				}
			}
            break;

        default:
            break;
    }

	return  bRet;
}


/*************************************************************************
* ��������: snmp_table_set_column
* ��    ��: SNMP���޸ļ�¼��
* ��    ��: 
* ��������			����/���				����
* ��������: 
* ˵    ��: 
*************************************************************************/
BOOL snmp_table_set_column(void *data_context,
                           data_t *var,
                           SNMP_TABLE_T *pstTable,
                           UINT16   usColumnID)
{
    UINT32          i;
    CHAR            *szColumnValue;
    SNMP_COLUMN_T   *pstColumn = NULL;
    BOOL            bRet = TRUE;
    BOOL            bIsKeyColumn = FALSE;

    /* ��ȡ��������Ϣ */
    //pstTable = snmp_get_table_by_oid(pstOID, usOIDSize);
    if (NULL == pstTable)
    {
        return FALSE;
    }

    /* ��ȡ�ֶ�������Ϣ */
    for (i=0; i<pstTable->ulColumnNum; i++)
    {
        if (pstTable->astColumns[i].usColumnID == usColumnID)
        {
            pstColumn = &pstTable->astColumns[i];
            break;
        }
    }

    if (NULL == pstColumn)
    {
        return FALSE;
    }

    /* �ж��Ƿ��������ֶ� */
    for (i=0; i<pstTable->ulKeyColumnNum; i++)
    {
        if (pstTable->apstKeyColumns[i]->usColumnID == usColumnID)
        {
            bIsKeyColumn = TRUE;
            break;
        }
    }

    szColumnValue = ((CHAR*)data_context);
    szColumnValue += pstColumn->usOffSet;
	
	if (pstColumn->ucMibOper == MIB_R)
	{
		bRet = snmp_compare_column_value(pstColumn->ucColumnType, pstColumn->usColumnLen, 
			var->buffer, 
			var->encoded_length, szColumnValue);
		if (FALSE == bRet)
		{			
			agent_printf(LOG_ERR, "snmp_table_set_column() table:%s column:%u value changed.\n",
				pstTable->acTableName,
			pstColumn->usColumnID);
		}

		return bRet;
	}

    bRet = snmp_set_column(pstColumn->ucColumnType, pstColumn->usColumnLen, 
		var->buffer, 
		var->encoded_length, szColumnValue);

    return bRet;
}

/*************************************************************************
* ��������: snmp_table_add_column
* ��    ��: SNMP�����Ӽ�¼�ֶ�
* ��    ��: 
* ��������			����/���				����
* ��������: 
* ˵    ��: 
*************************************************************************/
BOOL snmp_table_add_column(data_t* var,
                           SNMP_TABLE_T *pstTable,
                           UINT16   usColumnID,
                           unsigned int      *pstIndeOID,
                           UINT16   usIndexOIDSize)
{
    UINT32          i;
    CHAR            *szRecValue;
    CHAR            *szColumnValue;
    SNMP_COLUMN_T   *pstColumn = NULL;
//    VOID            *pRecData = NULL;
    BOOL            bRet = TRUE;
	
	UINT32 ulIndex = 0;
	UINT32 ulLen = 0;
	UINT32 j=0;
	CHAR   acIndex[MAX_OID_LEN] = {0};
	BOOL   bFlag = FALSE;

    /* ��ȡ��������Ϣ */
    //pstTable = snmp_get_table_by_oid(pstOID, usOIDSize);
    if (NULL == pstTable)
    {
		agent_printf(LOG_ERR, "snmp_get_table_by_oid failed\n");
        return FALSE;
    }

	if (pstTable->ulTableRecNum > pstTable->ulMaxTableRecNum)
    {
		agent_printf(LOG_ERR, "pstTable->ulTableRecNum:%u larger than pstTable->ulMaxTableRecNum:%u\n",
			pstTable->ulTableRecNum,
			pstTable->ulMaxTableRecNum);
        return FALSE;
    }

    /* ��ȡ�ֶ�������Ϣ */
    for (i=0; i<pstTable->ulColumnNum; i++)
    {
    	 if (pstTable->astColumns[i].usColumnID == usColumnID)
        {
            pstColumn = &pstTable->astColumns[i];
            break;
        }
    }

    if (NULL == pstColumn)
    {
		agent_printf(LOG_ERR, "get column by columnId:%u of table %s failed\n",
			usColumnID,
			pstTable->acTableName);
        return FALSE;
    }

    /* �ж��Ƿ��Ѿ���������ʱ��¼ */
    if (NULL == pstTable->pNewTableData)
    {
		agent_printf(LOG_ERR, "pstTable->pNewTableData is null\n");
        return FALSE;
    }

    szRecValue = (CHAR*)pstTable->pNewTableData;
	szColumnValue = szRecValue + pstColumn->usOffSet;

	if (pstColumn->ucColumnType == TYPE_ROWSTATUS &&
		pstTable->aulColumnState[usColumnID] == SNMP_COLUMN_STATE_SET)
	{		
		agent_printf(LOG_ERR, "ulColumnState=%u,szColumnValue=%u, val=%u\n",
			pstTable->aulColumnState[usColumnID],
			*(UINT32 *)szColumnValue,
			*(UINT32 *)(var->buffer));
		
		if (*(UINT32 *)szColumnValue == ENUM_ROWSTATUS_CREATEANDWAIT &&
		*(UINT32 *)(var->buffer) == ENUM_ROWSTATUS_ACTIVE)
		{
			agent_printf(LOG_INFO, "snmp_set_column CreateAndWait to Active\n");		
			bFlag = TRUE;
		}
	}

    /* ���ò��� */
    bRet = snmp_set_column(pstColumn->ucColumnType, pstColumn->usColumnLen, 
    	var->buffer, var->encoded_length, szColumnValue);
    if (bRet)
    {
		agent_printf(LOG_INFO, "snmp_set_column succeed\n");
		
        pstTable->aulColumnState[usColumnID] = SNMP_COLUMN_STATE_SET;
        if (pstTable->ulSetStartTime == 0)
        {
            pstTable->ulSetStartTime = snmp_get_systime();
        }
    }
    else
    {
		agent_printf(LOG_ERR, "snmp_set_column failed\n");
        pstTable->aulColumnState[usColumnID] = SNMP_COLUMN_STATE_NULL;
    }
	
	/* 2011-06-27 fengjing add */
	/* �ж��Ƿ��������ֶΣ�����ǣ���Ҫ��鸳ֵ�Ƿ���OID
	�е�����ֵһ�¡������һ�£��򷵻ش���
	���һ�£�����*/
	/* ������������ֶΣ���������ֶ��Ƿ��Ѿ�����ֵ��
	���û�У���Ҫ��OID�д��ŵ�����ֵ�������ֶν�������ֵ*/
	for (i=0;i<pstTable->ulKeyColumnNum; i++)
	{
		if (pstTable->apstKeyColumns[i]->usColumnID == usColumnID)
		{
			break;
		}
	}

	if (i !=pstTable->ulKeyColumnNum)
	{		
		if (i == 0)
		{
			ulIndex = 0;
		}
	
		/* �����ֶ� */
		switch(pstColumn->ucColumnType)
		{
			case TYPE_CHAR:	
				ulLen = pstIndeOID[ulIndex];
				ulIndex++;
				memset(acIndex, 0, sizeof(MAX_OID_LEN));
				for (j=0;j<ulLen;j++)
				{
					acIndex[j] = pstIndeOID[ulIndex++];
				}
				if (!snmp_compare_column_value(pstColumn->ucColumnType,
					pstColumn->usColumnLen,
					acIndex,
					ulLen,
					szColumnValue))
				{
					agent_printf(LOG_ERR, "szColumnValue=%s, pstIndexOID=%s",
						szColumnValue,
						acIndex);
					return FALSE;
				}
				break;
			case TYPE_LONG:
				if (!snmp_compare_column_value(pstColumn->ucColumnType,
					pstColumn->usColumnLen,
					&(pstIndeOID[ulIndex]),
					sizeof(UINT32),
					szColumnValue))
				{
					agent_printf(LOG_ERR, "szColumnValue=%u, pstIndexOID=%u",
						*(UINT32*)szColumnValue,
						pstIndeOID[ulIndex]);
					ulIndex++;
					return FALSE;
				}
				ulIndex++;
				break;
			default:
				agent_printf(LOG_ERR, "invalid key column type:%u.", pstColumn->ucColumnType);
				return FALSE;
		}
	}
	else
	{
		ulIndex = 0;
		
		for (i=0;i<pstTable->ulKeyColumnNum; i++)
		{
			pstColumn = pstTable->apstKeyColumns[i];			
    		szColumnValue = szRecValue + pstColumn->usOffSet;

			if (pstTable->aulColumnState[pstColumn->usColumnID] == SNMP_COLUMN_STATE_NULL)
			{
				/* δ��������ֵ */
				switch(pstColumn->ucColumnType)
				{
					case TYPE_CHAR:
						ulLen = pstIndeOID[ulIndex];
						ulIndex++;
						memset(acIndex, 0, sizeof(MAX_OID_LEN));
						for (j=0;j<ulLen;j++)
						{
							acIndex[j] = pstIndeOID[ulIndex++];
						}
						if (!snmp_set_column(pstColumn->ucColumnType,
							pstColumn->usColumnLen,
							acIndex,
							ulLen,
							szColumnValue))
						{
							agent_printf(LOG_ERR, "szColumnValue=%s, pstIndexOID=%s",
								szColumnValue,
								acIndex);
							return FALSE;
						}
						break;
					case TYPE_LONG:
						if (!snmp_set_column(pstColumn->ucColumnType,
							pstColumn->usColumnLen,
							&(pstIndeOID[ulIndex]),
							sizeof(UINT32),
							szColumnValue))
						{
							ulIndex++;
							return FALSE;
						}
						ulIndex++;
						break;
					default:
						return FALSE;
				}		
					
				pstTable->aulColumnState[pstColumn->usColumnID] = SNMP_COLUMN_STATE_SET;
			}
			else
			{
				switch(pstColumn->ucColumnType)
				{
					case TYPE_CHAR:
						ulLen = pstIndeOID[ulIndex];
						ulIndex++;
						ulIndex+=ulLen;
						break;
					case TYPE_LONG:
						ulIndex++;
						break;
					default:
						return FALSE;
				}				
			}
		}
	}

	/* ����Wait����������RowStatus��CreateAndWait->Active��
	��Ҫ��û�������ϵ������ֶ����ú�*/
	if (bFlag)
	{
		agent_printf(LOG_ERR, "snmp_table_add_column  bFlag TRUE.");
		snmp_set_table_status(pstTable);
	}
	else
	{		
		agent_printf(LOG_ERR, "snmp_table_add_column  bFlag FALSE.");
	}
	
    return bRet;
}


/*************************************************************************
* ��������: snmp_set_table_status
* ��    ��: ��ȡ����޸�״̬
* ��    ��: 
* ��������			����/���				����
* ��������: 
* ˵    ��: 
*************************************************************************/
BOOL snmp_table_get_column(SNMP_TABLE_T *pstTable, UINT32 ulColumnIndex, oid_t *row, CHAR * pValueAddr, response_t *response, client_t *client)
{	
	BOOL bRet = FALSE;
	
	bRet = snmp_get_column(pstTable, ulColumnIndex, row, pValueAddr, response, client);
	if (FALSE == bRet)
	{
	}

	return bRet;
}

/*************************************************************************
* ��������: snmp_set_table_status
* ��    ��: ��ȡ����޸�״̬
* ��    ��: 
* ��������			����/���				����
* ��������: 
* ˵    ��: 
*************************************************************************/
VOID * snmp_extract_iterator_context(SNMP_TABLE_T *pstTable, request_t *request, int i, UINT32 *pj)
{
//	UINT16 usColumnLen = 0;
	
	UINT32 ulOffset = 0;
	UINT32 ulStrLen = 0;
	
	UINT32 ulKeyColumnNum = 0;
	UINT16 usKeyColumnType = 0;
	CHAR  *pValueAddr = NULL;
	
	UINT32 j=0;
	UINT32 k=0;
	
	CHAR   aacKey[2][64];
	UINT32 aulKey[4] = {0};
	UINT32 m=0;
	UINT32 n=0;
    UINT32 loop1 = 0;
    UINT32 loop2 = 0;
    UINT32 loop3 = 0;
    UINT32 loop4 = 0;
    UINT8 aucKeyMac[2][6]={0};
    UINT8 aucKeyIp[2][4]={0};

	memset(aacKey, 0, sizeof(aacKey));
		
	ulOffset = pstTable->ulOIDSize;	

	// entry node
	ulOffset++;

	// column node
	ulOffset++;

	// row index
	pValueAddr = pstTable->pTableData;
	ulKeyColumnNum = pstTable->ulKeyColumnNum;
	for(j=0;j<ulKeyColumnNum;j++)
	{
		usKeyColumnType = pstTable->apstKeyColumns[j]->ucColumnType;
		switch(usKeyColumnType)
		{
			case TYPE_CHAR:
				/*for example:mac */
				if(ulOffset>=request->oid_list[i].subid_list_length)
				{
					return NULL;
				}
				ulStrLen = request->oid_list[i].subid_list[ulOffset];
				ulOffset++;
				for(k=0;k<ulStrLen;k++)
				{
					if((ulOffset+k)>=request->oid_list[i].subid_list_length)
					{
						return NULL;
					}
					aacKey[m][k] = (char)request->oid_list[i].subid_list[ulOffset+k];
				}
				ulOffset+= ulStrLen;
				m++;
				break;
			case TYPE_LONG:
				/*for example:id*/
				if(ulOffset>=request->oid_list[i].subid_list_length)
				{
					return NULL;
				}
				aulKey[n] = request->oid_list[i].subid_list[ulOffset];
				ulOffset++;
				n++;
				break;
            /* BEGIN: Added by zoucaihong, 2015/3/27 */
            case TYPE_MAC:
				if(ulOffset>=request->oid_list[i].subid_list_length)
				{
					return NULL;
				}
				for(loop1=0;loop1<6;loop1++)
				{
					if((ulOffset+loop1)>=request->oid_list[i].subid_list_length)
					{
						return NULL;
					}
					aucKeyMac[loop2][loop1] = (UINT8)request->oid_list[i].subid_list[ulOffset+loop1];
				}
				ulOffset+= 6;
				loop2++;
				break;
            case TYPE_IP:
				if(ulOffset>=request->oid_list[i].subid_list_length)
				{
					return NULL;
				}
				for(loop3=0;loop3<4;loop3++)
				{
					if((ulOffset+loop3)>=request->oid_list[i].subid_list_length)
					{
						return NULL;
					}
					aucKeyIp[loop4][loop3] = (UINT8)request->oid_list[i].subid_list[ulOffset+loop3];
				}
				ulOffset+= 4;
				loop4++;
				break;
            /* END: Added by zoucaihong, 2015/3/27 */
			default:
				break;
		}
	}

	#if 0
	agent_printf(LOG_INFO, "m=%u", m);
	for(j=0;j<pstTable->ulTableRecNum;j++)
	{
		agent_printf(LOG_INFO,"%s,", aacKey[j]);		
	}
	
	agent_printf(LOG_INFO,"n=%u", n);
	for(j=0;j<pstTable->ulTableRecNum;j++)
	{
		agent_printf(LOG_INFO, "%u,", aulKey[j]);		
	}
	#endif

	agent_printf(LOG_INFO, "snmp_extract_iterator_context() oid:%s", oid_ntoa(&(request->oid_list[i])));
	
	for(j=0;j<pstTable->ulTableRecNum;j++)
	{		
		for(k=0;k<m;k++)
		{
			if(strcmp(pValueAddr+pstTable->apstKeyColumns[k]->usOffSet,
				aacKey[k]))
			{
				break;
			}
		}

		if(k != m)
		{
			pValueAddr+=pstTable->ulTableRecLen;
			continue;
		}

		for(k=0;k<n;k++)
		{
			if(*(UINT32 *)(pValueAddr+pstTable->apstKeyColumns[k]->usOffSet) != aulKey[k])
			{
				break;
			}
		}
		
		if(k != n)
		{
			pValueAddr+=pstTable->ulTableRecLen;
			continue;
		}

        for ( loop1 = 0 ; loop1 < loop2 ; loop1++ )
        {
			if(memcmp(pValueAddr+pstTable->apstKeyColumns[loop1]->usOffSet,
				aucKeyMac[loop1], 6))
			{
				break;
			}
        }
		if(loop1 != loop2)
		{
			pValueAddr+=pstTable->ulTableRecLen;
			continue;
		}
        for ( loop3 = 0 ; loop3 < loop4 ; loop3++ )
        {
			if(memcmp(pValueAddr+pstTable->apstKeyColumns[loop3]->usOffSet,
				aucKeyIp[loop3], 4))
			{
				break;
			}
        }
		if(loop3 != loop4)
		{
			pValueAddr+=pstTable->ulTableRecLen;
			continue;
		}

		break;				
	}

	if (j == pstTable->ulTableRecNum)
	{
		return NULL;
	}

	*pj = j;
	return pValueAddr;
}

/*************************************************************************
* ��������: snmp_set_table_status
* ��    ��: ��ȡ����޸�״̬
* ��    ��: 
* ��������			����/���				����
* ��������: 
* ˵    ��: 
*************************************************************************/
BOOL snmp_table_check_row_integrity(SNMP_TABLE_T *pstTable, oid_t *row)
{
	UINT32 ulOffset = 0;
	UINT32 ulStrLen = 0;
	
	UINT32 ulKeyColumnNum = 0;
	UINT16 usKeyColumnType = 0;
	CHAR  *pValueAddr = NULL;
	
	UINT32 j=0;
	UINT32 k=0;
	
	CHAR   aacKey[2][64];
	UINT32 aulKey[4] = {0};
	UINT32 m=0;
	UINT32 n=0;

    /* BEGIN: Added by zoucaihong, 2015/3/27 */
    UINT32 loop1=0;
    UINT32 loop2=0;
    UINT32 loop3=0;
    UINT32 loop4=0;
    UINT32 aulKeyMac[2][6]={0};
    UINT32 aulKeyIp[2][4]={0};
    /* END: Added by zoucaihong, 2015/3/27 */

	memset(aacKey, 0, sizeof(aacKey));
		
	//ulOffset = pstTable->ulOIDSize;	

	// entry node
	//ulOffset++;

	// column node
	//ulOffset++;
	
	// row index
	pValueAddr = pstTable->pTableData;
	ulKeyColumnNum = pstTable->ulKeyColumnNum;
	for(j=0;j<ulKeyColumnNum;j++)
	{
		usKeyColumnType = pstTable->apstKeyColumns[j]->ucColumnType;
		switch(usKeyColumnType)
		{
			case TYPE_CHAR:
				/*for example:mac */
				if(ulOffset>=row->subid_list_length)
				{
					return FALSE;
				}
				ulStrLen = row->subid_list[ulOffset];
				ulOffset++;
				for(k=0;k<ulStrLen;k++)
				{
					if((ulOffset+k)>=row->subid_list_length)
					{
						return FALSE;
					}
					aacKey[m][k] = (char)row->subid_list[ulOffset+k];
				}
				ulOffset+= ulStrLen;
				m++;
				break;
			case TYPE_LONG:
				/*for example:id*/
				if(ulOffset>=row->subid_list_length)
				{
					return FALSE;
				}
				aulKey[n] = row->subid_list[ulOffset];
				ulOffset++;
				n++;
				break;
            /* BEGIN: Added by zoucaihong, 2015/3/27 */
            case TYPE_MAC:
				if(ulOffset>=row->subid_list_length)
				{
					return FALSE;
				}
				for(loop1=0;loop1<6;loop1++)
				{
					if((ulOffset+loop1)>=row->subid_list_length)
					{
						return FALSE;
					}
					aulKeyMac[loop2][loop1] = row->subid_list[ulOffset+loop1];
				}
				ulOffset+= 6;
				loop2++;
				break;
            case TYPE_IP:
				if(ulOffset>=row->subid_list_length)
				{
					return FALSE;
				}
				for(loop3=0;loop3<4;loop3++)
				{
					if((ulOffset+loop3)>=row->subid_list_length)
					{
						return FALSE;
					}
					aulKeyMac[loop4][loop3] = row->subid_list[ulOffset+loop3];
				}
				ulOffset+= 4;
				loop4++;
				break;
            /* END: Added by zoucaihong, 2015/3/27 */
			default:
				break;
		}
	}

	if(ulOffset == row->subid_list_length)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*************************************************************************
* ��������: snmp_set_table_status
* ��    ��: ��ȡ����޸�״̬
* ��    ��: 
* ��������			����/���				����
* ��������: 
* ˵    ��: 
*************************************************************************/
int snmp_table_handler(SNMP_TABLE_T *pstTable, request_t *request, int i, response_t *response, client_t *client)
{
	UINT32 ulColumnIndex = 0;
	UINT16 usColumnType = 0;
	UINT16 usColumnLen = 0;
//	UINT32 ulKeyColumnNum = 0;
	UINT32 j=0;

//	UINT16 usKeyColumnType = 0;
	CHAR  *pValueAddr = NULL;

	UINT32 ulOffset = 0;
//	UINT32 ulStrLen = 0;
//	UINT32 k=0;
//	CHAR   aacKey[2][64] = {0};
//	UINT32 aulKey[4] = 0;
//	UINT32 m=0;
//	UINT32 n=0;

	SNMP_TABLE_T *pstNextTable;	
	oid_t row;

    void *data_context = NULL;
	BOOL  bRet = FALSE;
	BOOL  bSetTable = FALSE;
	BOOL  bAddTable = FALSE;
	BOOL  bFreeData = FALSE;

	UINT32 ulTableStatus = 0;

	CHAR *szOperator = NULL;
	BOOL bPermitSet = FALSE;
	
	/* �ж��Ƿ�ʱ */
	if (snmp_is_table_add_timeout(pstTable))
	{
		snmp_table_reset_set_status(pstTable);
	}

	/* ��ȡtable��¼ */
	if (!snmp_get_table_data(pstTable))
	{
		agent_printf(LOG_ERR, "snmp_get_table_data:pstTable->acTableName:%s failed\n", pstTable->acTableName);
        goto generr;
	}
		
	pValueAddr = pstTable->pTableData;

	ulOffset = pstTable->ulOIDSize;

	//entry node
	ulOffset++;
	
	// column node
	if(request->oid_list[i].subid_list_length>ulOffset)
	{
		ulColumnIndex = request->oid_list[i].subid_list[ulOffset] - 1;
		if (ulColumnIndex >= pstTable->ulColumnNum)
		{
			agent_printf(LOG_INFO, "table:%s ulColumnIndex:%u >= pstTable->ulColumnNum:%u\n",
				pstTable->acTableName,
				ulColumnIndex,
				pstTable->ulColumnNum);
			goto generr;
		}
		usColumnType = pstTable->astColumns[ulColumnIndex].ucColumnType;
		usColumnLen = pstTable->astColumns[ulColumnIndex].usColumnLen;
		
		ulOffset++;
	}

	agent_printf(LOG_INFO, "oid:%s", oid_ntoa(&(request->oid_list[i])));
	agent_printf(LOG_INFO, "acTableName:%s ulColumnIndex:%u", pstTable->acTableName, ulColumnIndex);

	// row index
	if(request->oid_list[i].subid_list_length>ulOffset)
	{
		row.subid_list_length = request->oid_list[i].subid_list_length-ulOffset;
		memcpy(row.subid_list, 
			&(request->oid_list[i].subid_list[ulOffset]),
			sizeof(unsigned int)*(row.subid_list_length));
		agent_printf(LOG_INFO, "row oid:%s", oid_ntoa(&(row)));

		/*need check row validity(integrity)*/
		if(snmp_table_check_row_integrity(pstTable, &row))
		{
			bPermitSet = TRUE;
		}
	}
	
	switch(request->type)
	{
		case BER_TYPE_SNMP_GET:
			if(!bPermitSet)
			{
            	goto nosuchinstance;				
			}
			data_context = snmp_extract_iterator_context(pstTable, request, i, &j);
			if (data_context == NULL)
            {
            	goto nosuchinstance;
            }
			
			memcpy(&response->value_list[response->value_list_length].oid,
				&request->oid_list[i], sizeof (request->oid_list[i]));
			bRet = snmp_table_get_column(pstTable, ulColumnIndex, &row, data_context, response, client);
			if (!bRet)
            {
				lprintf(LOG_ERR, "File:%s, Line:%d: snmp_table_get_column failed\n",
					__FILE__, __LINE__);
            	goto generr;
            }

			break;
		case BER_TYPE_SNMP_GETNEXT:
		case BER_TYPE_SNMP_GETBULK:
			data_context = snmp_extract_iterator_context(pstTable, request, i, &j);
			if (data_context == NULL)
			{
				/* check exist records,
				   if exists,first record,first column value;
				   else,next table
				   */
				agent_printf(LOG_INFO, "check exist record:NULL");
				agent_printf(LOG_INFO, "ulTableRecNum:%u", pstTable->ulTableRecNum);
				if(0 == pstTable->ulTableRecNum)
				{
					goto getnext;
				}
				snmp_row_build(pstTable, ulColumnIndex, pValueAddr, &row);
				bRet = snmp_table_get_column(pstTable, ulColumnIndex, &row, pValueAddr, response, client);
				if (!bRet)
	            {
					lprintf(LOG_ERR, "File:%s, Line:%d: snmp_table_get_column failed\n",
						__FILE__, __LINE__);
	            	goto generr;
	            }
			}
			else
			{
				if (bPermitSet)
				{
					if (j != (pstTable->ulTableRecNum-1))
					{
						/*not the last record*/				
						data_context+=pstTable->ulTableRecLen;		
					}
					else
					{
						/* is the last record */
						if(ulColumnIndex != (pstTable->ulColumnNum-1))
						{
							/*last record,non-last column*/
							/*next:first record,next column*/
							ulColumnIndex++;
							data_context = pstTable->pTableData;
						}
						else
						{
							goto getnext;
						}
					}
				}
				
				snmp_row_build(pstTable, ulColumnIndex, data_context, &row);
				bRet = snmp_table_get_column(pstTable, ulColumnIndex, &row, data_context, response, client);
				if (!bRet)
	            {
					lprintf(LOG_ERR, "File:%s, Line:%d: snmp_table_get_column failed\n",
						__FILE__, __LINE__);
	            	goto generr;
	            }
			}
			
			break;
		case BER_TYPE_SNMP_SET:
			if(!bPermitSet)
			{
				lprintf(LOG_ERR, "File:%s, Line:%d: not permit set\n",
					__FILE__, __LINE__);
				goto generr;				
			}
			data_context = snmp_extract_iterator_context(pstTable, request, i, &j);
			if (data_context)
            {
				agent_printf(LOG_INFO, "set table\n");
                bRet = snmp_table_set_column(data_context, 
					&(request->data_list[i]), 
					pstTable, 
					(UINT16)(ulColumnIndex));
                bSetTable = TRUE;
                /* BEGIN: Added by Zhou Shouya, 2013/9/11 */
                /* ���ӷ���ֵ���� */
				snmp_row_build(pstTable, ulColumnIndex, data_context, &row);
				bRet = snmp_table_get_column(pstTable, ulColumnIndex, &row, data_context, response, client);
				if (!bRet)
	            {
					lprintf(LOG_ERR, "File:%s, Line:%d: snmp_table_get_column failed\n",
						__FILE__, __LINE__);
	            	goto generr;
	            }
                /* END:   Added by Zhou Shouya, 2013/9/11 */
            }
            else
            {
				agent_printf(LOG_INFO, "add table\n");
                bRet = snmp_table_add_column(&(request->data_list[i]), 
					pstTable, 
					(UINT16)(ulColumnIndex),
					row.subid_list,
    				(UINT16)row.subid_list_length);
                bAddTable = TRUE;
                /* BEGIN: Added by Zhou Shouya, 2013/9/11 */
                /* ���ӷ���ֵ���� */
    			memcpy(&response->value_list[response->value_list_length].oid,
    				&request->oid_list[i], sizeof (request->oid_list[i]));
    			bRet = snmp_table_get_column(pstTable, ulColumnIndex, &row, (CHAR *)pstTable->pNewTableData, response, client);
    			if (!bRet)
                {
    				lprintf(LOG_ERR, "File:%s, Line:%d: snmp_table_get_column failed\n",
    					__FILE__, __LINE__);
                	goto generr;
                }
                /* END:   Added by Zhou Shouya, 2013/9/11 */
            }

            if (!bRet)
            {
				lprintf(LOG_ERR, "File:%s, Line:%d: set/add table failed\n",
						__FILE__, __LINE__);
            	goto generr;
            }

			if(i==(request->oid_list_length-1))
			{
				agent_printf(LOG_INFO, "bFreeData:%u,bSetTable:%u,bAddTable:%u\n",
					bFreeData,
					bSetTable,
					bAddTable);

				bFreeData = TRUE;
				if (bSetTable)
				{
					bRet = snmp_table_set_commit(pstTable, szOperator);
					if (!bRet)
					{
						lprintf(LOG_ERR, "File:%s, Line:%d: snmp_table_set_commit failed\n",
							__FILE__, __LINE__);
						goto generr;
					}
				}

				if (bAddTable)
				{
					ulTableStatus = snmp_get_table_status(pstTable);

					agent_printf(LOG_INFO, "ulTableStatus:%u\n",ulTableStatus);

					if (SNMP_TABLE_STATE_SETALL == ulTableStatus)
					{
						bRet = snmp_table_add_commit(pstTable, szOperator);
						if (!bRet)
						{
							lprintf(LOG_ERR, "File:%s, Line:%d: snmp_table_add_commit failed\n",
								__FILE__, __LINE__);
							goto generr;
						}
					} 
					else if(SNMP_TABLE_STATE_WAIT == ulTableStatus)
					{
						bRet = snmp_table_add_wait(pstTable, szOperator);
						if (!bRet)
						{
							lprintf(LOG_ERR, "File:%s, Line:%d: snmp_table_add_wait failed\n",
								__FILE__, __LINE__);
							goto generr;
						}
						bFreeData = FALSE;
					}
					else
					{
						bFreeData = FALSE;
					}
				}
				
				if (bFreeData)
				{
					snmp_table_reset_set_status(pstTable);
				}
                /* BEGIN: Added by zoucaihong, 2013/5/15 */
                #if 0
				snmp_row_build(pstTable, ulColumnIndex, pValueAddr, &row);
				bRet = snmp_table_get_column(pstTable, ulColumnIndex, &row, pValueAddr, response, client);
				if (!bRet)
	            {
					lprintf(LOG_ERR, "File:%s, Line:%d: snmp_table_get_column failed\n",
						__FILE__, __LINE__);
	            	goto generr;
	            }
	            #endif
			    /* END: Added by zoucaihong, 2013/5/15 */
			}
			
			break;
	}
	return 0;
	
getnext:
	
	pstNextTable = snmp_get_next_table(pstTable);
	if(NULL == pstNextTable)
	{
		goto endofmib;
	}

	agent_printf(LOG_INFO, "prevTable:%s nextTable:%s",
		pstTable->acTableName,
		pstNextTable->acTableName);

	if(pstNextTable->bIsTable)
	{ 
		/*table,the first record,the first column value*/		
		snmp_row_build(pstNextTable, 0, pstNextTable->pTableData, &row);
		bRet = snmp_table_get_column(pstNextTable, 0, &row, pstNextTable->pTableData, response, client);
		if (!bRet)
		{
			lprintf(LOG_ERR, "File:%s, Line:%d: snmp_table_get_column failed\n",
				__FILE__, __LINE__);
			goto generr;
		}
	}
	else
	{
		/*scalar,the first column value*/
		bRet = snmp_scalar_get_column(pstNextTable, 0, response, client);
		if (!bRet)
		{
			lprintf(LOG_ERR, "File:%s, Line:%d: snmp_scalar_get_column failed\n",
				__FILE__, __LINE__);
			goto generr;
		}
	}
	return 0;

nosuchinstance:	
	agent_printf(LOG_INFO, "No such Instance:%s\n",
		oid_ntoa(&row));

	memcpy(&response->value_list[response->value_list_length].oid,
		&request->oid_list[i], sizeof (request->oid_list[i]));
    /* BEGIN: Added by zoucaihong, 2014/5/29 */
    #if 0
	memcpy(&response->value_list[response->value_list_length].data,
		&m_no_such_instance, sizeof (m_no_such_instance));
    #endif
	response->value_list[response->value_list_length].data.max_length = m_no_such_instance.max_length;
	response->value_list[response->value_list_length].data.encoded_length = m_no_such_instance.encoded_length;
	response->value_list[response->value_list_length].data.buffer = malloc(m_no_such_instance.max_length);
	memcpy(response->value_list[response->value_list_length].data.buffer,
			m_no_such_instance.buffer,
			m_no_such_instance.max_length);
    /* END: Added by zoucaihong, 2014/5/29 */
	response->value_list_length++;
	return 0;

endofmib:
	
	memcpy(&response->value_list[response->value_list_length].oid,
		&request->oid_list[i], sizeof (request->oid_list[i]));
    /* BEGIN: Added by zoucaihong, 2014/5/29 */
    #if 0
	memcpy(&response->value_list[response->value_list_length].data,
		&m_end_of_mib_view, sizeof (m_end_of_mib_view));
    #endif
	response->value_list[response->value_list_length].data.max_length = m_end_of_mib_view.max_length;
	response->value_list[response->value_list_length].data.encoded_length = m_end_of_mib_view.encoded_length;
	response->value_list[response->value_list_length].data.buffer = malloc(m_end_of_mib_view.max_length);
	memcpy(response->value_list[response->value_list_length].data.buffer,
			m_end_of_mib_view.buffer,
			m_end_of_mib_view.max_length);
    /* END: Added by zoucaihong, 2014/5/29 */
	response->value_list_length++;
	return 0;		
	
generr:

	response->error_status = SNMP_STATUS_GEN_ERR;
	response->error_index = i;
    /* BEGIN: Modified by zoucaihong, 2015/4/29 */
	//return -1;
	return 0;
    /* END: Modified by zoucaihong, 2015/4/29 */
}

