///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	host proxy dll main   version:  1.0   ? date: 11/17/2011
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 dasjack- All Rights Reserved
//	Written By Jack Liu
//	created:	12:21
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ihostproxy.h"
#include <utility/stringfun.h>
#include "utility.h"
#include <atlsync.h>
//////////////////////////////////////////////////////////////////////////
LPCTSTR strSideA=_T("Global\\gtplma");
LPCTSTR strSideB=_T("Global\\gtplmb");
//////////////////////////////////////////////////////////////////////////
//#define ATLTRACE(x) { CString msg;msg.Format(x);OutputDebugString(msg)}
/**
*
*/
BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			CString msg;
			msg.Format(_T("hostproxy loaded,hmodule=%d\n"),hModule);
			ATLTRACE(msg);

		}
		break;
	case DLL_PROCESS_DETACH:
		{
			CString msg;
			msg.Format(_T("hostproxy unloaded,hmodule=%d\n"),hModule);
			ATLTRACE(msg);
			
		}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}
/**
*get current process/thread real handle
*see GetCurrentThread()return a proxy handle not real
*/
static DWORD GetCurrentThreadReal(HANDLE* phProcess,HANDLE* phThread)
{
	HANDLE hCurProcess = GetCurrentProcess();
	HANDLE hCurThread = GetCurrentThread();
	DWORD dwDesc=PROCESS_DUP_HANDLE ;
	BOOL bInherit=FALSE;
	DWORD dwOpts=DUPLICATE_SAME_ACCESS;
	DWORD res = 0;
	if (phProcess)
	{
		res = DuplicateHandle(hCurProcess,hCurThread,hCurProcess,phProcess,dwDesc,bInherit,dwOpts);
	}
	if (phThread)
	{
		res = DuplicateHandle(hCurProcess,hCurThread,hCurProcess,phThread,dwDesc,bInherit,dwOpts);
	}
	return res;
}


/************************************************************************
 * 
 *
 *
/************************************************************************/
class CHostProxy : public IHostProxy
{
public:
	/**
	*
	*/
	CHostProxy():m_isSideA(TRUE),m_pNotify(0),m_beQuit(FALSE)
	{
		
	}
	/**
	*call the functions
	*@strCmd:
	*@pparam:
	*
	*/
	virtual BOOL Invoke(const CMD* preq,CMD** pres)
	{
		BOOL res = FALSE;
		switch (preq->id)
		{
		case CMD_REGISTER:
			{
				res = Register(preq);
			}		
			break;
		default:
			{
				res = SendRequest();
			}
			break;
		}
		return res;
		
	}
protected:
	BOOL SendRequest()
	{
		BOOL res = FALSE;
		if (m_isSideA)
		{
			m_eventSideB.Set();
		}
		else
		{
			m_eventSideA.Set();
		}
		return res;
	}
	/**
	*
	*/
	BOOL Register(const CMD* preq)
	{
		BOOL res=FALSE;
		//
		m_pNotify = ((ReqRegister*)preq)->pNotify;

		if(!OpenEvent(SYNCHRONIZE|EVENT_ALL_ACCESS | EVENT_MODIFY_STATE,FALSE,strSideA))
		{
			m_isSideA = TRUE;
			SECURITY_ATTRIBUTES sa;
			SECURITY_DESCRIPTOR sd;
			ZeroMemory(&sd, sizeof(sd));
			if(!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION))
			{
				return FALSE;
			}

			if(!SetSecurityDescriptorDacl(&sd, TRUE, (PACL)NULL, FALSE))
			{
				return FALSE;
			}

			ZeroMemory(&sa, sizeof(sa));
			sa.nLength = sizeof(sa);
			sa.lpSecurityDescriptor = &sd;
			sa.bInheritHandle = FALSE;
			res = m_eventSideA.Create(&sa,FALSE,FALSE,strSideA);
			if(!res)
			{
				DWORD res = GetLastError();
				return res;
			}
			res = m_eventSideB.Create(&sa,FALSE,FALSE,strSideB);
			if(!res)
			{
				DWORD res = GetLastError();
				return res;
			}
		}
		else
		{
			m_isSideA = FALSE;
			m_eventSideA.Open(SYNCHRONIZE|EVENT_ALL_ACCESS | EVENT_MODIFY_STATE,FALSE,strSideA);
			m_eventSideB.Open(SYNCHRONIZE|EVENT_ALL_ACCESS | EVENT_MODIFY_STATE,FALSE,strSideB);
		}
		//start recieve thread
		res = StartRecieve();
		if (!res)
		{
			ATLTRACE(_T("StartRecieve error\n"));
		}
		return res;
	}
	/**
	*
	*/
	BOOL StartRecieve()
	{
		uintptr_t pres = _beginthreadex(0,0,RecvThread,this,0,0);
		//wait for activing

		return pres != 0;
	}
	/**
	*
	*/
	static unsigned CALLBACK RecvThread(void *param)
	{
		ATLTRACE(_T("hostproxy listen thread start.\n"));
		CHostProxy* pThis = (CHostProxy*)param;
		HANDLE hcurEvent = pThis->m_isSideA ? pThis->m_eventSideA :pThis->m_eventSideB;
		while(!pThis->m_beQuit)
		{
			//INFINITE
			if (WaitForSingleObject(hcurEvent,2000) == WAIT_OBJECT_0)
			{
				pThis->m_pNotify->OnRequest(0);
			}
		}
		ATLTRACE(_T("hostproxy listen thread shutdown.\n"));
		return 0;
	}
public:
	BOOL		m_isSideA;
	INotifyInterface*	m_pNotify;
	CEvent		m_eventSideA;
	CEvent		m_eventSideB;
	BOOL		m_beQuit;
};
/**
*
*/
HOST_PROXY_API IInterface* Create()
{
	return new CHostProxy();
}
/**
*
*/
HOST_PROXY_API int Release(IInterface* pinterface)
{
	IHostProxy* ps = (IHostProxy*)pinterface;
	if (ps)
	{
		delete ps;
	}
	return 0;
}

