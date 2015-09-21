////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	miandlg   version:  1.0   ? date: 09/29/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 ztgame Corporation- All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "resource.h"
#include "aboutdlg.h"
#include <atlcrack.h>

class Event;
class CUpdator;

class CColorStatic : public CWindowImpl<CColorStatic,CStatic>
{
public:
	BEGIN_MSG_MAP(CColorStatic)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(OCM_CTLCOLORSTATIC, OnCtlColorStatic)
	END_MSG_MAP()

	LRESULT OnCtlColorStatic(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CDCHandle dc( (HDC) wParam);
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(m_txtcolor);
		return (LRESULT) (HBRUSH) (m_hbrNormalBk );
	}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		_Initialize();
		return lRes;
	}
public:
	/**
	*
	*/
	BOOL SubclassWindow(HWND hWnd)
	{
		ATLASSERT(m_hWnd==NULL);
		ATLASSERT(::IsWindow(hWnd));
		BOOL bRet =  CWindowImpl<CColorStatic,CStatic>::SubclassWindow(hWnd);
		if(bRet)
		{
			_Initialize();
		}
		return bRet;
	}
	/**
	*
	*/
	void SetColors(COLORREF bkcolor,COLORREF txtcolor)
	{
		m_bkcolor = bkcolor;
		m_txtcolor = txtcolor;
	}
protected:
	/**
	*make the defaults
	*/
	void _Initialize()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		m_hbrNormalBk.CreateSolidBrush(m_bkcolor);
	}
protected:
	CBrush	m_hbrNormalBk;
	COLORREF	m_bkcolor;
	COLORREF	m_txtcolor;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CMainDlg : public CDialogImpl<CMainDlg>, 
							  public CMessageFilter, 
							  public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };
	//////////////////////////////////////////////////////////////////////////
	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorDlg)
		MESSAGE_HANDLER(WM_COPYDATA,OnCopyData)
		COMMAND_ID_HANDLER(IDC_RUN_UPDATE, OnUpdate)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()
	//////////////////////////////////////////////////////////////////////////
	CMainDlg(CUpdator *pu):m_pupdator(pu)
	{
	}
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();
	int UIEventHandler(const Event&);
	
protected:
	LRESULT OnInitDialog(UINT,WPARAM,LPARAM,BOOL&);
	LRESULT OnCtlColorDlg(UINT , WPARAM , LPARAM, BOOL&);
	LRESULT OnCopyData(UINT,WPARAM,LPARAM,BOOL&);
	LRESULT OnDestroy(UINT,WPARAM,LPARAM,BOOL&);
	LRESULT OnUpdate(WORD, WORD, HWND , BOOL& );
	LRESULT OnOK(WORD, WORD wID, HWND , BOOL&);
	LRESULT OnCancel(WORD, WORD wID, HWND, BOOL&);
	void CloseDialog(int nVal);
	void Running();
	void ShowMsg(const int type,const int res);
protected:
	CColorStatic				m_txtreport;
	CProgressBarCtrl		m_processbar;
	CUpdator					*m_pupdator;
	CBrush						m_brBack;
};
