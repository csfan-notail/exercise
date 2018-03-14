/******************************************************************************

                  版权所有 (C), 2007-2012, 上海寰创通信科技有限公司

******************************************************************************
   文 件 名   : lancfg_cmd.c
   版 本 号   : 初稿
   作    者   : 曹瑜
   生成日期   : 2014年4月23日
   最近修改   :
   功能描述   : lancfg cmd
   函数列表   :
   修改历史   :
   1.日    期   : 2014年4月23日
    作    者   : 曹瑜
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
* 包含头文件                                   *
*----------------------------------------------*/
#include "pdu.h"
/*----------------------------------------------*
* 外部变量说明                                 *
*----------------------------------------------*/

/*----------------------------------------------*
* 外部函数原型说明                             *
*----------------------------------------------*/

/*----------------------------------------------*
* 内部函数原型说明                             *
*----------------------------------------------*/

INT32 pdu_to_telephone_number(char *pstStr, UINT32 ulStrLen, char *pstTel, UINT32 ulTelLen)
{
    int iLoop;

    /*pdu表示的电话号码必须是偶数位，如果是基数位号码，会以F补为偶数位*/
    if ((ulStrLen % 2) != 0)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    if (ulStrLen > ulTelLen)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    for (iLoop = 0; iLoop < ulStrLen; iLoop += 2)
    {
        pstTel[iLoop] = pstStr[iLoop + 1];
        pstTel[iLoop + 1] = pstStr[iLoop];
    }

    if (pstTel[ulStrLen - 1] == 'F')
    {
        pstTel[ulStrLen - 1] = 0;
    }

    return PDU_E_NONE;
}

INT32 pdu_to_time(char *pstStr, char *pstTime, UINT32 ulTimeLen, char cSign)
{
    /*ulTimeLen包含两个分隔符长度*/
    if (ulTimeLen < TIME_PDU_LEN + 2)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    pstTime[0] = pstStr[1];
    pstTime[1] = pstStr[0];
    pstTime[2] = cSign;
    pstTime[3] = pstStr[3];
    pstTime[4] = pstStr[2];
    pstTime[5] = cSign;
    pstTime[6] = pstStr[5];
    pstTime[7] = pstStr[4];

    return PDU_E_NONE;
}

INT8 char_to_hex(char c)
{
    if (c >= '0' && c <= '9')
    {
        return (c - 0x30);
    }
    else if (c >= 'A' && c <= 'F')
    {
        return (c - 0x37);
    }
    else if (c >= 'a' && c <= 'f')
    {
        return (c - 0x57);
    }

    return 0;
}

int enc_unicode_to_utf8_one(UINT32 unic, UINT8 *pOutput, int outSize)
{
    if (unic <= 0x0000007F)
    {
        // * U-00000000 - U-0000007F:  0xxxxxxx
        if (outSize < 1)
        {
            return PDU_E_DEFAULT_FAIL;
        }

        *pOutput = (unic & 0x7F);
        /* BEGIN: Added by wujian, 2015/3/19   PN:解决界面换行符显示^M的bug，将回车符转成空格 */
        if( *pOutput == 0x0d)
            *pOutput = 0x20;
        /* END:   Added by wujian, 2015/3/19   PN:解决界面换行符显示^M的bug，将回车符转成空格 */
        return 1;
    }
    else if (unic >= 0x00000080 && unic <= 0x000007FF)
    {
        // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
        if (outSize < 2)
        {
            return PDU_E_DEFAULT_FAIL;
        }

        *(pOutput + 1) = (unic & 0x3F) | 0x80;
        *pOutput = ((unic >> 6) & 0x1F) | 0xC0;
        return 2;
    }
    else if (unic >= 0x00000800 && unic <= 0x0000FFFF)
    {
        // * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
        if (outSize < 3)
        {
            return PDU_E_DEFAULT_FAIL;
        }

        *(pOutput + 2) = (unic & 0x3F) | 0x80;
        *(pOutput + 1) = ((unic >> 6) & 0x3F) | 0x80;
        *pOutput = ((unic >> 12) & 0x0F) | 0xE0;
        return 3;
    }
    else if (unic >= 0x00010000 && unic <= 0x001FFFFF)
    {
        // * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        if (outSize < 4)
        {
            return PDU_E_DEFAULT_FAIL;
        }

        *(pOutput + 3) = (unic & 0x3F) | 0x80;
        *(pOutput + 2) = ((unic >> 6) & 0x3F) | 0x80;
        *(pOutput + 1) = ((unic >> 12) & 0x3F) | 0x80;
        *pOutput = ((unic >> 18) & 0x07) | 0xF0;
        return 4;
    }
    else if (unic >= 0x00200000 && unic <= 0x03FFFFFF)
    {
        // * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        if (outSize < 5)
        {
            return PDU_E_DEFAULT_FAIL;
        }

        *(pOutput + 4) = (unic & 0x3F) | 0x80;
        *(pOutput + 3) = ((unic >> 6) & 0x3F) | 0x80;
        *(pOutput + 2) = ((unic >> 12) & 0x3F) | 0x80;
        *(pOutput + 1) = ((unic >> 18) & 0x3F) | 0x80;
        *pOutput = ((unic >> 24) & 0x03) | 0xF8;
        return 5;
    }
    else if (unic >= 0x04000000 && unic <= 0x7FFFFFFF)
    {
        // * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        if (outSize < 6)
        {
            return PDU_E_DEFAULT_FAIL;
        }

        *(pOutput + 5) = (unic & 0x3F) | 0x80;
        *(pOutput + 4) = ((unic >> 6) & 0x3F) | 0x80;
        *(pOutput + 3) = ((unic >> 12) & 0x3F) | 0x80;
        *(pOutput + 2) = ((unic >> 18) & 0x3F) | 0x80;
        *(pOutput + 1) = ((unic >> 24) & 0x3F) | 0x80;
        *pOutput = ((unic >> 30) & 0x01) | 0xFC;
        return 6;
    }

    return 0;
}

INT32 string_to_hex(char *pstStr, UINT32 ulStrLen, UINT8 *pucHex, UINT32 ulHexLen)
{
    int iLoop, iNum, iHex;

    if (ulHexLen * 2 < ulStrLen)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    for (iLoop = 0; iLoop < ulStrLen; iLoop++)
    {
        iHex = char_to_hex(pstStr[iLoop]);

        iNum = iLoop / 2;
        if ((iLoop % 2) == 0)
        {
            pucHex[iNum] = (iHex & 0x0000000f) << 4;
        }
        else
        {
            pucHex[iNum] |= (iHex & 0x0000000f);
        }
    }

    return PDU_E_NONE;
}


/*****************************************************************************
 函 数 名  : pdu_bit7_indiv_char_deal
 功能描述  : 7bit编码方式特殊字符处理，基本字符。参考3gpp 23038协议6.2.1章
 输入参数  : UINT8 ucTmp  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年1月21日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
UINT8 pdu_bit7_indiv_char_deal(UINT8 ucTmp)
{

    switch(ucTmp)
    {
        case 0x00: /* @ */
            ucTmp = 0x40;
            break;
        case 0x02: /* $ */
            ucTmp = 0x24;
            break;
        case 0x11: /* _ */
            ucTmp = 0x5f;
            break; 
        case 0x1b: /* ^[  扩展字符 转意*/
            break;
        /* BEGIN: Added by wujian, 2015/3/19   PN:解决界面换行符显示^M的bug，将回车符转成空格 */
        case 0x0d: /* _ */
            ucTmp = 0x20;
            break; 
        /* END:   Added by wujian, 2015/3/19   PN:解决界面换行符显示^M的bug，将回车符转成空格 */
        default:
            break;

    }
    return ucTmp;

}


/*****************************************************************************
 函 数 名  : pdu_bit7_indiv_char_deal_ex
 功能描述  : 7bit编码方式扩展字符处理，一个扩展字符用两个字符表示:^[ + 扩展字符
 输入参数  : UINT8 ucTmp  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年1月21日
    作    者   : wujian
    修改内容   : 新生成函数

*****************************************************************************/
UINT8 pdu_bit7_indiv_char_deal_ex(UINT8 ucTmp)
{

    switch(ucTmp)
    {
        case 0x28: /* { */
            ucTmp = 0x7b;         
            break;
        case 0x40: /* | */
            ucTmp = 0x7c;
            break;
        case 0x29: /* } */
            ucTmp = 0x7d;
            break;
        case 0x3d: /* ~ */
            ucTmp = 0x7e;
            break;
        case 0x3c: /* [ */
            ucTmp = 0x5b;
            break;
        case 0x2f: /* \ */
            ucTmp = 0x5c;
            break;
        case 0x3e: /* ] */
            ucTmp = 0x5d;
            break;
        case 0x14: /* ^ */
            ucTmp = 0x5e;
            break;
        default:
            break;
                    
    }
    return ucTmp;
}


INT32 bit7_to_ascii(char **ppstStr, UINT32 *pulStrLen, UINT32 ulBegin, INT32 ulLength, UINT8 *pstAscii, UINT32 ulAsciiLen, UINT32 *pulAsciiOff, UINT32 *pulLenOff)
{
    UINT8 aucBuf[BIT7_BUFFER_LEN+1] = {0};
    //UINT8 aucTmp = 0;	
    UINT32 ulBit7StrLen, ulTranslateLen;
    UINT8 ucMask;
    int iSecondFlag = 0;
	//int iLoopSec = 0;
    int iCount = 0;
    int iLoop, iLoopMax, iRet;

    STRING_PUSH(pstAscii, ulAsciiLen, *pulAsciiOff);
    ulLength -= *pulLenOff;

    //printf("--BIT7TOASCII: pulAsciiOff %d, pulLenOff %d\n", *pulAsciiOff, *pulLenOff);
    do
    {
        //printf("BIT7TOASCII:strLen %d Length %d AsciiLan %d Offset %d\n", *pulStrLen, ulLength, ulAsciiLen, *pulAsciiOff);
        if (ulLength < TRANSLATE_LEN)
        {
            ulTranslateLen = ulLength;
            ulBit7StrLen = *pulStrLen;
        }
        else
        {
            ulTranslateLen = TRANSLATE_LEN;
            ulBit7StrLen = (BIT7_BUFFER_LEN - ulBegin) * 2;
        }

        if (*pulStrLen < ulBit7StrLen)
        {
            return PDU_READMORE;
        }
        memset( aucBuf, 0, sizeof(aucBuf) );
        
        //printf("BIT7TOASCII:%d :TranslateLen %d, Bit7StrLen %d\n",__LINE__, ulTranslateLen, ulBit7StrLen);
        iRet = string_to_hex(*ppstStr, ulBit7StrLen, (aucBuf + ulBegin), (BIT7_BUFFER_LEN - ulBegin));
        if (iRet < 0)
        {
            return PDU_ERROR;
        }
        STRING_PUSH(*ppstStr, *pulStrLen, ulBit7StrLen);
        
        iLoopMax = ulAsciiLen < ulTranslateLen ? ulAsciiLen : ulTranslateLen;
        
        //printf("%d :ulLength: %d, ulBit7StrLen: %d\n",__LINE__, ulLength, ulBit7StrLen);
        *pstAscii = 0;
        for (iLoop = 0; iLoop < iLoopMax; iLoop++)
        {
            ucMask = BIT7_MASK >> iLoop;
            pstAscii[iLoop] |= (aucBuf[iLoop] & ucMask) << iLoop;
            pstAscii[iLoop+1] = (aucBuf[iLoop] & (~ucMask)) >> (BIT7_BUFFER_LEN - iLoop);

            pstAscii[iLoop] = pdu_bit7_indiv_char_deal(pstAscii[iLoop]);
        
            if(pstAscii[iLoop] == 0x1b) /*7bit特殊字符处理，参考3gpp 23038协议6.2.1章*/
            {
                iSecondFlag = 1;
                iCount++;
				
            }
			else if(iSecondFlag == 1)
            {
                pstAscii[iLoop-iCount] = pdu_bit7_indiv_char_deal_ex(pstAscii[iLoop]);
                iSecondFlag = 0;
            }
            else
            {
                pstAscii[iLoop-iCount] = pstAscii[iLoop];
				
            }
			
            //printf("%d :iCount: %d, iLoop-iCount: %d\n",__LINE__,iCount,iLoop-iCount);
            //printf("TRANSLATE ASCII %d: bit7 %02x mask %02x ~mask %02x Ascii %02x [%c] Ascii+1 %02x\n", iLoop, aucBuf[iLoop], ucMask, ~ucMask, pstAscii[iLoop], pstAscii[iLoop], pstAscii[iLoop+1]);
        }
        pstAscii[iLoop-iCount]= 0;
        //printf("%d :pstAscii: %s\n",__LINE__, pstAscii);
        if(ulBegin != 0)
        {
            ulTranslateLen = BIT7_BUFFER_LEN - ulBegin;
            memmove(pstAscii, pstAscii + (TRANSLATE_LEN - ulTranslateLen) , ulTranslateLen);
            memset(pstAscii + (TRANSLATE_LEN - ulTranslateLen), 0,  ulTranslateLen);
            
            STRING_PUSH(pstAscii, ulAsciiLen, ulTranslateLen);
            ulLength -= ulTranslateLen;
            *pulAsciiOff += ulTranslateLen;
            *pulLenOff += ulTranslateLen;
        }
        else
        {
            STRING_PUSH(pstAscii, ulAsciiLen, (iLoop-iCount));
            ulLength -= ulTranslateLen;
            *pulAsciiOff += (iLoop-iCount);
            *pulLenOff += ulTranslateLen;
        }

        ulBegin = 0;
        iCount = 0;
    }
    while (ulLength > 0);

    return PDU_OK;
}

INT32 bit8_to_hex(char **ppstStr, UINT32 *pulStrLen, INT32 ulLength, UINT8 *pstHex, UINT32 ulHexLen, UINT32 *pulHexOff, UINT32 *pulLenOff)
{
    UINT32 ulBit8StrLen, ulTranslateLen;
    int iRet;

    STRING_PUSH(pstHex, ulHexLen, *pulHexOff);
    ulLength -= *pulLenOff;

    ulTranslateLen = ulLength < ulHexLen ? ulLength : ulHexLen;
    ulBit8StrLen = ulTranslateLen * 2;

    if (*pulStrLen < ulBit8StrLen)
    {
        if (*pulStrLen % 2 == 0)
        {
            ulBit8StrLen = *pulStrLen;
        }
        else
        {
            ulBit8StrLen = *pulStrLen - 1;
        }
        ulTranslateLen = ulBit8StrLen / 2;
        iRet = string_to_hex(*ppstStr, ulBit8StrLen, pstHex, ulHexLen);
        if (iRet < 0)
        {
            return PDU_ERROR;
        }
        STRING_PUSH(*ppstStr, *pulStrLen, ulBit8StrLen);
        *pulHexOff += ulTranslateLen;
        *pulLenOff += ulTranslateLen;
        return PDU_READMORE;
    }
    else
    {
        iRet = string_to_hex(*ppstStr, ulBit8StrLen, pstHex, ulHexLen);
        if (iRet < 0)
        {
            return PDU_ERROR;
        }
        STRING_PUSH(*ppstStr, *pulStrLen, ulBit8StrLen);
        *pulHexOff += ulTranslateLen;
        *pulLenOff += ulTranslateLen;
        return PDU_OK;
    }

    return PDU_OK;
}

INT32 usc2_to_utf8(char **ppstStr, UINT32 *pulStrLen, INT32 ulLength, UINT8 *pstUtf8, UINT32 ulUtf8Len, UINT32 *pulUtf8Off, UINT32 *pulLenOff)
{
    UINT16 ulUsc2;
    UINT32 ulUsc2StrLen;
    UINT32 ulTranslateLen;
    UINT8 ucMask;
    int iRet;

    ulUsc2StrLen = sizeof(ulUsc2) * 2;
    STRING_PUSH(pstUtf8, ulUtf8Len, *pulUtf8Off);
    ulLength -= *pulLenOff;
    do
    {
        if (*pulStrLen < ulUsc2StrLen)
        {
            return PDU_READMORE;
        }

        iRet = string_to_hex(*ppstStr, ulUsc2StrLen, (UINT8 *)&ulUsc2, sizeof(ulUsc2));
        if (iRet < 0)
        {
            return PDU_ERROR;
        }
        STRING_PUSH(*ppstStr, *pulStrLen, ulUsc2StrLen);

        ulUsc2 = ntohs(ulUsc2); /* mips下这个宏是空的，不影响 */
        ulTranslateLen = enc_unicode_to_utf8_one(ulUsc2, pstUtf8, ulUtf8Len);
        STRING_PUSH(pstUtf8, ulUtf8Len, ulTranslateLen);
        ulLength -= sizeof(ulUsc2);
        *pulUtf8Off += ulTranslateLen;
        *pulLenOff += sizeof(ulUsc2);
    }
    while (ulLength > 0);

    return PDU_OK;
}

INT32 pdu_addr(char **ppstStr, UINT32 *pulStrLen, ADDR_INFO *pstAddr, UINT32 ulAddrInfoType)
{
    int iRet;

    /*ADDR 长度*/
    if (*pulStrLen < ADDR_LENGTH_LEN)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    iRet = string_to_hex(*ppstStr, ADDR_LENGTH_LEN, &pstAddr->ucLength, sizeof(pstAddr->ucLength));
    if (iRet != PDU_E_NONE)
    {
        return iRet;
    }

    if (ulAddrInfoType == ADDR_INFO_TYPE_SERVICE)
    {
        /*短信中心的长度字段按字节表示,包含类型字段2个字符，字符串中两个字符表示1个字节*/
        pstAddr->ucLength *= 2;
    }
    else
    {
        /*用户的长度字段按字符表示，不包含类型字段2个字符，需要加上，长度为奇数时，PDU中用F补齐，长度加1*/
        if ((pstAddr->ucLength % 2) != 0)
        {
            pstAddr->ucLength += 1;
        }
        pstAddr->ucLength += 2;
    }

    STRING_PUSH(*ppstStr, *pulStrLen, ADDR_LENGTH_LEN);

    /*SCA 号码类型*/
    if (*pulStrLen < ADDR_TYPE_LEN)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    iRet = string_to_hex(*ppstStr, ADDR_TYPE_LEN, &pstAddr->ucType, sizeof(pstAddr->ucType));
    if (iRet != PDU_E_NONE)
    {
        return iRet;
    }
    STRING_PUSH(*ppstStr, *pulStrLen, ADDR_TYPE_LEN);

    if (*pulStrLen < (pstAddr->ucLength - ADDR_TYPE_LEN))
    {
        return PDU_E_DEFAULT_FAIL;
    }

    if (((pstAddr->ucType & ADDR_IDENTY_MASK) == ADDR_IDENTY_ISDN) &&
        ((pstAddr->ucType & ADDR_TYPE_MASK) == ADDR_TYPE_INTERNATIONAL))
    {
        pstAddr->aucAddr[0] = '+';
        iRet = pdu_to_telephone_number(*ppstStr, (pstAddr->ucLength - ADDR_TYPE_LEN),
            (pstAddr->aucAddr + 1), (sizeof(pstAddr->aucAddr) - 1));
        if (iRet != PDU_E_NONE)
        {
            return iRet;
        }
    }
    else
    {
        iRet = pdu_to_telephone_number(*ppstStr, (pstAddr->ucLength - ADDR_TYPE_LEN),
            pstAddr->aucAddr, sizeof(pstAddr->aucAddr));
        if (iRet != PDU_E_NONE)
        {
            return iRet;
        }
    }

    STRING_PUSH(*ppstStr, *pulStrLen, (pstAddr->ucLength - ADDR_TYPE_LEN));

    return PDU_E_NONE;
}

INT32 pdu_fo(char **ppstStr, UINT32 *pulStrLen, UINT8 *pucFO)
{
    int iRet;

    if (*pulStrLen < FO_LENGTH)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    iRet = string_to_hex(*ppstStr, FO_LENGTH, pucFO, sizeof(*pucFO));
    if (iRet != PDU_E_NONE)
    {
        return iRet;
    }

    STRING_PUSH(*ppstStr, *pulStrLen, FO_LENGTH);

    return PDU_E_NONE;
}

INT32 pdu_pid(char **ppstStr, UINT32 *pulStrLen, UINT8 *pucPID)
{
    int iRet;

    if (*pulStrLen < PID_LENGTH)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    iRet = string_to_hex(*ppstStr, PID_LENGTH, pucPID, sizeof(*pucPID));
    if (iRet != PDU_E_NONE)
    {
        return iRet;
    }

    STRING_PUSH(*ppstStr, *pulStrLen, PID_LENGTH);

    return PDU_E_NONE;
}

INT32 pdu_dsc(char **ppstStr, UINT32 *pulStrLen, UINT8 *pucDSC)
{
    int iRet;

    if (*pulStrLen < DSC_LENGTH)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    iRet = string_to_hex(*ppstStr, DSC_LENGTH, pucDSC, sizeof(*pucDSC));
    if (iRet != PDU_E_NONE)
    {
        return iRet;
    }

    STRING_PUSH(*ppstStr, *pulStrLen, DSC_LENGTH);

    return PDU_E_NONE;
}

INT32 pdu_scts(char **ppstStr, UINT32 *pulStrLen, TIME_INFO *pstSCTS)
{
    int iRet;
    char aucZone[4] = {0};

    /*日期*/
    if (*pulStrLen < TIME_PDU_LEN)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    iRet = pdu_to_time(*ppstStr, pstSCTS->aucDate, TIME_STRING_LEN, '/');
    if (iRet != PDU_E_NONE)
    {
        return iRet;
    }

    STRING_PUSH(*ppstStr, *pulStrLen, TIME_PDU_LEN);

    /*时间*/
    if (*pulStrLen < TIME_PDU_LEN)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    iRet = pdu_to_time(*ppstStr, pstSCTS->aucTime, TIME_STRING_LEN, ':');
    if (iRet != PDU_E_NONE)
    {
        return iRet;
    }

    STRING_PUSH(*ppstStr, *pulStrLen, TIME_PDU_LEN);

    /*时区*/
    if (*pulStrLen < TIME_ZONE_LEN)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    memcpy(aucZone, *ppstStr, TIME_ZONE_LEN);
    pstSCTS->ucZone = atoi(aucZone);

    STRING_PUSH(*ppstStr, *pulStrLen, TIME_ZONE_LEN);

    return PDU_E_NONE;
}

INT32 pdu_ud_len(char **ppstStr, UINT32 *pulStrLen, UINT8 *pucUDLen)
{
    int iRet;

    if (*pulStrLen < UDLEN_LENGTH)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    iRet = string_to_hex(*ppstStr, UDLEN_LENGTH, pucUDLen, sizeof(*pucUDLen));
    if (iRet != PDU_E_NONE)
    {
        return iRet;
    }

    STRING_PUSH(*ppstStr, *pulStrLen, UDLEN_LENGTH);

    return PDU_E_NONE;
}

INT32 pdu_udh(char **ppstStr, UINT32 *pulStrLen, UDH_INFO *pucUDH)
{
    int iRet;

    /*UDHL*/
    if (*pulStrLen < UDHL_LENGTH)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    iRet = string_to_hex(*ppstStr, UDHL_LENGTH, &pucUDH->ucLength, sizeof(pucUDH->ucLength));
    if (iRet != PDU_E_NONE)
    {
        return iRet;
    }

    STRING_PUSH(*ppstStr, *pulStrLen, UDHL_LENGTH);

    /*UDHI*/
    if (*pulStrLen < pucUDH->ucLength * 2)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    iRet = string_to_hex(*ppstStr, UDHI_LENGTH, &pucUDH->ucIdentify, sizeof(pucUDH->ucIdentify));
    if (iRet != PDU_E_NONE)
    {
        return iRet;
    }

    STRING_PUSH(*ppstStr, *pulStrLen, UDHI_LENGTH);

    if (pucUDH->ucIdentify != UDH_LDENTIFY_LONG)
    {
        STRING_PUSH(*ppstStr, *pulStrLen, (pucUDH->ucLength * 2 - UDHI_LENGTH));
        return PDU_E_NONE;
    }

    /*UDHSH*/
    iRet = string_to_hex(*ppstStr, UDHSH_LENGTH, &pucUDH->ucSubHeadLen, sizeof(pucUDH->ucSubHeadLen));
    if (iRet != PDU_E_NONE)
    {
        return iRet;
    }

    STRING_PUSH(*ppstStr, *pulStrLen, UDHSH_LENGTH);

    /*UDHSHID*/
    iRet = string_to_hex(*ppstStr, UDHSHID_LENGTH, &pucUDH->ucId, sizeof(pucUDH->ucId));
    if (iRet != PDU_E_NONE)
    {
        return iRet;
    }

    STRING_PUSH(*ppstStr, *pulStrLen, UDHSHID_LENGTH);

    /*UDHLSHMAX*/
    iRet = string_to_hex(*ppstStr, UDHSHMAX_LENGTH, &pucUDH->ucMax, sizeof(pucUDH->ucMax));
    if (iRet != PDU_E_NONE)
    {
        return iRet;
    }

    STRING_PUSH(*ppstStr, *pulStrLen, UDHSHMAX_LENGTH);

    /*UDHLSHNUM*/
    iRet = string_to_hex(*ppstStr, UDHSHNUM_LENGTH, &pucUDH->ucNum, sizeof(pucUDH->ucNum));
    if (iRet != PDU_E_NONE)
    {
        return iRet;
    }

    STRING_PUSH(*ppstStr, *pulStrLen, UDHSHNUM_LENGTH);
    
    return PDU_E_NONE;
}

INT32 pdu_analyse(char **ppstStr, UINT32 *pulStrLen, UINT32 ulEnd, SMS_INFO *pstSms)
{
    int iRet;
    UINT32 ulBegin = 0;
        
    /*解析PUD头部*/
    if (pstSms->ulState < PDU_STATUS_DATE)
    {
        iRet = pdu_addr(ppstStr, pulStrLen, &pstSms->stSCA, ADDR_INFO_TYPE_SERVICE);
        if (iRet != PDU_E_NONE)
        {
            return PDU_ERROR;
        }

        iRet = pdu_fo(ppstStr, pulStrLen, &pstSms->ucFO);
        if (iRet != PDU_E_NONE)
        {
            return PDU_ERROR;
        }

        iRet = pdu_addr(ppstStr, pulStrLen, &pstSms->stOA, ADDR_INFO_TYPE_USER);
        if (iRet != PDU_E_NONE)
        {
            return PDU_ERROR;
        }

        iRet = pdu_pid(ppstStr, pulStrLen, &pstSms->ucPID);
        if (iRet != PDU_E_NONE)
        {
            return PDU_ERROR;
        }

        iRet = pdu_dsc(ppstStr, pulStrLen, &pstSms->ucDSC);
        if (iRet != PDU_E_NONE)
        {
            return PDU_ERROR;
        }

        iRet = pdu_scts(ppstStr, pulStrLen, &pstSms->stSCTS);
        if (iRet != PDU_E_NONE)
        {
            return PDU_ERROR;
        }

        iRet = pdu_ud_len(ppstStr, pulStrLen, &pstSms->ucUDLen);
        if (iRet != PDU_E_NONE)
        {
            return PDU_ERROR;
        }
        if ((pstSms->ucFO & FO_UDHI) == FO_UDHI)
        {
            iRet = pdu_udh(ppstStr, pulStrLen, &pstSms->stUDh);
            if (iRet != PDU_E_NONE)
            {
                return PDU_ERROR;
            }

            if ((pstSms->ucDSC & DSC_TYPE_MASK) == DSC_TYPE_7BIT) /*7bit解码，以septet为单位,UDLen表示的字符个数，不是字节数*/
            {
                if( (pstSms->stUDh.ucLength + 1)*ASCII_BIT % BIT7_BIT != 0)
                {
                    pstSms->ucUDLen -= ( (pstSms->stUDh.ucLength + 1)*ASCII_BIT / BIT7_BIT + 1 );
                }
                else
                {
                    pstSms->ucUDLen -= (pstSms->stUDh.ucLength + 1)*ASCII_BIT / BIT7_BIT;
                }
            }
            else
            {
                pstSms->ucUDLen -= pstSms->stUDh.ucLength + 1;
            }
            //printf("jiema UD %d:  pstSms->ucUDLen:%x\n", __LINE__, pstSms->ucUDLen);
        }

        pstSms->ulState = PDU_STATUS_DATE;
    }
    
    /*解析PDU用户数据*/
    if ((pstSms->ucDSC & DSC_TYPE_MASK) == DSC_TYPE_7BIT)
    {
        if(pstSms->ulLenOffset == 0 &&  pstSms->stUDh.ucLength  != 0)
            ulBegin = (pstSms->stUDh.ucLength + 1) % BIT7_BUFFER_LEN;
        else
            ulBegin = 0;
        iRet = bit7_to_ascii(ppstStr, pulStrLen, ulBegin, pstSms->ucUDLen, pstSms->aucUD,
            sizeof(pstSms->aucUD), &pstSms->ulUDOffset, &pstSms->ulLenOffset);
        //iRet = pdu_bit7_indiv_char_deal(pstSms->aucUD, pstSms->ulUDOffset);
       
    }
    else if ((pstSms->ucDSC & DSC_TYPE_MASK) == DSC_TYPE_8BIT)
    {
        iRet = bit8_to_hex(ppstStr, pulStrLen, pstSms->ucUDLen, pstSms->aucUD,
            sizeof(pstSms->aucUD), &pstSms->ulUDOffset, &pstSms->ulLenOffset);
    }
    else if ((pstSms->ucDSC & DSC_TYPE_MASK) == DSC_TYPE_USC2)
    {
        iRet = usc2_to_utf8(ppstStr, pulStrLen, pstSms->ucUDLen, pstSms->aucUD,
            sizeof(pstSms->aucUD), &pstSms->ulUDOffset, &pstSms->ulLenOffset);
    }
    else
    {
        return PDU_ERROR;
    }
    //printf("------%d:  %s\n", __LINE__, pstSms->aucUD);
    if (ulEnd == 0)
    {
        iRet = PDU_READMORE;
    }
    else if (ulEnd == 1 && iRet == PDU_READMORE)
    {
        LTE_LOG(LTE_LOG_ERR, "%d,%s:warning pdu UD len > string len\n", __LINE__, __func__);
        // printf("warning pdu UD len > string len\n");
        iRet = PDU_OK;
    }

    if (iRet == PDU_OK)
    {
       
        LTE_LOG(LTE_LOG_INFO, "Telephone %s DataTime %s %s Index %d\n", 
            pstSms->stOA.aucAddr, pstSms->stSCTS.aucDate, pstSms->stSCTS.aucTime, pstSms->ulIndex);
        LTE_LOG(LTE_LOG_INFO, "%s\n\n", pstSms->aucUD);
        /*
        printf("Telephone %s DataTime %s %s Index %d\n", 
            pstSms->stOA.aucAddr, pstSms->stSCTS.aucDate, pstSms->stSCTS.aucTime, pstSms->ulIndex);
        printf("%s\n\n", pstSms->aucUD);
        */
        pstSms->ulState = PDU_STATUS_DONE;
    }

    return iRet;
}

INT32 sms_recv(char * pucFile, SMS_INFO *pstSms, UINT32 ulSmsMax)
{
    CHAR aucBuf[BUFFER_MAX_LEN + 1] = {0};
    FILE *pFile;
    char *pstCmgl, *pstPdu, *pstEnd;
    UINT32 ulBufLen, ulPduLen, ulEnd = 0;
    char acCiev[16] = {0};
    char acBufTmp[16] = {0};
    char acSmsIndex[16] = {0};
    UINT32 ulSmsCount;
    int iRet;

    if (ulSmsMax < 2)
    {
        strncpy(acCiev, "+CMGR: ", sizeof(acCiev));
    }
    else
    {
        strncpy(acCiev, "+CMGL: ", sizeof(acCiev));
    }
    pFile = fopen(pucFile, "r");
    if (NULL == pFile)
    {
        //LTE_LOG(LTE_LOG_ERR, "%d,%s:open %s failed\n", __LINE__, __func__, pucFile);
        return PDU_E_DEFAULT_FAIL;
    }

    ulSmsCount = 0;

    while (!feof(pFile) || ulSmsCount < SMS_MAX)
    {
        if (NULL == fgets(aucBuf, sizeof(aucBuf), pFile))
        {
            break;
        }

        pstCmgl = strstr(aucBuf, acCiev);
        if (pstCmgl != NULL)
        {
            sscanf(pstCmgl, "%[^:]:%[^,]", acBufTmp, acSmsIndex);
            pstSms[ulSmsCount].ulIndex = atoi(acSmsIndex) + 1; /* 添加短信序号 */
            pstPdu = aucBuf;
            ulBufLen = sizeof(aucBuf);
            pstSms[ulSmsCount].ulState = PDU_STATUS_INIT;
            iRet = PDU_OK;
            while (1)
            {
                pstPdu = fgets(pstPdu, ulBufLen, pFile); /*读第二行 短信内容*/
                if (NULL == pstPdu)
                {
                    iRet = PDU_ERROR;
                    break;
                }

                ulPduLen = strlen(aucBuf);
                pstPdu = aucBuf;
                if ((pstEnd = strstr(aucBuf, "\r"))
                    || (pstEnd = strstr(aucBuf, "\n")))
                {
                    ulEnd = 1;
                    *pstEnd = 0;
                    ulPduLen = strlen(aucBuf);
                }
                else
                {
                    ulEnd = 0;
                }

                iRet = pdu_analyse(&pstPdu, &ulPduLen, ulEnd, &(pstSms[ulSmsCount]));
                if (iRet != PDU_READMORE)
                {
                    break;
                }

                if (ulPduLen == 0)
                {
                    pstPdu = aucBuf;
                    ulBufLen = sizeof(aucBuf);
                }
                else
                {
                    memmove(aucBuf, pstPdu, ulPduLen);
                    pstPdu = aucBuf + ulPduLen;
                    ulBufLen = sizeof(aucBuf) - ulPduLen;
                }
            }

            if (iRet < 0)
            {
                pstSms[ulSmsCount].ulState = PDU_STATUS_ERROR;
            }
            ulSmsCount++;

            if (ulSmsCount >= ulSmsMax)
            {
                break;
            }
        }
    }

    fclose(pFile);
    return PDU_E_NONE;
}

/**************pdu发短信相关函数*************************************/
INT32 pdu_convert_tele_num(char *pstDst, UINT32 ulDstLen, char *pstSrc, UINT32 ulSrcLen)
{
    int iLoop;
    char acTmp[ADDR_MAX_LEN] = {0};
    if (ulDstLen < ulSrcLen)
    {
        return PDU_E_DEFAULT_FAIL;
    }
    strncpy(acTmp, pstSrc, sizeof(acTmp));
    /*pdu表示的电话号码必须是偶数位，如果是基数位号码，会以F补为偶数位*/
    if ((ulSrcLen % 2) != 0)
    {
        strcat(acTmp, "F");
        ulSrcLen++;
        //*ulTelLen++;
    }

    for (iLoop = 0; iLoop < ulSrcLen; iLoop += 2)
    {
        pstDst[iLoop] = acTmp[iLoop + 1];
        pstDst[iLoop + 1] = acTmp[iLoop];
    }
    return PDU_E_NONE;
}

UINT8 pdu_ud_encode_type(char *pucUd, UINT32 ulUdLen)
{
    int iLoop;

    for (iLoop = 0; iLoop < ulUdLen; iLoop++)
    {
        if ((*(pucUd + iLoop) & 0x80) == 0x80)
        {
            return DSC_TYPE_USC2;
        }
    }

    return DSC_TYPE_7BIT;
}

INT32 pdu_encode_da(SMS_SEND_INFO *pstSms, char **ppucPdu, UINT32 *pulPduLen)
{
    UINT32 ulScaLen, iAddrLen;
    CHAR acTmp[32] = {0};
    INT32 iRet;

    iAddrLen = strlen(pstSms->stDA.aucAddr);
    ulScaLen = iAddrLen + ADDR_LENGTH_LEN + ADDR_TYPE_LEN;

    if (*pulPduLen < ulScaLen)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    if (pstSms->stDA.aucAddr[0] == '+')
    {
        pstSms->stDA.ucType = ADDR_TYPE_INTERNATIONAL | ADDR_IDENTY_ISDN | 0x80;
        iRet = pdu_convert_tele_num(acTmp, sizeof(acTmp), pstSms->stDA.aucAddr + 1, iAddrLen - 1);
        pstSms->stDA.ucLength = iAddrLen - 1;
    }
    else
    {
        pstSms->stDA.ucType = ADDR_TYPE_INTERNAL | ADDR_IDENTY_ISDN | 0x80;
        iRet = pdu_convert_tele_num(acTmp, sizeof(acTmp), pstSms->stDA.aucAddr, iAddrLen);
        pstSms->stDA.ucLength = iAddrLen;
    }

    if (iRet != PDU_E_NONE)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    /*用户的长度字段按字符表示，不包含类型字段2个字符，需要加上，长度为奇数时，PDU中用F补齐，长度加1*/
    //pstSms->stDA.ucLength = iAddrLen;
    // printf("%d--iAddrLen = %d\n", __LINE__, iAddrLen);
    iRet = snprintf(*ppucPdu, *pulPduLen, "%02x%02x%s", pstSms->stDA.ucLength, pstSms->stDA.ucType, acTmp);
    if (iRet <= 0)
    {
        return PDU_E_DEFAULT_FAIL;
    }
    STRING_PUSH(*ppucPdu, *pulPduLen, strlen(acTmp) + ADDR_LENGTH_LEN + ADDR_TYPE_LEN);

    return PDU_E_NONE;
};

INT32 pdu_encode_fo(SMS_SEND_INFO *pstSms, char **ppucPdu, UINT32 *pulPduLen)
{
    UINT8 ucDSC;
    UINT32 ulUdMax;
    INT32 iRet;

    if (*pulPduLen < sizeof(pstSms->ucFO) * 2)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    pstSms->ucFO |= FO_MTI_SEND;
    pstSms->ucFO |= FO_VPF_8BIT;

    if (pstSms->ucCount != 1 || (pstSms->pstSmsSendHead != NULL && pstSms->pstSmsSendHead->eSmsHead == SMS_ADD_HEAD))
    {
        pstSms->ucFO |= FO_UDHI;
    }

    iRet = snprintf(*ppucPdu, *pulPduLen, "%02x", pstSms->ucFO);
    if (iRet <= 0)
    {
        return PDU_E_DEFAULT_FAIL;
    }
    STRING_PUSH(*ppucPdu, *pulPduLen, sizeof(pstSms->ucFO) * 2);

    return PDU_E_NONE;
};

INT32 pdu_encode_mr(SMS_SEND_INFO *pstSms, char **ppucPdu, UINT32 *pulPduLen)
{
    UINT8 ucMR = 0;
    INT32 iRet;

    if (*pulPduLen < sizeof(ucMR) * 2)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    iRet = snprintf(*ppucPdu, *pulPduLen, "%02x", ucMR);
    if (iRet <= 0)
    {
        return PDU_E_DEFAULT_FAIL;
    }
    STRING_PUSH(*ppucPdu, *pulPduLen, sizeof(ucMR) * 2);

    return PDU_E_NONE;
};

INT32 pdu_encode_pid(SMS_SEND_INFO *pstSms, char **ppucPdu, UINT32 *pulPduLen)
{
    UINT8 ucPID = 0;
    INT32 iRet;

    if (*pulPduLen < sizeof(ucPID) * 2)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    iRet = snprintf(*ppucPdu, *pulPduLen, "%02x", ucPID);
    if (iRet <= 0)
    {
        return PDU_E_DEFAULT_FAIL;
    }
    STRING_PUSH(*ppucPdu, *pulPduLen, sizeof(ucPID) * 2);

    return PDU_E_NONE;
};

INT32 pdu_encode_dcs(SMS_SEND_INFO *pstSms, char **ppucPdu, UINT32 *pulPduLen)
{
    INT32 iRet;

    //printf("%d--dsc = %d\n",__LINE__, ucDSC);
    if (*pulPduLen < sizeof(pstSms->ucDSC) * 2)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    iRet = snprintf(*ppucPdu, *pulPduLen, "%02x", pstSms->ucDSC);
    if (iRet <= 0)
    {
        return PDU_E_DEFAULT_FAIL;
    }

    STRING_PUSH(*ppucPdu, *pulPduLen, sizeof(pstSms->ucDSC) * 2);
    return PDU_E_NONE;
}

INT32 pdu_encode_vp(SMS_SEND_INFO *pstSms, char **ppucPdu, UINT32 *pulPduLen)
{
    UINT8 ucVp = 0;
    INT32 iRet;

    if ( ((pstSms->ucFO & FO_VPF) == FO_VPF_NONE) || ( (pstSms->ucFO & FO_VPF_RSV) == FO_VPF_RSV) )
    {
        return PDU_E_NONE;
    }
    else if ((pstSms->ucFO & FO_VPF) == FO_VPF_8BIT)
    {
        ucVp = 0xC4;        /*30天*/
    }
    else if ((pstSms->ucFO & FO_VPF) == FO_VPF_4BIT)
    {
        return PDU_E_DEFAULT_FAIL;
    }
    else
    {
        return PDU_E_DEFAULT_FAIL;
    }

    if (*pulPduLen < sizeof(ucVp) * 2)
    {
        return PDU_E_DEFAULT_FAIL;
    }
    iRet = snprintf(*ppucPdu, *pulPduLen, "%02x", ucVp);
    if (iRet <= 0)
    {
        return PDU_E_DEFAULT_FAIL;
    }
    STRING_PUSH(*ppucPdu, *pulPduLen, sizeof(ucVp) * 2);

    return PDU_E_NONE;
};

INT32 pdu_get_utf8_size(UINT8 utf)
{
    UINT8 iLoop;

    for (iLoop = 0; iLoop < sizeof(utf) * 8; iLoop++)
    {
        if (((0x80 >> iLoop) & utf) == 0)
        {
            break;
        }
    }
    if (iLoop == 0)
    {
        iLoop = 1;
    }
    return iLoop;
}

UINT32 pdu_utf8_to_unicode_one(UINT8 * pInput, INT32 ulInLen, UINT8 *pOutput, INT32 ulOutLen, UINT32 *pOutOff)
{
    // b1 表示UTF-8编码的pInput中的高字节, b2 表示次高字节, ...
    char b1, b2, b3, b4, b5, b6;
    int utfbytes;

    *pOutput = 0x0; // 把 *Unic 初始化为全零
    *pOutOff = 0;
    utfbytes = pdu_get_utf8_size(*pInput);

    if (ulInLen < utfbytes)
    {
        return 0;
    }

    switch (utfbytes)
    {
        case 1:
            if (ulOutLen < 1)
            {
                return 0;
            }
            *(pOutput + 1) = *pInput;
            *pOutput = 0;
            *pOutOff = 2;
            break;
        case 2:
            if (ulOutLen < 2)
            {
                return 0;
            }
            b1 = *pInput;
            b2 = *(pInput + 1);
            if ( (b2 & 0xE0) != 0x80)
            {
                return 0;
            }
            //*pOutput = (b1 << 6) + (b2 & 0x3F);
            //*(pOutput + 1) = (b1 >> 2) & 0x07;
            /*修改字节序，只修改了1，2 ，3字节的，其他暂时用不到*/
            *(pOutput + 1) = (b1 << 6) + (b2 & 0x3F);
            *pOutput = (b1 >> 2) & 0x07;
            *pOutOff = 2;
            break;
        case 3:
            if (ulOutLen < 2)
            {
                return 0;
            }
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) )
            {
                return 0;
            }
            //*pOutput = (b2 << 6) + (b3 & 0x3F);
            //*(pOutput + 1) = (b1 << 4) + ((b2 >> 2) & 0x0F);
            *pOutput = (b1 << 4) + ((b2 >> 2) & 0x0F);
            *(pOutput + 1) = (b2 << 6) + (b3 & 0x3F);
            *pOutOff = 2;
            break;
        case 4:
            if (ulOutLen < 3)
            {
                return 0;
            }

            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                 || ((b4 & 0xC0) != 0x80) )
            {
                return 0;
            }
            *pOutput = (b3 << 6) + (b4 & 0x3F);
            *(pOutput + 1) = (b2 << 4) + ((b3 >> 2) & 0x0F);
            *(pOutput + 2) = ((b1 << 2) & 0x1C) + ((b2 >> 4) & 0x03);
            *pOutOff = 3;
            break;
        case 5:
            if (ulOutLen < 4)
            {
                return 0;
            }
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            b5 = *(pInput + 4);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                 || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80) )
            {
                return 0;
            }
            *pOutput = (b4 << 6) + (b5 & 0x3F);
            *(pOutput + 1) = (b3 << 4) + ((b4 >> 2) & 0x0F);
            *(pOutput + 2) = (b2 << 2) + ((b3 >> 4) & 0x03);
            *(pOutput + 3) = (b1 << 6);
            *pOutOff = 4;
            break;
        case 6:
            if (ulOutLen < 4)
            {
                return 0;
            }

            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            b5 = *(pInput + 4);
            b6 = *(pInput + 5);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                 || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80)
                 || ((b6 & 0xC0) != 0x80) )
            {
                return 0;
            }
            *pOutput = (b5 << 6) + (b6 & 0x3F);
            *(pOutput + 1) = (b5 << 4) + ((b6 >> 2) & 0x0F);
            *(pOutput + 2) = (b3 << 2) + ((b4 >> 4) & 0x03);
            *(pOutput + 3) = ((b1 << 6) & 0x40) + (b2 & 0x3F);
            *pOutOff = 4;
            break;
        default:
            return 0;
            break;
    }

    return utfbytes;
}

INT32 pdu_utf8_to_bit7(UINT8 * pInput, INT32 ulInLen, UINT8 *pOutput, INT32 ulOutLen, UINT32 *pOutOff)
{
    UINT8 ucHMask = 0x7F, ucLMask = 0;
    int iLoop, iLoopMax;

    if (ulOutLen < BIT7_BUFFER_LEN)
    {
        return 0;
    }

    *pOutOff = 0;
    iLoopMax = ulInLen < TRANSLATE_LEN ?  ulInLen : TRANSLATE_LEN;
    for (iLoop = 0; iLoop < iLoopMax; iLoop++)
    {
        //printf("=========%d ucHMask %x  ucLMask %x  In %x\n", __LINE__, ucHMask, ucLMask, *(pInput + iLoop));
        if (ucHMask != 0)
        {
            *(pOutput + iLoop) = (*(pInput + iLoop) & ucHMask) >> iLoop;
            //printf("%d %d pOutput %x\n", __LINE__, iLoop, *(pOutput + iLoop));
            (*pOutOff)++;
        }

        if (ucLMask != 0)
        {
            // printf("%d %d pOutput %x %d\n", __LINE__, iLoop,  (*(pInput + iLoop) &  ucLMask) , (ASCII_BIT - iLoop));
            *(pOutput + iLoop - 1) |= (*(pInput + iLoop) & ucLMask) << (ASCII_BIT - iLoop);
            //printf("%d %d pOutput %x\n", __LINE__, iLoop,  *(pOutput + iLoop - 1) );
        }

        //printf("%d in %d  out %d\n", __LINE__, iLoop, *pOutOff);
        ucHMask &= ~(0x01 << iLoop);
        ucLMask |= (0x01 << iLoop);
    }

    return iLoop;
}

INT32 pdu_encode_ud(SMS_SEND_INFO *pstSms, UINT8 ucCount, char **ppucPdu, UINT32 *pulPduLen)
{
    UINT8 *pucUd;
    INT32 iLoop, iRet;
    UINT8 ucLongId;
    UINT8 ucLongHead[] = "050003"; /*长短信头信息:05 后面有5个字节,00拆分短信，03后面有3个字节*/

    if ((pstSms->ucFO & FO_UDHI) == 0 || (pstSms->pstSmsSendHead != NULL && pstSms->pstSmsSendHead->eSmsHead == SMS_ADD_HEAD))//没有分段
    {
        if (*pulPduLen < sizeof(pstSms->stUD[ucCount].ucUDLen) * 2)
        {
            return PDU_E_DEFAULT_FAIL;
        }

        iRet = snprintf(*ppucPdu, *pulPduLen, "%02x", pstSms->stUD[ucCount].ucUDLen);
        if (iRet <= 0)
        {
            return PDU_E_DEFAULT_FAIL;
        }
        STRING_PUSH(*ppucPdu, *pulPduLen, sizeof(pstSms->stUD[ucCount].ucUDLen) * 2);
    }
    else
    {
        if (*pulPduLen < sizeof(pstSms->stUD[ucCount].ucUDLen) * 2)
        {
            return PDU_E_DEFAULT_FAIL;
        }
        // printf("%d: = %d\n",__LINE__, pstSms->stUD[ucCount].ucUDLen +6);
        if (pstSms->ucDSC == DSC_TYPE_USC2)
        {
            iRet = snprintf(*ppucPdu, *pulPduLen, "%02x", pstSms->stUD[ucCount].ucUDLen + 6);
        }
        else if (pstSms->ucDSC == DSC_TYPE_7BIT)   /*UD头6个字节，加上填充的一位，总共7个septet，再加上左移的UD首字节共8个septet*/
        {
            iRet = snprintf(*ppucPdu, *pulPduLen, "%02x", pstSms->stUD[ucCount].ucUDLen + 8);
        }
        else
        {
            return PDU_E_DEFAULT_FAIL;
        }

        if (iRet <= 0)
        {
            return PDU_E_DEFAULT_FAIL;
        }
        STRING_PUSH(*ppucPdu, *pulPduLen, sizeof(pstSms->stUD[ucCount].ucUDLen) * 2);

        iRet = snprintf(*ppucPdu, *pulPduLen, "%s", ucLongHead, strlen(ucLongHead));
        if (iRet <= 0)
        {
            return PDU_E_DEFAULT_FAIL;
        }
        STRING_PUSH(*ppucPdu, *pulPduLen, strlen(ucLongHead));

        iRet = snprintf(*ppucPdu, *pulPduLen, "%02x", pstSms->ucSmsID);
        if (iRet <= 0)
        {
            return PDU_E_DEFAULT_FAIL;
        }
        STRING_PUSH(*ppucPdu, *pulPduLen, sizeof(pstSms->ucSmsID) * 2);

        iRet = snprintf(*ppucPdu, *pulPduLen, "%02x", pstSms->ucCount);
        if (iRet <= 0)
        {
            return PDU_E_DEFAULT_FAIL;
        }
        STRING_PUSH(*ppucPdu, *pulPduLen, sizeof(pstSms->ucCount) * 2);

        iRet = snprintf(*ppucPdu, *pulPduLen, "%02x", ucCount + 1);
        if (iRet <= 0)
        {
            return PDU_E_DEFAULT_FAIL;
        }
        STRING_PUSH(*ppucPdu, *pulPduLen, sizeof(ucCount) * 2);

    }

    pucUd = pstSms->stUD[ucCount].aucUD;
    if (*pulPduLen < strlen(pucUd) * 2)
    {
        return PDU_E_DEFAULT_FAIL;
    }
    //printf("%d: = %d\n",__LINE__, pstSms->stUD[ucCount].ucUDLen);
    if (pstSms->ucDSC == DSC_TYPE_USC2)
    {
        for (iLoop = 0; iLoop < pstSms->stUD[ucCount].ucUDLen; iLoop++)
        {
            iRet = snprintf(*ppucPdu, *pulPduLen, "%02x", *(pucUd + iLoop));
            if (iRet <= 0)
            {
                return PDU_E_DEFAULT_FAIL;
            }
            STRING_PUSH(*ppucPdu, *pulPduLen, sizeof(UINT8) * 2);
        }
    }
    else if (pstSms->ucDSC == DSC_TYPE_7BIT)
    {
        for (iLoop = 0; iLoop < strlen(pucUd); iLoop++)
        {
            iRet = snprintf(*ppucPdu, *pulPduLen, "%02x", *(pucUd + iLoop));
            if (iRet <= 0)
            {
                return PDU_E_DEFAULT_FAIL;
            }
            STRING_PUSH(*ppucPdu, *pulPduLen, sizeof(UINT8) * 2);
        }
    }
    else if (pstSms->ucDSC == DSC_TYPE_8BIT)
    {
        if(pstSms->pstSmsSendHead != NULL && SMS_ADD_HEAD == pstSms->pstSmsSendHead->eSmsHead)
        {
            strcpy(*ppucPdu, pstSms->pstSmsSendHead->acSmsHead);
            //*ppucPdu += strlen(pstSms->pstSmsSendHead->acSmsHead);
            STRING_PUSH(*ppucPdu, *pulPduLen, sizeof(UINT8) * strlen(pstSms->pstSmsSendHead->acSmsHead));
        }
        for (iLoop = 0; iLoop < strlen(pucUd); iLoop++)
        {
            iRet = snprintf(*ppucPdu, *pulPduLen, "%02x", *(pucUd + iLoop));
            if (iRet <= 0)
            {
                return PDU_E_DEFAULT_FAIL;
            }
            STRING_PUSH(*ppucPdu, *pulPduLen, sizeof(UINT8) * 2);
        }
    }
    else
    {
        return PDU_E_DEFAULT_FAIL;
    }

    return PDU_E_NONE;
}

INT32 pdu_encode(SMS_SEND_INFO *pstSms, UINT8 ucCount, char *pucPdu, UINT32 ulPduLen)
{
    int iRet;

    pucPdu[0] = '0';
    pucPdu[1] = '0';
    STRING_PUSH(pucPdu, ulPduLen, sizeof(char) * 2 );

    iRet = pdu_encode_fo(pstSms, &pucPdu, &ulPduLen);
    if (iRet != PDU_E_NONE)
    {
        return PDU_ERROR;
    }

    //printf("%d--dsc = %d\n",__LINE__, ucDSC);
    iRet = pdu_encode_mr(pstSms, &pucPdu, &ulPduLen);
    if (iRet != PDU_E_NONE)
    {
        return PDU_ERROR;
    }
    iRet = pdu_encode_da(pstSms, &pucPdu, &ulPduLen);
    if (iRet != PDU_E_NONE)
    {
        return PDU_ERROR;
    }

    iRet = pdu_encode_pid(pstSms, &pucPdu, &ulPduLen);
    if (iRet != PDU_E_NONE)
    {
        return PDU_ERROR;
    }

    iRet = pdu_encode_dcs(pstSms, &pucPdu, &ulPduLen);
    if (iRet != PDU_E_NONE)
    {
        return PDU_ERROR;
    }
    iRet = pdu_encode_vp(pstSms, &pucPdu, &ulPduLen);
    if (iRet != PDU_E_NONE)
    {
        return PDU_ERROR;
    }
    iRet = pdu_encode_ud(pstSms, ucCount, &pucPdu, &ulPduLen);
    if (iRet != PDU_E_NONE)
    {
        return PDU_ERROR;
    }

    return PDU_E_NONE;
}

UINT8 pdu_sms_id(void)
{
    static UINT8 ucSmsId = 0;

    ucSmsId++;

    return ucSmsId;
}

INT32 pdu_encode_sms(LTE_SMS_SEND_T *pstSendInfo, INT8 (*pcSmsStr)[SMS_SEND_BUFFER_MAX], INT32 iSmsStrLen, INT32 *pcSmsNum)
{
    UINT8 *pstAddr, *pstData;
    INT32 iDataLen, iAddrLen;
    char *pSmsSendHead;
    INT32 iDataHeadLen;
    SMS_SEND_INFO stSms = {0};
    UINT8 *pstUD, ucLoop;
    UINT8 *pcTmp;
    UINT32 ulInOff, ulOutOff, ulTmp;
    INT32 iUDLen, iRet, iTmpLen;
    INT32 iCount = 0;
    INT32 iDataTmpLen;

    if(pstSendInfo == NULL)
    {
        return PDU_E_NULLPTR;
    }
    pstAddr = pstSendInfo->acRecvMsgCode;
    iAddrLen = strlen(pstAddr);
    pstData = pstSendInfo->acMsgCont;
    iDataLen = strlen(pstData);
    iDataTmpLen = iDataLen;
    

    if (iAddrLen > sizeof(stSms.stDA.aucAddr))
    {
        return PDU_E_DEFAULT_FAIL;
    }

    memcpy(stSms.stDA.aucAddr, pstAddr, iAddrLen);
    stSms.ucDSC = pdu_ud_encode_type(pstData, iDataLen);
    /*add by wangqi */
    if (SMS_ADD_HEAD == pstSendInfo->stSmsSendHead.eSmsHead)
    {
        pSmsSendHead = pstSendInfo->stSmsSendHead.acSmsHead;
        iDataHeadLen = strlen(pSmsSendHead);
        if(iDataHeadLen % 2 != 0)
        {
            return PDU_E_DEFAULT_FAIL;
        }
        if(stSms.ucDSC == DSC_TYPE_7BIT)
        {
            stSms.ucDSC = DSC_TYPE_8BIT;
        }  
        stSms.pstSmsSendHead = &(pstSendInfo->stSmsSendHead);
    }
    /*add by wangqi */
    do
    {
        if (stSms.ucDSC == DSC_TYPE_USC2)
        {
            pcTmp = pstData;
            iTmpLen = iDataLen;
            while (iTmpLen > 0)
            {
                ulTmp = pdu_get_utf8_size(*pcTmp);
                STRING_PUSH(pcTmp, iTmpLen, ulTmp);
                iCount += 2;
            }
            //printf("%d :iCount=%d\n",__LINE__, iCount);

            pstUD = stSms.stUD[stSms.ucCount].aucUD;
            iUDLen = sizeof(stSms.stUD[stSms.ucCount].aucUD);
            if (iCount > 140)
            {
                iUDLen = iUDLen - 6;  /*长短信，去除短信内容头占的六个字节*/
            }

            do
            {
                ulInOff = pdu_utf8_to_unicode_one(pstData, iDataLen, pstUD, iUDLen, &ulOutOff);
                STRING_PUSH(pstData, iDataLen, ulInOff);
                STRING_PUSH(pstUD, iUDLen, ulOutOff);
                stSms.stUD[stSms.ucCount].ucUDLen += ulOutOff;
            }
            while ((iDataLen > 0) && (iUDLen > 0));

        }
        else if (stSms.ucDSC == DSC_TYPE_7BIT)
        {

            pstUD = stSms.stUD[stSms.ucCount].aucUD;
            iUDLen = sizeof(stSms.stUD[stSms.ucCount].aucUD);

            if (iDataTmpLen > 160)
            {
                *pstUD = (*pstData) << 1;
                STRING_PUSH(pstData, iDataLen, 1);
                STRING_PUSH(pstUD, iUDLen, 1);
                iUDLen -= 6;
            }
            //printf("%d :iUDLen = %d\n",__LINE__,iUDLen);
            do
            {
                ulInOff = pdu_utf8_to_bit7(pstData, iDataLen, pstUD, iUDLen, &ulOutOff);
                STRING_PUSH(pstData, iDataLen, ulInOff);
                STRING_PUSH(pstUD, iUDLen, ulOutOff);
                stSms.stUD[stSms.ucCount].ucUDLen += ulInOff;
                //printf("%d :ulInOff = %d, ulOutOff = %d\n",__LINE__,ulInOff, ulOutOff );
                //printf("%d :iDataLen = %d, iUDLen = %d\n",__LINE__,iDataLen, iUDLen );
            }
            while ((iDataLen > 0) && (iUDLen > 0));
            // printf("---%d :%d\n",__LINE__, stSms.stUD[stSms.ucCount].ucUDLen);
        }
		/*add by wangqi */
        else if (stSms.ucDSC == DSC_TYPE_8BIT)
        {
            pstUD = stSms.stUD[stSms.ucCount].aucUD;
            iUDLen = sizeof(stSms.stUD[stSms.ucCount].aucUD);
            if(SMS_ADD_HEAD == pstSendInfo->stSmsSendHead.eSmsHead)
            {
                iUDLen = iUDLen - iDataHeadLen/2;
                stSms.stUD[stSms.ucCount].ucUDLen += iDataHeadLen/2;
            }
            //if((iDataHeadLen+1)/2 + iDataLen  > 140)
            if(iDataLen  > iUDLen)
            {
                iUDLen = iUDLen - 6;
                strncpy(pstUD, pstData, iUDLen);
                stSms.stUD[stSms.ucCount].ucUDLen += iUDLen;
                STRING_PUSH(pstData, iDataLen, iUDLen);
                //STRING_PUSH(pstUD, iUDLen, iUDLen);
            }
            else
            {
                strcpy(pstUD, pstData);
                stSms.stUD[stSms.ucCount].ucUDLen += iDataLen;
                STRING_PUSH(pstData, iDataLen, iDataLen);
                //STRING_PUSH(pstUD, iUDLen, iDataLen);
            }
            printf("%d :pstUD==%s\n",__LINE__, pstUD);
              
        }
		/*add by wangqi */
        else
        {
            return PDU_E_DEFAULT_FAIL;
        }

        stSms.ucCount++;

    }
    while (iDataLen > 0);

    stSms.ucSmsID = pdu_sms_id();
    *pcSmsNum = stSms.ucCount;
    for (ucLoop = 0; ucLoop < stSms.ucCount; ucLoop++)
    {
        iRet = pdu_encode(&stSms, ucLoop, pcSmsStr[ucLoop], iSmsStrLen );
        //printf("%s\n", pcSmsStr);
    }
    return PDU_E_NONE;
}

