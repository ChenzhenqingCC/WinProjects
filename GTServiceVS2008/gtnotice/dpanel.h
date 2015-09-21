/************************************************************************
* THIS is DasJack DirectUI lib for WTL
*
*
//	Copyright (C) 2009 dasjack- All Rights Reserved
//	Written By Jack Liu
//	created:	18:9:2009   16:17
/************************************************************************/
#ifndef DDUI_DPANEL_H
#define DDUI_DPANEL_H

#pragma once
#include "htmlui.h"
#include "ddui.h"
#if _DEBUG
#pragma comment(lib,"htmlui_d.lib")
#else
#pragma comment(lib,"htmlui.lib")
#endif
/************************************************************************
* 
*
*
/************************************************************************/
class CWebPanel : public CDPanel<CWebPanel>
								,public CHtmlPanel
								,public CMessageFilter
{
public:
	/**
	*
	*/
	CWebPanel(LPCTSTR clsname,LPCTSTR cfgpath,const BOOL isheaped=FALSE):
						CHtmlPanel(clsname,isheaped)
						,CDPanel<CWebPanel>(cfgpath)
	{
	}
	/**
	*override CDPanel
	*@1 void Close(int nFlags);
	*@2 void Render(CDCHandle& dc);
	*@3 DWORD NcHitTesting(CPoint point);
	*@4 void Layout(int nType,CSize size);
	*@5 lpctstr GetName()
	*/
	void Close(UINT nFlags)
	{
		CloseDialog(nFlags);
	}
	/**
	*
	*/
	void UnInitialize()
	{
		UnRegMsgFilter();
	}
	/**
	*overwrite CHtmlPanel 
	//CallByScript
	//SetOptions
	//CloseDialog
	//InitControls
	//PreTranslateMessage
	//OnIdle
	*/
	virtual BOOL InitControls()
	{
		CHtmlPanel::InitControls();
		Initialize(0);
		RegMsgFilter();
		return TRUE;
	}
	/*
	*	
	*/
	virtual void CloseDialog(int nVal)
	{
		DestroyWindow();
	}
	/**
	*
	*/
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		//these check is to let web client got its msg
		CPoint pt = pMsg->pt;
		ScreenToClient(&pt);
		if(m_webwnd == ChildWindowFromPoint(pt))
		{
			return CHtmlPanel::PreTranslateMessage(pMsg);
		}
		//
		return CDPanel<CWebPanel>::PreTranslateMessage(pMsg);
	}
	/**
	*call from js
	*/
	virtual BOOL PreTranslateWebInvoke(CString& apiname,vector<CString>& params,CComVariant& res)
	{
		return FALSE;
	}
protected:
	/*
	*	
	*/
	BOOL RegMsgFilter()
	{
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		if(!pLoop)
		{
			return FALSE;
		}
		pLoop->AddMessageFilter(this);
		
		return TRUE;
	}
	/*
	 *	
	 */
	BOOL UnRegMsgFilter()
	{
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		if(!pLoop)
		{
			return FALSE;
		}
		pLoop->RemoveMessageFilter(this);
		
		return TRUE;
	}
public:
	BEGIN_MSG_MAP_EX(CWebPanel)
		MSG_WM_SIZE(OnSize)
		CHAIN_MSG_MAP(CDPanel<CWebPanel>)
		CHAIN_MSG_MAP(CHtmlPanel)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()
	/**
	*
	*/
	void OnSize(UINT nType, CSize size)
	{
		CDPanel<CWebPanel>::OnSize(nType,size);

		SetRoundFrame(9,9);

		if(m_webwnd)
		{
			m_webwnd.MoveWindow(m_clientRect);
		}
	}
};
/************************************************************************
* 
*
*
/************************************************************************/
template<class TWinTraits = CControlWinTraits >
class CPanelImpl : public CWindowImpl< CPanelImpl<TWinTraits>, CWindow, TWinTraits >
								,public CDPanel<CPanelImpl<TWinTraits> >
								,public CMessageFilter
						
{
public:
	DECLARE_WND_CLASS(_T("dasjack.panel"))

	typedef CWindowImpl< CPanelImpl, CWindow, TWinTraits > baseType;
	/**
	*
	*/
	CPanelImpl(LPCTSTR name,LPCTSTR cfgpath):CDPanel<CPanelImpl>(cfgpath),m_name(name)
	{

	}
	~CPanelImpl()
	{
	}
	/**
	*
	*/
	void SetName(LPCTSTR name)
	{
		m_name = name;
	}
	/**
	*
	*/
	CString GetName()
	{
		return m_name;
	}
	/**
	*
	*/
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return  CDPanel<CPanelImpl<TWinTraits> >::PreTranslateMessage(pMsg);
	}
	/**
	*
	*/
	BOOL SubclassWindow(HWND hWnd)
	{
		ATLASSERT(m_hWnd==NULL);
		ATLASSERT(::IsWindow(hWnd));
#ifdef _DEBUG
		// Check class
		TCHAR szBuffer[32];
		if( ::GetClassName(hWnd, szBuffer, (sizeof(szBuffer)/sizeof(TCHAR))-1) ) {
			ATLASSERT(::lstrcmpi(szBuffer, GetWndClassName())==0);
		}
#endif
		BOOL bRet = baseType::SubclassWindow(hWnd);
		if( bRet )
		{
			Initialize(0);
		}
		return bRet;
	}
	/**
	*
	*/
	virtual BOOL Initialize(LPCTSTR cfg_path)
	{
		CDPanel<CPanelImpl>::Initialize(cfg_path);
		RegMsgFilter();
		CenterWindow();
		return TRUE;
	}
	/**
	*
	*/
	virtual void UnInitialize()
	{
		UnRegMsgFilter();
	}
protected:
	/*
	*	
	*/
	BOOL RegMsgFilter()
	{
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		if(!pLoop)
		{
			return FALSE;
		}
		pLoop->AddMessageFilter(this);

		return TRUE;
	}
	/*
	*	
	*/
	BOOL UnRegMsgFilter()
	{
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		if(!pLoop)
		{
			return FALSE;
		}
		pLoop->RemoveMessageFilter(this);

		return TRUE;
	}
public:
	BEGIN_MSG_MAP_EX(CPanelImpl)
		MSG_WM_SIZE(OnSize)
		MSG_WM_CREATE(OnCreate)
		CHAIN_MSG_MAP(CDPanel<CPanelImpl>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()
	/**
	*
	*/
	int OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		return Initialize(0);
	}
	/**
	*
	*/
	void OnSize(UINT nType, CSize size)
	{
		CDPanel<CPanelImpl>::OnSize(nType,size);
		SetRoundFrame(9,9);
	}
protected:
	CString m_name;
};

typedef CWinTraits<WS_CHILD | WS_VISIBLE, 0>	CDControlWinTraits;
typedef CWinTraits<WS_POPUP|WS_VISIBLE, WS_EX_APPWINDOW>		CDFrameWinTraits;

typedef CPanelImpl<CDFrameWinTraits> CFramePanel;
typedef CPanelImpl<CDControlWinTraits> CPanel;
#endif