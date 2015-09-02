////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	workitem   version:  1.0   ? date: 1/15/2013
//  -------------------------------------------------------------
// 
//
//
//  -------------------------------------------------------------
//	Copyright (C) 2013 dasjack- All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef COLLECTWORKITEM_H
#define COLLECTWORKITEM_H

//#include <utility/ajaxhelper.h>
#include <vector>
#include <map>
using namespace std;
#include "workitem.h"
#include <psapi.h>
#pragma comment(lib,"Psapi.lib")
/**
*
*/
class CCollectWork : public CWorkItem{
public:
	CString                                     m_popUrl;
	CString                                     popWidth;
	CString                                     potHeight;
	/**
	*
	*/
	CCollectWork();
	~CCollectWork();
	/**
	*
	*/
	virtual void Run(const DWORD interval,const DWORD timed);
protected:
	/**
	*
	*/
	CString GetProcName(const DWORD pid,LPTSTR path=0);
	/**
	*
	*/
	CString GetGtUserName();
	/**
	*
	*/
	map<CString,CString> GenCurProcessList();
	/**
	*
	*/
	BOOL AnyliseProccessList();
	/**
	*
	*/
	CString HttpSendLog(CString& data);
	CString HttpSendHb();
	/**
	*
	*/
	void ParseResponse(const CString&);
	/**
	*
	*/
	CString NormalName(const CString&);
	/**
	*
	*/
	CString GetDeviceID();
	/**
	*
	*/
	void KillProcess(CString processName);
	
protected:
	//CAjaxRequest								m_ajax;
	CString										m_deviceID;
	CString										m_oldDeviceID;
	map<CString,vector< pair<DWORD,DWORD> > >	m_history;
	map<CString,CString>						m_games;
	map<CString,CString>						m_gamealases;
	int											m_timed;
	int											m_psinterval;
};

#endif