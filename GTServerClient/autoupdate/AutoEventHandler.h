////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	autoupdate_event   version:  1.0   ? date: 09/30/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 ztgame Corporation- All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AUTOUPDATE_EVENT_H
#define AUTOUPDATE_EVENT_H
#include "eventmodule.h"
class CMainDlg;

/************************************************************************/
/* 
//handler proxy for host app
*/
/************************************************************************/
class AutoEventHostHandler : public EventHandler
{
public:
	AutoEventHostHandler(const HWND wnd);
	virtual ~AutoEventHostHandler();
	virtual int Handler(const Event &pe);
protected:
	HWND	m_targwnd;
};
/************************************************************************/
/* 
//	handler for main dialog
*/
/************************************************************************/
class AutoEventUIHandler : public EventHandler
{
public:
	AutoEventUIHandler(CMainDlg *pmaindlg);
	virtual ~AutoEventUIHandler();
	virtual int Handler(const Event &pe);
protected:
	CMainDlg	*m_pmdlg;
};
#endif