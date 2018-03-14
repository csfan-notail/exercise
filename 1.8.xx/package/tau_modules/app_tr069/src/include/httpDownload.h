/*
 * Copyright (C) GBCOM ltd.,co 2015
 *
 * This is a part of project tr69cd, which implements TR069 protocal(aka
 * CWMP).
 * 
 * This is a PRIVATE software.
 * All rights reserved.
 */
 
void downloadStart( void *handle); 
void downloadStop(char *msg, int status);
void updateDownLoadKey( DownloadReq *);
//#ifdef RPC_UPLOAD    
void uploadStart( void *handle); 
//#endif
