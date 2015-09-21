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
#include <library/utility/memmanage/AnonyePipe.h>
#include <library/utility/stringfun.h>
#include <library/utility/utility.h>
//////////////////////////////////////////////////////////////////////////
#pragma data_seg(".pipeShared")
HANDLE g_hSideAWrite=0;
HANDLE g_hSideARead=0;
HANDLE g_hSideBWrite=0;
HANDLE g_hSideBRead=0;
DWORD	g_CenterId =0;
#pragma data_seg()

#pragma comment(linker, "/SECTION:.pipeShared,RWS")

AnonymousePipe*			pipeA=0;
AnonymousePipe*			pipeB=0;
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
	static CMD* RecvResponse(HANDLE curHandle)
	{
		CMD* pres=0;
		CMD cmd;

		DWORD dwSize =0;
		ReadFile(curHandle,&cmd,cmd.size,&dwSize,0);

		if(dwSize == sizeof(CMD))
		{
			BYTE *pbuffer = new BYTE[cmd.size];
			memcpy(pbuffer,&cmd,sizeof(CMD));
			if(cmd.size >sizeof(CMD))
			{
				ReadFile(curHandle,pbuffer+sizeof(CMD),cmd.size -sizeof(CMD),&dwSize,0);
				if (dwSize != (cmd.size -sizeof(CMD)))
				{
					delete []pbuffer;
					pbuffer=0;
				}
			}
			pres = (CMD*)pbuffer;
		}
		return pres;
	}
public:
	/**
	*
	*/
	CHostProxy()
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
				res = SendRequest(preq);
			}
			break;
		}
		return res;
	}
protected:
	/**
	*
	*/
	BOOL Register(const CMD* preq)
	{
		BOOL res=FALSE;
		//
		m_localPath =  ::GetShortPath(::GetPath());

		//
		m_pNotify = ((ReqRegister*)preq)->pNotify;
		ReqRegister* prequest = (ReqRegister*)preq;

		if(prequest->subid==0)//side A
		{
			pipeA = new AnonymousePipe;
			pipeB = new AnonymousePipe;

			g_hSideARead = pipeA->m_hread;

			g_hSideAWrite = pipeA->m_hwrite;

			g_hSideBRead = pipeB->m_hread;

			g_hSideBWrite = pipeB->m_hwrite;
			g_CenterId = GetCurrentProcessId();

			m_isSideA = TRUE;
		}
		else
		{
			m_isSideA = FALSE;
		}
		if(m_isSideA){//sideA
			m_hCurWrite = g_hSideAWrite;
			m_hCurRead = g_hSideBRead;
		}
		else//sideB
		{
			//copy handles for this client
			HANDLE cur_hprocess = GetCurrentProcess();
			DWORD dwDesiredAccess = 0;
			BOOL bInheritHandle = FALSE;
			DWORD dwOptions = DUPLICATE_SAME_ACCESS;

			HANDLE hpCenter = OpenProcess(PROCESS_DUP_HANDLE,FALSE,(DWORD)g_CenterId);
			BOOL res = 0;
			res = DuplicateHandle(hpCenter,g_hSideARead,cur_hprocess,&m_hCurRead,dwDesiredAccess,bInheritHandle,dwOptions);
			if (!res)
			{
				DWORD err = GetLastError();
				ATLTRACE(_T("DuplicateHandle error:%d\n"),err);
				return FALSE;
			}
			res = DuplicateHandle(hpCenter,g_hSideBWrite,cur_hprocess,&m_hCurWrite,dwDesiredAccess,bInheritHandle,dwOptions);
			if (!res)
			{
				DWORD err = GetLastError();
				ATLTRACE(_T("DuplicateHandle error:%d\n"),err);
				return FALSE;
			}
		}
		//start recieve thread
		res = StartRecieve();
		if (!res)
		{
			ATLTRACE(_T("StartRecieve error\n"));
		}
		//send to the other
		//res = SendRequest(preq);
		return res;
	}
	/**
	*
	*/
	BOOL SendRequest(const CMD* preq)
	{
		return AnonymousePipe::Write(m_hCurWrite,(LPVOID)preq,preq->size) == preq->size;
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
		while (true)
		{
			CMD* pres = CHostProxy::RecvResponse(pThis->m_hCurRead);
			if (!pres)
			{
				//in debug,this will raise exception,if handle if anvlidate
				__try
				{
					CloseHandle(pThis->m_hCurRead);
					CloseHandle(pThis->m_hCurWrite);
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{

				}
				
				break;
			}
			BYTE* pbuffer = (BYTE*)pres;
			if (pThis->m_pNotify)
			{
				pThis->m_pNotify->OnRequest(pres);
			}
			delete []pbuffer;
		}
		ATLTRACE(_T("hostproxy listen thread shutdown.\n"));
		return 0;
	}
public:
	HANDLE		m_hCurRead;
	HANDLE		m_hCurWrite;
	CString			m_localPath;
	INotifyInterface*	m_pNotify;
	BOOL			m_isSideA;
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
	delete pipeA;
	delete pipeB;
	IHostProxy* ps = (IHostProxy*)pinterface;
	if (ps)
	{
		delete ps;
	}
	return 0;
}

