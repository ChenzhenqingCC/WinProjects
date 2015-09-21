////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	my_utility   version:  1.0   ? date: 09/30/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 ztgame Corporation- All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef MYUTILITY_H
#define MYUTILITY_H
//////////////////////////////////////////////////////////////////////////
#pragma  warning (disable:4996)
#include <atlbase.h>
#include <atlapp.h>
#include <atlstr.h>
#include <vector>
#include <map>
#include <string>
using namespace std;
typedef basic_string<TCHAR> tstring;
#include <boost/algorithm/string.hpp>
/////////////////////////////////////////////////////////////////////////////////////////////
#define  STRSAFE_NO_DEPRECATE
#define	STRSAFE_LIB
#include <strsafe.h>
#pragma comment(lib,"strsafe.lib")
/////////////////////////////////////////////////////////////////////////////////////////////
/**
*	for copy the old str array(char[]) format with str pointer
*/
#define  SafeStrCpy(tararray,lpstr) StringCbCopy(tararray,_countof(tararray),lpstr)

#define  SafeStrCpyA(tararray,lpstr) StringCbCopyA(tararray,_countof(tararray),lpstr)

#define  SafeStrCpyW(tararray,lpstr) StringCbCopyW(tararray,_countof(tararray),lpstr)
/**
*	for copy str a str buf but not an array,tarlen should means the buf size ,some likesizeof(tar)/sizeof(TCHAR)
*/
#define  SafeStrCpy2(tarpointer,tarlen,lpstr) StringCbCopyN(tarpointer,tarlen,lpstr,lstrlen(lpstr))

#define  SafeStrCpy2A(tarpointer,tarlen,lpstr) StringCbCopyNA(tarpointer,tarlen,lpstr,lstrlen(lpstr))

#define  SafeStrCpy2W(tarpointer,tarlen,lpstr) StringCbCopyNW(tarpointer,tarlen,lpstr,lstrlen(lpstr))
/**
*	parse the tstring options,return in a vector
*/
inline vector<CString> ParseCmdLine(LPCTSTR pcmdline,LPCTSTR filter=_T(" "))
{
	vector<CString> res_vec;
	//split tstring into vector
	vector<tstring> pws;
	boost::split(pws,pcmdline,boost::is_any_of(filter));

	for(size_t i=0;i<pws.size();i++)
	{
		if (pws[i].length())
		{
			res_vec.push_back(pws[i].c_str());
			//OutputDebugString(pws[i].c_str());
		}
	}
	return res_vec;
}
/**
*	return current process path
*/
inline CString GetLocalPath(HMODULE hm=0)
{
	CString m_path;
	TCHAR buffer[MAX_PATH]={0};
	if (::GetModuleFileName(hm,buffer,MAX_PATH))
	{
		m_path = buffer;
		int dist = m_path.ReverseFind(_T('\\'));
		if (dist > -1)
		{
			m_path.Delete(dist,m_path.GetLength()-dist);
		}
	}
	return m_path;
}
/**
*	return current process short path
*/
inline CString GetShortPath(LPCTSTR path)
{
	CString m_path = path;
	long     length = 0;
	TCHAR*   buffer = NULL;
	// First obtain the size needed by passing NULL and 0.
	length = GetShortPathName(m_path, NULL, 0);
	if (length == 0)
	{
		return m_path;
	}
	buffer = new TCHAR[length];
	length = GetShortPathName(m_path, buffer, length);
	if (length == 0)
	{
		delete [] buffer;
		return m_path;
	}
	m_path = buffer;
	delete [] buffer;
	return m_path;
}
/**
*	return current process name
*/
inline CString GetName(HMODULE hm=0)
{
	CString m_path;
	TCHAR buffer[MAX_PATH]={0};
	if (::GetModuleFileName(hm,buffer,MAX_PATH))
	{
		m_path = buffer;
		int dist = m_path.ReverseFind(_T('\\'));
		if (dist > -1)
		{
			m_path.Delete(0,dist);
		}
	}
	return m_path;
}
#ifdef UNICODE	
#else
#endif

typedef basic_string<TCHAR> tstring;

#include <map>
using namespace std;
//my wrapper fun for error id to string description by call windows api or get my errormsg form my res dll
inline basic_string<TCHAR>  ReturnErrorMsg(DWORD errorid,LPCVOID emsgdll = 0,\
										   DWORD languageid = MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US))
{
	HLOCAL hlocal = NULL;
	basic_string<TCHAR>  msg;
	int bsc = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER,emsgdll,\
		errorid,languageid,(PTSTR)&hlocal,0,0);
	if(bsc > 0)
	{
		if(hlocal)
		{
			msg = (LPTSTR)LocalLock(hlocal);
			LocalFree(hlocal);
		}
	}
	else
	{
		msg = ReturnErrorMsg(GetLastError());
	}
	return msg;
}
#endif