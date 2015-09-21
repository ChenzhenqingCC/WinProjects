#ifndef UNICODEMULTI_H
#define UNICODEMULTI_H
#include <atlstr.h>
#include <exception>
using namespace std;
#pragma warning (disable : 4267)
/****************************************************************************************************************************/
/* 
		MBCS to UNICODE
 *
 *
*/
/****************************************************************************************************************************/
struct MB2UN
{
	CStringW operator()(const char* pmb)
	{
		CStringW wstr;
		int bytesneeded = MultiByteToWideChar(CP_ACP, 0, pmb, strlen(pmb) +1 ,0, 0);
		wchar_t  *revser =0;
		try{
			revser= new wchar_t[bytesneeded];
			bytesneeded = MultiByteToWideChar(CP_ACP, 0, pmb, strlen(pmb) +1,revser, bytesneeded);
			wstr=revser;
			delete [] revser;
		}catch(const exception &e)
		{
			e;
		}
		return wstr;
	}
};
//wrapper function
inline CStringW mb2un(const char* pc)
{
	return MB2UN()(pc);
}
/****************************************************************************************************************************/
/* 
	UNICODE to MBCS
 *
 *
*/
/****************************************************************************************************************************/
struct UN2MB
{
	CStringA operator()(const wchar_t* pw)
	{
		CStringA mbstr;
		char *revser = 0;
		int bytesneeded = WideCharToMultiByte(CP_ACP, 0, pw, -1,0, 0, NULL, NULL );
		try{
			revser = new char[bytesneeded];
			bytesneeded = WideCharToMultiByte(CP_ACP, 0, pw, -1,revser, bytesneeded, NULL, NULL );
			mbstr = revser;
			delete []revser;
		}catch(const exception &e)
		{
			e;
		}
		return mbstr;
	}
};
//wrapper function
inline CStringA un2mb(const wchar_t* pw)
{
	return UN2MB()(pw);
}
/****************************************************************************************************************************/
/* 
	UTF8 to UNICODE
 *
 *
*/
/****************************************************************************************************************************/
struct UTF82UN
{
	CStringW operator()(const char* pmb)
	{
		CStringW wstr;
		int bytesneeded = ::MultiByteToWideChar(CP_UTF8, 0, pmb, strlen(pmb) +1 ,0, 0);
		wchar_t  *revser =0;
		try{
			revser= new wchar_t[bytesneeded];
			bytesneeded = MultiByteToWideChar(CP_UTF8, 0, pmb, strlen(pmb) +1,revser, bytesneeded);
			wstr=revser;
			delete [] revser;
		}catch(const exception &e)
		{
			e;
		}
		return wstr;
	}
};
//wrapper function
inline CStringW utf2un(const char* pmb)
{
	return UTF82UN()(pmb);
}
/****************************************************************************************************************************/
/* 
	UNICODE to UTF8
 *
 *
*/
/****************************************************************************************************************************/
struct UN2UTF8
{
	CStringA operator()(const wchar_t* pw)
	{
		CStringA mbstr;
		char *revser = 0;
		int bytesneeded = WideCharToMultiByte(CP_UTF8, 0, pw, -1,0, 0, NULL, NULL );
		try{
			revser = new char[bytesneeded];
			bytesneeded = WideCharToMultiByte(CP_UTF8, 0, pw, -1,revser, bytesneeded, NULL, NULL );
			mbstr = revser;
			delete []revser;
		}catch(const exception &e)
		{
			e;
		}
		return mbstr;
	}
};
//wrapper function
inline CStringA un2utf(const wchar_t* pw)
{
	return UN2UTF8()(pw);
}

#endif