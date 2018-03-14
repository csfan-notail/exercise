/******************************************************************************
* COPYRIGHT GBCOM
*******************************************************************************
* 功    能: 
* 修改历史: 
* 2012-9-10     冯静              新建
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
#include "my_snmp_protocol.h"

/*************************************************************************/
extern SNMP_TABLE_INFO_T *g_pstTableInfo;
extern UINT32      g_ulRootOIDSize;
extern OID         g_rootOID[MAX_SNMP_OID_SIZE];

/* 字节序 */
extern UINT32      g_ulEndian;

/*************************************************************************/


/*************************************************************************
* 函数名称: snmp_init_column_info
* 功    能: 初始化列信息
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回:
* 说    明:
*************************************************************************/
BOOL snmp_init_column_info(SNMP_TABLE_T  *pstTable,
						   CHAR          *szColumnName,
						   UINT8         ucColumnType,
                           UINT16        usColumnLen,
                           UINT16        usOffSet,
						   UINT8         ucMibOper,
                           BOOL          bIsKeyField)
{
    SNMP_COLUMN_T   *pstColumn;
	UINT16			usColumnID;

    if (pstTable->ulColumnNum >= ARRAY_SIZE(pstTable->astColumns))
    {
        return FALSE;
    }

	usColumnID = pstTable->ulColumnNum;

    pstColumn = &pstTable->astColumns[pstTable->ulColumnNum];
    pstColumn->usColumnID   = usColumnID;
    /* BEGIN: Added by Zhou Shouya, 2013/8/9   PN:VLAN */
    strncpy(pstColumn->acColumnName, szColumnName, sizeof(pstColumn->acColumnName)-1);
    /* END:   Added by Zhou Shouya, 2013/8/9   PN:VLAN */
    pstColumn->ucColumnType = ucColumnType;
    pstColumn->usColumnLen  = usColumnLen;
	pstColumn->ucMibOper    = ucMibOper;
    pstColumn->usOffSet     = usOffSet;

    pstTable->ulColumnNum ++;

    if (bIsKeyField)
    {
        if (pstTable->ulKeyColumnNum >= sizeof(pstTable->apstKeyColumns)/sizeof(pstTable->apstKeyColumns[0]))
        {
			agent_printf(LOG_ERR, "snmp_init_column_info() ulKeyColumnNum is too more\n");
            return FALSE;
        }

        pstTable->apstKeyColumns[pstTable->ulKeyColumnNum] = pstColumn;
        pstTable->ulKeyColumnNum ++;
    }

    return TRUE;
}
/*************************************************************************
* 函数名称: snmp_init_column_enum_info
* 功    能: 
* 参    数: 
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明: 
*************************************************************************/
BOOL snmp_init_column_enum_info(SNMP_TABLE_T  *pstTable, CHAR *szEnum)
{
	// enum format:critical(1), main(2), minor(3), warning(4)
	SNMP_COLUMN_T *pstColumn = NULL;
	CHAR        *original = NULL;
	CHAR        *position = NULL;
	CHAR        acPair[32] = {0};

	CHAR       acLeft[32] = {0};
	UINT32     ulValue = 0;
	CHAR       acValue[32] = {0};
		
	if (NULL == pstTable)
	{
		return FALSE;
	}

	if (NULL == szEnum)
	{
		return FALSE;
	}

	pstColumn = &pstTable->astColumns[pstTable->ulColumnNum-1];
	if (NULL == pstColumn)
	{
		agent_printf(LOG_INFO,"snmp_init_column_enum_info() pstColumn null\n");
		return FALSE;
	}

	memset(pstColumn->astColumnEnums, 0, sizeof(pstColumn->astColumnEnums));

	original = szEnum;
	position = strstr(original, ",");
	while (position != NULL)
	{			
		strncpy(acPair, original, position-original);
		acPair[position-original] = '\0';
		
		memset(acLeft, 0, sizeof(acLeft));
		memset(acValue, 0, sizeof(acValue));
		if (sscanf(acPair, "%[^(](%s)", acLeft, acValue) == 2)
		{			
			ulValue = atoi(acValue);
			if (pstColumn->ulEnumNum>=MAX_TABLE_COLUMN_ENUM_NUM)
			{
				return FALSE;
			}
			strncpy(pstColumn->astColumnEnums[pstColumn->ulEnumNum].acDisValue,
				acLeft, strlen(acLeft));
			pstColumn->astColumnEnums[pstColumn->ulEnumNum].ulValue = ulValue;
			pstColumn->ulEnumNum ++;			
		}			

		while(*(position+1) == ' ')
		{
			position++;
		}
		original = position+1;
		if (original == NULL)
		{
			break;
		}
		position = strstr(original, ",");
	}
	
	if (original != NULL)
	{
		strncpy(acPair, original, strlen(original));
		acPair[strlen(original)] = '\0';

		memset(acLeft, 0, sizeof(acLeft));
		memset(acValue, 0, sizeof(acValue));
		if (sscanf(acPair, "%[^(](%s)", acLeft, acValue) == 2)
		{			
			ulValue = atoi(acValue);
			if (pstColumn->ulEnumNum>=MAX_TABLE_COLUMN_ENUM_NUM)
			{
				return FALSE;
			}
			strncpy(pstColumn->astColumnEnums[pstColumn->ulEnumNum].acDisValue,
				acLeft, strlen(acLeft));
			pstColumn->astColumnEnums[pstColumn->ulEnumNum].ulValue = ulValue;
			pstColumn->ulEnumNum ++;
		}			
	}

	return TRUE;
}

/*************************************************************************
* 函数名称: snmp_init_column_range
* 功    能: 
* 参    数: 
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明: 
*************************************************************************/
BOOL snmp_init_column_range(SNMP_TABLE_T  *pstTable, 
	                                INT32 lMin, 
	                                INT32 lMax)
{
	SNMP_COLUMN_T *pstColumn = NULL;
	if (NULL == pstTable)
	{
		return FALSE;
	}

	if (pstTable->ulColumnNum == 0)
	{
		return FALSE;
	}

	pstColumn = &pstTable->astColumns[pstTable->ulColumnNum-1];
	if (NULL == pstColumn)
	{
		return FALSE;
	}

	if (lMin>lMax)
	{
		return FALSE;
	}

	pstColumn->lMin = lMin;
	pstColumn->lMax = lMax;

	return TRUE;
}

/*************************************************************************
* 函数名称: snmp_init_column_units
* 功    能: 
* 参    数: 
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明: 
*************************************************************************/
BOOL snmp_init_column_units(SNMP_TABLE_T  *pstTable, 
								   CHAR *szUnits)
{
	SNMP_COLUMN_T *pstColumn = NULL;
	if (NULL == pstTable)
	{
		return FALSE;
	}

	if (pstTable->ulColumnNum == 0)
	{
		return FALSE;
	}

	pstColumn = &pstTable->astColumns[pstTable->ulColumnNum-1];
	if (NULL == pstColumn)
	{
		return FALSE;
	}

	if (NULL == szUnits)
	{
		return FALSE;
	}

	if (strlen(szUnits)>=sizeof(pstColumn->acUnits))
	{
		return FALSE;
	}

	sprintf(pstColumn->acUnits, "%s", szUnits);
	return TRUE;
}
/*************************************************************************
* 函数名称: snmp_init_column_units
* 功    能: 
* 参    数: 
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明: 
*************************************************************************/
BOOL snmp_init_column_defval(SNMP_TABLE_T  *pstTable, 
						   VOID *pValue, UINT32 ulValue)
{
	SNMP_COLUMN_T *pstColumn = NULL;
	UINT32 ulLen = 0;
	
	if (NULL == pstTable)
	{
		return FALSE;
	}

	if (pstTable->ulColumnNum == 0)
	{
		return FALSE;
	}

	pstColumn = &pstTable->astColumns[pstTable->ulColumnNum-1];
	if (NULL == pstColumn)
	{
		return FALSE;
	}
			
	switch(pstColumn->ucColumnType)
	{
		case TYPE_CHAR:
			if (NULL != pValue)
			{
				ulLen = strlen((CHAR *)pValue)+1;
                /* BEGIN: Modified by zhangjiasi, 2015/7/22   PN:内存泄露 */
                if (pstColumn->pucDefValue )
                {
                    SNMP_AGENT_FREE(pstColumn->pucDefValue);
                    pstColumn->pucDefValue = NULL;
                }
                /* END:   Modified by zhangjiasi, 2015/7/22   PN:内存泄露 */
                
				pstColumn->pucDefValue = SNMP_AGENT_MALLOC(ulLen);
				if (NULL == pstColumn->pucDefValue)
				{
					return FALSE;
				}
				memset(pstColumn->pucDefValue,0,ulLen);
				strcpy((char *)pstColumn->pucDefValue, (CHAR *)pValue);
			}
			else
			{
				return FALSE;
			}
			break;
		case TYPE_BYTE:
			if (NULL != pValue)
			{
				pstColumn->ulDefValue = *(UINT8*)(pValue);
			}
			else
			{
				pstColumn->ulDefValue = ulValue;
			}
			break;
		case TYPE_SHORT:
			if (NULL != pValue)
			{
				pstColumn->ulDefValue = *(UINT16*)(pValue);
			}
			else
			{
				pstColumn->ulDefValue = ulValue;
			}
			break;
		case TYPE_LONG:
			if (NULL != pValue)
			{
				pstColumn->ulDefValue = *(UINT32*)(pValue);
			}
			else
			{
				pstColumn->ulDefValue = ulValue;
			}
			break;
		case TYPE_BOOL:
			if (NULL != pValue)
			{
				pstColumn->ulDefValue = *(BOOL*)(pValue);
			}
			else
			{
				pstColumn->ulDefValue = ulValue;
			}
			break;
		case TYPE_INT:
			if (NULL != pValue)
			{
				pstColumn->lDefValue = *(INT32*)(pValue);
			}
			else
			{
				pstColumn->lDefValue = ulValue;
			}
			break;
		case TYPE_BIN:
			// no
			break;
		case TYPE_IP:
			if (NULL != pValue)
			{
				ulLen = 4;
                /* BEGIN: Modified by zhangjiasi, 2015/7/22   PN:内存泄露 */
                if (pstColumn->pucDefValue )
                {
                    SNMP_AGENT_FREE(pstColumn->pucDefValue);
                    pstColumn->pucDefValue = NULL;
                }
                /* END:   Modified by zhangjiasi, 2015/7/22   PN:内存泄露 */
                
				pstColumn->pucDefValue = SNMP_AGENT_MALLOC(ulLen);
				if (NULL == pstColumn->pucDefValue)
				{
					return FALSE;
				}
				memcpy(pstColumn->pucDefValue, (UINT8 *)pValue, ulLen);
			}
			else
			{
				return FALSE;
			}
			break;
		case TYPE_MAC:
			if (NULL != pValue)
			{
				ulLen = 6;
                /* BEGIN: Modified by zhangjiasi, 2015/7/22   PN:内存泄露 */
                if (pstColumn->pucDefValue )
                {
                    SNMP_AGENT_FREE(pstColumn->pucDefValue);
                    pstColumn->pucDefValue = NULL;
                }
                /* END:   Modified by zhangjiasi, 2015/7/22   PN:内存泄露 */
                
				pstColumn->pucDefValue = SNMP_AGENT_MALLOC(ulLen);
				if (NULL == pstColumn->pucDefValue)
				{
					return FALSE;
				}
				memcpy(pstColumn->pucDefValue, (UINT8 *)pValue, ulLen);
			}
			else
			{
				return FALSE;
			}
			break;
		case TYPE_TIME:
			if (NULL != pValue)
			{
				pstColumn->ulDefValue = *(UINT32*)(pValue);
			}
			else
			{
				pstColumn->ulDefValue = ulValue;
			}
			break;
		case TYPE_IPV6:
			if (NULL != pValue)
			{
				ulLen = 16;
                /* BEGIN: Modified by zhangjiasi, 2015/7/22   PN:内存泄露 */
                if (pstColumn->pucDefValue )
                {
                    SNMP_AGENT_FREE(pstColumn->pucDefValue);
                    pstColumn->pucDefValue = NULL;
                }
                /* END:   Modified by zhangjiasi, 2015/7/22   PN:内存泄露 */
                
				pstColumn->pucDefValue = SNMP_AGENT_MALLOC(ulLen);
				if (NULL == pstColumn->pucDefValue)
				{
					return FALSE;
				}
				memcpy(pstColumn->pucDefValue, (UINT8 *)pValue, ulLen);
			}
			else
			{
				return FALSE;
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
                /* BEGIN: Modified by zhangjiasi, 2015/7/22   PN:内存泄露 */
                if (pstColumn->pucDefValue )
                {
                    SNMP_AGENT_FREE(pstColumn->pucDefValue);
                    pstColumn->pucDefValue = NULL;
                }
                /* END:   Modified by zhangjiasi, 2015/7/22   PN:内存泄露 */
                
				pstColumn->pucDefValue = SNMP_AGENT_MALLOC(ulLen);
				if (NULL == pstColumn->pucDefValue)
				{
					return FALSE;
				}
				memcpy(pstColumn->pucDefValue, (UINT8 *)pValue, ulLen);
			}
			else
			{
				return FALSE;
			}
			break;
		case TYPE_LONGLONG:
			// no
			break;
	}
		
	return TRUE;
}

/*************************************************************************
* 函数名称: snmp_table_index_valid
* 功    能: （缺省的）主键有效性判断函数
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回:
* 说    明:
*************************************************************************/
BOOL snmp_table_index_valid(UINT32 ulTableID, UINT32 ulTableRecLen, VOID *pKey)
{
    UINT32 *pulIndex = (UINT32*)pKey;

    if (0 == *pulIndex)
    {
        return FALSE;
    }

    return TRUE;
}



/*************************************************************************
* 函数名称: snmp_init_table_info
* 功    能: 初始化表信息
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回:
* 说    明:
*************************************************************************/
BOOL snmp_init_table_info(SNMP_TABLE_T  *pstTable,
                          BOOL          bIsTable,
                          UINT32        ulTableID,
                          CHAR          *szTableName,
                          SNMP_TABLE_VALID_FUNC* pfTableValidFunc,
                          UINT32        ulOIDSize,
                          OID           *pstOID,
                          UINT32        ulMaxTableRecNum,
                          UINT32        ulTableRecLen)
{
	if (g_pstTableInfo->ulTableNum >= ARRAY_SIZE(g_pstTableInfo->astTables))
	{
		return FALSE;
	}

    if (!pstTable)
    {
        return FALSE;
    }

    memset(pstTable, 0 ,sizeof(SNMP_TABLE_T));

    pstTable->ulTableID        = ulTableID;
    pstTable->bIsTable         = bIsTable;
    strncpy(pstTable->acTableName, szTableName, sizeof(pstTable->acTableName)-1);
    pstTable->pfKeyValidFunc   = snmp_table_index_valid;
    pstTable->pfTableValidFunc = pfTableValidFunc;

	pstTable->ulOIDSize = g_ulRootOIDSize+ulOIDSize;
	memcpy(pstTable->astOID, &(g_rootOID[0]), g_ulRootOIDSize*sizeof(OID));
	memcpy((UINT8 *)&(pstTable->astOID[0])+(g_ulRootOIDSize*sizeof(OID)), pstOID, ulOIDSize*sizeof(OID));
    
	{
		lprintf(LOG_INFO, "pstTable->ulOIDSize:%u\n", pstTable->ulOIDSize);
		UINT32 ulLoopNo = 0;
		CHAR   acOid[128] = {0};

        for(ulLoopNo=0; ulLoopNo<pstTable->ulOIDSize; ulLoopNo++)
		{
			sprintf(acOid+strlen(acOid), "%lu.", pstTable->astOID[ulLoopNo]);
        }
		lprintf(LOG_INFO, "ulTableID:%u,szTableName:%32s,pstOID:%s\n", ulTableID, szTableName, acOid);
	}
    
	pstTable->ulMaxTableRecNum = ulMaxTableRecNum;
    pstTable->ulTableRecLen    = ulTableRecLen;

	g_pstTableInfo->ulTableNum ++;

    return TRUE;
}



/*************************************************************************
* 函数名称: snmp_get_table_by_oid
* 功    能: 根据表ID获取表信息
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回:
* 说    明:
*************************************************************************/
SNMP_TABLE_T *snmp_get_table_by_oid(const oid_t *oid)
{
    UINT32          i;
	//BOOL            bIsTable = FALSE;
	SNMP_TABLE_T *  pstTable = NULL;
	int cmp = -1;
	UINT32 ulMinOIDSize = 0;
	UINT32 ulMatchBest = 0;

	if(oid->subid_list_length<g_ulRootOIDSize)
	{
		pstTable = &(g_pstTableInfo->astTables[0]);
		return pstTable;
	}
	else
	{
		cmp = memcmp(g_rootOID, 
	            oid->subid_list, 
	            g_ulRootOIDSize*sizeof(OID));
		if (cmp<0)
		{
			return NULL;
		}
		else if(cmp>0)
		{
			pstTable = &(g_pstTableInfo->astTables[0]);
			return pstTable;
		}
	}

	// cmp==0 && length>=rootOIDSize
    for (i=0; i<g_pstTableInfo->ulTableNum; i++)
    {
    	/* choose match best */
		ulMinOIDSize = g_pstTableInfo->astTables[i].ulOIDSize>oid->subid_list_length ?
			           oid->subid_list_length:g_pstTableInfo->astTables[i].ulOIDSize;
		
		if (0 == memcmp(g_pstTableInfo->astTables[i].astOID, 
	         	oid->subid_list, 
	           	ulMinOIDSize*sizeof(OID)))
        {
			if(ulMatchBest<ulMinOIDSize)
			{
            	pstTable = &g_pstTableInfo->astTables[i];
				ulMatchBest = ulMinOIDSize;
			}
        }	
    }

	if(NULL != pstTable)
	{
		lprintf(LOG_INFO, "oid:%s table:%s\n", oid_ntoa(oid),
			pstTable->acTableName);
	}
	else
	{
		lprintf(LOG_INFO, "oid:%s table:NULL\n", oid_ntoa(oid));		
	}
	
    return pstTable;
}


/* -----------------------------------------------------------------------------
 * Helper functions for requests
 */
/*************************************************************************
* 函数名称: snmp_get_next_table
* 功    能: 根据表ID获取表信息
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回:
* 说    明:
*************************************************************************/
SNMP_TABLE_T *snmp_get_next_table(SNMP_TABLE_T *pstPrevTable)
{
	SNMP_TABLE_T *  pstTable = NULL;
	UINT32 j=0;
	UINT32 ulIndex = 0;
#if 0//dp ok return NULL?
	if (NULL == pstPrevTable ||
		pstPrevTable->ulTableID>=g_pstTableInfo->ulTableNum+2)
	{
		return NULL;
	}
#endif

	ulIndex = ((UINT32)pstPrevTable-(UINT32)&(g_pstTableInfo->astTables[0]))/sizeof(SNMP_TABLE_T);

	agent_printf(LOG_INFO, "prevTable:%s tableIndex:%u", pstPrevTable->acTableName,
		ulIndex);
	//agent_printf(LOG_INFO, "prevTable:%u oriTable:%u tableSize=%u", (UINT32)pstPrevTable,
	//	(UINT32)&(g_pstTableInfo->astTables[0]), sizeof(SNMP_TABLE_T));

	for(j=ulIndex+1; j<g_pstTableInfo->ulTableNum; j++)
	{
		pstTable = &g_pstTableInfo->astTables[j];
		if (!pstTable->bIsTable)
		{
			if(!snmp_get_scalar_data(pstTable))
			{
				continue;
			}
			else if(NULL != pstTable->pTableData)
			{
				break;
			}
			else
			{
				continue;
			}
		}
		else if (!snmp_get_table_data(pstTable))
		{
			continue;
		}
		else if(NULL != pstTable->pTableData &&
			   pstTable->ulTableRecNum > 0)
		{
			break;
		}
		else
		{
			continue;
		}
	}

	if (j != g_pstTableInfo->ulTableNum)
	{
		return pstTable;//by dp badly
	}
	
	return NULL;
}


/*************************************************************************
* 函数名称: snmp_get_column
* 功    能: SNMP表查询记录列
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回:
* 说    明:
*************************************************************************/
int snmp_encoded_length(oid_t *oid)
{
	int i=0;
	int length;
	
	/* Calculate the encoded length of the created OID (note: first the length
	 * of the subid list, then the length of the length/type header!)
	 */
	length = 1;
	for (i = 2; i < oid->subid_list_length; i++) {
		if (oid->subid_list[i] >= (1 << 28)) {
			length += 5;
		} else if (oid->subid_list[i] >= (1 << 21)) {
			length += 4;
		} else if (oid->subid_list[i] >= (1 << 14)) {
			length += 3;
		} else if (oid->subid_list[i] >= (1 << 7)) {
			length += 2;
		} else {
			length += 1;
		}
	}
	if (length > 0xFFFF) {
		lprintf(LOG_ERR, "could not encode '%s': oid overflow\n", oid_ntoa(oid));
		return -1;
	} else if (length > 0xFF) {
		length += 4;
	} else if (length > 0x7F) {
		length += 3;
	} else {
		length += 2;
	}
	oid->encoded_length = length;

	return 0;
}


/*************************************************************************
* 函数名称: snmp_get_column
* 功    能: SNMP表查询记录列
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回:
* 说    明:
*************************************************************************/
int snmp_row_build(SNMP_TABLE_T *pstTable, UINT32 ulColumnIndex, CHAR  *pValueAddr, oid_t *row)
{
	UINT32 ulKeyColumnNum = 0;

	UINT8 ucKeyColumnType=0;
	UINT16 usKeyColumnLen = 0;

	UINT32 j=0;
	UINT32 k=0;

	UINT32 ulStrLen = 0;
	UINT32 ulOffset = 0;

	//row->subid_list[ulOffset] = ulColumnIndex+1;
	//ulOffset++;
	ulKeyColumnNum = pstTable->ulKeyColumnNum;
	
	for(j=0;j<ulKeyColumnNum;j++)
	{
		ucKeyColumnType = pstTable->apstKeyColumns[j]->ucColumnType;
		usKeyColumnLen = pstTable->apstKeyColumns[j]->usColumnLen;
		switch(ucKeyColumnType)
		{
			case TYPE_CHAR:
				/*for example:mac */
				ulStrLen = strlen((char *)pValueAddr);
			    row->subid_list[ulOffset] = ulStrLen;
				ulOffset++;
				for(k=0;k<ulStrLen;k++)
				{
					row->subid_list[ulOffset+k] = pValueAddr[k];
				}
				ulOffset+= ulStrLen;
				pValueAddr += usKeyColumnLen;
				break;
			case TYPE_LONG:
				/*for example:id*/
				row->subid_list[ulOffset] = *(UINT32 *)pValueAddr;
				ulOffset++;
				pValueAddr += usKeyColumnLen;
				break;
			default:
				break;
		}
	}

	row->subid_list_length = ulOffset;

	return 0;
}

 
/*************************************************************************
* 函数名称: snmp_get_column
* 功    能: SNMP表查询记录列
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回:
* 说    明:
*************************************************************************/
int snmp_value_build(SNMP_TABLE_T *pstTable, UINT16 usColumnID,oid_t *row, const void *default_value, value_t *value)
{	
	int length;
	int i;
	
	oid_t *prefix;
	oid_t tableOid;

	UINT8 ucColumnType = 0;
	UINT16 usColumnLen = 0;
	UINT8  type = 0;
	UINT16 column = 0;
	
	prefix = &tableOid;

	// table/scalar node
	tableOid.subid_list_length = pstTable->ulOIDSize;
	memcpy(tableOid.subid_list, pstTable->astOID, pstTable->ulOIDSize*sizeof(OID));

	if (pstTable->bIsTable)
	{
		// entry node
		tableOid.subid_list[tableOid.subid_list_length] = 1;
		tableOid.subid_list_length++;
	}

	ucColumnType =  pstTable->astColumns[usColumnID].ucColumnType;
	usColumnLen =  pstTable->astColumns[usColumnID].usColumnLen;
	type = snmp_get_asn1_type(ucColumnType);
	column = usColumnID+1;

	/* feng jing add*/
	if(value->oid.subid_list_length == 0)
	{
		//snmp_encoded_length(&tableOid);
		
		/* Create the OID from the prefix, the column and the row */
		memcpy(&value->oid, prefix, sizeof (value->oid));
		if (value->oid.subid_list_length < MAX_NR_SUBIDS) {
			value->oid.subid_list[value->oid.subid_list_length++] = column;
		} else {
			lprintf(LOG_ERR, "could not create MIB entry '%s.%d.%s': oid overflow\n",
				oid_ntoa(prefix), column, oid_ntoa(row));
			return -1;
		}
		if (value->oid.subid_list_length < MAX_NR_SUBIDS) {
			memcpy(&(value->oid.subid_list[value->oid.subid_list_length]),
				row->subid_list,
				sizeof(OID)*row->subid_list_length);
			value->oid.subid_list_length += row->subid_list_length;
		} else {
			lprintf(LOG_ERR, "could not create MIB entry '%s.%d.%s': oid overflow\n",
				oid_ntoa(prefix), column, oid_ntoa(row));
			return -1;
		}

		/* Calculate the encoded length of the created OID (note: first the length
		 * of the subid list, then the length of the length/type header!)
		 */
		length = 1;
		for (i = 2; i < value->oid.subid_list_length; i++) {
			if (value->oid.subid_list[i] >= (1 << 28)) {
				length += 5;
			} else if (value->oid.subid_list[i] >= (1 << 21)) {
				length += 4;
			} else if (value->oid.subid_list[i] >= (1 << 14)) {
				length += 3;
			} else if (value->oid.subid_list[i] >= (1 << 7)) {
				length += 2;
			} else {
				length += 1;
			}
		}
		if (length > 0xFFFF) {
			lprintf(LOG_ERR, "could not encode '%s': oid overflow\n", oid_ntoa(&value->oid));
			return -1;
		} else if (length > 0xFF) {
			length += 4;
		} else if (length > 0x7F) {
			length += 3;
		} else {
			length += 2;
		}
		value->oid.encoded_length = length;

	}

	/* Paranoia check against invalid default parameter (null pointer) */
	switch (type) {
		case BER_TYPE_OCTET_STRING:
		case BER_TYPE_OID:
			if (default_value == NULL) {
				lprintf(LOG_ERR, "could not create MIB entry '%s.%d.%s': invalid default value\n",
					oid_ntoa(prefix), column, oid_ntoa(row));
				return -1;
			}
			break;
		default:
			break;
	}

	/* Create a data buffer for the value depending on the type:
	 *
	 * - strings and oids are assumed to be static or have the maximum allowed length
	 * - integers are assumed to be dynamic and don't have more than 32 bits
	 */
	switch (type) {
		case BER_TYPE_INTEGER:
			value->data.max_length = sizeof (int) + 2;
			value->data.encoded_length = 0;
			value->data.buffer = malloc(value->data.max_length);
			if (encode_snmp_element_integer(value, *(int *)default_value) == -1) {
				return -1;
			}
			break;
		case BER_TYPE_OCTET_STRING:
			if (ucColumnType == TYPE_MAC ||
				ucColumnType == TYPE_IPV6 ||
				ucColumnType == TYPE_BIN ||
				ucColumnType == TYPE_TADDR)
			{				
				value->data.max_length = usColumnLen + 4;
				value->data.encoded_length = 0;
				value->data.buffer = malloc(value->data.max_length);
				if (encode_snmp_element_octetstring(value, (const char *)default_value, usColumnLen) == -1) {
					return -1;
				}
			}
			else
			{
				value->data.max_length = strlen((const char *)default_value) + 4;
				value->data.encoded_length = 0;
				value->data.buffer = malloc(value->data.max_length);
				if (encode_snmp_element_string(value, (const char *)default_value) == -1) {
					return -1;
				}
			}
			break;
		/* feng jing add */
		case BER_TYPE_IP:			
			value->data.max_length = 4 + 4;
			value->data.encoded_length = 0;
			value->data.buffer = malloc(value->data.max_length);
			if (encode_snmp_element_ip(value, (const char *)default_value) == -1) {
				return -1;
			}
			break;
			
		case BER_TYPE_OID:
			value->data.max_length = MAX_NR_SUBIDS * 5 + 4;
			value->data.encoded_length = 0;
			value->data.buffer = malloc(value->data.max_length);
			if (encode_snmp_element_oid(value, oid_aton((const char *)default_value)) == -1) {
				return -1;
			}
			break;
		case BER_TYPE_COUNTER:
		case BER_TYPE_GAUGE:
		case BER_TYPE_TIME_TICKS:
			value->data.max_length = sizeof (unsigned int) + 2;
			value->data.encoded_length = 0;
			value->data.buffer = malloc(value->data.max_length);
			if (encode_snmp_element_unsigned(value, type, *(unsigned int *)default_value) == -1) {
				return -1;
			}
			break;
		case BER_TYPE_COUNTER64:
			value->data.max_length = sizeof (unsigned long long) + 2;
			value->data.encoded_length = 0;
			value->data.buffer = malloc(value->data.max_length);
			if (encode_snmp_element_unsigned_longlong(value, type, *(unsigned long long *)default_value) == -1) {
				return -1;
			}
			break;
		default:
			lprintf(LOG_ERR, "could not create MIB entry '%s.%d.%s': unsupported type %d\n",
				oid_ntoa(prefix), column, oid_ntoa(row), type);
			return -1;
	}

	return 0;
	
}

/*************************************************************************
* 函数名称: snmp_get_column
* 功    能: SNMP表查询记录列
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回:
* 说    明:
*************************************************************************/
BOOL snmp_get_column(SNMP_TABLE_T *pstTable,
                           UINT16    usColumnID,
                           oid_t     *pRow,
                           void      *data_context,
                           response_t *response, 
                           client_t *client)
{
    CHAR            *retval;
    UINT16          usColumnLen = 0;
    SNMP_COLUMN_T   *pstColumn;
	UINT32          ulOIDLen = 0;

	if (NULL == pstTable)
	{
		agent_printf(LOG_ERR, "snmp_get_column() pstTable is NULL.");
		return FALSE;
	}

	if (NULL == data_context)
	{
		agent_printf(LOG_ERR, "snmp_get_column() data_context is NULL.");
		return FALSE;
	}

	if (usColumnID>=pstTable->ulColumnNum)
	{
		agent_printf(LOG_ERR, "snmp_get_column() usColumnID:%u is invalid.", usColumnID);
		return FALSE;
	}
	
	pstColumn = &(pstTable->astColumns[usColumnID]);
    if (NULL == pstColumn)
    {
		agent_printf(LOG_ERR, "snmp_get_column() pstColumn is NULL.");
        return FALSE;
    }

    retval = ((CHAR*)data_context);
    retval += pstColumn->usOffSet;

    usColumnLen = pstColumn->usColumnLen;
    //ucType = snmp_get_asn1_type(pstColumn->ucColumnType);

	if (pstColumn->ucColumnType == TYPE_CHAR)
	{
		usColumnLen = strlen(retval);
	}				
	
	if (pstColumn->ucColumnType == TYPE_OID)
	{
		ulOIDLen = *(UINT32 *)(data_context-sizeof(UINT32));
		usColumnLen = ulOIDLen*sizeof(OID);
	}

	if (response->value_list_length < MAX_NR_VALUES) 
	{		
		snmp_value_build(pstTable, 
			usColumnID,
			pRow, 
			retval, 
			&response->value_list[response->value_list_length]);		
		response->value_list_length++;
	} 
	else 
	{
		lprintf(LOG_ERR, "could not handle SNMP GET: value list overflow\n");
		return FALSE;
	}
	return TRUE;
}


/*************************************************************************
* 函数名称: snmp_set_column
* 功    能: 设置列
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回:
* 说    明:
*************************************************************************/
BOOL snmp_set_column(UINT32     ulColumnType,
                     UINT32     ulColumnLen,
                     VOID       *pInValue,
                     UINT32     ulInLen,
                     VOID       *pOutValue)
{
    BOOL    bRet = TRUE;
    UINT8   aucMac[6];
    UINT32  ulMac[32];
    UINT32  ulLoop;
    if (NULL == pInValue || NULL == pOutValue)
    {
        agent_printf(LOG_ERR, "snmp_set_column: pInValue or pOutValue is null");
        return FALSE;
    }

    switch(ulColumnType)
    {
        case TYPE_CHAR:
            if (ulInLen > ulColumnLen)
            {
                agent_printf(LOG_WARNING, "snmp_set_column: ulInLen(%u) is too big(%u)", ulInLen, ulColumnLen);
            }
            memset(pOutValue, 0, ulColumnLen);
            strncpy((CHAR*)pOutValue, (CHAR*)pInValue, (ulInLen>ulColumnLen? ulColumnLen : ulInLen));
            break;

        case TYPE_BYTE:
            /* BEGIN: Modified by 李文, 2012/10/12   PN:CPE远程维护 
            鉴于SNMP协议，对于数字类型的值，是变长，则不应该与定值做等于比较*/
            //if (ulInLen == sizeof(UINT32) && ulColumnLen == sizeof(UINT8))
            /* END:   Modified by 李文, 2012/10/12   PN:CPE远程维护 */
            if (ulInLen <= sizeof(UINT8) && ulColumnLen == sizeof(UINT8))
            {
                /* BEGIN: Modified by 李文, 2012/10/12   PN:CPE远程维护 */
                //*((UINT8*)pOutValue) = (UINT8)(*((UINT32*)pInValue));
                /* END:   Modified by 李文, 2012/10/12   PN:CPE远程维护 */
                *((UINT8*)pOutValue) = (UINT8)(*((UINT8*)pInValue));
            }
            else
            {
                agent_printf(LOG_ERR, "snmp_set_column: ulInLen(%u) should be less than %u", ulInLen, sizeof(UINT32));
                bRet = FALSE;
            }
            break;

        case TYPE_SHORT:
            /* BEGIN: Modified by 李文, 2012/10/12   PN:CPE远程维护 
            鉴于SNMP协议，对于数字类型的值，是变长，则不应该与定值做等于比较*/
            //if (ulInLen == sizeof(UINT32) && ulColumnLen == sizeof(UINT16))
            /* END:   Modified by 李文, 2012/10/12   PN:CPE远程维护 */
            if (ulInLen <= sizeof(UINT16) && ulColumnLen == sizeof(UINT16))
            {
                /* BEGIN: Modified by 李文, 2012/10/12   PN:CPE远程维护 */
                //*((UINT16*)pOutValue) = (UINT16)(*((UINT32*)pInValue));
                /* END:   Modified by 李文, 2012/10/12   PN:CPE远程维护 */
                memcpy(pOutValue,pInValue,ulInLen);
                /* BEGIN: Added by 李文, 2012/10/12   PN:CPE远程维护 
                    添加变长数字类型的解析
                */
                if(g_ulEndian == BIG_ENDIAN)
                {
	                if(ulInLen < sizeof(UINT16))
	                {
	                    *((UINT16 *)pOutValue)>>=((sizeof(UINT16)-ulInLen)*BYTE_BITNUM);
	                }
                }
                /* END:   Added by 李文, 2012/10/12   PN:CPE远程维护 */
            }
            else
            {
                agent_printf(LOG_ERR, "snmp_set_column: ulInLen(%u) should be %u", ulInLen, sizeof(UINT32));
                bRet = FALSE;
            }
            break;

        case TYPE_LONG:
		case TYPE_GAUGE32:
		case TYPE_COUNTER32:
            /* BEGIN: Modified by 李文, 2012/10/12   PN:CPE远程维护 
            鉴于SNMP协议，对于数字类型的值，是变长，则不应该与定值做等于比较*/
            //if (ulInLen == sizeof(UINT32) && ulColumnLen == sizeof(UINT32))
            /* END:   Modified by 李文, 2012/10/12   PN:CPE远程维护 */
            if (ulInLen <= sizeof(UINT32) && ulColumnLen == sizeof(UINT32))
            {
                /* BEGIN: Modified by 李文, 2012/10/12   PN:CPE远程维护 */
                //*((UINT32*)pOutValue) = (UINT32)(*((UINT32*)pInValue));
                /* END:   Modified by 李文, 2012/10/12   PN:CPE远程维护 */
                memcpy(pOutValue,pInValue,ulInLen);
                /* BEGIN: Added by 李文, 2012/10/12   PN:CPE远程维护 
                    添加变长数字类型的解析,该段代码只能用于大头网络设备
                */

	                if(ulInLen < sizeof(UINT32))
	                {
	                    *((UINT32 *)pOutValue)>>=((sizeof(UINT32)-ulInLen)*BYTE_BITNUM);
	                }
       
                /* END:   Added by 李文, 2012/10/12   PN:CPE远程维护 */
            }
            else
            {
                agent_printf(LOG_ERR, "snmp_set_column: ulInLen(%u) should be %u", ulInLen, sizeof(UINT32));
                bRet = FALSE;
            }
            break;

		/* 2011-06-17 fengjing add */
		case TYPE_LONGLONG:
            /* BEGIN: Modified by 李文, 2012/10/12   PN:CPE远程维护 
            鉴于SNMP协议，对于数字类型的值，是变长，则不应该与定值做等于比较*/
			//if (ulInLen == sizeof(UINT64) && ulColumnLen == sizeof(UINT64))
            /* END:   Modified by 李文, 2012/10/12   PN:CPE远程维护 */
			if (ulInLen <= sizeof(UINT64) && ulColumnLen == sizeof(UINT64))
			{
                /* BEGIN: Modified by 李文, 2012/10/12   PN:CPE远程维护 */
                //*((UINT64*)pOutValue) = (UINT64)(*((UINT64*)pInValue));
                /* END:   Modified by 李文, 2012/10/12   PN:CPE远程维护 */
                memcpy(pOutValue,pInValue,ulInLen);
                /* BEGIN: Added by 李文, 2012/10/12   PN:CPE远程维护 
                    添加变长数字类型的解析,该段代码只能用于大头网络设备
                */
                if(g_ulEndian == BIG_ENDIAN)
                {
	                if(ulInLen < sizeof(UINT64))
	                {
	                    *((UINT64 *)pOutValue)>>=((sizeof(UINT64)-ulInLen)*BYTE_BITNUM);
	                }
                }
                /* END:   Added by 李文, 2012/10/12   PN:CPE远程维护 */
			}
			else
			{
				agent_printf(LOG_ERR, "snmp_set_column: ulInLen(%u) should be less than %u", ulInLen, sizeof(UINT64));
				bRet = FALSE;
			}
			break;

		/* 2010/07/02 fengjing cmcc test */
		case TYPE_INT:
            /* BEGIN: Modified by 李文, 2012/10/12   PN:CPE远程维护 
            鉴于SNMP协议，对于数字类型的值，是变长，则不应该与定值做等于比较*/
			//if (ulInLen == sizeof(INT32) && ulColumnLen == sizeof(INT32))
            /* END:   Modified by 李文, 2012/10/12   PN:CPE远程维护 */
			if (ulInLen <= sizeof(INT32) && ulColumnLen == sizeof(INT32))
			{
                /* BEGIN: Modified by 李文, 2012/10/12   PN:CPE远程维护 */
                //*((INT32*)pOutValue) = (INT32)(*((INT32*)pInValue));
                /* END:   Modified by 李文, 2012/10/12   PN:CPE远程维护 */
                memcpy(pOutValue,pInValue,ulInLen);
                /* BEGIN: Added by 李文, 2012/10/12   PN:CPE远程维护 
                    添加变长数字类型的解析,该段代码只能用于大头网络设备
                */
                if(g_ulEndian == BIG_ENDIAN)
                {
	                if(ulInLen < sizeof(INT32))
	                {
	                    *((INT32 *)pOutValue)>>=((sizeof(INT32)-ulInLen)*BYTE_BITNUM);
	                }
                }
                /* END:   Added by 李文, 2012/10/12   PN:CPE远程维护 */
			}
			else
			{
				agent_printf(LOG_ERR, "snmp_set_column: ulInLen(%u) should be less than %u", ulInLen, sizeof(UINT32));
				bRet = FALSE;
			}
			break;

        case TYPE_BIN:
            if (ulInLen == ulColumnLen)
            {
                memcpy(pOutValue, pInValue, ulColumnLen);
            }
            else
            {
                agent_printf(LOG_ERR, "snmp_set_column: ulInLen(%u) should be %u", ulInLen, ulColumnLen);
                bRet = FALSE;
            }
            break;

        case TYPE_IP:
            if (ulInLen == ulColumnLen && ulColumnLen == 4)
            {
                memcpy(pOutValue, pInValue, ulColumnLen);
            }
            else
            {
                agent_printf(LOG_ERR, "snmp_set_column: ulInLen(%u) should be 4", ulInLen);
                bRet = FALSE;
            }
            break;
		// 2010/05/13 fengjing add
		case TYPE_IPV6:
			if (ulInLen == ulColumnLen && ulColumnLen == 16)
            {
                memcpy(pOutValue, pInValue, ulColumnLen);
            }
            else
            {
                agent_printf(LOG_ERR, "snmp_set_column: ulInLen(%u) should be 16", ulInLen);
                bRet = FALSE;
            }
			break;

        case TYPE_MAC:
            /* BEGIN: Added by Zhou Shouya, 2013/5/14   PN:Dlink 临时修改 */
            if( ulInLen == 17)
            {
                sscanf(pInValue, "%02x:%02x:%02x:%02x:%02x:%02x",
                    &ulMac[0],&ulMac[1],&ulMac[2],&ulMac[3],&ulMac[4],&ulMac[5]);
                memset(pInValue, 0, ulInLen);
                for(ulLoop=0; ulLoop<6;ulLoop++)
                {
                    aucMac[ulLoop] = ulMac[ulLoop];
                }
                memcpy(pInValue, aucMac, 6);
                ulInLen = 6;
            }
            /* END:   Added by Zhou Shouya, 2013/5/14 */
            if (ulInLen == ulColumnLen && ulColumnLen == 6)
            {
                memcpy(pOutValue, pInValue, ulColumnLen);
            }
            else
            {
                agent_printf(LOG_ERR, "snmp_set_column: ulInLen(%u) should be 6", ulInLen);
                bRet = FALSE;
            }
            break;

		/* 2011-05-17 fengjing add */
		case TYPE_ROWSTATUS:
            /* BEGIN: Modified by zoucaihong, 2012/11/19 */
            agent_printf(LOG_INFO, "TYPE_ROWSTATUS: ulInLen:%u.",ulInLen);
            if ( ulInLen <= sizeof(INT32) && ulColumnLen == sizeof(INT32))
            {
            	/* 2013-05-06 fengjing modify begin: */
				/* Motorola的PowerPC系列采用big endian方式存储数据，而Intel x86系列则采用little endian方式存储数据。 
					BIG-ENDIAN、LITTLE-ENDIAN是跟CPU有关的，每一种CPU不是BIG-ENDIAN就是LITTLE-ENDIAN
					
					字节顺序是指占内存多于一个字节类型的数据在内存中的存放顺序，通常有小端、大端两种字节顺序。
					小端字节序指低字节数据存放在内存低地址处，高字节数据存放在内存高地址处；
					大端字节序是高字节数据存放在低地址处，低字节数据存放在高地址处。
					所有网络协议也都是采用big endian的方式来传输数据的。
					所以有时我们也会把big endian方式称之为网络字节序 
					当两台采用不同字节序的主机通信时，在发送数据之前都必须经过字节序的转换成为网络字节序后再进行传输。 
					判断小端还是大端规则的方法：
					int x = 1; 
					if(*(char *)&x == 1)    //取x指针强制转换为char*类型再取值，此时取到的值是int最低字节值 
					    printf("little-endian/n"); 
					else 
					    printf("big-endian/n");
					    
				*/			
                memcpy(pOutValue,pInValue,ulInLen);

				lprintf(LOG_INFO, "pOutValue=%d", *((INT32 *)pOutValue));
				
                if(g_ulEndian == BIG_ENDIAN)
                {
	                if ( ulInLen < sizeof(INT32))
	                {
	                    *((INT32 *)pOutValue) >>= ((sizeof(INT32)-ulInLen)*BYTE_BITNUM);
	                }
                }
				lprintf(LOG_INFO, "pOutValue=%d", *((INT32 *)pOutValue));
            	/* 2013-05-06 fengjing modify end */
            }
            else
            {
                agent_printf(LOG_ERR, "snmp_set_column: ulInLen(%u) shouldn't be larger than %u.", ulInLen, sizeof(INT32));
            }
            /* END: Modified by zoucaihong, 2012/11/19 */
			/*if (ulInLen == sizeof(INT32) && ulColumnLen == sizeof(INT32))
			{
				*((INT32*)pOutValue) = (INT32)(*((INT32*)pInValue));
			}
			else
			{
				agent_printf(LOG_ERR, "snmp_set_column: ulInLen(%u) should be %u", ulInLen, sizeof(UINT32));
				bRet = FALSE;
			}*/
			break;

		/* 2011-05-27 fengjing add */
		case TYPE_TADDR:		
			agent_printf(LOG_ERR, "snmp_set_column: pInValue("MAC_FMT") should be 6", 
				MAC_ARG((UINT8 *)pInValue));
			if (ulInLen == ulColumnLen && ulColumnLen == 6)
			{
				memcpy(pOutValue, pInValue, ulColumnLen);
			}
			else
			{
				agent_printf(LOG_ERR, "snmp_set_column: ulInLen(%u) should be 6", ulInLen);
				bRet = FALSE;
			}
			break;
		/* 2011-06-23 fengjing add */
		case TYPE_TIME:
            if (ulInLen == sizeof(UINT32) && ulColumnLen == sizeof(UINT32))
            {
                *((UINT32*)pOutValue) = (UINT32)(*((UINT32*)pInValue));
            }
            else
            {
                agent_printf(LOG_ERR, "snmp_set_column: ulInLen(%u) should be %u", ulInLen, sizeof(UINT32));
                bRet = FALSE;
            }
			break;

        default:
            agent_printf(LOG_ERR, "snmp_set_column: unknow var type(%u)", ulColumnType);
            bRet = FALSE;
            break;
    }

    return bRet;
}



/*************************************************************************
* 函数名称: snmp_get_column
* 功    能: SNMP表查询记录列
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回:
* 说    明:
*************************************************************************/
int handle_snmp_get(request_t *request, response_t *response, client_t *client)
{
	int pos;
	int i;

	SNMP_TABLE_T *pstTable = NULL;
	int ret = -1;

	/* Search each varbinding of the request and append the value to the
	 * response. Note that if the length does not match, we might have found a
	 * subid of the requested one (table cell of table column)!
	 */
	for (i = 0; i < request->oid_list_length; i++) 
	{				
		pos = mib_find(&request->oid_list[i]);
		if (pos == -1) {
			return -1;
		} else if (pos >= g_mib_length) {
			if (request->version == SNMP_VERSION_1) {
				goto nosuchobject;

			} else if (response->value_list_length < MAX_NR_VALUES) {
				goto nosuchobject;
			} else {
				lprintf(LOG_ERR, "could not handle SNMP GET: value list overflow\n");
				return -1;
			}
		} else if (g_mib[pos].oid.subid_list_length == (request->oid_list[i].subid_list_length + 1)) {
			if (request->version == SNMP_VERSION_1) {
				goto nosuchname;
			} else if (response->value_list_length < MAX_NR_VALUES) {
				goto nosuchinstance;
			} else {
				lprintf(LOG_ERR, "could not handle SNMP GET: value list overflow\n");
				return -1;
			}
		} else if (g_mib[pos].oid.subid_list_length != request->oid_list[i].subid_list_length) {
			if (request->version == SNMP_VERSION_1) {
				goto nosuchname;
			} else if (response->value_list_length < MAX_NR_VALUES) {
				goto nosuchobject;
			} else {
				lprintf(LOG_ERR, "could not handle SNMP GET: value list overflow\n");
				return -1;
			}
		} else {
			if (response->value_list_length < MAX_NR_VALUES) {			
				response->value_list[response->value_list_length].data.max_length = g_mib[pos].data.max_length;
				response->value_list[response->value_list_length].data.encoded_length = g_mib[pos].data.encoded_length;
				response->value_list[response->value_list_length].data.buffer = malloc(g_mib[pos].data.max_length);
				memcpy(response->value_list[response->value_list_length].data.buffer,
						g_mib[pos].data.buffer,
						g_mib[pos].data.max_length);
				
				
				response->value_list_length++;
			} else {
				lprintf(LOG_ERR, "could not handle SNMP GET: value list overflow\n");
				return -1;
			}
		}

		continue;
	
	nosuchobject:
	nosuchinstance:
		pstTable = snmp_get_table_by_oid(&(request->oid_list[i]));
		if(pstTable == NULL) 
		{
		    printf("Becacouse of nosuchobject\n");
		    
		    #if 0//added by caihong
			memcpy(&response->value_list[response->value_list_length].oid,
				&request->oid_list[i], sizeof (request->oid_list[i]));	
			#endif
			response->value_list[response->value_list_length].data.max_length = m_no_such_object.max_length;
			response->value_list[response->value_list_length].data.encoded_length = m_no_such_object.encoded_length;
			response->value_list[response->value_list_length].data.buffer = malloc(m_no_such_object.max_length);
			memcpy(response->value_list[response->value_list_length].data.buffer,
					m_no_such_object.buffer,
					m_no_such_object.max_length);
		
			response->value_list_length++;
			ret = 0;
		} 
		else if (pstTable->bIsTable)
		{
			ret = snmp_table_handler(pstTable, request, i, response, client);
	
		}
		else
		{
			ret = snmp_scalar_handler(pstTable, request, i, response, client);
		}

		if (-1 == ret)
		{
			lprintf(LOG_ERR, "could not handle SNMP GET: value list overflow\n");
			return -1;
		}
	}

	return 0;

nosuchname:	
	response->error_status = SNMP_STATUS_NO_SUCH_NAME;
	response->error_index = i;
	return 0;
}



/*************************************************************************
* 函数名称: snmp_get_column
* 功    能: SNMP表查询记录列
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回:
* 说    明:
*************************************************************************/
int handle_snmp_getnext(request_t *request, response_t *response, client_t *client)
{
	int pos;
	int i;
	SNMP_TABLE_T *pstTable = NULL;
	int ret = -1;

	/* Search each varbinding of the request and append the value to the
	 * response. Note that if the length does not match, we might have found a
	 * subid of the requested one (table cell of table column)!
	 */
	for (i = 0; i < request->oid_list_length; i++) 
	{		
		pos = mib_findnext(&request->oid_list[i]);
		if (pos == -1) {
			return -1;
		} else if (pos >= g_mib_length) {
			if (request->version == SNMP_VERSION_1) {
				goto endofmib;
			} else if (response->value_list_length < MAX_NR_VALUES) {
				goto endofmib;
			} else {
				lprintf(LOG_ERR, "File:%s, Line:%d: could not handle SNMP GETNEXT: value list overflow\n",
					__FILE__, __LINE__);
				return -1;
			}
		} else {
			if (response->value_list_length < MAX_NR_VALUES) {			
				memcpy(&response->value_list[response->value_list_length].oid,
					  &g_mib[pos].oid, sizeof(g_mib[pos].oid));
				response->value_list[response->value_list_length].data.max_length = g_mib[pos].data.max_length;
				response->value_list[response->value_list_length].data.encoded_length = g_mib[pos].data.encoded_length;
				response->value_list[response->value_list_length].data.buffer = malloc(g_mib[pos].data.max_length);
				memcpy(response->value_list[response->value_list_length].data.buffer,
						g_mib[pos].data.buffer,
						g_mib[pos].data.max_length);

				response->value_list_length++;
			} else {
				lprintf(LOG_ERR, "File:%s, Line:%d: could not handle SNMP GETNEXT: value list overflow\n",
					__FILE__, __LINE__);
				return -1;
			}
		}
		
		continue;

	endofmib:
		pstTable = snmp_get_table_by_oid(&(request->oid_list[i]));
		if(pstTable == NULL) 
		{
			memcpy(&response->value_list[response->value_list_length].oid,
				&request->oid_list[i], sizeof (request->oid_list[i]));
			
			response->value_list[response->value_list_length].data.max_length = m_end_of_mib_view.max_length;
			response->value_list[response->value_list_length].data.encoded_length = m_end_of_mib_view.encoded_length;
			response->value_list[response->value_list_length].data.buffer = malloc(m_end_of_mib_view.max_length);
			memcpy(response->value_list[response->value_list_length].data.buffer,
					m_end_of_mib_view.buffer,
					m_end_of_mib_view.max_length);

			response->value_list_length++;
			ret = 0;
		} 
		else if (pstTable->bIsTable)
		{
			ret = snmp_table_handler(pstTable, request,i,response,client);
	
		}
		else
		{
			ret = snmp_scalar_handler(pstTable, request,i,response,client);
		}
		
		if (-1 == ret)
		{
			lprintf(LOG_ERR, "File:%s, Line:%d: could not handle SNMP GETNEXT: value list overflow\n",
				__FILE__, __LINE__);
			return -1;
		}		
	}
 
	return 0;

}

/*************************************************************************
* 函数名称: snmp_get_column
* 功    能: SNMP表查询记录列
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回:
* 说    明:
*************************************************************************/
int handle_snmp_set(request_t *request, response_t *response, client_t *client)
{
	int i;
	SNMP_TABLE_T *pstTable = NULL;
	int ret = -1;

	/* Search each varbinding of the request and append the value to the
	 * response. Note that if the length does not match, we might have found a
	 * subid of the requested one (table cell of table column)!
	 */
	for (i = 0; i < request->oid_list_length; i++) 
	{	
		pstTable = snmp_get_table_by_oid(&(request->oid_list[i]));
		if(pstTable == NULL) 
		{
		    printf("Becascaue of no Table!\n");
		    #if 0
			memcpy(&response->value_list[response->value_list_length].oid,
				&request->oid_list[i], sizeof (request->oid_list[i]));
			
			memcpy(&response->value_list[response->value_list_length].data,
				&m_no_such_object, sizeof (m_no_such_object));	
		    #endif
			memcpy(&response->value_list[response->value_list_length].oid,
				&request->oid_list[i], sizeof (request->oid_list[i]));

		    response->value_list[response->value_list_length].data.max_length = m_no_such_object.max_length;
			response->value_list[response->value_list_length].data.encoded_length = m_no_such_object.encoded_length;
			response->value_list[response->value_list_length].data.buffer = malloc(m_no_such_object.max_length);
			memcpy(response->value_list[response->value_list_length].data.buffer,
					m_no_such_object.buffer,
					m_no_such_object.max_length);
			response->value_list_length++;
			ret = 0;
		} 
		else if (pstTable->bIsTable)
		{
			ret = snmp_table_handler(pstTable, request,i,response,client);
		}
		else
		{
			ret = snmp_scalar_handler(pstTable, request,i,response,client);
		}

		if (-1 == ret)
		{
			lprintf(LOG_ERR, "could not handle SNMP SET: value list overflow\n");
			return -1;
		}
		
	}


	return 0;
}

/*************************************************************************
* 函数名称: snmp_get_column
* 功    能: SNMP表查询记录列
* 参    数:
* 参数名称			输入/输出				描述
* 函数返回:
* 说    明:
*************************************************************************/
int handle_snmp_getbulk(request_t *request, response_t *response, client_t *client)
{
	oid_t oid_list[MAX_NR_OIDS];
	int oid_list_length;
	int found_repeater;
	int pos;
	int i;
	int j;
    /* BEGIN: Added by Zhou Shouya, 2013/8/30 */
    oid_t new_oid_list[MAX_NR_OIDS];
    /* END:   Added by Zhou Shouya, 2013/8/30 */
	
	SNMP_TABLE_T *pstTable = NULL;
	int ret = -1;

	memset(new_oid_list, 0, sizeof(new_oid_list));

	/* Make a local copy of the OID list since we are going to modify it */
	memcpy(oid_list, request->oid_list, sizeof (request->oid_list));
	oid_list_length = request->oid_list_length;

	/* Limit the non-repeaters and the maximum repetitions to zero */
	if (request->non_repeaters < 0) {
		request->non_repeaters = 0;
	}
	if (request->max_repetitions < 0) {
		request->max_repetitions = 0;
	}

	/* The non-repeaters are handled like with the GETNEXT request */
	for (i = 0; i < oid_list_length; i++) {
		if (i >= request->non_repeaters) {
			break;
		}

		pos = mib_findnext(&oid_list[i]);
		if (pos == -1) {
			return -1;
		} else if (pos >= g_mib_length) {
			if (response->value_list_length < MAX_NR_VALUES) {
				goto endofmib;
			} else {
				lprintf(LOG_ERR, "could not handle SNMP GETNEXT: value list overflow\n");
				return -1;
			}
		} else {
			if (response->value_list_length < MAX_NR_VALUES) {
				/* 2013-05-16 fengjing modify BEGIN */
				#if 0
				memcpy(&response->value_list[response->value_list_length],
					&g_mib[pos], sizeof (g_mib[pos]));
				#else				
				memcpy(&response->value_list[response->value_list_length].oid,
					  &g_mib[pos].oid, sizeof(g_mib[pos].oid));
				response->value_list[response->value_list_length].data.max_length = g_mib[pos].data.max_length;
				response->value_list[response->value_list_length].data.encoded_length = g_mib[pos].data.encoded_length;
				response->value_list[response->value_list_length].data.buffer = malloc(g_mib[pos].data.max_length);
				memcpy(response->value_list[response->value_list_length].data.buffer,
						g_mib[pos].data.buffer,
						g_mib[pos].data.max_length);
				#endif
				/* 2013-05-16 fengjing modify END */
				response->value_list_length++;
			} else {
				lprintf(LOG_ERR, "could not handle SNMP GETNEXT: value list overflow\n");
				return -1;
			}
		}

		continue;

	endofmib:
		pstTable = snmp_get_table_by_oid(&(oid_list[i]));
		if(pstTable == NULL) 
		{
			memcpy(&response->value_list[response->value_list_length].oid,
				&oid_list[i], sizeof (oid_list[i]));
			/* 2013-05-16 fengjing modify BEGIN */
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
			/* 2013-05-16 fengjing modify END */
			response->value_list_length++;
			ret = 0;
		} 
		else if (pstTable->bIsTable)
		{
			ret = snmp_table_handler(pstTable, request,i,response,client);

		}
		else
		{
			ret = snmp_scalar_handler(pstTable, request,i,response,client);
		}
		
		if (-1 == ret)
		{
			lprintf(LOG_ERR, "could not handle SNMP GETNEXT: value list overflow\n");
			return -1;
		}
	}

	/* The repeaters are handled like with the GETNEXT request, except that:
	 *
	 * - the access is interleaved (i.e. first repetition of all varbinds,
	 *   then second repetition of all varbinds, then third,...)
	 * - the repetitions are aborted as soon as there is no successor found
	 *   for all of the varbinds
	 * - other than with getnext, the last variable in the MIB is named if
	 *   the variable queried is not after the end of the MIB
	 */
	for (j = 0; j < request->max_repetitions; j++) {
		found_repeater = 0;
		for (i = request->non_repeaters; i < oid_list_length; i++) 
		{
			pos = mib_findnext(&oid_list[i]);
			if (pos == -1) {
				return -1;
			} else if (pos >= g_mib_length) {
				if (response->value_list_length < MAX_NR_VALUES) {
					goto endofmib2;
				} else {
					lprintf(LOG_ERR, "could not handle SNMP GETNEXT: value list overflow\n");
					return -1;
				}
			} else {
				if (response->value_list_length < MAX_NR_VALUES) {
					/* 2013-05-16 fengjing modify BEGIN */
					#if 0
					memcpy(&response->value_list[response->value_list_length],
						&g_mib[pos], sizeof (g_mib[pos]));
					#else				
					memcpy(&response->value_list[response->value_list_length].oid,
						  &g_mib[pos].oid, sizeof(g_mib[pos].oid));
					response->value_list[response->value_list_length].data.max_length = g_mib[pos].data.max_length;
					response->value_list[response->value_list_length].data.encoded_length = g_mib[pos].data.encoded_length;
					response->value_list[response->value_list_length].data.buffer = malloc(g_mib[pos].data.max_length);
					memcpy(response->value_list[response->value_list_length].data.buffer,
							g_mib[pos].data.buffer,
							g_mib[pos].data.max_length);
					#endif
					/* 2013-05-16 fengjing modify END */
					response->value_list_length++;
					memcpy(&oid_list[i], &g_mib[pos].oid, sizeof (g_mib[pos].oid));
					found_repeater++;
				} else {
					lprintf(LOG_ERR, "could not handle SNMP GETNEXT: value list overflow\n");
					return -1;
				}
			}
			continue;
			
		endofmib2:
			
			pstTable = snmp_get_table_by_oid(&(oid_list[i]));
			if(pstTable == NULL) 
			{
				memcpy(&response->value_list[response->value_list_length].oid,
					&oid_list[i], sizeof (oid_list[i]));
				/* 2013-05-16 fengjing modify BEGIN */
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
				/* 2013-05-16 fengjing modify END */
				response->value_list_length++;
				ret = 0;
			} 
			else if (pstTable->bIsTable)
			{
				ret = snmp_table_handler(pstTable, request,i,response,client);
			}
			else
			{
				ret = snmp_scalar_handler(pstTable, request,i,response,client);
			}
			
			if (-1 == ret)
			{
				lprintf(LOG_ERR, "could not handle SNMP GETNEXT: value list overflow\n");
				return -1;
			}			
			
			/* no j repeater, record no i old_old getnext response new_oid */
			/* BEGIN: Modified by Zhou Shouya, 2013/8/30 */
            memcpy(&new_oid_list[i],
				&response->value_list[response->value_list_length-1].oid,
                   sizeof(new_oid_list[i]));
            /* END:   Modified by Zhou Shouya, 2013/8/30 */
			found_repeater++;
			/* BEGIN: Added by Zhou Shouya, 2013/8/30 */
            continue;
            /* END:   Added by Zhou Shouya, 2013/8/30 */
		}
		
		if (found_repeater == 0) {
			break;
		}

		/* cover request oid_list by no j repeater getnext response new_oid list,then j+1 repeater */
        /* BEGIN: Added by Zhou Shouya, 2013/8/30 */
        for (i = request->non_repeaters; i < oid_list_length; i++)
        {            
            memcpy(&oid_list[i], &new_oid_list[i], sizeof(oid_list[i]));
			/* BEGIN: Modified by Zhou Shouya, 2013/8/30 */
            memcpy(&request->oid_list[i], &(oid_list[i]), sizeof (oid_list[i]));
            /* END:   Modified by Zhou Shouya, 2013/8/30 */
        }
        /* END:   Added by Zhou Shouya, 2013/8/30 */
	}

	return 0;
}



