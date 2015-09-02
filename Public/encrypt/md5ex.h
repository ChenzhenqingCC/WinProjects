#ifndef _MD5EX_H
#define _MD5EX_H
#include <string>
//class
bool MD5Data(const void* pData, size_t size, unsigned char *pMD5 /* 16 Byte*/);
//bool MD5Stream(Stream * pStream, unsigned char *pMD5 /* 16 Byte*/);
//bool MD5File(const char* pszFile, unsigned char *pMD5 /* 16 Byte*/);
bool MD5String(const char* string, unsigned char *pMD5 /* 16 Byte*/);
//std::string MD5StringBase16(const char* string, unsigned char *pMD5 /* 16 Byte*/);
#endif

