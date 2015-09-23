////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	httphelper   version:  1.0   ? date: 04/30/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 - All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AJAXHELPER_H
#define	AJAXHELPER_H
//////////////////////////////////////////////////////////////////////////
#include <WinInet.h>
#pragma comment(lib,"wininet.lib")
#import   "msxml3.dll" 
//#import   "msxml6.dll" 
#include <atlwfile.h>
using   namespace   MSXML2;

class CAjaxRequest
{
public:
	/**
	*
	*/
	CString Request(const CString& path,const CString& verb=_T("GET"), const CString& data=_T(""))
	{
		CString response;
		try{
			if (!m_ajax)
			{
				//CreateInstance("Msxml2.XMLHTTP.6.0");
				m_ajax.CreateInstance(__uuidof(XMLHTTP));
			}
			m_ajax->abort();
			//set headers
			m_ajax->open(_bstr_t(verb), _bstr_t(path),_variant_t(VARIANT_FALSE));
			m_ajax->setRequestHeader(_T("Accept"),_T("*/*"));
			//m_ajax->setRequestHeader(_T("Connection"),_T("close"));
			m_ajax->setRequestHeader(_T("User-Agent"),_T("dasjack"));
			m_ajax->setRequestHeader(_T("Accept-Charset"),_T("GB2312,utf-8;q=0.7,*;q=0.7"));
			if (verb ==_T("POST") && data.GetLength())
			{
				m_ajax->setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
			}
			//set body
			CComVariant vtBody = data;
			m_ajax->send(vtBody);
			CComBSTR rtext;
			HRESULT hr = m_ajax->get_responseText(&rtext);
			response = rtext;
			m_ajax->abort();
		}catch(_com_error e)
		{
		}
		return response;
	}
	/**
	*
	*/
	static BOOL DownLoad(const CString& srcpath,const CString& targpath)
	{
		LPVOID			m_hinet=0;
		m_hinet = InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
		if (!m_hinet)
		{
			return FALSE;
		}
		LPVOID			m_http=0;
		CString headstr = _T("Accept: */*\r\n\r\n");
		DWORD flag =INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_RELOAD;
		
		m_http = InternetOpenUrl(m_hinet,srcpath,headstr,headstr.GetLength(),flag,NULL);
		if(!m_http)
		{
			return FALSE;
		}
		
		if (!PathFileExists(targpath))
		{
			SHCreateDirectoryEx(0,targpath,0);
		}
		CString fname = srcpath;
		int pos = fname.ReverseFind(_T('/'));
		if (pos != -1)
		{
			fname.Delete(0,pos+1);
		}
		CFile file;
		file.Create(targpath+fname);
		byte sttmp[4096]={0};
		DWORD dwSize =0;
		do 
		{
			if (!InternetReadFile(m_http,sttmp,4095,&dwSize))
			{
				return FALSE;
			}
			if (!dwSize)
			{
				break;
			}
			else
			{
				file.Write(sttmp,dwSize);
			}
		} while(true);
		InternetCloseHandle(m_http);
		return TRUE;
	}
	/**
	*
	*/
	BOOL MakeUrlInfo(const CString& path,URL_COMPONENTS& m_urlComp)
	{
		CString m_path = path;
		m_path.Replace(_T("\\"),_T("/"));
		// Initialize the URL_COMPONENTS structure.
		ZeroMemory(&m_urlComp, sizeof(m_urlComp));
		m_urlComp.dwStructSize = sizeof(m_urlComp);
		// Set required component lengths to non-zero 
		// so that they are cracked.
		m_urlComp.dwSchemeLength    = (DWORD)-1;
		m_urlComp.dwHostNameLength  = (DWORD)-1;
		m_urlComp.dwUrlPathLength   = (DWORD)-1;
		m_urlComp.dwExtraInfoLength = (DWORD)-1;
		return ::InternetCrackUrl(m_path,m_path.GetLength(), 0, &m_urlComp);
	}
	
protected:
	IXMLHTTPRequestPtr m_ajax;
};

#endif