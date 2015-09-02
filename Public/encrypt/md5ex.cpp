#include "MD5.h"
#include "md5ex.h"
#include <stdio.h>
#include <string.h>

bool MD5Data(const void* pData, size_t size, unsigned char *pMD5 /* 16 Byte*/)
{
	MD5_CTX context;
	MD5Init(&context);
	MD5Update(&context, (unsigned char*)pData, size);
	MD5Final(pMD5, &context);
	return true;
}

bool MD5String(const char* string, unsigned char *pMD5 /* 16 Byte*/)
{
	size_t size = strlen(string);
	return MD5Data(string, size, pMD5);
}
