////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	stringfun   version:  1.0   ? date: 08/20/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 dasjack- All Rights Reserved
//	Written By Jack Liu
//	created:	15:9:2009   15:59
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef STRINGFUN_H
#define STRINGFUN_H
#pragma  warning (disable:4996)
#include <atlbase.h>
#include <atlapp.h>
#include <atlstr.h>

#include <vector>
#include <map>
using namespace std;
#include <boost/algorithm/string.hpp>
/////////////////////////////////////////////////////////////////////////////////////////////
#define  STRSAFE_NO_DEPRECATE
#define	STRSAFE_LIB
#include <strsafe.h>
#pragma comment(lib,"strsafe.lib")
/////////////////////////////////////////////////////////////////////////////////////////////
#ifdef UNICODE
#define tstring wstring
#else
#define tstring string
#endif
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
inline vector<CString> split_to_array(LPCTSTR pcmdline,LPCTSTR filter=_T(" "))
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
		}
	}
	return res_vec;
}
/**
*	parse the tstring options,return in a map
*/
inline map<CString,CString> split_to_map(LPCTSTR pcmdline,LPCTSTR filter=_T("&"),LPCTSTR sub_filter=_T("="))
{
	map<CString,CString> res_map;
	//split tstring into vector
	vector<tstring> pws;
	boost::split(pws,pcmdline,boost::is_any_of(filter));
	for(size_t i=0;i<pws.size();i++)
	{
		if (pws[i].length())
		{
			vector<CString> f2v = split_to_array(pws[i].c_str(),sub_filter);
			if(f2v.size()>1) res_map[f2v[0]] = f2v[1];
		}
	}
	return res_map;
}
/**
 *	replace string use other string
 */
inline void string_replace(tstring& strBig, const tstring& strsrc, const tstring& strdst) 
{
	tstring::size_type pos=0;
	tstring::size_type srclen=strsrc.size();
	tstring::size_type dstlen=strdst.size();
	while( (pos=strBig.find(strsrc, pos)) != tstring::npos)
	{
		strBig.replace(pos, srclen, strdst);
		pos += dstlen;
	}
}
/**
*do some works on the string
*fun is like : void fun(tstring::reference_type c);
*/
template<class char_fun>void string_transform(tstring& str_src,char_fun& fun)
{
	transform(str_src.begin(),str_src.end(),str_src.begin(),fun);
}
//////////////////////////////////////////////////////////////////////////
inline basic_string<wchar_t> MB2UN(const char* pmb)
{
	basic_string<wchar_t> wstr;
	int bytesneeded = ::MultiByteToWideChar(CP_ACP, 0, pmb, strlen(pmb) +1 ,0, 0);
	wchar_t  *revser =0;
	try{
		revser= new wchar_t[bytesneeded];
		wstr.resize(bytesneeded);
		bytesneeded = MultiByteToWideChar(CP_ACP, 0, pmb, strlen(pmb) +1,revser, bytesneeded);
		wstr=revser;
		delete [] revser;
	}catch(const exception &e)
	{
		e;
	}
	return wstr;
};
//////////////////////////////////////////////////////////////////////////
inline basic_string<char> UN2MB(const wchar_t* pw)
{
	basic_string<char> mbstr;
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
};
#endif