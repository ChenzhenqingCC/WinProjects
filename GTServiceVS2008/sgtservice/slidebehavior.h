////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	slide behavior   version:  1.0   ? date: 04/09/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) ztgame 2009 - All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef SLIDEBEHAVIOR_H
#define SLIDEBEHAVIOR_H
//////////////////////////////////////////////////////////////////////////
#define BUF_MAXSIZE 256
/************************************************************************/
/* Wnd Slide behavior
/1your wnd should derived from this class
/2 call Slide when your want to slide you wnd
*/
/************************************************************************/
template<class T>class CSlideBehavior
{
public:
	void Slide(BOOL isshow=FALSE)
	{
		T *pthis = static_cast<T*>(this);
		if(pthis->IsWindowVisible() && isshow)
		{
			return;
		}

		if (pthis->IsWindowVisible())
		{
			pthis->ShowWindow(SW_HIDE);
			WinAnimation(true);
		}
		else
		{
			WinAnimation(false);
			pthis->ShowWindow(SW_NORMAL);
		}
	}
protected:
	/**
	*
	*/
	static BOOL CALLBACK FindTrayWnd(HWND wnd,LPARAM lParam)
	{
		TCHAR szclassname[BUF_MAXSIZE]={0};
		GetClassName(wnd,szclassname,BUF_MAXSIZE-1);
		if (_tcscmp(szclassname,_T("TrayNotifyWnd")) ==0)
		{
			CRect *prect = (CRect*)lParam;
			GetWindowRect(wnd,prect);
			return FALSE;
		}
		if (_tcscmp(szclassname,_T("TrayClockWClass")) ==0)
		{
			CRect *prect = (CRect*)lParam;
			CRect rectclock;
			GetWindowRect(wnd,rectclock);
			prect->right = rectclock.left;
			return FALSE;
		}
		return TRUE;
	}
	/**
	*
	*/
	void WinAnimation(BOOL isshow)
	{
		T *pthis = static_cast<T*>(this);

		CRect rect(0,0,0,0);
		CWindow pwnd = FindWindow(_T("Shell_TrayWnd"),NULL);
		if (pwnd)
		{
			pwnd.GetWindowRect(rect);
			EnumChildWindows(pwnd.m_hWnd,FindTrayWnd,(LPARAM)&rect);
			CRect rcwnd;
			pthis->GetWindowRect(rcwnd);
			if (isshow)
			{
				DrawAnimatedRects(pthis->m_hWnd,IDANI_CAPTION,rcwnd,rect);
			}
			else
			{
				DrawAnimatedRects(pthis->m_hWnd,IDANI_CAPTION,rect,rcwnd);
			}
		}
	}
	/**
	*
	*/
	//void DrawAnimatedRectsByTime(HWND wnd,CRect rc_src,CRect rc_tag,DWORD delay)
	//{
	//	int dy = (rc_tag.top - rc_src.top);
	//	int dx = (rc_tag.left - rc_src.left);
	//	int curtime = 2;
	//	CRect currect=rc_src;
	//	CRect oldrect=rc_src;
	//	while (curtime <1000)
	//	{
	//		
	//		currect.OffsetRect(dx*curtime/1000,dy*curtime/1000);
	//		DrawAnimatedRects(wnd,IDANI_CAPTION,oldrect,currect);
	//		oldrect = currect;
	//		curtime++;
	//		//Sleep(delay);
	//	}
	//}
};
#endif