/******************************************************************************

                  ��Ȩ���� (C), 2010-2012, �Ϻ�徴�ͨ�ſƼ����޹�˾

 ******************************************************************************
  �� �� ��   : CPEframework.h
  �� �� ��   : ����
  ��    ��   : zhanghaozhong
  ��������   : 2013��5��16��
  ����޸�   :
  ��������   : CPE table access/mgmt functions
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��5��16��
    ��    ��   : zhanghaozhong
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
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
