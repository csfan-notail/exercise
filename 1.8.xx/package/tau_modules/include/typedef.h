/*******************************************************************************
* COPYRIGHT GBCOM
********************************************************************************
* 文件内容: 单板board模块
* 修改历史: 
*******************************************************************************/
#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

typedef unsigned long  UINT32;
typedef unsigned short UINT16;
typedef unsigned char  UINT8;
typedef unsigned char  BYTE;
typedef signed long    INT32;
typedef signed short   INT16;
typedef signed char    INT8;
typedef int STATUS ;


typedef struct
       {
           UINT8   boardinfo;
       }  BOARDPRVDATE;



#define ERROR -1
#define OK 0


#endif
/* __TYPEDEF_H__ */

