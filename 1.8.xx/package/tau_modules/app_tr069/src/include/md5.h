/*
 * Copyright (C) GBCOM ltd.,co 2015
 *
 * This is a part of project tr69cd, which implements TR069 protocal(aka
 * CWMP).
 * 
 * This is a PRIVATE software.
 * All rights reserved.
 */
 

#ifndef MD5_H
#define MD5_H

#define MD5_DIGEST_LEN     16

typedef struct MD5Context {
  u_int32_t buf[4];
  u_int32_t bits[2];
  u_char in[64];
} MD5Context;

void MD5Init(MD5Context *ctx);
void MD5Update(MD5Context *ctx, u_char const *buf, u_int len);
void MD5Final(u_char digest[16], MD5Context *ctx);
void tr69_md5it(unsigned char *out, const unsigned char *in);

#endif
