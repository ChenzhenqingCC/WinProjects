////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	autoupdate   version:  1.0   ? date: 09/28/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 ztgame Corporation- All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "autoupdate.h"
#include "core.h"
#include "maindlg.h"
#include <atlsync.h>
//#include "AutoEventHandler.h"
#include "utility/utility.h"
CString CAutoUpdateModule::GetLocalPath()
{
	if(m_path.GetLength()==0)
	{
		TCHAR buffer[MAX_PATH]={0};
		if (::GetModuleFileName(GetModuleInstance(),buffer,MAX_PATH))
		{
			m_path = buffer;
			int dist = m_path.ReverseFind('\\');
			if (dist > -1)
			{
				m_path.Delete(dist,m_path.GetLength()-dist);
			}
		}
	}
	return m_path;
}
/**
*
*/
void CAutoUpdateModule::Check_Exist()
{
	static CMutex	mutex;
	if (!mutex.Create(NULL,TRUE,_T("autoupdator_for_gtservice_mutex_flag")))
	{
		throw CString(_T("内部错误."));
	}
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		throw CString(_T("更新服务已在运行"));
	}
}
/**
*
*/
void CAutoUpdateModule::Init(ATL::_ATL_OBJMAP_ENTRY* pObjMap, HINSTANCE hInstance, const GUID* pLibID)
{
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));
	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);
	// add flags to support other controls
	AtlInitCommonControls(ICC_BAR_CLASSES);	
	//check the single instance ,this will throw exception
	Check_Exist();
	//init default 
	hRes = CAppModule::Init(pObjMap, hInstance,pLibID);
	if(FAILED(hRes))
	{
		throw CString(_T("app init error."));
	}
}
/**
*
*/
int CAutoUpdateModule::Run(LPTSTR lpstrCmdLine , int nCmdShow)
{
	//add message loop for cur thread
	CMessageLoop theLoop;
	AddMessageLoop(&theLoop);
	//
	CString config_path = GetLocalPath()+_T("\\autoupdate.xml");
	//make the updator
	CUpdator updator((LPCTSTR)config_path,(LPCTSTR)GetLocalPath());
	int nRet  = updator.RunUpdate();
	updator.UpdateRestart();
	return nRet;
}
/**
*
*/
void CAutoUpdateModule::Term()
{
	//remove message loop for this thread
	RemoveMessageLoop();
	CAppModule::Term();
	::CoUninitialize();
}
//global app
CAutoUpdateModule  _Module;
/**
*
*/
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE , LPTSTR lpstrCmdLine, int nCmdShow)
{
	try
	{
		//module init ,include com env init,common ctrl init default message handler init
		GUID lib_guid;
		_Module.Init(NULL,hInstance,&lib_guid);
		//analyse the cmdline and cmdshow ,then create the main dialog
		int nRet = _Module.Run(lpstrCmdLine, SW_HIDE);
		//clear up works
		_Module.Term();
		return nRet;
	}
	catch (CString &e)
	{
		//MessageBox(0,e,0,MB_OK);
	}
	catch(...)
	{
	}
	return 0;
}