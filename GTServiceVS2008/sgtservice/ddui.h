/************************************************************************
* THIS is DasJack DirectUI lib for WTL
*
*
//	Copyright (C) 2009 dasjack- All Rights Reserved
//	Written By Jack Liu
//	created:	18:9:2009   16:17
/************************************************************************/
#ifndef DDUI_H
#define DDUI_H

#pragma once
#include <map>
using namespace std;

#include <atlwin.h>
#include	<atlcrack.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlmisc.h>
#include <atlimage.h>
#include <atlgdi.h>
#define  STRSAFE_NO_DEPRECATE
#define	STRSAFE_LIB
#include <strsafe.h>
#pragma comment(lib,"strsafe.lib")

#include <boost/shared_ptr.hpp>
//#include "library/utility/allcast.h"
#include "utility.h"
/************************************************************************
* texture resource manager
*
*
/************************************************************************/
class TextureManager
{
public:
	/**
	*
	*/
	CImage* Load(LPCTSTR name,LPCTSTR path)
	{
		ATLASSERT(name);
		if (m_images[name])
		{
			return m_images[name].get();
		}

		CImage* pnew = new CImage();
		pnew->Load(path);
		if (!pnew->IsNull())
		{
			m_images[name].reset(pnew);
		}
		else
		{
			delete pnew;
			pnew=0;
		}
		return pnew;
	}
	/**
	*
	*/
	void Add(LPCTSTR name,CImage* pnew)
	{
		if(!name) return;
		m_images[name].reset(pnew);
	}
	/**
	*
	*/
	const CImage* Get(LPCTSTR name)
	{
		if(!name) return 0;
		return m_images[name].get();
	}
	/**
	*
	*/
	void Remove(LPCTSTR name)
	{
		if(!name) return;
		m_images.erase(name);
	}
protected:
	map<CString,boost::shared_ptr<CImage> >	m_images;
};

//extern TextureManager g_tmg;
inline TextureManager* GetTmg()
{
	static TextureManager s_tmg;
	return &s_tmg;
}
/************************************************************************
* render engine
*
*
/************************************************************************/
class RenderEngine
{
public:
	/**
	*
	*/
	static void DrawImage(CDCHandle dc,const CImage& image,const DWORD bkMode,CRect& rect)
	{
		if (image.IsNull())
		{
			return;
		}
		
		int bkImageWidth = image.GetWidth();
		int bkImageHeight = image.GetHeight();

		if (bkMode==1)
		{
			int curWidth = 0;
			while (curWidth <rect.Width())
			{
				image.StretchBlt(dc,rect.left+curWidth,rect.top,bkImageWidth,bkImageHeight,SRCCOPY);
				curWidth += bkImageWidth;
			}
		}
		else if(bkMode==2)
		{
			int curHeight = 0;
			while (curHeight <rect.Height())
			{
				image.StretchBlt(dc,rect.left,rect.top+curHeight,bkImageWidth,bkImageHeight,SRCCOPY);
				curHeight += bkImageHeight;
			}
		}
		else if (bkMode==3)
		{
			//rect.right = rect.left+bkImageWidth;
			//rect.bottom = rect.top+bkImageHeight;
			image.StretchBlt(dc,rect,rect,SRCCOPY);
		}
		else if (bkMode==4)
		{
			rect.right = rect.left+bkImageWidth;
			rect.bottom = rect.top+bkImageHeight;
			image.AlphaBlend(dc,rect.TopLeft(),255);
		}
		else
		{
			rect.right = rect.left+bkImageWidth;
			rect.bottom = rect.top+bkImageHeight;
			image.StretchBlt(dc,rect,SRCCOPY);
		}
	}
	/**
	*
	*/
	static void DrawText(CDCHandle& dc
		,const CString& text
		,CRect& rect
		,const CString& fontname=_T("宋体")
		,const int fontsize=9
		,COLORREF color=RGB(0,0,0))
	{
		dc.SetTextColor(color);
		CFont  font;
		int height = -MulDiv(fontsize, GetDeviceCaps(dc, LOGPIXELSY), 72);

		HFONT r = font.CreateFont(height,0,0,0,FW_BOLD,FALSE,FALSE,\
			FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,\
			CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, \
			DEFAULT_PITCH,fontname);
		if (r)
		{
			dc.SelectFont(font);
		}

		dc.DrawText(text,text.GetLength(),rect,DT_LEFT);
	}
};
//forward declare
class CDControl;

#define  DECLARE_PANEL_CLASS(name)\
	LPCTSTR GetName()\
{\
	return name;\
}\
	DECLARE_WND_CLASS(name)

/************************************************************************
* 
* To use this class: Derive from CDPanel<Base> and then
*	chain the message map CHAIN_MSG_MAP(CColoredDialog<Base>)...
*should supply these methods
*@1 void Close(int nFlags);
*@2 void Render(CDCHandle& dc);
*@3 DWORD NcHitTesting(CPoint point);
*@4 void Layout(int nType,CSize size);
*@5 lpctstr GetName()
/************************************************************************/
template<class T> class CDPanel //: 	public CMessageFilter
{
public:
	/**
	*
	*/

public:
	/**
	*overwrite the base method
	*/
	BOOL PreTranslateMessage(MSG* pMsg)
	{
		BOOL res=FALSE;
		map<DWORD,CDControl*>::iterator iter = m_children.begin();
		while (iter != m_children.end())
		{
			res = (*iter).second->ProcessMessage(pMsg);
			if (res)
			{
				break;
			}
			iter++;
		}
		return FALSE;
	}
	// Message map and handlers
	BEGIN_MSG_MAP_EX(CDPanel)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_SIZE(OnSize)
		MSG_WM_NCHITTEST(OnNcHitTest)
		MSG_WM_NCLBUTTONDOWN(OnNCLButtonDown)
		MSG_WM_NCLBUTTONUP(OnNcLButtonUp)
		//MSG_WM_NCMOUSEMOVE(OnNcMouseMove)
		MSG_WM_DESTROY(OnDestroy)
		//MSG_WM_SETCURSOR(OnSetCursor)
		//MSG_WM_SETFOCUS(OnSetFocus)
		//REFLECT_NOTIFICATIONS()
	END_MSG_MAP()
		/**
		*
		*/
		BOOL OnSetCursor(CWindow wnd, UINT nHitTest, UINT message)
		{
			SetMsgHandled(FALSE);
			return FALSE;
		}
	/**
	*
	*/
	void OnSetFocus(CWindow wndOld)
	{
		SetMsgHandled(FALSE);
	}
	/**
	*
	*/
	void OnDestroy()
	{
		SetMsgHandled(FALSE);
		UnInitialize();
	}
	void OnNcMouseMove(UINT nHitTest, CPoint point)
	{
		if(nHitTest != HTCLIENT)
		{

		}
	}
	/**
	*called when hittest result is not htclient
	*/
	void OnNCLButtonDown(UINT nHitTest, CPoint point)
	{
		SetMsgHandled(FALSE);
	}
	/**
	*
	*/
	void OnNcLButtonUp(UINT nHitTest, CPoint point)
	{
		if (nHitTest==HTSYSMENU)
		{
			T* pT = static_cast<T*>(this);
			//@1
			pT->Close(nHitTest);
		}
		SetMsgHandled(FALSE);
	}
	/**
	*
	*/
	BOOL OnEraseBkgnd(CDCHandle dc)
	{
		dc.SetBkMode(TRANSPARENT);
		T* pT = static_cast<T*>(this);
		//@2
		pT->Render(dc);
		return TRUE;
	}
	/**
	*
	*/
	UINT OnNcHitTest(CPoint point)
	{
		T* pT = static_cast<T*>(this);
		//@3
		UINT res = pT->NcHitTesting(point);
		if (res != HTCAPTION )
		{
			SetMsgHandled(FALSE);
		}
		return res;
	}
	/**
	*
	*/
	void OnSize(UINT nType, CSize size)
	{
		T* pT = static_cast<T*>(this);
		//@4
		pT->Layout(nType,size);
	}
public:
	/**
	*
	*/
	void UpdateStatus(LPCTSTR text)
	{
		if (text)
		{
			m_statusText = text;
		}
		T* pT = static_cast<T*>(this);
		pT->InvalidateRect(m_statusRect);
	}
	/**
	*
	*/
	void UpdateCaption()
	{
		T* pT = static_cast<T*>(this);
		pT->InvalidateRect(m_captionRect);
	}
	/**
	*
	*/
	void UpdateClient()
	{
		T* pT = static_cast<T*>(this);
		pT->InvalidateRect(m_clientRect);
	}
	/**
	*
	*/
	void UpdateControl(const DWORD cid)
	{
		CDControl* pchild = GetControl(cid);
		if(pchild)
		{
			T* pT = static_cast<T*>(this);
			pT->InvalidateRect(pchild->GetRect());
		}
	}
public:
	/**
	*
	*/
	int Append(CDControl* pchild)
	{
		m_children[pchild->GetId()] = pchild;
		T* pT = static_cast<T*>(this);
		pchild->SetWnd(pT->m_hWnd);
		pchild->Initialize();
		return m_children.size();
	}
	/**
	*
	*/
	BOOL Remove(const DWORD cid)
	{
		CDControl* pchild = GetControl(cid);
		if(pchild)
		{
			delete pchild;
			m_children.erase(cid);
			return TRUE;
		}
		return FALSE;
	}
	/**
	*
	*/
	CDControl* GetControl(const DWORD cid)
	{
		map<DWORD,CDControl*>::iterator res = m_children.find(cid);
		if(res != m_children.end())
		{
			return  res->second;
		}
		return 0;
	}
protected:
	/**
	*
	*/
	CDPanel(LPCTSTR cfgpath):m_cfgpath(cfgpath)
		,m_captionHeight(50)
		,m_statusHeight(30)
		,m_innerBorder(0)
		,m_outBorder(0)
		,m_pbkimage(0)
	{
		m_captionRect.SetRectEmpty();
		m_clientRect.SetRectEmpty();
		m_statusRect.SetRectEmpty();
	}
	/**
	*
	*/
	virtual ~CDPanel()
	{
		map<DWORD,CDControl*>::iterator iter = m_children.begin();
		if(iter != m_children.end())
		{
			delete iter->second;
			iter++;
		}
	}
	/**
	*
	*/
	LRESULT Initialize(LPCTSTR cfg_path)
	{
		// register object for message filtering and idle updates
		//CMessageLoop* pLoop = _Module.GetMessageLoop();
		//ATLASSERT(pLoop != NULL);
		//pLoop->AddMessageFilter(this);
		//
		if (cfg_path)
		{
			m_cfgpath = cfg_path;
		}
		//
		T* pT = static_cast<T*>(this);
		//@5
		return InitResource(m_cfgpath,pT->GetName());
	}
	/**
	*
	*/
	void UnInitialize()
	{
		// unregister message filtering and idle updates
		//CMessageLoop* pLoop = _Module.GetMessageLoop();
		//ATLASSERT(pLoop != NULL);
		//pLoop->RemoveMessageFilter(this);
		T* pT = static_cast<T*>(this);
		pT->UnInitialize();
	}
	/**
	*
	*/
	LRESULT InitResource(LPCTSTR cfg_path,LPCTSTR name)
	{
		LRESULT res = S_OK;

		CString strTitle = GetConfig(m_cfgpath,name,_T("title"));
		T* pT = static_cast<T*>(this);
		if (strTitle.GetLength())
		{
			pT->SetWindowText(strTitle);
		}
		CString strBkImage = GetConfig(m_cfgpath,name,_T("bkimage"));
		m_pbkimage = GetTmg()->Get(strBkImage);
		CRect nwrect;
		pT->GetWindowRect(nwrect);

		int width = GetConfigInt(m_cfgpath,name,_T("width"));
		int height = GetConfigInt(m_cfgpath,name,_T("height"));

		m_captionHeight = GetConfigInt(m_cfgpath,name,_T("headheight"),0);
		m_statusHeight = GetConfigInt(m_cfgpath,name,_T("footheight"),0);
		m_innerBorder = GetConfigInt(m_cfgpath,name,_T("innerborder"),0);
		m_outBorder = GetConfigInt(m_cfgpath,name,_T("outborder"),0);
		m_bkMode = GetConfigInt(m_cfgpath,name,_T("bkmode"),0);
		if(width && height)
		{
			nwrect.bottom = nwrect.top + height;
			nwrect.right = nwrect.left + width;
		}
		//if bk mode is 1,means the window should got the same rect with bk image
		if(m_pbkimage && !m_pbkimage->IsNull() && m_bkMode == 0)
		{
			nwrect.top = nwrect.top - m_outBorder;
			nwrect.bottom = nwrect.top + m_pbkimage->GetHeight()+ 2*m_outBorder;
			nwrect.left = nwrect.left - m_outBorder;
			nwrect.right = nwrect.left + m_pbkimage->GetWidth()+ 2*m_outBorder;
		}
		
		pT->SetWindowPos(0,nwrect,SWP_NOMOVE);
		pT->Layout(0,0);

		return res;
	}
	/**
	*
	*/
	void Layout(UINT nType, CSize size)
	{
		T* pT = static_cast<T*>(this);

		CRect crect;
		pT->GetClientRect(&crect);

		m_captionRect.left=crect.left;
		m_captionRect.right = crect.right;
		m_captionRect.top = crect.top;
		m_captionRect.bottom =m_captionRect.top + m_captionHeight;

		m_clientRect.left=crect.left + m_innerBorder;
		m_clientRect.right = crect.right-m_innerBorder;
		m_clientRect.top =m_captionRect.bottom;
		m_clientRect.bottom =crect.bottom-m_statusHeight;

		m_statusRect.left=crect.left;
		m_statusRect.right = crect.right;
		m_statusRect.top = m_clientRect.bottom ;
		m_statusRect.bottom =crect.bottom;
	}
	/**
	*
	*/
	UINT NcHitTesting(CPoint& point)
	{
		T* pT = static_cast<T*>(this);
		pT->ScreenToClient(&point);
		CRect crect;
		pT->GetClientRect(&crect);

		//
		UINT res = HTNOWHERE;

		if(res != HTNOWHERE)
		{
			return res;
		}

		if (m_captionRect.PtInRect(point) || m_statusRect.PtInRect(point))
		{
			res = HTCAPTION;
			map<DWORD,CDControl*>::iterator iter = m_children.begin();
			while (iter != m_children.end())
			{
				if ((*iter).second->GetRect().PtInRect(point))
				{
					res = HTCLIENT;
					break;
				}
				iter++;
			}
		}
		else if(m_clientRect.PtInRect(point))
		{
			res = HTCLIENT;
		}
		else 
		{
			res = HTNOWHERE;
		}
		return res;
	}
	/**
	*
	*/
	void Render(CDCHandle& dc)
	{
		T* pT = static_cast<T*>(this);
		pT->DrawBackground(dc);
		pT->DrawBorder(dc);
		pT->DrawCaption(dc);
		pT->DrawClient(dc);
		pT->DrawStatus(dc);
	}
	/**
	*
	*/
	void Close(UINT nFlags)
	{
		T* pT = static_cast<T*>(this);
		pT->DestroyWindow();
	}
	/**
	*
	*/
	void SetRoundFrame(const int radx1=5,const int radx2=5)
	{
		CRgn m_rgn;
		CRect rc; 
		T* pT = static_cast<T*>(this);
		pT->GetWindowRect(&rc);
		rc -= rc.TopLeft();
		m_rgn.CreateRoundRectRgn(rc.left,rc.top,rc.right,rc.bottom,radx1,radx2);
		pT->SetWindowRgn(m_rgn,TRUE);
	}
	/**
	*
	*/
	void DrawCaption(CDCHandle dc)
	{
		//
		CString title;
		T* pT = static_cast<T*>(this);
		pT->GetWindowText(title);
		//
		if (title.GetLength())
		{
			CRect txtRect = m_captionRect;
			txtRect.top += 4;//txtRect.Height()/2;
			txtRect.left +=10;

			RenderEngine::DrawText(dc,title,txtRect,TEXT("宋体"),9,RGB(255,255,255));
		}
	}
	/**
	*
	*/
	void DrawClient(CDCHandle dc)
	{
		map<DWORD,CDControl*>::iterator iter = m_children.begin();
		while (iter != m_children.end())
		{
			(*iter).second->Render(dc);
			iter++;
		}
	}
	/**
	*
	*/
	void DrawBorder(CDCHandle dc)
	{
		if (!m_outBorder)
		{
			return ;
		}

		CRect wrect;
		T* pT = static_cast<T*>(this);
		pT->GetWindowRect(wrect);
		CRect rect(0,0,wrect.Width(),wrect.Height());
		//
		CBrush frameBrush;
		frameBrush.CreateSolidBrush(RGB(255,255, 255));

		dc.FrameRect(rect,frameBrush);
	}
	/**
	*
	*/
	void DrawBackground(CDCHandle dc)
	{
		T* pT = static_cast<T*>(this);
		CRect wrect;
		pT->GetWindowRect(wrect);
		CRect rect(0,0,wrect.Width(),wrect.Height());

		if (m_pbkimage)
		{
			RenderEngine::DrawImage(dc, *m_pbkimage, m_bkMode, rect);
		}
	}
	/**
	*
	*/
	void DrawStatus(CDCHandle dc)
	{
		if (m_statusText.GetLength())
		{
			RenderEngine::DrawText(dc,m_statusText,m_statusRect,TEXT("宋体"),9,RGB(220,220,220));
		}
	}
protected:
	CRect												m_captionRect;
	CRect												m_clientRect;
	CRect												m_statusRect;
	int													m_innerBorder;
	int													m_outBorder;
	int													m_captionHeight;
	int													m_statusHeight;
	int													m_bkMode;
	CString											m_statusText;
	CString											m_cfgpath;
	CString											m_basepath;
	const CImage*								m_pbkimage;
	map<DWORD,CDControl*>		m_children;
};

/************************************************************************
* 
*
*
/************************************************************************/
class CDControl
{
public:
	/**
	*
	*/
	CDControl(const DWORD id):m_id(id),m_state(1),m_focused(FALSE)
	{
		m_rect.SetRectEmpty();
	}
	/**
	*
	*/
	virtual ~CDControl()
	{

	}
	/**
	*
	*/
	virtual void Render(CDCHandle& dc)
	{

	}
	/**
	*called when be appendTo parent wnd
	*/
	virtual LRESULT Initialize()
	{
		return S_OK;
	}
	/**
	*
	*/
	virtual BOOL ProcessMessage(MSG* pMsg)
	{
		BOOL res = FALSE;//not handled
		CPoint curPt = pMsg->pt;
		ScreenToClient(m_wnd,&curPt);

		if (pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_NCLBUTTONDOWN)
		{
			if (m_rect.PtInRect(curPt))
			{
				m_focused = TRUE;
				ActiveChild(m_id);
			}
			else
			{
				m_focused = FALSE;
			}
		}
		return res;
	}
	/**
	*
	*/
	BOOL IsHover(const CPoint& point)
	{
		return m_rect.PtInRect(point);
	}
	/**
	*
	*/
	CRect& GetRect()
	{
		return m_rect;
	}
	/**
	*
	*/
	void SetRect(const CRect& rect)
	{
		m_rect = rect;
	}
	/**
	*
	*/
	DWORD GetId()
	{
		return m_id;
	}
	/**
	*
	*/
	static DWORD GenID()
	{
		static DWORD cid=1;
		return cid++;
	}
	/**
	*
	*/
	static DWORD ActiveChild(const DWORD cid=0)
	{
		static DWORD s_activeChild=0;
		if (cid !=0)//set
		{
			s_activeChild = cid;
		}
		return s_activeChild;
	}
	/**
	*
	*/
	DWORD GetState()
	{
		return m_state;
	}
	/**
	*
	*/
	void SetState(const DWORD st)
	{
		m_state = st;
	}
	/**
	*
	*/
	CWindow& GetWnd()
	{
		return m_wnd;
	}
	/**
	*
	*/
	void SetWnd(HWND wnd)
	{
		m_wnd = wnd;
	}
protected:
	CRect					m_rect;
	DWORD				m_id;
	DWORD				m_state;
	CWindow			m_wnd;
	BOOL					m_focused;
};
/************************************************************************
* 
*
*
/************************************************************************/
class CDLabel : public CDControl
{
public:
	/**
	*
	*/
	CDLabel(LPCTSTR text,LPCTSTR bkimage,const DWORD bkmode=0,const DWORD id=CDControl::GenID()):
	  CDControl(id),m_text(text),m_pbkimage(0),
		  m_fontname(_T("宋体")),m_fontsize(9),m_fontcolor(RGB(0,0,0))
	  {
		  SetBkImage(bkimage,bkmode);
	  }
	  /**
	  *
	  */
	  void SetText(LPCTSTR text,LPCTSTR fname,int fsize,COLORREF fcolor)
	  {
		  m_text = text;
		  m_fontname = fname;
		  m_fontsize = fsize;
		  m_fontcolor = fcolor;
	  }
	  /**
	  *
	  */
	  CString& GetText()
	  {
		  return m_text;
	  }
	  /**
	  *
	  */
	  void SetBkImage(LPCTSTR name,const DWORD mode=0)
	  {
		  m_pbkimage = GetTmg()->Get(name);
		  m_bkMode =mode;
	  }
	  /**
	  *
	  */
	  virtual void Render(CDCHandle& dc)
	  {
		  if (m_pbkimage)
		  {
			  RenderEngine::DrawImage(dc,*m_pbkimage,m_bkMode,m_rect);
		  }

		  if (m_text.GetLength())
		  {
			  RenderEngine::DrawText(dc,m_text,m_rect,m_fontname,m_fontsize,m_fontcolor);
		  }
	  }
	  /**
	  *
	  */
	  int GetWidth()
	  {
		 int width = m_rect.Width();
		 if(!width && m_pbkimage)
		 {
			 width = m_pbkimage->GetWidth();
		 }
		 return width;
	  }
	  /**
	  *
	  */
	  int GetHeight()
	  {
		  int height = m_rect.Height();
		  if(!height && m_pbkimage)
		  {
			  height = m_pbkimage->GetHeight();
		  }
		  return height;
	  }
protected:
	CString						m_text;
	CString						m_fontname;
	COLORREF					m_fontcolor;
	int								m_fontsize;
	const CImage*			m_pbkimage;
	DWORD						m_bkMode;
};
/************************************************************************
* 
*
*
/************************************************************************/
class CDButton : public CDLabel
{
public:
	enum{BT_DISABLE,BT_NORMAL,BT_HOVERED};
	/**
	*
	*/
	CDButton(LPCTSTR text,LPCTSTR bkimage,LPCTSTR hbkimage,
		LPCTSTR disbkimage=0,const DWORD mode=4,const DWORD id=CDControl::GenID()):CDLabel(text,bkimage,mode,id)
	{
		SetHoverImage(hbkimage);
		SetDisableImage(disbkimage);
	}
	/**
	*
	*/
	void SetHoverImage(LPCTSTR name)
	{
		m_phoverimage = GetTmg()->Get(name);
	}
	/**
	*
	*/
	void SetDisableImage(LPCTSTR name)
	{
		m_pdisableimage= GetTmg()->Get(name);
	}
	/**
	*
	*/
	virtual void Render(CDCHandle& dc)
	{
		if(m_state==BT_NORMAL && m_pbkimage)
		{
			RenderEngine::DrawImage(dc,*m_pbkimage,m_bkMode,m_rect);
		}
		else if(m_state==BT_DISABLE && m_pdisableimage)
		{
			RenderEngine::DrawImage(dc,*m_pdisableimage,m_bkMode,m_rect);
		} 
		else if(m_state==BT_HOVERED && m_phoverimage)
		{
			RenderEngine::DrawImage(dc,*m_phoverimage,m_bkMode,m_rect);
		}

		if (m_text.GetLength())
		{
			dc.DrawText(m_text,m_text.GetLength(),m_rect,DT_LEFT|DT_BOTTOM);
		}
	}
	/**
	*
	*/
	virtual BOOL ProcessMessage(MSG* pMsg)
	{
		BOOL res = FALSE;//not handled
		CPoint curPt = pMsg->pt;
		ScreenToClient(m_wnd,&curPt);

		if (pMsg->message == WM_NCMOUSEMOVE || pMsg->message == WM_MOUSEMOVE)
		{
			if (IsHover(curPt))
			{
				bool needUpdate = (m_state != BT_HOVERED);
				m_state = BT_HOVERED;
				if (needUpdate)
				{
					InvalidateRect(m_wnd,m_rect,TRUE);
				}
				SetCursor(LoadCursor(0,IDC_HAND));
			}
			else
			{
				bool needUpdate = (m_state != BT_NORMAL);
				m_state = BT_NORMAL;
				if (needUpdate)
				{
					InvalidateRect(m_wnd,m_rect,TRUE);
				}
				SetCursor(LoadCursor(0,IDC_ARROW));
			}
		}
		else if (pMsg->message==WM_NCLBUTTONUP || pMsg->message==WM_LBUTTONUP)
		{
			if (IsHover(curPt))
			{
				WPARAM wParam = MAKEWPARAM(GetId(),0);
				PostMessage(m_wnd,WM_COMMAND,wParam,0);
			}
		}
		else if (pMsg->message == WM_SETCURSOR)
		{
			if (IsHover(curPt))
			{
				SetCursor(LoadCursor(0,IDC_HAND));
			}
			else
			{
				SetCursor(LoadCursor(0,IDC_ARROW));
			}
		}
		return CDControl::ProcessMessage(pMsg);
	}
protected:
	const CImage*		m_phoverimage;
	const CImage*		m_pdisableimage;
};
/************************************************************************
* 
*
*
/************************************************************************/
/**
*
*/
template< class T, class TBase = CEdit, class TWinTraits = CControlWinTraits >
class ATL_NO_VTABLE CSkinnedEditImpl : public CWindowImpl< T, TBase, TWinTraits >{

public:
	DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())
	// Message map and handlers
	BEGIN_MSG_MAP_EX(CSkinnedEditImpl)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(OCM_CTLCOLOREDIT, OnCtlColorEdit)
		DEFAULT_REFLECTION_HANDLER()
		END_MSG_MAP()
		/**
		*
		*/
		LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		_Initialize();
		return lRes;
	}
	/**
	*
	*/
	LRESULT OnCtlColorEdit(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CDCHandle dc( (HDC) wParam );
		return (LRESULT)Render(dc);
	}
public:
	/**
	*
	*/
	CSkinnedEditImpl():m_fontName(_T("宋体"))
	{
	}
	/**
	*
	*/
	virtual HBRUSH Render(CDCHandle& dc)
	{
		HBRUSH hbr =0;

		return hbr;
	}
	/**
	*
	*/
	void SetFontName(LPCTSTR name)
	{
		m_fontName = name;
		ATLASSERT(::IsWindow(m_hWnd));
		CRect wrect;
		GetWindowRect(wrect);

		CFontHandle orgfont(GetFont());
		if(orgfont.IsNull())
		{
			orgfont.CreateFont(wrect.Height()-4,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL,                 // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				m_fontName);                 // lpszFacename
			//
			SetFont(orgfont);
		}
		else
		{
			LOGFONT logfont={0};
			orgfont.GetLogFont(&logfont);
			logfont.lfWidth=0;
			//logfont.lfHeight -= wrect.Height()/11;
			logfont.lfHeight = wrect.Height()-4;
			StringCbCopy(logfont.lfFaceName,_countof(logfont.lfFaceName),m_fontName);
			CFontHandle newfont;
			newfont.CreateFontIndirect(&logfont);
			SetFont(newfont);
		}
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
			ATLASSERT(::lstrcmpi(szBuffer, TBase::GetWndClassName())==0);
		}
#endif
		BOOL bRet = CWindowImpl< T, TBase, TWinTraits >::SubclassWindow(hWnd);
		if( bRet )
		{
			_Initialize();
		}
		return bRet;
	}
protected:
	/**
	*
	*/
	void _Initialize()
	{
		SetFontName(m_fontName);
	}
protected:
	CString				m_fontName;
};
/**
*
*/
class CSkinnedEdit : public CSkinnedEditImpl<CSkinnedEdit>{
public:
	DECLARE_WND_CLASS(_T("das.skinnededit"))
	/**
	*
	*/
	CSkinnedEdit():m_txtcolor(CLR_INVALID)
	{	
	}
	/**
	*
	*/
	virtual HBRUSH Render(CDCHandle& dc)
	{
		dc.SetTextColor(m_txtcolor);
		//COLORREF txtColor = m_txtcolor;
		return m_brush;
	}
	/**
	*
	*/
	void SetBkImage(const CImage* image)
	{
		if (image && !image->IsNull())
		{
			if( !m_brush.IsNull() ) 
			{
				m_brush.DeleteObject();
			}
			m_brush.CreatePatternBrush(*image);
		}
	}
	/**
	*
	*/
	void SetTextColor(COLORREF clr)
	{
		m_txtcolor = clr;
	}
protected:
	COLORREF		m_txtcolor;
	CBrush			m_brush;
};

class CDEdit : public CDLabel
{
public:
	/**
	*
	*/
	CDEdit(LPCTSTR text,LPCTSTR bkimage,const DWORD mode=0,COLORREF clr=RGB(0,0,0),
		const DWORD id=CDControl::GenID()):CDLabel(text,bkimage,mode,id),m_textColor(clr)
	{

	}
	/**
	*
	*/
	virtual void Render(CDCHandle& dc)
	{
	}
	/**
	*
	*/
	virtual LRESULT Initialize()
	{
		if (!m_edit)
		{
			DWORD style=ES_LEFT|ES_AUTOHSCROLL|WS_VISIBLE|WS_CHILD;
			int cx = m_pbkimage->GetWidth();
			int cy = m_pbkimage->GetHeight();
			m_rect.right = m_rect.left+cx;
			m_rect.bottom=m_rect.top+cy;
			m_edit.Create(m_wnd,m_rect,0,style,0,m_id,0);

			m_edit.SetBkImage(m_pbkimage);
			m_edit.SetTextColor(m_textColor);
			m_edit.ShowWindow(SW_SHOWNOACTIVATE);
		}
		return TRUE;
	}
	/**
	*
	*/
	void SetTextColor(COLORREF color)
	{
		m_textColor = color;
	}
protected:
	CSkinnedEdit	m_edit;
	COLORREF			m_textColor;
};
/************************************************************************
* 
*
*
/************************************************************************/
class CDEditEx : public CDLabel
{
public:
	/**
	*
	*/
	CDEditEx(LPCTSTR text,LPCTSTR bkimage,const DWORD mode=0,
		const DWORD id=CDControl::GenID()):CDLabel(text,bkimage,mode,id),m_txtSize(0,0)
	{

	}
	/**
	*
	*/
	virtual void Render(CDCHandle& dc)
	{
		CDLabel::Render(dc);
	}
	/**
	*
	*/
	virtual BOOL ProcessMessage(MSG* pMsg)
	{
		BOOL res = FALSE;//not handled
		CPoint curPt = pMsg->pt;
		ScreenToClient(m_wnd,&curPt);

		if (pMsg->message == WM_MOUSEMOVE)
		{

		}
		else if (pMsg->message==WM_LBUTTONDOWN)
		{
			if (IsHover(curPt))
			{
				//GetCursorPos(&curPt);
				//ScreenToClient(&curPt);

				int txtPt_x = m_rect.left+m_txtSize.cx;
				if (curPt.x > txtPt_x)
				{
					curPt.x = txtPt_x;
				}
				curPt.y=m_rect.top;

				m_wnd.CreateSolidCaret(2,20);
				SetCaretPos(curPt.x,curPt.y);
				m_wnd.ShowCaret();
			}

		}
		else if (pMsg->message==WM_CHAR)
		{
			GetCaretPos(&curPt);

			if (m_rect.PtInRect(curPt))
			{
				int caret_offset = 0;
				if (pMsg->wParam ==0x08)
				{
					m_text.Delete(m_text.GetLength()-1);
					caret_offset = -m_txtSize.cx/(m_text.GetLength() ? m_text.GetLength() : 1);
				}
				else
				{
					m_text.AppendChar((TCHAR)(pMsg->wParam));
					caret_offset = m_txtSize.cx/(m_text.GetLength() ? m_text.GetLength() : 1);
				}

				m_wnd.InvalidateRect(m_rect);

				m_wnd.CreateSolidCaret(2,20);
				SetCaretPos(curPt.x+caret_offset,curPt.y);
				m_wnd.ShowCaret();

			}
		}

		return CDControl::ProcessMessage(pMsg);
	}
protected:
	/**
	*
	*/
	void InitCaret()
	{
		//ImmSimulateHotKey();
		//CreateCaret(m_wnd,);
	}
protected:
	CSize		m_txtSize;
};

#endif