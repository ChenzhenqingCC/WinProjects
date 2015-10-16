// NewTreeListCtrlView.h : interface of the CNewTreeListCtrlView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWTREELISTCTRLVIEW_H__6279AD9C_3B25_47AB_BE59_20255873C9AF__INCLUDED_)
#define AFX_NEWTREELISTCTRLVIEW_H__6279AD9C_3B25_47AB_BE59_20255873C9AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TreeListCtrl.h"
#include "NewTreeListCtrlDoc.h"
#include "TreeToolTipCtrl.h"

class CNewTreeListCtrlView : public CView
{
protected: // create from serialization only
	CNewTreeListCtrlView();
	DECLARE_DYNCREATE(CNewTreeListCtrlView)

// Attributes
public:
	CNewTreeListCtrlDoc* GetDocument();
	CImageList m_cImageList;	

//attributes
public:
	BOOL m_RTL;
	CFont m_headerFont;
	CTreeListCtrl m_tree;
	CScrollBar m_horScrollBar;
private:	
	void DisplayToolTip(CPoint point,TVHITTESTINFO hInfo, CString colText);		
	CTreeToolTipCtrl m_tooltip;	
	int m_IntervalTime;
	int m_CurrentProgress;
	BOOL m_TLInitialized;	

// Operations
public:
	static LONG FAR PASCAL DummyWndProc(HWND, UINT, WPARAM, LPARAM);
	void Initialize();

public:
	//static void RegisterClass();	
	void SortTree(int nCol, BOOL bAscending, HTREEITEM hParent);	
	void ResetScrollBar();

	BOOL VerticalScrollVisible();
	BOOL HorizontalScrollVisible();

	int StretchWidth(int m_nWidth, int m_nMeasure);








// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewTreeListCtrlView)
	public:
		virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		virtual void OnDraw(CDC* pDC);  // overridden to draw this view
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
		virtual void OnInitialUpdate();		
		virtual BOOL PreTranslateMessage(MSG* pMsg);	
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNewTreeListCtrlView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNewTreeListCtrlView)
		afx_msg void OnSize(UINT nType, int cx, int cy);		
		afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);	    
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in NewTreeListCtrlView.cpp
inline CNewTreeListCtrlDoc* CNewTreeListCtrlView::GetDocument()
   { return (CNewTreeListCtrlDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWTREELISTCTRLVIEW_H__6279AD9C_3B25_47AB_BE59_20255873C9AF__INCLUDED_)
