/******************************************************************************

  Copyright (C), 2010-2011, GBCOM Co., Ltd.

 ******************************************************************************
  File Name     : aglinkcli.h
  Version       : Initial Draft
  Author        : lvjianlin
  Created       : 2015/3/17
  Last Modified :
  Description   : aglinkcliÍ·ÎÄ¼þ
  Function List :
  History       :
  1.Date        : 2015/3/17
    Author      : lvjianlin
    Modification: Created file

******************************************************************************/
#ifndef AGLINK_CLI_H
#define AGLINK_CLI_H
#include "ag_module.h"
/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/
int aglinkcli_get_module_info(int argc, char *argv[]);
int aglinkcli_get_online_info(int argc, char *argv[]);
int aglinkcli_get_ap_list_info(int argc, char *argv[]);
int aglinkcli_set_keeplive_tm(int argc, char *argv[]);
int aglinckcli_set_debug_level(int argc, char *argv[]);

#ifndef GW_X86
int aglinkcli_register_apinfo_to_gw(int argc, char *argv[]);
#endif


/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/
typedef struct
{
	const char *pcName;
	int  (*pFunc)(int argc, char *argv[]);
	const char *pcHelp;
}AGLINKCLI_HELP_T;

#endif
