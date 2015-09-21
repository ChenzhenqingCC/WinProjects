////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	plugin model   version:  1.0   ? date: 08/20/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 dasjack- All Rights Reserved
//	Written By Jack Liu
//	created:	26:10:2009   10:34
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
///////////////////////////////////////////////////////////////////////////////////
#if GHOSTPROXY_EXPORTS
#define HOST_PROXY_API extern "C" __declspec(dllexport)
#elif GHOST_PROXY_IMPORTS
#define HOST_PROXY_API __declspec(dllimport)
#else
#define HOST_PROXY_API 
#endif
///////////////////////////////////////////////////////////////////////////////////
#include <utility/iinterface.h>
#include <utility/stringfun.h>
#include <atldef.h>
/************************************************************************
 * host proxy interface
 *
 *
/************************************************************************/
class IHostProxy : public IInterface
{
public://struct define

#define SMBUFSIZE									50
#define BUFSIZE										MAX_PATH
#define LGBUFSIZE									102400

#define	CMD_REGISTER					0
#define CMD_UNREGISTER				1

#pragma pack(1)

	struct RpcCmd : public CMD 
	{
		RpcCmd(const BYTE id):CMD(id),result(0)
		{
			size = sizeof(RpcCmd);
			ZeroMemory(text,sizeof(TCHAR)*BUFSIZE);
		}
		BYTE		result;
		TCHAR text[BUFSIZE];
	};
	
	typedef RpcCmd ResCommon;

	struct ReqRegister : public CMD
	{
		ReqRegister(const BYTE side,INotifyInterface* pct):CMD(CMD_REGISTER,side),pNotify(pct),flag(0)
		{
			size = sizeof(ReqRegister);
		}
		INotifyInterface*	pNotify;
		DWORD	flag;
	};
	
	struct ReqUnRegister : public CMD
	{
		ReqUnRegister():CMD(CMD_UNREGISTER)
		{

		}
	};
#pragma  pack()
public://method define
	/**
	*call the functions
	*@strCmd:
	*@pparam:
	*
	*/
	virtual BOOL Invoke(const CMD* preq,CMD** pres=0)=0;
	/**
	*
	*/
	virtual ~IHostProxy(){}
};
/**
*create a cmd which got additional space
*@ppcmd: the pointer of pcmd with the class define
*@additional:the extern space wanted
*@return the original pointer to the buffer 
*/
template<class T> inline BYTE* CreateCmd(T** ppcmd,const DWORD additional=0)
{
	DWORD total = sizeof(T)+additional;
	BYTE* pbuffer = new BYTE[total];
	*ppcmd = new(pbuffer)T;
	(*ppcmd)->size = total;
	return pbuffer;
}