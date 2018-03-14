//功能：实现DES及3DES加解密的算法
//作者：XXX
//日期：2008-9-23
 
#ifndef DES_H
#define DES_H

//函数声明
int Do_DES(char* strSrc, char* strKey, char* strDest, char flag);
int Do_3DES(char* strSrc, char* strKey, char* strDest, char flag);

#endif
