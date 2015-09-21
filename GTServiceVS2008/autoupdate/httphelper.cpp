////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	httphelper   version:  1.0   ? date: 09/29/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 ztgame Corporation- All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "httphelper.h"
#include <WinInet.h>
#include <ShlObj.h>
#include <atlmisc.h>

#include <list>
using namespace std;

Http_Loader::Http_Loader(const string& url,const string& username,
						const string& pwd):m_base(url),m_username(username),
						m_pwd(pwd),m_hinet(0),m_http(0)
{
	Init();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Http_Loader::Init()
{
	m_hinet = InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
	if (!m_hinet)
	{
		throw exception(_T("internet open error."));
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Http_Loader::~Http_Loader()
{
	if (m_hinet)
	{
		InternetCloseHandle(m_hinet);
		m_hinet = 0;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Http_Loader::Read(const string& srcfile,char *buf,const DWORD len,const DWORD start)
{
	if(!OpenUrl(srcfile)) return 0;
	
	DWORD dwSize=0;
	InternetReadFile(m_http,buf,len,&dwSize);
	CloseUrl();
	return dwSize ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Http_Loader::Write(const string& srcfile,const char*buf,const DWORD len)
{
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool  Http_Loader::OpenUrl(const string& srcfile)
{
	//http header
	string headstr = _T("Accept: */*\r\n\r\n");
	DWORD flag =INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_RELOAD;
	string srcpath= m_base +srcfile;
	m_http = InternetOpenUrl(m_hinet,srcpath.c_str(),headstr.c_str(),headstr.length(),flag,NULL);
	return m_http != 0;
	//if (!m_http )
	//{
	//	throw exception(_T("http open error."));
	//}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void  Http_Loader::CloseUrl()
{
	if (m_http)
	{
		InternetCloseHandle(m_http);
		m_http = 0;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FILE * Http_Loader::CreateFile(const string &targfile)
{
	FILE *pfile =0;
	string curpath = targfile;
	size_t pos = curpath.find_last_of(_T("\\"));
	if (pos != string::npos)
	{
		//create new dir
		string dir =curpath.substr(0,pos);
		if (!PathFileExists(dir.c_str()))
		{
			SHCreateDirectoryEx(0,dir.c_str(),0);
		}
	}
	
	pfile = fopen(targfile.c_str(),_T("wb"));
	return pfile;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Http_Loader::DownLoad(const string& srcfile,const string &targfile)
{
	if(!OpenUrl(srcfile)) return false;

	FILE *pfile = CreateFile(targfile);
	if (!pfile)
	{
		throw exception(_T("local file create errro."));
	}
	//////////////////////////////////////////////////////////////////////////
	DWORD dwSize =0;
	//why use like this?
	//void *sttmp[4096]={0};
	byte sttmp[4096]={0};
	do 
	{
		if (!InternetReadFile(m_http,sttmp,4095,&dwSize))
		{
			fclose(pfile);
			throw exception(_T("write local file error."));
		}
		if (!dwSize)
		{
			break;
		}
		else
		{
			fwrite(sttmp,sizeof(char),dwSize,pfile);
		}
	} while(true);
	//////////////////////////////////////////////////////////////////////////
	CloseUrl();
	
	return fclose(pfile) ==0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

