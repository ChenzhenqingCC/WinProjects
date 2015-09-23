/************************************************************************
* THIS is DasJack DirectUI lib for WTL
*
*
//	Copyright (C) 2009 dasjack- All Rights Reserved
//	Written By Jack Liu
//	created:	18:9:2009   16:17
/************************************************************************/
#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H
#define SECURITY_WIN32
#include <taskschd.h>
#include <wincred.h>
#include <atlbase.h>
#define SECURITY_WIN32
#include "Security.h"
#pragma comment(lib, "Secur32.lib")
using namespace ATL;
#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")
#pragma comment(lib, "Credui.lib")


/**
* my task scheduler
*/
class CTaskScheduler{
public:
	CTaskScheduler()
	{
		HRESULT hr;
		hr = m_service.CoCreateInstance(__uuidof(TaskScheduler));
		if(FAILED(hr))
		{
			return;
		}
		hr = m_service->Connect(CComVariant(),//local pc
			CComVariant(),//current user
			CComVariant(),//current domain
			CComVariant()//no password
			);
		if(FAILED(hr))
		{
			return;
		}
		hr = m_service->GetFolder(CComBSTR(_T("\\")),&m_folder);
		if(FAILED(hr))
		{
			return;
		}
	}
	/**
	*
	*/
	CComPtr<ITaskFolder> CreateFolder(LPCTSTR name)
	{
		CComPtr<ITaskFolder> folder;
		HRESULT hr = m_service->GetFolder(CComBSTR(name),&folder);
		if(FAILED(hr))
		{
			//CComVariant(L"D:(A;;FA;;;BA)(A;;FA;;;SY)")
			m_folder->CreateFolder(CComBSTR(name),CComVariant(),&folder);
		}
		return folder;
	}
	/**
	*
	*/
	HRESULT CreateTask(CComPtr<ITaskFolder>& folder,LPCTSTR name,LPCTSTR path,LPCTSTR cmdline,LPCTSTR tname,LPCTSTR user,LPCTSTR stime,LPCTSTR etime,LPCTSTR interval)
	{
		HRESULT hr;
		//setup definition
		CComPtr<ITaskDefinition> definition;
		hr= m_service->NewTask(0,&definition);
		//setup registration info
		CComPtr<IRegistrationInfo> reginfo;
		hr=definition->get_RegistrationInfo(&reginfo);
		hr=reginfo->put_Author(_T("dasjack"));
		//setup settings
		CComPtr<ITaskSettings> settings;
		hr=definition->get_Settings(&settings);
		hr=settings->put_StartWhenAvailable(VARIANT_TRUE);

		//setup actions
		CComPtr<IActionCollection> actions;
		hr=definition->get_Actions(&actions);
		CComPtr<IAction> action;
		hr=actions->Create(TASK_ACTION_EXEC,&action);
		//get execaction
		CComQIPtr<IExecAction> execAction(action);
		hr=execAction->put_Path(CComBSTR(path));
		if(cmdline)
		{
			hr=execAction->put_Arguments(CComBSTR(cmdline));
		}

		//trigger
		if(tname)
		{
			CComPtr<ITriggerCollection> triggers;
			hr=definition->get_Triggers(&triggers);
			CComPtr<ITrigger> trigger;
			//TASK_TRIGGER_LOGON
			hr=triggers->Create(TASK_TRIGGER_LOGON,&trigger);
			CComQIPtr<ILogonTrigger> logonTrigger(trigger);
			//CComQIPtr<IDailyTrigger> logonTrigger=trigger;
			hr=logonTrigger->put_Id(CComBSTR(tname));
			if(stime)
			{
				hr=logonTrigger->put_StartBoundary(CComBSTR(stime));
			}
			if(etime)
			{

				hr=logonTrigger->put_EndBoundary(CComBSTR(etime));
			}
			if(user)
			{
				hr=logonTrigger->put_UserId(CComBSTR(user));
			}
			if(interval)
			{
				CComPtr<IRepetitionPattern> rep;

				hr = logonTrigger->get_Repetition(&rep);
				//rep->put_Duration(CComBSTR(_T("PT4M")));
				rep->put_Interval(CComBSTR(interval));
			}

		}

		//register
		CComPtr<IRegisteredTask> regTask;
		hr=folder->RegisterTaskDefinition(CComBSTR(name),//
			definition,//
			TASK_CREATE_OR_UPDATE,//
			CComVariant(),//user
			CComVariant(),//pwd
			TASK_LOGON_INTERACTIVE_TOKEN,
			CComVariant(),//sddl
			&regTask
			);
		if(FAILED(hr))
		{
			CString msg(_T(""));
			msg.Format(_T("%s,%d"), _T("RegisterTaskDefinition"),GetLastError());
			SendErrorUrl(_T("install") , msg);
		}
		return hr;
	}

	HRESULT DeleteTask(LPCTSTR tname,LPCTSTR fname=_T("\\"))
	{
		CComPtr<ITaskFolder> folder;
		HRESULT hr = m_service->GetFolder(CComBSTR(fname),&folder);
		if(FAILED(hr))
		{
			CString msg(_T(""));
			msg.Format(_T("%s,%d"), _T("GetFolder"), GetLastError());
			SendErrorUrl(_T("uninstall"),msg);
			return hr;
		}
		hr = folder->DeleteTask(CComBSTR(tname),0);
		if (FAILED(hr))
		{
			CString msg(_T(""));
			msg.Format(_T("%s,%d"), _T("DeleteTask"), GetLastError());
			SendErrorUrl(_T("uninstall"),msg);
		};
		return hr;
	}
	/**
	*get domain uesr name,can use to check 
	*the login user under domain or local
	*/
	static CComBSTR GetDomainUser()
	{
		TCHAR buffer[MAX_PATH]={0};
		unsigned long len=MAX_PATH;
		GetUserNameEx(EXTENDED_NAME_FORMAT::NameSamCompatible,buffer,&len);
		return buffer;
	}
protected:
	CComPtr<ITaskService> m_service;
	CComPtr<ITaskFolder> m_folder;

};

#endif