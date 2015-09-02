////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	gtservice   version:  1.0   ? date: 1/15/2013
//  -------------------------------------------------------------
// 
//
//
//  -------------------------------------------------------------
//	Copyright (C) 2013 dasjack- All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>
using namespace std;
#include "utility.h"
#include <boost/smart_ptr.hpp>
#include "StringConv.h"
#include "WebDispWindow.h"
//#include <utility/ajaxhelper.h>
//////////////////////////////////////////////////////////////////////////////////////////
class CWorkItem;
class CMainDlg;
//class CPopPanel;
/**
*
*/
//extern CString g_baseUrl;

inline CString GetOSVersion()
{
	OSVERSIONINFOEX osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO*) &osvi);
	CString ver;
	ver.Format(_T("%d.%d.%d.%d"), osvi.dwMajorVersion,osvi.dwMinorVersion,osvi.wServicePackMajor,osvi.wServicePackMinor);
	return ver;
}
inline CString SendErrorUrl(CString type, CString msg)
{
	//CAjaxRequest ajax;
	CString url = CString(_T("/error?"));
	url += _T("type=");
	url += type;
	url +="&";
	url += _T("os=");
	url += GetOSVersion();
	url += _T("&");
	url += _T("msg=");
	url += msg;
	//CString res = ajax.Request(url);
	extern CString baseip;
	extern int baseport;
	CString res;
	std::string hres = HttpRequest(baseip, baseport, url, _T("GET"), NULL, 0);
	//res.Format(_T("%s"), hres.c_str());
	res = ANSIToUniCode(hres.c_str());
	return res;
}

inline BOOL WriteReg(CString Key, CString KeyValue)
{
	CRegKey reg;
	long res = reg.Create(HKEY_CURRENT_USER,_T("SOFTWARE\\GtCenter"));
	if(res != ERROR_SUCCESS)
	{
		CString msg(_T(""));
		msg.Format(_T("%s,%d"), _T("CRegKey::Create"), GetLastError());
		SendErrorUrl(_T("regread"),msg);
		return FALSE;
	}
	res = reg.SetStringValue(Key,KeyValue);
	if (res != ERROR_SUCCESS)
	{
		CString msg(_T(""));
		msg.Format(_T("%s,%d"), _T("CRegKey::SetStringValue"), GetLastError());
		SendErrorUrl(_T("regwrite"),msg);
		return FALSE;
	}
	return TRUE;
}
inline CString ReadReg(CString key)
{
	CRegKey reg;
	long res = reg.Create(HKEY_CURRENT_USER,_T("SOFTWARE\\GtCenter"));
	if(res != ERROR_SUCCESS)
	{
		CString msg(_T(""));
		msg.Format(_T("%s,%d"), _T("CRegKey::Create"), GetLastError());
		SendErrorUrl(_T("regread"),msg);
		return _T("");
	}
	//key.GetKeySecurity();
	TCHAR strid[256]={0};
	DWORD szid=256;
	res = reg.QueryStringValue(key,strid,&szid);
	if (res != ERROR_SUCCESS)
	{
		return _T("");
	}
	return strid;
}
inline BOOL IsXP()
{
	//return TRUE;//test
	OSVERSIONINFOEX osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO*) &osvi);
	return osvi.dwMajorVersion < 6 ;
}
/**
*
*/
inline CString GetSysFolderPath(int fid)
{
	CString path;
	LPITEMIDLIST pidl=0;
	HRESULT hr = SHGetFolderLocation(NULL,fid,NULL,NULL,&pidl);
	if (pidl)
	{
		TCHAR fpath[MAX_PATH]={0};
		if(SHGetPathFromIDList(pidl,fpath))
		{
			TCHAR ffpath[MAX_PATH]={0};
			GetShortPathName(fpath,ffpath,MAX_PATH);
			path = ffpath;
		}
	}
	return path;
}
/**
*create a link for app
pszPathObj:目标文件
pszParam:运行参数（一般为NULL）
pszPath:运行目录
pszPathLink:快捷方式文件名（eg:"c:\\temp\\test.lnk")
pszDesc:快捷方式名称
*/
inline HRESULT CreateShortcut(LPCTSTR pszPathObj,LPCTSTR pszPathLink,LPCTSTR pszParam,\
							  LPCTSTR pszPath,LPCTSTR pszDesc)
{
	CoInitialize(NULL);
	IShellLink* psl=0;

	HRESULT hres = (HRESULT)CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,IID_IShellLink,(void**)&psl) ;
	if( SUCCEEDED(hres))
	{
		// set the path to the shortcut target, and add the description
		psl -> SetPath(pszPathObj);
		psl -> SetArguments(pszParam) ;
		psl -> SetDescription(pszDesc);
		psl -> SetWorkingDirectory(pszPath);

		// query IShellLink for the IPersistFile interface for saving the shortcut in persistent storage
		IPersistFile* ppf=0 ;
		hres = (HRESULT)(psl -> QueryInterface( IID_IPersistFile, (void**)&ppf)) ;
		if( SUCCEEDED( hres))
		{
			// ensure that that string is ANSI
			hres = ppf -> Save((LPWSTR)pszPathLink, STGM_READWRITE) ;
			if (FAILED(hres))
			{
				CString msg;
				msg.Format(_T("%s,%d"), _T("IPersistFile::Save"),GetLastError());
				SendErrorUrl(_T("install"), msg);
			}
		}
		else
		{
			CString msg;
			msg.Format(_T("%s,%d"), _T("IShellLink::QueryInterface::IID_IPersistFile"),GetLastError());
			SendErrorUrl(_T("install"), msg);
		}
		// release the IPersistFile interface
		ppf -> Release() ;
		// release the IShellLink interface
		psl -> Release() ;
	}
	else
	{
		CString msg;
		msg.Format(_T("%s,%d"), _T("CoCreateInstance"),GetLastError());
		SendErrorUrl(_T("install"), msg);
	}
	CoUninitialize();
	return hres ;
}
/**
*
*/
void CALLBACK OnTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
//////////////////////////////////////////////////////////////////////////////////////////
/**
* the main service class
*
*/
class CGtserviceModule : public CAppModule
{
public :
	typedef vector<boost::shared_ptr<CWorkItem> > WorkItems;
	/**
	*
	*/
	CGtserviceModule() throw();
	/**
	*
	*/
	~CGtserviceModule()throw();
	/**
	*
	*/
	HRESULT UnInstallTaskScheduler() throw();
	/**
	*
	*/
	HRESULT InstallTaskScheduler();
	/**
	*
	*/
	int Run(int nShowCmd = SW_HIDE);
	/**
	*
	*/
	void Stop() throw();

	void ForceUpdate();
public:
#ifdef NDEBUG
	void LogEvent(LPCTSTR pszFormat, ...)
	{
	}
#else
	void __cdecl LogEvent(LPCTSTR pszFormat, ...) throw()
	{
		const int LOG_EVENT_MSG_SIZE = 256;

		TCHAR chMsg[LOG_EVENT_MSG_SIZE];
		HANDLE hEventSource;
		LPTSTR lpszStrings[1];
		va_list pArg;

		va_start(pArg, pszFormat);
		_vsntprintf_s(chMsg, LOG_EVENT_MSG_SIZE, LOG_EVENT_MSG_SIZE-1, pszFormat, pArg);
		va_end(pArg);

		chMsg[LOG_EVENT_MSG_SIZE - 1] = 0;

		lpszStrings[0] = chMsg;

		/* Get a handle to use with ReportEvent(). */
		hEventSource = RegisterEventSource(NULL, _T("CGtserviceModule"));
		if (hEventSource != NULL)
		{
			/* Write to event log. */
			ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
			DeregisterEventSource(hEventSource);
		}
	}
#endif
	/**
	* important,update this value,when release a new version
	*/
	CString GetVersion()
	{
		return _T("5");
	}
	void NotifyCenter(LPCTSTR cmdline=0);
	/**
	*
	*/
	//void LoadConfig();
	void StartWork();
	/**
	*
	*/
	void DoTimerWork(const DWORD timed);
	/**
	*
	*/
	void AddWork(CWorkItem* pwork);
protected:
	/**
	*
	*/
	void StartTimer()
	{
		m_htimer=SetTimer(NULL,9999,m_timeout,OnTimerProc);
	}
	/**
	*
	*/
	void CloseTimer()
	{
		if(m_htimer)
		{
			KillTimer(NULL,m_htimer);
		}
		m_htimer=0;
	}
public:
	BOOL LaunchGtCenter(LPCTSTR cmdline=NULL,DWORD dwTimeout=INFINITE);
	void UpdateInterval(DWORD interval);
public:
	HINSTANCE					m_hinstance;
	DWORD						m_timeout;
	UINT_PTR					m_htimer;
	WorkItems					m_items;
	BOOL						m_noService;
	CMainDlg*					m_pdlgMain;
	std::vector<WebDispWindow*>		m_webWindows;
	//CPopPanel*                  m_pdlgPop;
};
