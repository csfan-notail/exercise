/******************************************************************************

                  版权所有 (C), 2010-2012, 上海寰创通信科技有限公司

 ******************************************************************************
  文 件 名   : CPEframework.c
  版 本 号   : 初稿
  作    者   : zhanghaozhong
  生成日期   : 2013年5月16日
  最近修改   :
  功能描述   : TR-69 Object tables for CPE equipment.
  函数列表   :
              buildExternalConnParamName
              buildInterfaceNameParamPath
              checkInstancePath
              checkInstanceStackPath
              clearInstanceStack
              clearNodeStack
              clearStacks
              deleteDope
              deleteInstanceDesc
              deleteInstanceDescNoPathCheck
              findDopeInstance
              findGWParameter
              findInstanceDesc
              findInstanceDescNoPathCheck
              findParameter
              getCurrentInstanceDesc
              getCurrentInstanceID
              getCurrentNode
              getGWParameterValue
              getHigherInstanceDesc
              getInstanceCount
              getInstanceCountNoPathCheck
              getNewInstanceDesc
              getValTypeStr
              isWriteable
              popInstance
              popNode
              pushInstance
              pushNode
              replaceInstanceTop
              searchInstances
              searchNode
              searchTree
  修改历史   :
  1.日    期   : 2013年5月16日
    作    者   : zhanghaozhong
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "sysWrapper.h"
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

 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#include <ctype.h>
#include "utils.h"
#include "standard/rootParams.h"
#include "CPEframework.h"
#include "RPCState.h"

extern int g_log_level;


/* forward refernces */
static TRxObjNode  *searchNode(TRxObjNode *node, const char *p, int lth);
static TRxObjNode  *findParameter(TRxObjNode *node, const char *pstr);
/* 
* global variable: defined when calling getter/setter functions of a parameter node. 
*/
int TRxObjNode_Node3_Num; //add by duanguiyuan 20150827
int             nodeDepth;
TRxObjNode      *nodeStack[MAX_DEPTH];
/*
*
*/
InstanceDesc    *instanceStack[MAX_DEPTH];
int             instanceDepth;

void pushInstance(InstanceDesc *idp)
{
    instanceStack[++instanceDepth] = idp;
}

void popInstance(void)
{
    --instanceDepth;
}
void clearInstanceStack(void)
{
	instanceDepth=0;
}
void replaceInstanceTop(InstanceDesc *idp)
{
    instanceStack[instanceDepth] = idp;
}


void pushNode(TRxObjNode *n)
{
    nodeStack[++nodeDepth] = n;
}

void popNode(void)
{
    --nodeDepth;
}
void clearNodeStack(void)
{
	nodeDepth = 0;
}
void clearStacks(void)
{
	instanceDepth = nodeDepth=0;
}
TRxObjNode  *getCurrentNode(void)
{
    return nodeStack[nodeDepth];
}

int getCurrentInstanceID(void)
{
    if (instanceStack[instanceDepth])
        return instanceStack[instanceDepth]->instanceID;
    return 0;
}
/*
* Get the instance description that was last traversed in the parameter tree
*/
/* 获取当前的实例，在getparametervalue时会找到此实例，并且将其PUSH到栈里 by zhanghaozhong*/
InstanceDesc *getCurrentInstanceDesc()
{
    return instanceStack[instanceDepth];

}
/*
* retrieve instance descriptor from stack 
* 0 is current, 1 is one instance level up, 2 - two level up 
* Return InstanceDesc * == NULL - to far back.
*/                     
InstanceDesc  *getHigherInstanceDesc(int level)
{
	if (level > instanceDepth )
		return NULL;
	else
        return instanceStack[instanceDepth-level];
}

/***********************************************************************
* Parameter framework support functions
************************************************************************/

const char *getValTypeStr(eTRxType etype)
{
    const char *t;
    switch (etype) {
    case tString:
    case tStringSOnly:
        t="string";
        break;
    case tInt:
        t="int";
        break;
    case tUnsigned:
        t="unsignedInt";
        break;
    case tBool:
        t="boolean";
        break;	
	case tselect:
        t="select";
        break;		
	case tipv4:
        t="ipv4";
		break;
    case tDateTime:
        t="dateTime";
        break;
    case tBase64:
        t="base64";
        break;

    default: /*tObject tInstance*/
        t="";
        break;
    }
    return t;
}

/* 
*  Creates a new instanceDesc and links it to the
*  node n if n is instance object; otherwise is steps
* on level thru objdetail link to check if that is an
* instance obect. If so it links it there.
* If id is zero then getNewInstanceId.
*/
InstanceDesc *getNewInstanceDesc( TRxObjNode *n, InstanceDesc *parent, int id)
{
    InstanceDesc    *idp = NULL;
    
    if ( n ) {
        if (!streq(n->name, instanceIDMASK)){
            TRxObjNode *nxt = n->objDetail;
            /* may have to step one level to the instance Object desc */
            if (nxt && nxt->name==instanceIDMASK) 
                n = nxt;
            else
                return idp;
        }
		
        if ((idp = (InstanceDesc*)malloc(sizeof( struct InstanceDesc)))) {
            idp->hwUserData = NULL;
            idp->instanceID = id? id: getNewInstanceId();
            idp->next = n->paramAttrib.instance;
			idp->parent = parent;
            n->paramAttrib.instance = idp;
        }
    }
    return idp;
}

/* check that the Instance parentage is the same as the
*  instance descriptor pushed onto the instanceStack.
*  Returns: 0- No parents found, 1: parents the same.
*/
int checkInstancePath(InstanceDesc *idp)
{
	InstanceDesc	*parent = idp->parent;
	int				i = instanceDepth;
	for (;i>0 && parent!=NULL;--i) {
		if (parent->instanceID != instanceStack[i]->instanceID)
			return 0;
		parent = parent->parent; /* step back thru instance parents */
	}
	return 1;
}
/*
* Find Instance Description (id) in the node
*/
InstanceDesc    *findInstanceDesc( TRxObjNode *n, int id)
{
    InstanceDesc    *idp;
    
    if ( (n && streq(n->name, instanceIDMASK))){
        idp = n->paramAttrib.instance;
        while (idp) {
            if (idp->instanceID == id && checkInstancePath(idp))
					return idp;
            idp = idp->next;
        }
    }
    return NULL;
}
/*
* Find Instance Description (id) in the node.  Do not check path.
*/
InstanceDesc    *findInstanceDescNoPathCheck( TRxObjNode *n, int id)
{
    InstanceDesc    *idp;
    
    if ( (n && streq(n->name, instanceIDMASK))){
        idp = n->paramAttrib.instance;
        while (idp) {
            if (idp->instanceID == id)
					return idp;
            idp = idp->next;
        }
    }
    return NULL;
}

/*
* Return instance count in instance object node without regard to parentage.
* Returns 0..n
*/
int getInstanceCountNoPathCheck( TRxObjNode *n)
{
    InstanceDesc    *idp;
    int         cnt = 0;
    
    if ( (n && streq(n->name, instanceIDMASK))){
        idp = n->paramAttrib.instance;
        while (idp){
			++cnt;	/**/
            idp = idp->next;
        }
    }
    return cnt;
}
/*
* Return instance count in instance object node
* Returns 0..n
*/
int getInstanceCount( TRxObjNode *n)
{
    InstanceDesc    *idp;
    int         cnt = 0;
    
    if ( (n && streq(n->name, instanceIDMASK))){
        idp = n->paramAttrib.instance;
        while (idp) {
			if (checkInstancePath(idp))
				++cnt;	/* parents are present- count it */
            idp = idp->next;
        }
    }
    return cnt;
}

/* check that the Instance chain on the instanceStack
*  is the same as the idp instance. The idp instance
*  is the top of the instanceStack..
*  Returns: 0- Parentage different, 1: parents the same.
*/
int checkInstanceStackPath(InstanceDesc *idp)
{
	InstanceDesc	*parent = idp;
	int				i = instanceDepth;
	for (;i>0 && parent!=NULL;--i) {
		if (parent->instanceID != instanceStack[i]->instanceID)
			return 0;
		parent = parent->parent; /* step back thru instance parents */
	}
	return 1;
}
/*
* find the Dope descriptor for the current node and instance
* Must be called from within node search framework.
*/
InstanceDope *findDopeInstance( TRxObjNode *n )
{
	InstanceDesc	*idp = getCurrentInstanceDesc();
	InstanceDope	*pDope;
	if ( (pDope=n->instanceDope)) {
		/* find dope that matches instance path */
		while ( pDope ) {
			if ( (idp==NULL && pDope->instance==NULL)
				|| (pDope->instance==idp && checkInstanceStackPath(pDope->instance))) {
				/* found instance dope */
				return pDope;
			}
			pDope = pDope->next;
		}
	}
	return NULL;
}

static void deleteDope(TRxObjNode *n, InstanceDesc *idp)
{
	InstanceDope **last;
	InstanceDope *p = n->instanceDope;
	last = &n->instanceDope;

	while ( p ) {
		if (p->instance == idp) {
			/* found it-delete */
			*last = p->next;
			free(p->pdata);
			p->pdata=NULL;
			break;
		}
		last = &p->next;
		p = p->next;
	}
	return;
}

/*
* Delete Instance Description (id) in the current node without checking
 * parentage.
*/
int         deleteInstanceDescNoPathCheck( TRxObjNode *n, int id)
{
    InstanceDesc    *idp, **lastp;
    
    if ( (n && streq(n->name, instanceIDMASK))){
        idp = n->paramAttrib.instance;
        lastp = &n->paramAttrib.instance;
        while (idp) {
            if (idp->instanceID == id){
				/* remove current */
                *lastp = idp->next;
				deleteDope( n, idp);
                free(idp);
				idp=NULL;
                return 0;
            }
            lastp = &idp->next;
            idp = idp->next;
        }
    }
    return -1;
}
/*
* Delete Instance Description (id) in the current node
*/
int         deleteInstanceDesc( TRxObjNode *n, int id)
{
    InstanceDesc    *idp, **lastp;
    
    if ( (n && streq(n->name, instanceIDMASK))){
        idp = n->paramAttrib.instance;
        lastp = &n->paramAttrib.instance;
        while (idp) {
            if (idp->instanceID == id && checkInstancePath(idp)){
				/* remove current */
                *lastp = idp->next;
                deleteDope( n, idp);
                free(idp);
				idp=NULL;
                return 0;
            }
            lastp = &idp->next;
            idp = idp->next;
        }
    }
    return -1;
}
/*
* Set function is not NULL if object/parameter is writeable
*/
int isWriteable(TRxObjNode *n)
{   
    return n->setTRxParam!=NULL;
}

static InstanceDesc *searchInstances( TRxObjNode *n, const char *p)
{
    InstanceDesc    *idp;
    int             id;
    
    id = atoi(p);   /* should parse to NNN. or null */
    idp = n->paramAttrib.instance;
    while (idp) {
        if (idp->instanceID == id && checkInstancePath(idp))
            return idp;
        idp = idp->next;
    }
    return NULL;
}
/*
* Search from node for the parameter(*p),. Isolate the next portion of the 
 * parameter string and search the node for it.
 * Returns: TRxObjNode  item pointer
 *       or NULL.
 */
static TRxObjNode  *searchNode(TRxObjNode *node, const char *p, int lth)
{
    TRxObjNode  *n = node;
    InstanceDesc    *idp;
    
    while( n->name ){
        if (isdigit(*p) && n->name==instanceIDMASK) {
            /* this instanceDescs  associated with the entry */
            /* to determine if the instance exists and to set the associated */
            /* instance variable for this node */
			/*	slog(TR69C_LOG_DEBUG,"isdigit p = %s",p);
			if(isdigit(*p))
			{
				slog(TR69C_LOG_DEBUG,"atoi(p) = %d",atoi(p));

			}
			*/
            if ( (idp = searchInstances( n,p))!=NULL){
                pushInstance(idp);
                nodeStack[++nodeDepth] = n;
                return n;
            }
            else
                return NULL;  /* instance was not found */
        }
        else if( (!strncmp(n->name,p,lth))&& strlen(n->name)==lth){
			//	slog(TR69C_LOG_DEBUG, "dgy_test Y  n->name = %s  ", n->name);
                nodeStack[++nodeDepth] = n;
                return n;
            }
        else if (n->name==instanceIDMASK)
                return NULL;    /* only a single entry is in an instance Node */
        ++n;
    };
    return NULL;
}
/* 
*/
static TRxObjNode  *findParameter(TRxObjNode *node, const char *pstr)
{
    const char *e;
    int     lth;
    TRxObjNode  *n = node;

    e=strchr(pstr,'.');
    lth = e? e-pstr: strlen(pstr);
//slog(TR69C_LOG_DEBUG, "dgy_test   nodeDepth = %d  instanceDepth = %d",nodeDepth,instanceDepth);
//slog(TR69C_LOG_DEBUG, "dgy_test   e=%s lth = %d  pstr =%s ", e,lth,pstr);
	if((nodeDepth ==3)&&isdigit(*pstr))
	{
		TRxObjNode_Node3_Num = atoi(pstr);
		slog(TR69C_LOG_DEBUG, "dgy_test  atoi(pstr) = %d ", TRxObjNode_Node3_Num);
	}
    if ( (n=searchNode(node, pstr, lth)) ) {
        if(e && strlen(e)>1 ){
            return findParameter(n->objDetail,e+1);
        }else 
            return n;
    }
    return NULL;
}

TRxObjNode  *findGWParameter(const char *pstr)
{
    nodeDepth = 0;
    instanceDepth = 0;
    return findParameter(rootDevice, pstr);
}

TRX_STATUS getGWParameterValue( const char *pstr, char **value)
{
	TRxObjNode *n;
	if ( (n=findGWParameter(pstr))) {
		if (n->getTRxParam)
		    return n->getTRxParam(value);
	}
if (g_debug)
	fprintf(stderr, "getGWParaemterValue() unable to find parameter name\n");
	return TRX_ERR;
}

/* Special walk tree to build the parameter name for the default IP address. At each node
* first walk thru the object list and look for objDetail. If any then searchTree with it
* Next walk thru list again and call cbfunc for each item in next list.
* Then return.
*/

static int searchTree( TRxObjNode *, char *, char * );
static char paramPath[300];
/*
* return 0: not found
* 		 1: found pname with value
*/
static int searchTree( TRxObjNode *n, char *pname, char *pvalue )
{
	int		pathLth = strlen(paramPath);
if (g_debug)
	fprintf(stderr, "searchTree %s paramPath=%s\n", n->name, paramPath);

	nodeStack[++nodeDepth] = n;
    while (n && n->name) {
        nodeStack[nodeDepth-1] = n;
		if ( n->name == instanceIDMASK) {
			InstanceDesc    *idp;
			idp = n->paramAttrib.instance;
			while (idp) {
				int	fnd =0;
				if (checkInstancePath(idp)) {
					pushInstance(idp);
					sprintf(paramPath, "%s%d.", paramPath, idp->instanceID);
					if (n->objDetail)
						fnd = searchTree( n->objDetail, pname, pvalue);
					popInstance();
					if (fnd)
						return 1;
				}
				paramPath[pathLth]='\0';
				idp = idp->next;
			}
			--nodeDepth;
			return 0;   /* return here instance nodes donot have a null terminator*/
		} else if (n->paramAttrib.attrib.etype==tObject
			       || n->paramAttrib.attrib.etype==tInstance) {
			if (n->objDetail){
				sprintf(paramPath, "%s%s.", paramPath, n->name);
				if ( searchTree( n->objDetail, pname, pvalue ) )
					return 1;
			}
		} if (strcmp(n->name,pname)==0 ) {
			/* found parameter name */
			char *nvalue = NULL;
                        if ( n->getTRxParam ) {
                                n->getTRxParam( &nvalue );
                                if ((nvalue != NULL) && strcmp(nvalue,pvalue)==0){
                                        /* found value */
                                        free(nvalue);
										nvalue=NULL;
                                        sprintf(paramPath, "%s%s", paramPath, pname);
                                        return 1;
                                }
                                if (nvalue != NULL)
                                        {
                                        	free(nvalue);
											nvalue=NULL;
										}
                        } else
                                return 0;
		}
        paramPath[pathLth]='\0';
        ++n;  /* try next item in node list */
    }
	--nodeDepth;
    return 0;
}
/* 
*        buildExternalConnParamName(int extIP)
* The Infrom msg requires the parameter path name of the default connection
* device. This parameter name is variable and may change whenever the connections are
* reconfigured. This function builds the parameter path name of the
* current default WAN connection. It has the form (See Table 33):
*  InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{j}.WANPPPConnection.{k}.ExternalIPAddress
* or
*  InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{j}.WANIPConnection.{k}.ExternalIPAddress
* The string is built by traverseing the parameter tree starting the search for the extIP address in an
*  ExternalIPAddress parameter at the InternetGatewayDevice.WANDevice. node. 
*/

extern TRxObjNode  WANDeviceInstanceDesc[];
/* extIP从int型改为char *型 */
char *buildExternalConnParamName(char *extIP)
{
	char extIPStr[30];
	snprintf(paramPath,sizeof(paramPath), "%s.%s.", "InternetGatewayDevice", "WANDevice");
	/* BEGIN: Modified by zhanghaozhong, 2013/4/22   PN:dev tr69c */
	//strcpy(extIPStr, writeIp(extIP));
    strcpy(extIPStr, extIP);
	/* END:   Modified by zhanghaozhong, 2013/4/22   PN:dev tr69c */

	instanceDepth = 0;
	nodeDepth = 0;
	if ( searchTree( WANDeviceInstanceDesc,"ExternalIPAddress", extIPStr)){ 
        slog(TR69C_LOG_DEBUG, "Ext Connection Param Name: %s", paramPath);
	}else {
		slog(TR69C_LOG_ERR, "TR69C: Unable to determine External IP Connection Parameter Name");
		paramPath[0]='\0';
	}
slog(TR69C_LOG_DEBUG, "Ext Connection Param Name: %s", paramPath);
	return paramPath;
}
/*
* build the full parameter path to the interface "Name" in WANIPConnection or WANPPPConnetion
* objects.
 * This call destroys the state of the instance and node stacks.
*/ 
char *buildInterfaceNameParamPath( char *ifname )
{
	char	*c;
	extern TRxObjNode WANDeviceWANConnectionDeviceInstanceDesc[];
	snprintf(paramPath,sizeof(paramPath), "%s.%s.1.%s.",
			  "InternetGatewayDevice", "WANDevice","WANConnectionDevice");

	instanceDepth = 0;
	nodeDepth = 0;
	if ( searchTree( WANDeviceWANConnectionDeviceInstanceDesc,"Name", ifname)){
if (g_debug)
		fprintf(stderr, "WANDeviceWANConnectionDevice_xxx.Name: %s or %s\n", paramPath, ifname);
               
		/* param path is of from InternetGatewayDevice.xx.xx.xxx.Name Need to remove .Name */
		if ( (c=strstr(paramPath,".Name")))
			 *c = '\0';	  /* terminate string at beginning of .Name */
    }else
		slog(TR69C_LOG_ERR, "TR69C: Unable to determine WANDeviceWANConnectionDevice_xxx.Name for %s", ifname);
	return paramPath;

}

