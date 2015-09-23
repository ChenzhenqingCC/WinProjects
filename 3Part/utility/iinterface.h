////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	interface    version:  1.0   ? date: 08/20/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 dasjack- All Rights Reserved
//	Written By Jack Liu
//	created:	26:10:2009   10:34
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef IINTERFACE_H
#define IINTERFACE_H
#include <tchar.h>
#include <windows.h>
/************************************************************************
 * 
 *
 *
/************************************************************************/
struct IInterface{
	/**
	*
	*/
	IInterface():hDll(0)
	{
	}
	//this function must be exported by the implement dll
	typedef IInterface* (*pfn_Create)(void);
	//this function must be exported by the implement dll
	typedef int (*pfn_Release)(IInterface*);
	/**
	*@namepath:the dll path
	*@return IInterface,should cast to the actual interface
	*/
	static IInterface* Get(LPCTSTR namepath)
	{
		IInterface* pinterface=0;
		HMODULE hDll=::LoadLibrary(namepath);
		if (hDll)
		{
			pfn_Create pcreator =(pfn_Create)GetProcAddress(hDll, "Create");
			if (pcreator)
			{
				pinterface = pcreator();
				if (pinterface)
				{
					pinterface->hDll = hDll;
				}
			}
		}
		return pinterface;
	}
	/**
	*@pinterface : the pointer need to released
	*@return:the result of operation
	*/
	static int Release(IInterface* pinterface)
	{
		int res = 0;
		HMODULE hDll = pinterface ?  pinterface->hDll : 0;
		if (hDll)
		{
			pfn_Release prelease =(pfn_Release)GetProcAddress(hDll, "Release");
			if (prelease)
			{
				res = prelease(pinterface);
			}
			FreeLibrary(hDll);
			pinterface=0;
		}
		return res;
	}
public:
	HMODULE hDll;
};
/************************************************************************
 * 
 *
 *
/************************************************************************/
struct INotifyInterface
{
	/**
	*
	*/
	virtual BOOL OnRequest(LPVOID pparam)=0;
	/**
	*
	*/
	virtual ~INotifyInterface(){}
};
/************************************************************************/
/* 
basic CMD define
all the cmd like this : cmd_size(4)+cmd_struct
*/
/************************************************************************/
//template<class T>
struct CMD
{
	CMD(const BYTE i=0,const BYTE si=0):id(i),subid(si)
	{
		//every CMD should set this size as its actual size
		//so,sub class should rewrite this member
		size = sizeof(CMD);
	}
	/**
	*	if id and sub id is "WORD"  use MAKEWORD
	*/
	WORD GetCmdId()
	{
		return MAKEWORD(id,subid);
	}
	/**
	*
	*/
	void SetCmdId(const BYTE cid=0,const BYTE csid=0)
	{
		id = cid? cid : id;
		subid = csid? csid:subid;
	}
	void SetCmdId(const DWORD)
	{

	}
	DWORD		size;//cmd buffer size not include the "size"(DWORD)
	BYTE			id;	//cmd id
	BYTE			subid;//cmd sub id
};
#endif