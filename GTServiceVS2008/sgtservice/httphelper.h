////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	httphelper   version:  1.0   ? date: 04/30/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 - All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef HTTPHELPER_H
#define	HTTPHELPER_H
//////////////////////////////////////////////////////////////////////////
#include <WinInet.h>
#include <ShlObj.h>
#include <atlstr.h>
#include <atlfile.h>
#include "../utility/utility.h"
using namespace ATL;

#include <map>
using namespace std;

#pragma comment(lib,"Wininet.lib")
/************************************************************************/
/* loader implement  InternetAPI
*/
/************************************************************************/
class CHttpClient
{
public:
	/**
	*
	*/
	static CHttpClient* GetInstance(LPCTSTR name=_T("hwinet"))
	{
		CHttpClient* pclient=0;
		map<CString,CHttpClient*>::iterator iter = s_clients.find(name);
		if (iter == s_clients.end())
		{
			pclient = new CHttpClient;
			if(pclient->Initialize())
			{
				s_clients[name] = pclient;
			}
		}
		else
		{
			pclient= iter->second;
		}
		return pclient;
	}
	/**
	*
	*/
	static void Release()
	{
		map<CString,CHttpClient*>::iterator iter = s_clients.begin();
		while (iter != s_clients.end())
		{
			delete iter->second;
			iter++;
		}
	}
public:
	/**
	*
	*/
	BOOL Initialize()
	{
		ATLASSERT(!m_hinet);
		//INTERNET_OPEN_TYPE_DIRECT
		//INTERNET_OPEN_TYPE_PRECONFIG
		m_hinet =InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
		
		ATLASSERT(m_hinet&&_T("internetopen error"));

		return m_hinet != 0;
	}
	/**
	*
	*/
	~CHttpClient()
	{
		if (m_hinet)
		{
			InternetCloseHandle(m_hinet);
			m_hinet = 0;
		}
	}
	/**
	*
	*/
	CString GetUserName()
	{
		return m_username;
	}
	/**
	*
	*/
	CString GetPassword()
	{
		return m_pwd;
	}
	/**
	*
	*/
	CString GetBaseUrl()
	{
		return m_base;
	}
	/**
	*
	*/
	LPVOID GetSafeHandle()
	{
		return m_hinet;
	}
protected:
	/**
	*
	*/
	CHttpClient(LPCTSTR url=0,LPCTSTR username=0,LPCTSTR pwd=0):m_base(url),
		m_username(username),
		m_pwd(pwd),
		m_hinet(0)
	{
	}
protected:
	CString				m_base;
	CString				m_username;
	CString				m_pwd;
	LPVOID				m_hinet;
	static	map<CString,CHttpClient*> s_clients;
};

map<CString,CHttpClient*> CHttpClient::s_clients;
/************************************************************************
 * 
 *
 *
/************************************************************************/
class CHttpFile
{
public:
	/**
	*
	*/
	CHttpFile():m_hinet(0),m_http(0)
	{
	}
	/**
	*
	*/
	BOOL Open(LPCTSTR path)
	{
		Close();
		if (!m_hinet)
		{
			m_hinet =InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
		}
		m_base = path;
		//http header
		CString headstr = _T("Accept: */*\r\n\r\n");
		DWORD flag =INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_RELOAD;
		m_base= path;
		m_base.Replace(_T("\\"),_T("/"));

		m_http = InternetOpenUrl(m_hinet,m_base,headstr,headstr.GetLength(),flag,NULL);
		
		return m_http != NULL;
	}
	/**
	*
	*/
	~CHttpFile()
	{
		Close();
	}
	/**
	*
	*/
	BOOL Read(LPVOID buf,const DWORD len,DWORD* pdwRead)
	{
		BOOL res = FALSE;
		
		res = InternetReadFile(m_http,buf,len,pdwRead);
		DWORD er=0;
		if (!res)
		{
			er = GetLastError();
			
		}
		return res ;
	}
	/**
	*
	*/
	BOOL Write(LPVOID buf,const DWORD len)
	{
		BOOL res = FALSE;

		DWORD dwSize=0;
		res = InternetWriteFile(m_http,buf,len,&dwSize);

		return res ;
	}
	/**
	*
	*/
	BOOL Seek(LONG lOffset, UINT nFrom)
	{
		return (INVALID_SET_FILE_POINTER  != InternetSetFilePointer(m_http,lOffset,0,nFrom,0));
	}
	/**
	*
	*/
	BOOL GetPosition(DWORD& curPos)
	{
		//InternetSetFilePointer()
		DWORD pos = InternetSetFilePointer(m_http,0,0,FILE_CURRENT,0);
		if (pos == INVALID_SET_FILE_POINTER)
		{
			return FALSE;
		}
		curPos = pos;

		return TRUE;
	}
	/**
	*
	*/
	DWORD GetSize()
	{
		DWORD dwsize=0;
		//
		BYTE *pbuffer=0;
		DWORD   dwLen=0; 
		DWORD   dwIndex=0;
		if(!HttpQueryInfo(m_http,HTTP_QUERY_CONTENT_LENGTH,pbuffer,&dwLen,&dwIndex))
		{
			pbuffer = new BYTE[dwLen];
			HttpQueryInfo(m_http,HTTP_QUERY_CONTENT_LENGTH,pbuffer,&dwLen,&dwIndex);
			
			is_numeric((LPCTSTR)pbuffer,&dwsize);
			delete pbuffer;
		}
		return dwsize;
	}
	/**
	*
	*/
	BOOL Close()
	{
		BOOL res = TRUE;
		if (m_http)
		{
			res = InternetCloseHandle(m_http);
			m_http = 0;
		}
		return res;
	}
	/**
	*
	*/
	LPVOID GetSafeHandle()
	{
		return m_http;
	}
	/**
	*
	*/
	CString GetName() const 
	{
		return m_base; 
	}
protected:
	CString				m_base;
	LPVOID				m_hinet;
	LPVOID				m_http;
};
#endif