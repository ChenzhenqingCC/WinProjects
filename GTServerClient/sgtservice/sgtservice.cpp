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
#include "stdafx.h"
#include <TlHelp32.h>
#include <atlsync.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
//#include <utility/taskscheduler.h>
#include <taskschd.h>
#include "resource.h"
#include "collectwork.h"
#include "sgtservice.h"
#include "MainDlg.h"
#include "lib/rapidxml/rapidxml.hpp"
#include "lib/rapidxml/rapidxml_utils.hpp"
#include "lib/rapidxml/rapidxml_print.hpp"
//#include "webui/jsobject.h"
//#include "webui/webformdispatchimpl.h"
//#include "vld.h"
//#include "mainpanel.h"
/**
*
	*/
	
BOOL Execute(LPCTSTR path,LPCTSTR param=0)
{
	SHELLEXECUTEINFO sei={0};
	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	//sei.hwnd = notifyWnd;
	sei.nShow = SW_SHOWNORMAL;
	sei.lpFile = (LPCTSTR)path;
	sei.lpParameters=param;
	//sei.fMask = SEE_MASK_FLAG_NO_UI;
	return ShellExecuteEx(&sei);
}

/**
*
*pszPathObj:目标文件
*pszParam:运行参数（一般为NULL）
*pszPath:运行目录
*pszDesc:快捷方式名称
*/
inline HRESULT AutoBoot(LPCTSTR pszPathObj,LPCTSTR pszParam,LPCTSTR pszPath,LPCTSTR pszDesc,BOOL enable=TRUE)
{
	CString Publicabpath = GetSysFolderPath(CSIDL_COMMON_STARTUP);
	Publicabpath +=_T("\\");
	Publicabpath +=pszDesc;
	Publicabpath +=_T(".lnk");

	CString Userabpath  = GetSysFolderPath(CSIDL_STARTUP);
	Userabpath +=_T("\\");
	Userabpath +=pszDesc;
	Userabpath +=_T(".lnk");

	HRESULT hr = E_UNEXPECTED;
	if (enable)
	{
		if (!PathFileExists(Publicabpath))
		{
			hr = CreateShortcut(pszPathObj,Publicabpath,pszParam,pszPath,pszDesc);
			if (FAILED(hr) && !PathFileExists(Userabpath))
			{
				hr = CreateShortcut(pszPathObj,Userabpath,pszParam,pszPath,pszDesc); 
			}
		}
	}
	else
	{
		hr =  DeleteFile(Publicabpath) || DeleteFile(Userabpath);
		if (!hr)
		{
			CString msg(_T(""));
			msg.Format(_T("%s,%d"), _T("DeleteFile"), GetLastError());
			SendErrorUrl(_T("uninstall"),msg);
		}
	}
	return hr;
}
/**
*
*/
CGtserviceModule::CGtserviceModule() throw()
{
	//m_timeout=60000;
	m_timeout = 1000;
	m_htimer=0;
	m_noService = FALSE;
	m_pdlgMain=0;
	m_webWindows.clear();
	//m_pdlgPop = 0;
}
/**
*
*/
CGtserviceModule::~CGtserviceModule()throw()
{
	for (size_t i = 0; i < m_webWindows.size(); ++i)
	{
		if (m_webWindows[i])
		{
			delete m_webWindows[i];
			m_webWindows[i] = NULL;
		}
	}
}
/**
* install as service,
* i knick out windows service because of the right too high
* 
*/
HRESULT CGtserviceModule::InstallTaskScheduler()
{
	HRESULT hr=E_UNEXPECTED;
	CString apppath = GetPath();
	apppath +=_T("\\autoupdate.exe");
	if (IsXP())
	{
		CString appname = _T("gtservice");
		hr = AutoBoot(apppath,0,0,appname,true);
	}
	else
	{
		/*try{
			TaskScheduler tsch;
			CComPtr<ITaskFolder> appfoloder = tsch.CreateFolder(_T("gtplatform"));
			if(appfoloder)
			{
			hr = tsch.CreateTask(appfoloder,_T("gtservice"),apppath,0,_T("mytrigger")
			,TaskScheduler::GetDomainUser(),0,0,0);
			}
			}catch(...)
			{
			LogEvent(_T("install task error."));
			}
			if(FAILED(hr))
			{
			CString appname = _T("gtservice");
			hr = AutoBoot(apppath,0,0,appname,true);
			}*/
	}
	if(FAILED(hr))
	{
		LogEvent(_T("install task error."));
	}
	return hr;
}
/**
* uninstall service

*/
HRESULT CGtserviceModule::UnInstallTaskScheduler() throw()
{
	HRESULT hr= E_UNEXPECTED;
	if (IsXP())
	{
		CString apppath = GetPath();
		apppath +=_T("\\autoupdate.exe");
		CString appname = _T("gtservice");
		hr = AutoBoot(apppath,0,0,appname,false);
	}
	else
	{
		/*CTaskScheduler tsch;
		hr = tsch.DeleteTask(_T("gtservice"),_T("gtplatform"));
		if(FAILED(hr))
		{
		CString apppath = GetPath();
		apppath +=_T("\\autoupdate.exe");
		CString appname = _T("gtservice");
		hr = AutoBoot(apppath,0,0,appname,false);
		}*/
	}
	if(FAILED(hr))
	{
		LogEvent(_T("install task error."));
	}
	return hr;
}

/**
*
*/
void CGtserviceModule::DoTimerWork(const DWORD timed)
{
	WorkItems::iterator iter = m_items.begin();
	while (iter != m_items.end())
	{
		if((*iter)->GetState() != 2)//work done
		{
			(*iter)->Run(m_timeout,timed);
		}
		else
		{
			m_items.erase(iter);
		}
		iter++;
	}
}
/**
*
*/
void CGtserviceModule::AddWork(CWorkItem* pwork)
{
	pwork->SetApp(this);
	m_items.push_back(boost::shared_ptr<CWorkItem>(pwork));
}
/**
*
*/
void CGtserviceModule::NotifyCenter(LPCTSTR cmdline)
{
	LaunchGtCenter(cmdline);
}

void CGtserviceModule::UpdateInterval(DWORD interval)
{
	CloseTimer();
	m_timeout=interval;
	StartTimer();
}
/**
*
*/
void CGtserviceModule::Stop() throw()
{
	LogEvent(_T("gtservice::OnStop"));
	CloseTimer();
	if (m_pdlgMain)
	{
		m_pdlgMain->CloseDialog(3);
	}
}

void CGtserviceModule::ForceUpdate()
{
	CString autoUpdate = GetPath() + _T("\\autoupdate.exe");
	Execute(autoUpdate);
}
/**
*
*/
extern vector<CString> split_to_array(CString & src, CString seed);

BOOL CGtserviceModule::LaunchGtCenter(LPCTSTR cmdline,DWORD dwTimeout)
{
	/*if(!m_pdlgPop)
	{
	m_pdlgPop = new CPopPanel(_T("poppanel"),_T(""));
	m_pdlgPop->m_cmdline = cmdline;
	if(m_pdlgPop->Create(NULL) == NULL)
	{
	ATLTRACE(_T("Main dialog creation failed!\n"));
	return FALSE;
	}
	_MainHwnd = m_pdlgPop->m_hWnd;
	}
	else
	{
	m_pdlgPop->m_cmdline = cmdline;
	m_pdlgPop->ShowPopWindow();
	}*/
	vector<CString> strVec = split_to_array(CString(cmdline), _T(" "));
	if (strVec.size() == 3 && m_webWindows.size() == 0)
	{
		//CString tagStr = strVec[2];

		int nWidth = _ttoi(strVec[0]);
		int nHeight = _ttoi(strVec[1]);
		//在桌面右下角显示
		RECT rc;
		SystemParametersInfo(SPI_GETWORKAREA, NULL, &rc, NULL);

		WebDispWindow* dispWin = new WebDispWindow();
		CString strNum;
		strNum.Format(_T("%d"), m_webWindows.size());
		CString className = CString(_T("WebDispClass_")) + strNum;
		dispWin->Create(m_hinstance, className.GetString(), rc.right - nWidth, rc.bottom - nHeight, nWidth, nHeight, strVec[2]);
		m_webWindows.push_back(dispWin);
	}
	return TRUE;
}
/**
*
*/
void CGtserviceModule::StartWork()
{
	//this work is default work
	CloseTimer();
	AddWork(new CCollectWork);
	StartTimer();
}
/**
*
*/
int CGtserviceModule::Run(int nShowCmd )
{
	CMessageLoop theLoop;
	AddMessageLoop(&theLoop);

	CMainDlg dlgMain;
	if(dlgMain.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		return 0;
	}
	m_pdlgMain = &dlgMain;
	dlgMain.ShowWindow(nShowCmd);

	
	StartWork();
	
	int nRet = theLoop.Run();
	_Module.RemoveMessageLoop();
	return nRet;
}

CGtserviceModule _Module;
/**
*
*/
void CALLBACK OnTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	_Module.DoTimerWork(dwTime);
}
/**
* 3 operations
* 1 register service with cmdline 'service'
* 2 run service without cmdline
* 3 unregister service with cmdline 'unregister'
*
*/
int Run(const CString& cmdline, int nCmdShow = SW_SHOWDEFAULT)
{
	CString autoUpdateExe = GetPath()+_T("\\autoupdate.exe_");
	if (PathFileExists(autoUpdateExe))
	{
		MoveFileEx(autoUpdateExe,GetPath()+_T("\\autoupdate.exe"),MOVEFILE_REPLACE_EXISTING);
	}

	int nRet = -1;
	if (cmdline.Find(_T("service")) != -1)//register
	{
		nRet = _Module.InstallTaskScheduler();
	}
	else if(cmdline.Find(_T("unregister")) != -1)//unregister
	{
		nRet = _Module.UnInstallTaskScheduler();
	}
	else
	{
#ifdef DEBUG
		nRet = _Module.Run(SW_SHOW);	
#else
		nRet = _Module.Run(SW_HIDE);
#endif
	}
	_Module.LogEvent(_T("gtservice is close with %d"),nRet);
	return nRet;
}
/**
*
*/
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(CString(lpstrCmdLine), nCmdShow);

	_Module.Term();

	::CoUninitialize();
	return nRet;
}
