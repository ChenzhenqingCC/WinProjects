////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	main   version:  1.0   ? date: 10/14/2009
//  -------------------------------------------------------------
//  this is a general client desktop application framework
// my goal is to make a framework for all the server port
// app server or web server,the point is to render the views
// use html/css/js
//  -------------------------------------------------------------
//	Copyright (C) 2009 dasjack- All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "mainpanel.h"
#include <atlsync.h>
//////////////////////////////////////////////////////////////////////////
//wtl global app
CAppModule		_Module;
//app wnd 
HWND			_MainHwnd=0;
//////////////////////////////////////////////////////////////////////////
//
HRESULT Initialize(HINSTANCE hInstance, LPTSTR lpstrCmdLine);
//
int Run(LPTSTR lpstrCmdLine = NULL, int nCmdShow = SW_SHOWDEFAULT);
//
void UnInitialize();
/**
*
*/
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE , LPTSTR lpstrCmdLine, int nCmdShow)
{
	int nRet = -1;
	//make singleton metux
	CMutex mutex;
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;
	ZeroMemory(&sd, sizeof(sd));
	if(!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION))
	{
		return nRet;
	}

	if(!SetSecurityDescriptorDacl(&sd, TRUE, (PACL)NULL, FALSE))
	{
		return nRet;
	}

	ZeroMemory(&sa, sizeof(sa));
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = &sd;
	sa.bInheritHandle = FALSE;

	if(!mutex.Create(&sa,TRUE,_T("Global\\gtcenter")))
	{
		return nRet;
	}
	
	if(SUCCEEDED(Initialize(hInstance,lpstrCmdLine)))
	{
		nRet = Run(lpstrCmdLine, nCmdShow);
		UnInitialize();
	}
	return nRet;
}
/**
*do the general init work for the framework
*@hInstance
*@lpstrCmdLine
*@return:
*/
HRESULT Initialize(HINSTANCE hInstance, LPTSTR lpstrCmdLine)
{
	// If you are running on NT 4.0 or higher you can use the following call instead to 
	// make the EXE free threaded. This means that calls come in on a random RPC thread.
	//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));
	
	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);
	// add flags to support other controls
	AtlInitCommonControls(ICC_BAR_CLASSES);	
	//
	//ParseCmdLine(lpstrCmdLine);
	
	//this is need by 
	GUID libid;
	hRes = _Module.Init(NULL, hInstance,&libid);
	ATLASSERT(SUCCEEDED(hRes));

	//register axwindow/axhostwindow
	hRes = AtlAxWinInit();
	return hRes;
}
/**
*start the app
*@lpstrCmdLine:
*@nCmdShow:
*@return:
*/
int Run(LPTSTR lpstrCmdLine, int nCmdShow)
{
	//register message loop 
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);	
	//load all the resource
	CString cfgpath = GetPath()+_T("\\cfg.ini");
	//SetConfig(cfgpath,_T("poppanel"),_T("headheight"),0);
	//SetConfig(cfgpath,_T("poppanel"),_T("footheight"),0);
	//SetConfig(cfgpath,_T("poppanel"),_T("innerborder"),0);
	//SetConfig(cfgpath,_T("poppanel"),_T("outborder"),0);
	//SetConfig(cfgpath,_T("poppanel"),_T("bkmode"),0);
	//create main panel to host other panels

	CPopPanel mainpanel(_T("poppanel"),cfgpath);
	mainpanel.m_cmdline = lpstrCmdLine;
	if(mainpanel.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		return 0;
	}
	_MainHwnd = mainpanel.m_hWnd;
	//DWORD sflag = (CString(_T("show")) == lpstrCmdLine ? SW_SHOW:SW_HIDE);
	//if(sflag ==SW_SHOW)
	//{
	//	mainpanel.SendMessage(WM_SHOWPOPPANEL,1,0);
	//}
	//enter message loop,block until app quit
	//::PostMessage(_MainHwnd,WM_SHOWPOPPANEL,1,0);
	int nRet = theLoop.Run();
	return nRet;
}
/**
*do the general clear work for the framework
*/
void UnInitialize()
{
	_Module.RemoveMessageLoop();
	_Module.Term();
	::CoUninitialize();
}