/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : CPEframework.h
  版 本 号   : 初稿
  作    者   : zhanghaozhong
  生成日期   : 2013年5月16日
  最近修改   :
  功能描述   : CPE table access/mgmt functions
  函数列表   :
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

#ifndef CPRFRAMEWORK_H
#define  CPRFRAMEWORK_H

#include "tr69cdefs.h"

#define MAX_DEPTH   20      /* maximum nodes in parameter tree path */
extern const char instanceIDMASK[];
extern TRxObjNode  rootDevice[];

extern int g_debug;

const char *getValTypeStr(eTRxType etype);
int isWriteable(TRxObjNode *n);
TRxObjNode  *findGWParameter(const char *pstr);
TRxObjNode  *getCurrentNode(void);
int         getCurrentInstanceID(void);
int         getInstanceCount( TRxObjNode *n);
int         getInstanceCountNoPathCheck( TRxObjNode *n);
InstanceDesc *getCurrentInstanceDesc(void);
InstanceDesc  *getHigherInstanceDesc(int level);
InstanceDesc *findInstanceDesc( TRxObjNode *n, int id);
InstanceDesc *findInstanceDescNoPathCheck( TRxObjNode *n, int id);
InstanceDesc *getNewInstanceDesc( TRxObjNode *n, InstanceDesc *parent, int id);
InstanceDope *findDopeInstance( TRxObjNode *n );
int deleteCurrentInstanceDesc(void);
int deleteInstanceDescNoPathCheck( TRxObjNode *n, int id);
int deleteInstanceDesc( TRxObjNode *n, int id);
int checkInstancePath(InstanceDesc *idp);
int checkInstanceStackPath(InstanceDesc *idp);

void pushNode(TRxObjNode *n);
void popNode(void);
void pushInstance(InstanceDesc *idp);
void popInstance(void);
void replaceInstanceTop(InstanceDesc *idp);
void clearInstanceStack(void);
void clearNodeStack(void);
void clearStacks(void);

char *buildInterfaceNameParamPath(char *ifname);
char *buildExternalConnParamName(char *extIP);

TRX_STATUS getGWParameterValue( const char *pstr, char **value);




#endif /* CPRFRAMEWORK_H */
