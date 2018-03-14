/******************************************************************************
模块名      : 
文件名      : 
相关文件    :
实现功能    : 
修改记录    :
------------------------------------------------------------------------------
日期        修改人      修改内容
2013-07-08  冯静        创建
*******************************************************************************/

/******************************* 包含文件声明 ********************************/
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "md5lib.h"

/******************************* 局部宏定义 **********************************/

/******************************* 全局变量定义/初始化 *************************/

/******************************* 局部常数和类型定义 **************************/

/******************************* 局部函数原型声明 ****************************/

/******************************* 函数实现 ************************************/

void MD5Init(struct MD5Context *ctx)
{
    ctx->buf[0] = 0x67452301;
    ctx->buf[1] = 0xefcdab89;
    ctx->buf[2] = 0x98badcfe;
    ctx->buf[3] = 0x10325476;

    ctx->bits[0] = 0;
    ctx->bits[1] = 0;
}

//#define byteReverse(a, b)
static void byteReverse(unsigned char *buf, unsigned longs)
{
    unsigned long t;
    do
    {
	    t = (unsigned long) ((unsigned) buf[3] << 8 | buf[2]) << 16 |
	    ((unsigned) buf[1] << 8 | buf[0]);
    	*(unsigned long *) buf = t;
	    buf += 4;
    } while (--longs);
}

/* The four core functions - F1 is optimized somewhat */

/* #define F1(x, y, z) (x & y | ~x & z) */
#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

/* This is the central step in the MD5 algorithm. */
#define MD5STEP(f, w, x, y, z, data, s) \
	( w += f(x, y, z) + data,  w = w<<s | w>>(32-s),  w += x )

/*
 * The core of the MD5 algorithm, this alters an existing MD5 hash to
 * reflect the addition of 16 longwords of new data.  MD5Update blocks
 * the data and converts bytes into longwords for this routine.
 */
static void MD5Transform(unsigned long buf[4], unsigned long const in[16])
{
    register unsigned long a, b, c, d;

    a = buf[0];
    b = buf[1];
    c = buf[2];
    d = buf[3];

    MD5STEP(F1, a, b, c, d, in[0] + 0xd76aa478, 7);
    MD5STEP(F1, d, a, b, c, in[1] + 0xe8c7b756, 12);
    MD5STEP(F1, c, d, a, b, in[2] + 0x242070db, 17);
    MD5STEP(F1, b, c, d, a, in[3] + 0xc1bdceee, 22);
    MD5STEP(F1, a, b, c, d, in[4] + 0xf57c0faf, 7);
    MD5STEP(F1, d, a, b, c, in[5] + 0x4787c62a, 12);
    MD5STEP(F1, c, d, a, b, in[6] + 0xa8304613, 17);
    MD5STEP(F1, b, c, d, a, in[7] + 0xfd469501, 22);
    MD5STEP(F1, a, b, c, d, in[8] + 0x698098d8, 7);
    MD5STEP(F1, d, a, b, c, in[9] + 0x8b44f7af, 12);
    MD5STEP(F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
    MD5STEP(F1, b, c, d, a, in[11] + 0x895cd7be, 22);
    MD5STEP(F1, a, b, c, d, in[12] + 0x6b901122, 7);
    MD5STEP(F1, d, a, b, c, in[13] + 0xfd987193, 12);
    MD5STEP(F1, c, d, a, b, in[14] + 0xa679438e, 17);
    MD5STEP(F1, b, c, d, a, in[15] + 0x49b40821, 22);

    MD5STEP(F2, a, b, c, d, in[1] + 0xf61e2562, 5);
    MD5STEP(F2, d, a, b, c, in[6] + 0xc040b340, 9);
    MD5STEP(F2, c, d, a, b, in[11] + 0x265e5a51, 14);
    MD5STEP(F2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
    MD5STEP(F2, a, b, c, d, in[5] + 0xd62f105d, 5);
    MD5STEP(F2, d, a, b, c, in[10] + 0x02441453, 9);
    MD5STEP(F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
    MD5STEP(F2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
    MD5STEP(F2, a, b, c, d, in[9] + 0x21e1cde6, 5);
    MD5STEP(F2, d, a, b, c, in[14] + 0xc33707d6, 9);
    MD5STEP(F2, c, d, a, b, in[3] + 0xf4d50d87, 14);
    MD5STEP(F2, b, c, d, a, in[8] + 0x455a14ed, 20);
    MD5STEP(F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
    MD5STEP(F2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
    MD5STEP(F2, c, d, a, b, in[7] + 0x676f02d9, 14);
    MD5STEP(F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

    MD5STEP(F3, a, b, c, d, in[5] + 0xfffa3942, 4);
    MD5STEP(F3, d, a, b, c, in[8] + 0x8771f681, 11);
    MD5STEP(F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
    MD5STEP(F3, b, c, d, a, in[14] + 0xfde5380c, 23);
    MD5STEP(F3, a, b, c, d, in[1] + 0xa4beea44, 4);
    MD5STEP(F3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
    MD5STEP(F3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
    MD5STEP(F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
    MD5STEP(F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
    MD5STEP(F3, d, a, b, c, in[0] + 0xeaa127fa, 11);
    MD5STEP(F3, c, d, a, b, in[3] + 0xd4ef3085, 16);
    MD5STEP(F3, b, c, d, a, in[6] + 0x04881d05, 23);
    MD5STEP(F3, a, b, c, d, in[9] + 0xd9d4d039, 4);
    MD5STEP(F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
    MD5STEP(F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
    MD5STEP(F3, b, c, d, a, in[2] + 0xc4ac5665, 23);

    MD5STEP(F4, a, b, c, d, in[0] + 0xf4292244, 6);
    MD5STEP(F4, d, a, b, c, in[7] + 0x432aff97, 10);
    MD5STEP(F4, c, d, a, b, in[14] + 0xab9423a7, 15);
    MD5STEP(F4, b, c, d, a, in[5] + 0xfc93a039, 21);
    MD5STEP(F4, a, b, c, d, in[12] + 0x655b59c3, 6);
    MD5STEP(F4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
    MD5STEP(F4, c, d, a, b, in[10] + 0xffeff47d, 15);
    MD5STEP(F4, b, c, d, a, in[1] + 0x85845dd1, 21);
    MD5STEP(F4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
    MD5STEP(F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
    MD5STEP(F4, c, d, a, b, in[6] + 0xa3014314, 15);
    MD5STEP(F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
    MD5STEP(F4, a, b, c, d, in[4] + 0xf7537e82, 6);
    MD5STEP(F4, d, a, b, c, in[11] + 0xbd3af235, 10);
    MD5STEP(F4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
    MD5STEP(F4, b, c, d, a, in[9] + 0xeb86d391, 21);

    buf[0] += a;
    buf[1] += b;
    buf[2] += c;
    buf[3] += d;
}


void MD5Update(struct MD5Context *ctx, unsigned char const *buf, unsigned len)
{
    unsigned long t;

    /* Update bitcount */

    t = ctx->bits[0];
    if ((ctx->bits[0] = t + ((unsigned long) len << 3)) < t)
	ctx->bits[1]++;		/* Carry from low to high */
    ctx->bits[1] += len >> 29;

    t = (t >> 3) & 0x3f;	/* Bytes already in shsInfo->data */

    /* Handle any leading odd-sized chunks */

    if (t)
    {
	    unsigned char *p = (unsigned char *) ctx->in + t;

    	t = 64 - t;
	    if (len < t)
        {
	        memcpy(p, buf, len);
    	    return;
	    }

    	memcpy(p, buf, t);
	    byteReverse(ctx->in, 16);
    	MD5Transform(ctx->buf, (unsigned long *) ctx->in);
	    buf += t;
    	len -= t;
    }
    /* Process data in 64-byte chunks */

    while (len >= 64)
    {
	    memcpy(ctx->in, buf, 64);
    	byteReverse(ctx->in, 16);
	    MD5Transform(ctx->buf, (unsigned long *) ctx->in);
    	buf += 64;
	    len -= 64;
    }

    /* Handle any remaining bytes of data. */

    memcpy(ctx->in, buf, len);
}

void MD5Final(unsigned char digest[16], struct MD5Context *ctx)
{
    unsigned count;
    unsigned char *p;

    /* Compute number of bytes mod 64 */
    count = (ctx->bits[0] >> 3) & 0x3F;

    /* Set the first char of padding to 0x80.  This is safe since there is
       always at least one byte free */
    p = ctx->in + count;
    *p++ = 0x80;

    /* Bytes of padding needed to make 64 bytes */
    count = 64 - 1 - count;

    /* Pad out to 56 mod 64 */
    if (count < 8)
    {
	    /* Two lots of padding:  Pad the first block to 64 bytes */
    	memset(p, 0, count);
	    byteReverse(ctx->in, 16);
    	MD5Transform(ctx->buf, (unsigned long *) ctx->in);

    	/* Now fill the next block with 56 bytes */
	    memset(ctx->in, 0, 56);
    }
    else
    {
	    /* Pad block to 56 bytes */
	    memset(p, 0, count - 8);
    }
    byteReverse(ctx->in, 14);

    /* Append length in bits and transform */
    ((unsigned long *) ctx->in)[14] = ctx->bits[0];
    ((unsigned long *) ctx->in)[15] = ctx->bits[1];

    MD5Transform(ctx->buf, (unsigned long *) ctx->in);
    byteReverse((unsigned char *) ctx->buf, 4);
    memcpy(digest, ctx->buf, 16);
    memset(ctx, 0, sizeof(ctx));	/* In case it's sensitive */
}

void md5_vector(size_t num_elem, unsigned char *addr[], size_t *len, unsigned char *mac)
{
	MD5_CTX ctx;
	size_t i;

	MD5Init(&ctx);
	for (i = 0; i < num_elem; i++)
		MD5Update(&ctx, addr[i], len[i]);

	MD5Final(mac, &ctx);
}

/* Digests a string and prints the result.
 */
char* MDString (char *string) 
{
	MD5_CTX context;
	unsigned char digest[16];
	char output1[32];
	static  char output[33]={""};
	unsigned int len = strlen (string);
	int i;
	MD5Init (&context);
	MD5Update (&context, (unsigned char*)string, len);
	MD5Final (digest, &context);

	for (i = 0; i < 16; i++)
	{
		sprintf(&(output1[2*i]),"%02x",(unsigned char)digest[i]);
		sprintf(&(output1[2*i+1]),"%02x",(unsigned char)(digest[i]<<4));
	}
	for(i=0;i<32;i++)
		output[i]=output1[i];
	return output;
}


/* Digests a file and prints the result.
*/
char* MDFile (char *filename)
{ 
	static char output[33]={""};
	FILE *file;
	MD5_CTX context;
	int len;
	unsigned char buffer[1024], digest[16];
	int i;
	char output1[32];
	if ((file = fopen (filename, "rb")) == NULL)
	{ 
		printf ("%s can't be openedn", filename);
		return 0;
	}
	else {
		MD5Init (&context);

		len = fread (buffer, 1, 1024, file);
		while (len>0)
		{
			MD5Update (&context, buffer, len);
			len = fread (buffer, 1, 1024, file);
		}
		MD5Final (digest, &context);
		fclose (file);
		for (i = 0; i < 16; i++)
		{
			sprintf(&(output1[2*i]),"%02x",(unsigned char)digest[i]);
			sprintf(&(output1[2*i+1]),"%02x",(unsigned char)(digest[i]<<4));
		}
		for(i=0;i<32;i++)
			output[i]=output1[i];
		return output;
	}
}


char* hmac_md5(char* text,char* key)
{
	char digest[16];
	char output1[32];
	static char output[33]={""};
	MD5_CTX context;
	unsigned char k_ipad[65]; /* inner padding -
	* key XORd with ipad
	*/
	unsigned char k_opad[65]; /* outer padding -
	* key XORd with opad
	*/
	unsigned char tk[16];
	int i;
	int text_len = strlen (text);
	int key_len=strlen(key);
	/* if key is longer than 64 bytes reset it to key=MD5(key) */
	if (key_len > 64) {

		MD5_CTX tctx;

		MD5Init(&tctx);
		MD5Update(&tctx,(unsigned char*) key, key_len);
		MD5Final(tk, &tctx);

		key = (char*)tk;
		key_len = 16;
	}

	/*
	* the HMAC_MD5 transform looks like:
	*
	* MD5(K XOR opad, MD5(K XOR ipad, text))
	*
	* where K is an n byte key
	* ipad is the byte 0x36 repeated 64 times
	* opad is the byte 0x5c repeated 64 times
	* and text is the data being protected
	*/

	/* start out by storing key in pads */

	/*bzero( k_ipad, sizeof k_ipad);
	bzero( k_opad, sizeof k_opad);
	*/

	for(i=0;i<65;i++)
		k_ipad[i]=(unsigned char)0;
	for(i=0;i<65;i++)
		k_opad[i]=(unsigned char)0;

	/*bcopy( key, k_ipad, key_len);
	bcopy( key, k_opad, key_len);
	*/
	for(i=0;i<key_len;i++)
	{
		k_ipad[i]=(unsigned char)key[i];
		k_opad[i]=(unsigned char)key[i];
	}

	/* XOR key with ipad and opad values */
	for (i=0; i<64; i++) {
		k_ipad[i] ^= 0x36;
		k_opad[i] ^= 0x5c;
	}
	/*
	* perform inner MD5
	*/
	MD5Init(&context); /* init context for 1st
	* pass */
	MD5Update(&context, k_ipad, 64); /* start with inner pad */
	MD5Update(&context, (unsigned char*)text, text_len); /* then text of datagram

	*/
	MD5Final((unsigned char*)digest, &context); /* finish up 1st pass */
	/*
	* perform outer MD5
	*/
	MD5Init(&context); /* init context for 2nd
	* pass */
	MD5Update(&context, k_opad, 64); /* start with outer pad */
	MD5Update(&context,(unsigned char*) digest, 16); /* then results of 1st
	* hash */
	MD5Final((unsigned char*)digest, &context); /* finish up 2nd pass */
	for (i = 0; i < 16; i++)
	{
		sprintf(&(output1[2*i]),"%02x",(unsigned char)digest[i]);
		sprintf(&(output1[2*i+1]),"%02x",(unsigned char)(digest[i]<<4));
	}
	
	for(i=0;i<32;i++)
		output[i]=output1[i];
	return output;
}

