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
#include <library/utility/stringfun.h>
#include <library/utility/utility.h>
#include <atlsync.h>
#include <library/utility/memmanage/memmapfile.h>
#include <atlfile.h>
//////////////////////////////////////////////////////////////////////////
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
	CHostProxy():m_pNotify(0),m_beQuit(FALSE)
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
	BOOL SendRequest(const CMD* preq)
	{
		BOOL res = TRUE;
		
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
		
		
		return res;
	}
	/**
	*
	*/
	BOOL StartRecieve()
	{
		
		return pres != 0;
	}

public:
	INotifyInterface*	m_pNotify;
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

