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

/*************************************************************************/
extern OSTOKEN gtSemConfig;	/* 配置互斥信号量 */
extern OSTOKEN gtSemStat;      /* 统计互斥信号量 */

/*************************************************************************
* 函数名称: snmp_get_scalar_data
* 功    能: 获取标量记录内容
* 参    数: 
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明: 
*************************************************************************/
CHAR *snmp_get_scalar_data(SNMP_TABLE_T *pstTable)
{	
	UINT8        *pucData = NULL;
	BOOL         bRet = FALSE;
	//UINT32       ulRecLen = 0;
	
	if (pstTable == NULL)
	{
		agent_printf(LOG_ERR, "%s:snmp_get_scalar_data pstTable is null\n",
			SNMPAGENT_CONTROL_NAME);
		return NULL;
	}	

	/* 加锁操作 */
	if ((pstTable->ulTableID>=TABLE_ID_AP_CONFIG_BEGIN &&
		pstTable->ulTableID<=TABLE_ID_AP_CONFIG_END))
	{		
		if (!OsSemaphoreP(gtSemConfig, WAIT_FOREVER, 1))
    	{
			agent_printf(LOG_ERR,
			"File:%s, Line:%d: gtSemConfig OsSemaphoreP fail\n",
			__FILE__, __LINE__);
        	return NULL;
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
        	return NULL;
    	}
	}
	
	bRet = snmp_get_table_reget_status(pstTable);
	if (!bRet)
	{
		agent_printf(LOG_INFO, "snmp_get_table_reget_status() not need reget\n");
		goto end;
	}
	
	pucData = snmp_get_table_record(pstTable);
	if (pucData == NULL)
	{
		agent_printf(LOG_ERR, "snmp_get_scalar_data:table:%u,%s pucData is null\n", 
			pstTable->ulTableID,
			pstTable->acTableName);
		goto end;
	}
		
	if ((pstTable->ulTableID>=TABLE_ID_AP_CONFIG_BEGIN &&
		pstTable->ulTableID <= TABLE_ID_AP_CONFIG_END)
		|| (pstTable->ulTableID>=TABLE_ID_AP_OPER_BEGIN &&
		pstTable->ulTableID <= TABLE_ID_AP_OPER_END))
	{		
		if (NULL == pstTable->pTableData)
		{
			pstTable->pTableData = SNMP_AGENT_MALLOC(pstTable->ulTableRecLen);
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

	if (NULL == pstTable->pTableData)
	{
		goto end;
	}
	
	if ((pstTable->ulTableID>=TABLE_ID_AP_CONFIG_BEGIN &&
		pstTable->ulTableID <= TABLE_ID_AP_CONFIG_END)
		|| (pstTable->ulTableID>=TABLE_ID_AP_OPER_BEGIN &&
		pstTable->ulTableID <= TABLE_ID_AP_OPER_END))
	{		
		memset(pstTable->pTableData, 0, pstTable->ulTableRecLen);
		pstTable->pOriTableData = pucData;
		
		memcpy(pstTable->pTableData, pucData, pstTable->ulTableRecLen);
	}
	else if ((pstTable->ulTableID>=TABLE_ID_AP_STAT_BEGIN &&
		      pstTable->ulTableID <= TABLE_ID_AP_STAT_END))
	{		
	}
	
	snmp_set_table_reget_status(pstTable);
	
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
	
	return pstTable->pTableData;
}

/*************************************************************************
* 函数名称: snmp_set_table_status
* 功    能: 获取表的修改状态
* 参    数: 
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明: 
*************************************************************************/
BOOL snmp_scalar_set_commit(SNMP_TABLE_T *pstTable, char *szOperator)
{	
	CHAR    *szRecData;
	CHAR    *szOriRecData;
	
	szRecData = pstTable->pTableData;
	szOriRecData = pstTable->pOriTableData;
    
	/* BEGIN: Modified by 李文, 2012/10/11   PN:CPE远程维护 
        防止回调函数为NULL，出现段错误
    */
	if (pstTable->pfTableValidFunc != NULL)
	{
    	if (!pstTable->pfTableValidFunc(szRecData, szOriRecData))
    	{
    		agent_printf(LOG_ERR, "%s: snmp_scalar_set_commit tableValidFunc fail\n", SNMPAGENT_CONTROL_NAME);
            /*失败，旧数据覆盖新数据*/
    		memcpy(szRecData, szOriRecData, pstTable->ulTableRecLen);
    	
    		return FALSE;
    	}
    }
	/* END:   Modified by 李文, 2012/10/11   PN:CPE远程维护 */
    /*成功，新数据覆盖旧数据*/
	memcpy(szOriRecData, szRecData, pstTable->ulTableRecLen);
	
	snmp_reset_table_reget_status(pstTable->ulTableID);
	return TRUE;
}


/*************************************************************************
* 函数名称: snmp_set_table_status
* 功    能: 获取表的修改状态
* 参    数: 
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明: 
*************************************************************************/
BOOL snmp_scalar_get_column(SNMP_TABLE_T *pstTable, UINT32 ulColumnIndex, response_t *response, client_t *client)
{		
	CHAR  *pValueAddr = NULL;
	oid_t row={{0}, 1, 2};
	BOOL  bRet = FALSE;

	pValueAddr = pstTable->pTableData;
	
	bRet = snmp_get_column(pstTable, ulColumnIndex, &row, pValueAddr, response, client);
	if (FALSE == bRet)
	{
		agent_printf(LOG_ERR, "snmp_scalar_get_column acTableName:%s ulColumnIndex:%u failed",
			pstTable->acTableName, ulColumnIndex);
	}

	return bRet;
}

/*************************************************************************
* 函数名称: snmp_set_table_status
* 功    能: 获取表的修改状态
* 参    数: 
* 参数名称			输入/输出				描述
* 函数返回: 
* 说    明: 
*************************************************************************/
int snmp_scalar_handler(SNMP_TABLE_T *pstTable, request_t *request, int i, response_t *response, client_t *client)
{
	UINT32 ulColumnIndex = 0;
	oid_t row;

	SNMP_TABLE_T *pstNextTable = NULL;
	
	UINT16 usColumnType = 0;
	UINT16 usColumnLen = 0;
	int ucType;

	BOOL bRet = FALSE;
		
	CHAR  *pValueAddr = NULL;
	CHAR  *pOriValueAddr = NULL;

	UINT32 ulOffset = 0;
	BOOL bPermitSet = FALSE;
	
	ulOffset = pstTable->ulOIDSize;

	// column node
	if(request->oid_list[i].subid_list_length>ulOffset)
	{
		ulColumnIndex = request->oid_list[i].subid_list[ulOffset] - 1;
		if (ulColumnIndex >= pstTable->ulColumnNum)
		{
			agent_printf(LOG_ERR, "table:%s ulColumnIndex:%u >= pstTable->ulColumnNum:%u\n",
				pstTable->acTableName,
				ulColumnIndex,
				pstTable->ulColumnNum);
			goto generr;
		}
		ulOffset++;
	}
	agent_printf(LOG_INFO, "oid:%s", oid_ntoa(&(request->oid_list[i])));
	agent_printf(LOG_INFO, "acTableName:%s ulColumnIndex:%u", pstTable->acTableName, ulColumnIndex);

	//.0
	if(request->oid_list[i].subid_list_length>ulOffset)
	{
		if(request->oid_list[i].subid_list[ulOffset] == 0)
		{
			bPermitSet = TRUE;
		}
	}
	
	usColumnType = pstTable->astColumns[ulColumnIndex].ucColumnType;
	usColumnLen = pstTable->astColumns[ulColumnIndex].usColumnLen;
	ucType = snmp_get_asn1_type(usColumnType);
	
	pValueAddr = snmp_get_scalar_data(pstTable);	
	if (NULL == pValueAddr)
	{			
		agent_printf(LOG_ERR, "snmp_get_scalar_data:pstTable->acTableName:%s failed\n", pstTable->acTableName);
		goto generr;
	}
	
	if (pValueAddr)
	{
		pValueAddr += pstTable->astColumns[ulColumnIndex].usOffSet;
		pOriValueAddr = ((CHAR*)pstTable->pOriTableData) + pstTable->astColumns[ulColumnIndex].usOffSet;
	}
	
	switch(request->type)
	{
		case BER_TYPE_SNMP_GET:			
			memcpy(&response->value_list[response->value_list_length].oid,
				&request->oid_list[i], sizeof (request->oid_list[i]));
			bRet = snmp_scalar_get_column(pstTable, ulColumnIndex, response, client);
			if(!bRet)
			{
				lprintf(LOG_ERR, "File:%s, Line:%d: snmp_scalar_get_column failed\n",
					__FILE__, __LINE__);
				goto generr;
			}
			break;
		case BER_TYPE_SNMP_GETNEXT:
		case BER_TYPE_SNMP_GETBULK:
			if (bPermitSet)
			{
				if (ulColumnIndex<(pstTable->ulColumnNum-1))
				{
					ulColumnIndex++;
				}
				else
				{
					goto getnext;
				}
			}
			
			bRet = snmp_scalar_get_column(pstTable, ulColumnIndex, response, client);
			if(!bRet)
			{
				lprintf(LOG_ERR, "File:%s, Line:%d: snmp_scalar_get_column failed\n",
					__FILE__, __LINE__);
				goto generr;
			}
			break;
		case BER_TYPE_SNMP_SET:			
			if(!bPermitSet)
			{
				lprintf(LOG_ERR, "File:%s, Line:%d: not permit set\n",
					__FILE__, __LINE__);
				goto generr;				
			}
			/* check val type and len */
			if (request->data_list[i].max_length != ucType)
			{
				lprintf(LOG_ERR, "File:%s, Line:%d: requetDataType:%u registerDataType:%u\n",
					__FILE__, __LINE__, request->data_list[i].max_length, ucType);
				goto generr;
			}

			if(request->data_list[i].encoded_length>usColumnLen)
			{
				lprintf(LOG_ERR, "File:%s, Line:%d: requetDataLen:%u registerDataLen:%u\n",
					__FILE__, __LINE__, request->data_list[i].encoded_length, usColumnLen);
				goto generr;
			}
			
			if (!snmp_set_column(usColumnType, 
				usColumnLen, 
				request->data_list[i].buffer, 
				request->data_list[i].encoded_length, 
				pValueAddr))
	        {
	            if (pValueAddr)
	            {
	                memcpy(pValueAddr, pOriValueAddr, usColumnLen);
	            }
				lprintf(LOG_ERR, "File:%s, Line:%d: snmp_set_column failed\n",
					__FILE__, __LINE__);
				goto generr;
	        }

			if(0 != memcmp(pstTable->pTableData, pstTable->pOriTableData, pstTable->ulTableRecLen))
		    {
		        bRet = snmp_scalar_set_commit(pstTable, request->community);
    			if (!bRet)
    		    {
    				lprintf(LOG_ERR, "File:%s, Line:%d: snmp_scalar_set_commit failed\n",
    					__FILE__, __LINE__);
    		    	goto generr;
    			}
		    }
		    /* BEGIN: Added by Zhou Shouya, 2013/9/11 */
		    /* 增加返回值处理 */
			bRet = snmp_scalar_get_column(pstTable, ulColumnIndex, response, client);
			if(!bRet)
			{
				lprintf(LOG_ERR, "File:%s, Line:%d: snmp_scalar_get_column failed\n",
					__FILE__, __LINE__);
				goto generr;
			}
			break;
		    /* END:   Added by Zhou Shouya, 2013/9/11 */
		    
			/* BEGIN: Modified by 李文, 2012/10/12   PN:CPE远程维护 
                代码上移，配置项与当前一样，不应该返回错误
            */
			//if (!bRet)
		    //{
			//	lprintf(LOG_ERR, "File:%s, Line:%d: snmp_scalar_set_commit failed\n",
			//		__FILE__, __LINE__);
		    //	goto generr;
			//}
			/* END:   Modified by 李文, 2012/10/12   PN:CPE远程维护 */
		
			break;
		default:
			break;
	}

	return 0;

getnext:
	
	pstNextTable = snmp_get_next_table(pstTable);
	if(NULL == pstNextTable)
	{
		goto endofmib;
	}

	if(pstNextTable->bIsTable)
	{ 
		/*table,the first record,the first column value*/
		snmp_row_build(pstNextTable, 0, pstNextTable->pTableData, &row);
		bRet = snmp_table_get_column(pstNextTable, 0, &row, pstNextTable->pTableData, response, client);
		if(!bRet)
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
		if(!bRet)
		{
			lprintf(LOG_ERR, "File:%s, Line:%d: snmp_scalar_get_column failed\n",
					__FILE__, __LINE__);
			goto generr;
		}
	}

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


