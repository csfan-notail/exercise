//功能：实现DES及3DES加解密的算法
//作者：XXX
//日期：2008-9-23
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "des.h"


static int pc_1_table[56]={57,49,41,33,25,17, 9,
                           1,58,50,42,34,26,18,
                           10, 2,59,51,43,35,27,
                           19,11, 3,60,52,44,36,
                           63,55,47,39,31,23,15,
                           7,62,54,46,38,30,22,
                           14, 6,61,53,45,37,29,
                           21,13, 5,28,20,12, 4};
      
static int pc_2_table[48]={14,17,11,24, 1, 5,
                           3,28,15, 6,21,10,
                           23,19,12, 4,26, 8,
                           16, 7,27,20,13, 2,
                           41,52,31,37,47,55,
                           30,40,51,45,33,48,
                           44,49,39,56,34,53,
                           46,42,50,36,29,32};
     
static int ls_num_table[16]={1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
 
static int ip_table[64]={58,50,42,34,26,18,10, 2,
                         60,52,44,36,28,20,12, 4,
                         62,54,46,38,30,22,14, 6,
                         64,56,48,40,32,24,16, 8,
                         57,49,41,33,25,17, 9, 1,
                         59,51,43,35,27,19,11, 3,
                         61,53,45,37,29,21,13, 5,
                         63,55,47,39,31,23,15, 7};
      
static int ip_1_table[64]={40, 8,48,16,56,24,64,32,
                           39, 7,47,15,55,23,63,31,
                           38, 6,46,14,54,22,62,30,
                           37, 5,45,13,53,21,61,29,
                           36, 4,44,12,52,20,60,28,
                           35, 3,43,11,51,19,59,27,
                           34, 2,42,10,50,18,58,26,
                           33, 1,41, 9,49,17,57,25};
 
static int e_table[48]={32, 1, 2, 3, 4, 5,
                        4, 5, 6, 7, 8, 9,
                        8, 9,10,11,12,13,
                        12,13,14,15,16,17,
                        16,17,18,19,20,21,
                        20,21,22,23,24,25,
                        24,25,26,27,28,29,
                        28,29,30,31,32, 1};
        
static int p_table[32]={16, 7,20,21,
                        29,12,28,17,
                        1,15,23,26,
                        5,18,31,10,
                        2, 8,24,14,
                        32,27, 3, 9,
                        19,13,30, 6,
                        22,11, 4,25};
 
static int s_box_table[8][4][16]={
                   {{14, 4,13, 1, 2,15,11, 8, 3,10, 6,12, 5, 9, 0, 7},
                     { 0,15, 7, 4,14, 2,13, 1,10, 6,12,11, 9, 5, 3, 8},
                     { 4, 1,14, 8,13, 6, 2,11,15,12, 9, 7, 3,10, 5, 0},
                     {15,12, 8, 2, 4, 9, 1, 7, 5,11, 3,14,10, 0, 6,13}},
                   {{15, 1, 8,14, 6,11, 3, 4, 9, 7, 2,13,12, 0, 5,10},
                    { 3,13, 4, 7,15, 2, 8,14,12, 0, 1,10, 6, 9,11, 5},
                    { 0,14, 7,11,10, 4,13, 1, 5, 8,12, 6, 9, 3, 2,15},
                    {13, 8,10, 1, 3,15, 4, 2,11, 6, 7,12, 0, 5,14, 9}},
                   {{10, 0, 9,14, 6, 3,15, 5, 1,13,12, 7,11, 4, 2, 8},
                    {13, 7, 0, 9, 3, 4, 6,10, 2, 8, 5,14,12,11,15, 1},
                    {13, 6, 4, 9, 8,15, 3, 0,11, 1, 2,12, 5,10,14, 7},
                    { 1,10,13, 0, 6, 9, 8, 7, 4,15,14, 3,11, 5, 2,12}},
                   {{ 7,13,14, 3, 0, 6, 9,10, 1, 2, 8, 5,11,12, 4,15},
                    {13, 8,11, 5, 6,15, 0, 3, 4, 7, 2,12, 1,10,14, 9},
                    {10, 6, 9, 0,12,11, 7,13,15, 1, 3,14, 5, 2, 8, 4},
                    { 3,15, 0, 6,10, 1,13, 8, 9, 4, 5,11,12, 7, 2,14}},
                   {{ 2,12, 4, 1, 7,10,11, 6, 8, 5, 3,15,13, 0,14, 9},
                    {14,11, 2,12, 4, 7,13, 1, 5, 0,15,10, 3, 9, 8, 6},
                    { 4, 2, 1,11,10,13, 7, 8,15, 9,12, 5, 6, 3, 0,14},
                    {11, 8,12, 7, 1,14, 2,13, 6,15, 0, 9,10, 4, 5, 3}},
                   {{12, 1,10,15, 9, 2, 6, 8, 0,13, 3, 4,14, 7, 5,11},
                    {10,15, 4, 2, 7,12, 9, 5, 6, 1,13,14, 0,11, 3, 8},
                    { 9,14,15, 5, 2, 8,12, 3, 7, 0, 4,10, 1,13,11, 6},
                    { 4, 3, 2,12, 9, 5,15,10,11,14, 1, 7, 6, 0, 8,13}},
                   {{ 4,11, 2,14,15, 0, 8,13, 3,12, 9, 7, 5,10, 6, 1},
                    {13, 0,11, 7, 4, 9, 1,10,14, 3, 5,12, 2,15, 8, 6},
                    { 1, 4,11,13,12, 3, 7,14,10,15, 6, 8, 0, 5, 9, 2},
                    { 6,11,13, 8, 1, 4,10, 7, 9, 5, 0,15,14, 2, 3,12}},
                   {{13, 2, 8, 4, 6,15,11, 1,10, 9, 3,14, 5, 0,12, 7},
                    { 1,15,13, 8,10, 3, 7, 4,12, 5, 6,11, 0,14, 9, 2},
                    { 7,11, 4, 1, 9,12,14, 2, 0, 6,10,13,15, 3, 5, 8},
                    { 2, 1,14, 7, 4,10, 8,13,15,12, 9, 0, 3, 5, 6,11}}
                    };
  
//函数声明
int Do_DES(char* strSrc, char* strKey, char* strDest, char flag);
int Do_3DES(char* strSrc, char* strKey, char* strDest, char flag);
 
//主函数
/*int main(int argc, char** argv)
{
    char src16[16+1],key16[16+1],key48[48+1],dest16[16+1];
 
    if(argc != 3)
   {
       fprintf(stderr,"Usage: [%s -e|-d s|3]\n",argv[0]);
       exit(1); 
   }
 
   if(strcmp(argv[2],"-s") == 0)
  {
      if(strcmp(argv[1],"-e") == 0)//加密
     {
          fprintf(stderr,"Please input the string that you want to encrypt(16 hex number):\n");
          memset(src16,0,sizeof(src16));
          scanf("%s",src16);
          fprintf(stderr,"Please input the Key string(16 hex number):\n");
          memset(key16,0,sizeof(key16));
          scanf("%s",key16);
          memset(dest16,0,sizeof(dest16));
          Do_DES(src16,key16,dest16,'e');
          fprintf(stderr,"Result: [%s]\n",dest16);
      }
      else if(strcmp(argv[1],"-d") == 0)//解密
     {
          fprintf(stderr,"Please input the string that you want to decrypt(16 hex number):\n");
          memset(src16,0,sizeof(src16));
          scanf("%s",src16);
          fprintf(stderr,"Please input the Key string(16 hex number):\n");
          memset(key16,0,sizeof(key16));
          scanf("%s",key16);
          memset(dest16,0,sizeof(dest16));
          Do_DES(src16,key16,dest16,'d');
          fprintf(stderr,"Result: [%s]\n",dest16);
      }
      else
          return -1;
   }
   else if(strcmp(argv[2],"-3") == 0)
  {
      if(strcmp(argv[1],"-e") == 0)
     {
          fprintf(stderr,"Please input the string that you want to encrypt(16 hex number):\n");
          memset(src16,0,sizeof(src16));
          scanf("%s",src16);
          fprintf(stderr,"Please input the Key string(16 hex number):\n");
          memset(key48,0,sizeof(key48));
          scanf("%s",key48);
          memset(dest16,0,sizeof(dest16));
          Do_3DES(src16,key48,dest16,'e');
          fprintf(stderr,"Result: [%s]\n",dest16);
      }
      else if(strcmp(argv[1],"-d") == 0)
     {
          fprintf(stderr,"Please input the string that you want to decrypt(16 hex number):\n");
          memset(src16,0,sizeof(src16));
          scanf("%s",src16);
          fprintf(stderr,"Please input the Key string(16 hex number):\n");
          memset(key48,0,sizeof(key48));
          scanf("%s",key48);
          memset(dest16,0,sizeof(dest16));
          Do_3DES(src16,key48,dest16,'d');
          fprintf(stderr,"Result: [%s]\n",dest16);
      }
      else
          return -1;
   }
   else
      return -1;
 
   return 0;  
}*/
 
//做DES加密或解密运算
int Do_DES(char* strSrc, char* strKey, char* strDest, char flag)
{
   int i,j;
   unsigned char subKey[16][48+1],byte8[8+1],bits[64+1],strTmp[64+1];
   unsigned char L0[32+1],R0[32+1],Lx[32+1],Rx[32+1];
 
   if(!( flag == 'e' || flag == 'E' || flag == 'd' || flag == 'D'))
       return -1;
   if(strSrc == NULL || strKey == NULL)
       return -2;
 
   if(flag == 'e' || flag == 'E')
  {
       memset(byte8,0,sizeof(byte8));
       BCDToByte(strKey, 16, byte8);
       memset(bits,0,sizeof(bits));
       ByteToBit(byte8, 8, bits);
  
       Des_GenSubKey(bits,subKey);
  
       BCDToByte(strSrc, 16, byte8);
       ByteToBit(byte8, 8, bits);
       Des_IP(bits, strTmp);
       memcpy(L0,strTmp,32);
       memcpy(R0,strTmp+32,32);
  
       for(i=0;i<16;i++)
      {
          memcpy(Lx,R0,32);
          Des_F(R0,subKey[i],Rx);
          Do_XOR(L0,32,Rx);
          memcpy(L0,Lx,32);
          memcpy(R0,Rx,32);
      }
      memcpy(bits,R0,32);
      memcpy(bits+32,L0,32);
      Des_IP_1(bits,strTmp);
      BitToByte(strTmp,64,byte8);
      ByteToBCD(byte8,8,strDest);
  }
  else
  {
      memset(byte8,0,sizeof(byte8));
      BCDToByte(strKey, 16, byte8);
      memset(bits,0,sizeof(bits));
      ByteToBit(byte8, 8, bits);
  
      Des_GenSubKey(bits,subKey);
  
      BCDToByte(strSrc, 16, byte8);
      ByteToBit(byte8, 8, bits);
      Des_IP(bits, strTmp);
      memcpy(L0,strTmp,32);
      memcpy(R0,strTmp+32,32);
  
      for(i=0;i<16;i++)
     {
         memcpy(Lx,R0,32);
         Des_F(R0,subKey[15-i],Rx);
         Do_XOR(L0,32,Rx);
         memcpy(L0,Lx,32);
         memcpy(R0,Rx,32);
     }
     memcpy(bits,R0,32);
     memcpy(bits+32,L0,32);
     Des_IP_1(bits,strTmp);
     BitToByte(strTmp,64,byte8);
     ByteToBCD(byte8,8,strDest);
  }
 
  return 0;
}
 
//做3DES加密或解密运算
int Do_3DES(char* strSrc, char* strKey, char* strDest, char flag)
{ 
   unsigned char strBCDKey[32+1],strByteKey[16+1];
   unsigned char strMidDest1[16+1],strMidDest2[16+1];
   unsigned char strLKey[16+1],strMKey[16+1],strRKey[16+1];
 
   if(!( flag == 'e' || flag == 'E' || flag == 'd' || flag == 'D'))
       return -1;
   if(strSrc == NULL || strKey == NULL)
       return -2;
 
   if(strlen(strKey) < 32)
       return -3;
   
   if(flag == 'e' || flag == 'E')
  {
      memset(strBCDKey,0,sizeof(strBCDKey));
      memcpy(strBCDKey,strKey,32);
   
      memset(strLKey,0,sizeof(strLKey));
      memcpy(strLKey,strBCDKey,16);
      memset(strRKey,0,sizeof(strRKey));
      memcpy(strRKey,strBCDKey+16,16);
   
      Do_DES(strSrc,strLKey,strMidDest1,'e');
      Do_DES(strMidDest1,strRKey,strMidDest2,'d');
      Do_DES(strMidDest2,strLKey,strMidDest1,'e');
   
      memcpy(strDest,strMidDest1,16);
  }
  else
  {
      memset(strBCDKey,0,sizeof(strBCDKey));
      memcpy(strBCDKey,strKey,32);
   
      memset(strLKey,0,sizeof(strLKey));
      memcpy(strLKey,strBCDKey,16);
      memset(strRKey,0,sizeof(strRKey));
      memcpy(strRKey,strBCDKey+16,16);
   
      Do_DES(strSrc,strLKey,strMidDest1,'d');
      Do_DES(strMidDest1,strRKey,strMidDest2,'e');
      Do_DES(strMidDest2,strLKey,strMidDest1,'d');
   
      memcpy(strDest,strMidDest1,16);
  }
 
  return 0;
}
 
//对输入的字节串作BCD编码扩展
int ByteToBCD(unsigned char* bytes, int count,unsigned char* strBCD)
{
   unsigned char cTmp;
   int i;
 
   for(i=0;i<count;i++)
  {
      cTmp = (bytes[i] & 0xF0) >> 4;
      strBCD[i*2] = (cTmp > 9) ? cTmp - 10 + 'A' : cTmp + '0';
      cTmp = bytes[i] & 0x0F;
      strBCD[i*2+1] = (cTmp > 9) ? cTmp - 10 + 'A' : cTmp + '0'; 
  }
 
  return (count*2); 
}
 
//把输入的BCD编码串还原成字节串
int BCDToByte(unsigned char* strBCD, int count, unsigned char* bytes)
{
   unsigned char cTmp;
   int i;
 
   for(i=0;i<count/2;i++)
  {
      cTmp = strBCD[i*2];
      if(cTmp >= 'A' && cTmp <= 'F')
          cTmp = cTmp - 'A' + 10;
      else if(cTmp >= 'a' && cTmp <= 'f')
          cTmp = cTmp - 'a' + 10;
      else
          cTmp &= 0x0F;
      bytes[i] = cTmp << 4;
      cTmp = strBCD[i*2+1];
      if(cTmp >= 'A' && cTmp <= 'F')
          cTmp = cTmp - 'A' + 10;
      else if(cTmp >= 'a' && cTmp <= 'f')
          cTmp = cTmp - 'a' + 10;
      else
          cTmp &= 0x0F;
      bytes[i] += cTmp;
  }
 
  return (count/2);
}
 
//把字节串变成比特串
int ByteToBit(unsigned char* bytes, int count, unsigned char* strBit)
{
   unsigned char cTmp;
   int i,j;
 
   for(i=0;i<count;i++)
  {
      cTmp = 0x80;
      for(j=0;j<8;j++)
     {
         strBit[i*8+j] = (bytes[i] & cTmp) >> (7-j);
         cTmp = cTmp >> 1;
     } 
  }
 
  return (count*8); 
}
 
//把比特串变成字节串
int BitToByte(unsigned char* strBit, int count, unsigned char* bytes)
{
    unsigned char cTmp;
    int i,j;
 
    for(i=0;i<(count/8);i++)
   {
       cTmp = 0x00;
       for(j=0;j<8;j++)
      {
          cTmp += (strBit[i*8+j] << (7-j));
      }
      bytes[i] = cTmp; 
   }
 
   return (count/8);
}
 
//做异或操作
int Do_XOR(unsigned char* strSrc, int count, unsigned char* strDest)
{
    int i;
 
    if(strSrc == NULL || strDest == NULL)
    return -1;
  
    for(i=0;i<count;i++)
    strDest[i] ^= strSrc[i];
 
    return 0;
}
 
//des算法PC-1变换，把64比特的密钥K变换成56比特
int Des_PC_1(unsigned char* strIn, unsigned char* strOut)
{
    int i;
 
    for(i=0;i<56;i++)
    strOut[i] = strIn[pc_1_table[i]-1];
 
    return 56; 
}
 
//des算法PC-2变换，把56比特变换成48比特
int Des_PC_2(unsigned char* strIn, unsigned char* strOut)
{
    int i;
 
    for(i=0;i<48;i++)
    strOut[i] = strIn[pc_2_table[i]-1];
 
    return 48;
}
 
//des算法的循环左移位运算
int Des_LS(unsigned char* strIn, int count, unsigned char* strOut)
{
    int i;
 
    for(i=0;i<28;i++)
    //strOut[i] = strIn[(i+count)];
 		strOut[(i-count+28)%28] = strIn[i];
    return 28; 
}
 
//des算法中通过父密钥产生16个48比特位的子密钥
int Des_GenSubKey(unsigned char* strKey, unsigned char strSubKey[16][48+1])
{
    unsigned char tmp[56+1],C0[28+1],D0[28+1],Cx[28+1],Dx[28+1];
    int i,j;
 
    memset(tmp,0,sizeof(tmp));
    memset(C0,0,sizeof(C0));
    memset(D0,0,sizeof(D0));
    memset(Cx,0,sizeof(Cx));
    memset(Dx,0,sizeof(Dx));
 
    Des_PC_1(strKey, tmp);
 
    memcpy(C0,tmp,28);
    memcpy(D0,tmp+28,28);
 
    for(i=0;i<16;i++)
   {
       Des_LS(C0,ls_num_table[i],Cx);
       Des_LS(D0,ls_num_table[i],Dx);
       memcpy(tmp,Cx,28);
       memcpy(tmp+28,Dx,28);
       Des_PC_2(tmp,strSubKey[i]);
       memcpy(C0,Cx,28);
       memcpy(D0,Dx,28);
   }
 
   return 0;
}
 
//des算法IP置换
int Des_IP(unsigned char* strIn, unsigned char* strOut)
{
    int i;
 
    for(i=0;i<64;i++)
    strOut[i] = strIn[ip_table[i]-1];
  
    return 64;
}
 
//des算法IP-1置换
int Des_IP_1(unsigned char* strIn, unsigned char* strOut)
{
    int i;
 
    for(i=0;i<64;i++)
    strOut[i] = strIn[ip_1_table[i]-1];
  
    return 64;
}
 
//des算法e变换，将32比特变成48比特
int Des_E(unsigned char* strIn, unsigned char* strOut)
{
    int i;
 
    for(i=0;i<48;i++)
        strOut[i] = strIn[e_table[i]-1];
  
    return 48;
}
 
//des算法P变换
int Des_P(unsigned char* strIn, unsigned char* strOut)
{
    int i;
 
    for(i=0;i<32;i++)
        strOut[i] = strIn[p_table[i]-1];
  
    return 32;
}
 
//des算法S盒变换
int Des_S_Box(unsigned char* strIn, int nSBox, unsigned char* strOut)
{
    int x,y,i,nValue;
    unsigned char c;
 
    if(nSBox < 1 || nSBox > 8)
        return -1;
 
    x = strIn[0] * 2 + strIn[5];
    y = strIn[1] * 8 + strIn[2] * 4 + strIn[3] * 2 + strIn[4];
 
    nValue = s_box_table[nSBox-1][x][y];
    c = 0x08;
    for(i=0;i<4;i++)
   {
        strOut[i] = (nValue & c) >> (3 - i);
        c = c >> 1; 
   }
 
   return 4;
}
 
//des算法F函数，对Ri-1和Ki进行运算
int Des_F(unsigned char* strR, unsigned char* strK, unsigned char* strOut)
{
    int i,j,k;
    unsigned char strAftE[48],strPreP[32],sbIn[8][6],sbOut[8][4];
 
    for(i=0;i<48;i++)
    strAftE[i] = strR[e_table[i]-1];
    Do_XOR(strK, 48, strAftE);
 
    for(i=0;i<8;i++)
        for(j=0;j<6;j++)
           sbIn[i][j] = strAftE[i*6+j];
 
    for(i=0;i<8;i++)
        Des_S_Box(sbIn[i], i+1, sbOut[i]);
  
    for(i=0;i<32;i++)
        strPreP[i] = sbOut[i/4][i%4];
    Des_P(strPreP, strOut);
 
    return 32;
}
