/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : commandline.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/11/25
  Last Modified :
  Description   : commandline.c header file
  Function List :
  History       :
  1.Date        : 2013/11/25
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#ifndef __COMMANDLINE_H__
#define __COMMANDLINE_H__

extern pid_t restart_orig_pid;

extern void parse_commandline(int argc, char **argv);

#endif /* __COMMANDLINE_H__ */
