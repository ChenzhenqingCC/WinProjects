// NewTreeListCtrlView.cpp : implementation of the CNewTreeListCtrlView class
//

#include "stdafx.h"
#include "NewTreeListCtrl.h"

#include "NewTreeListCtrlDoc.h"
#include "NewTreeListCtrlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define ID_EDIT_FIELD   333
#define ID_TREE_LIST_HEADER 337
#define ID_TREE_LIST_CTRL   373
#define ID_TREE_LIST_SCROLLBAR   377

/////////////////////////////////////////////////////////////////////////////
// CNewTreeListCtrlView

IMPLEMENT_DYNCREATE(CNewTreeListCtrlView, CView)

BEGIN_MESSAGE_MAP(CNewTreeListCtrlView, CView)
	//{{AFX_MSG_MAP(CNewTreeListCtrlView)
		ON_WM_SIZE()
		ON_WM_HSCROLL()		
		ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	// Standard printing commands	
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNewTreeListCtrlView construction/destruction

CNewTreeListCtrlView::CNewTreeListCtrlView()
//	: CFormView(CNewTreeListCtrlView::IDD)
{
	m_TLInitialized=FALSE;	
	m_CurrentProgress=0;
}

CNewTreeListCtrlView::~CNewTreeListCtrlView()
{
}

BOOL CNewTreeListCtrlView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CNewTreeListCtrlView drawing

void CNewTreeListCtrlView::OnDraw(CDC* pDC)
{
}

/////////////////////////////////////////////////////////////////////////////
// CNewTreeListCtrlView diagnostics

#ifdef _DEBUG
void CNewTreeListCtrlView::AssertValid() const
{
	CView::AssertValid();
}

void CNewTreeListCtrlView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNewTreeListCtrlDoc* CNewTreeListCtrlView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNewTreeListCtrlDoc)));
	return (CNewTreeListCtrlDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNewTreeListCtrlView message handlers

void CNewTreeListCtrlView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	Initialize();	

	// create image list for tree
	m_cImageList.Create(IDB_TREE_PICTURES, 16, 10, RGB(255, 255, 255));
	m_tree.SetImageList(&m_cImageList, TVSIL_NORMAL);

	CRect m_wndRect;
	GetClientRect(&m_wndRect);
	MoveWindow(0, 0, m_wndRect.Width(), m_wndRect.Height());

	// insert Columns in list for tree
	{
		m_tree.InsertColumn(0, "Order Num", LVCFMT_LEFT, 260);
		m_tree.InsertColumn(1, "ID", LVCFMT_RIGHT, 60);
		m_tree.InsertColumn(2, "Seq", LVCFMT_RIGHT, 60);		
	}
		
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	m_tooltip.AddTool(GetDlgItem(ID_TREE_LIST_CTRL),"whatever");

	m_TLInitialized = TRUE;		

	// populate tree list
	HTREEITEM hTmp;
		
	hTmp = m_tree.InsertItem("parent1",0,0,0,0);
	HTREEITEM Parent1=hTmp;

	hTmp = m_tree.InsertItem("child1ofparent1",hTmp);	
	m_tree.SetItemText(hTmp,1,"child1col2text");
	m_tree.SetItemText(hTmp,2,"child1col3text");
	hTmp = m_tree.InsertItem("child1ofchild1",hTmp);	
	m_tree.SetItemText(hTmp,1,"col2text");
	m_tree.SetItemText(hTmp,2,"col3text");
	hTmp = m_tree.InsertItem("child2ofparent1",Parent1);	
	m_tree.SetItemText(hTmp,1,"child2col2text");
	m_tree.SetItemText(hTmp,2,"child2col3text");
	hTmp = m_tree.InsertItem("child1ofchild2",hTmp);	
	m_tree.SetItemText(hTmp,1,"col2text");
	m_tree.SetItemText(hTmp,2,"col3text");
	
	hTmp = m_tree.InsertItem("parent2",NULL);
	
	hTmp = m_tree.InsertItem("parent3",1,1,NULL);
	hTmp = m_tree.InsertItem("childofparent3",hTmp);	
	m_tree.SetItemText(hTmp,1,"col2text");
	m_tree.SetItemText(hTmp,2,"col3text");
}


/////////////////////////////////////////////////////////////////////////////
// CNewTreeListCtrlView message handlers

LONG FAR PASCAL CNewTreeListCtrlView::DummyWndProc(HWND h, UINT u, WPARAM w, LPARAM l)
{
	return ::DefWindowProc(h, u, w, l);
}

void CNewTreeListCtrlView::Initialize()
{
	// creates all the objects in frame -
	// header, tree, horizontal scroll bar

	CRect m_wndRect;
	GetWindowRect(&m_wndRect);
	CRect m_headerRect;

	// create the header
	{
		m_headerRect.left = m_headerRect.top = -1;
		m_headerRect.right = m_wndRect.Width();

		m_tree.m_wndHeader.Create(WS_CHILD | WS_VISIBLE | HDS_BUTTONS | HDS_HORZ, m_headerRect, this, ID_TREE_LIST_HEADER);
	}

	CSize textSize;
	// set header's pos, dimensions and image list
	{
		CDC *pDC = m_tree.m_wndHeader.GetDC();
		pDC->SelectObject(&m_tree.m_headerFont);
		textSize = pDC->GetTextExtent("A");
		m_tree.m_wndHeader.ReleaseDC(pDC); 

		m_tree.m_wndHeader.SetWindowPos(&wndTop, -1, -1, m_headerRect.Width(), textSize.cy+4, SWP_SHOWWINDOW);

		m_tree.m_cImageList.Create(IDB_HEADER, 16, 10, 0);
		m_tree.m_wndHeader.SetImageList(&m_tree.m_cImageList);		
	}

	CRect m_treeRect;

	// create the tree itself
	{
		GetWindowRect(&m_wndRect);

		m_treeRect.left=0;
		m_treeRect.top = textSize.cy+4;
		m_treeRect.right = m_headerRect.Width()-5;
		m_treeRect.bottom = m_wndRect.Height()-GetSystemMetrics(SM_CYHSCROLL)-4;

		m_tree.Create(WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,m_treeRect, this, ID_TREE_LIST_CTRL);
	}

	// finally, create the horizontal scroll bar
	{
		CRect m_scrollRect;
		m_scrollRect.left=0;
		m_scrollRect.top = m_treeRect.bottom;
		m_scrollRect.right = m_treeRect.Width()-GetSystemMetrics(SM_CXVSCROLL);
		m_scrollRect.bottom = m_wndRect.bottom;

		m_horScrollBar.Create(WS_CHILD | WS_VISIBLE | WS_DISABLED | SBS_HORZ | SBS_TOPALIGN, m_scrollRect, this, ID_TREE_LIST_SCROLLBAR);
	}

	SortTree(0, TRUE, m_tree.GetRootItem());	
}

BOOL CNewTreeListCtrlView::VerticalScrollVisible()
{	
	int sMin, sMax;
	m_tree.GetScrollRange(SB_VERT, &sMin, &sMax);	
	return sMax!=0;
}

BOOL CNewTreeListCtrlView::HorizontalScrollVisible()
{
	int sMin, sMax;
	m_horScrollBar.GetScrollRange(&sMin, &sMax);
	return sMax!=0;
}

int CNewTreeListCtrlView::StretchWidth(int m_nWidth, int m_nMeasure)
{	
	return ((m_nWidth/m_nMeasure)+1)*m_nMeasure;//put the fixed for +1 in brackets f/0 error
}


void CNewTreeListCtrlView::ResetScrollBar()
{
	// resetting the horizontal scroll bar

	int m_nTotalWidth=0, m_nPageWidth;

	CRect m_treeRect;
	m_tree.GetClientRect(&m_treeRect);

	CRect m_wndRect;
	GetClientRect(&m_wndRect);

	CRect m_headerRect;
	m_tree.m_wndHeader.GetClientRect(&m_headerRect);

	CRect m_barRect;
	m_horScrollBar.GetClientRect(m_barRect);

	m_nPageWidth = m_treeRect.Width();

	m_nTotalWidth = m_tree.GetColumnsWidth();

	if(m_nTotalWidth > m_nPageWidth)
	{
		// show the scroll bar and adjust it's size
		{
			m_horScrollBar.EnableWindow(TRUE);

			m_horScrollBar.ShowWindow(SW_SHOW);

			// the tree becomes smaller
			m_tree.SetWindowPos(&wndTop, 0, 0, m_wndRect.Width(), m_wndRect.Height()-m_barRect.Height()-m_headerRect.Height(), SWP_NOMOVE);

			if(!VerticalScrollVisible())
				// i.e. vertical scroll bar isn't visible
			{
				m_horScrollBar.SetWindowPos(&wndTop, 0, 0, m_wndRect.Width(), m_barRect.Height(), SWP_NOMOVE);
			}
			else
			{
				m_horScrollBar.SetWindowPos(&wndTop, 0, 0, m_wndRect.Width() - GetSystemMetrics(SM_CXVSCROLL), m_barRect.Height(), SWP_NOMOVE);
			}
		}

		m_horScrollBar.SetScrollRange(0, m_nTotalWidth-m_nPageWidth);

		// recalculate the offset
		{
			CRect m_wndHeaderRect;
			m_tree.m_wndHeader.GetWindowRect(&m_wndHeaderRect);
			ScreenToClient(&m_wndHeaderRect);

			m_tree.m_nOffset = m_wndHeaderRect.left;
			m_horScrollBar.SetScrollPos(-m_tree.m_nOffset);
		}
	}
	else
	{
		m_horScrollBar.EnableWindow(FALSE);

		// we no longer need it, so hide it!
		{
			m_horScrollBar.ShowWindow(SW_HIDE);

			m_tree.SetWindowPos(&wndTop, 0, 0, m_wndRect.Width(), m_wndRect.Height() - m_headerRect.Height(), SWP_NOMOVE);
			// the tree takes scroll's place
		}

		m_horScrollBar.SetScrollRange(0, 0);

		// set scroll offset to zero
		{
			m_tree.m_nOffset = 0;
			m_tree.Invalidate();
			CRect m_headerRect;
			m_tree.m_wndHeader.GetWindowRect(&m_headerRect);
			CRect m_wndRect;
			GetClientRect(&m_wndRect);
			m_tree.m_wndHeader.SetWindowPos(&wndTop, m_tree.m_nOffset, 0, max(StretchWidth(m_tree.GetColumnsWidth(),m_wndRect.Width()),m_wndRect.Width()), m_headerRect.Height(), SWP_SHOWWINDOW);
		}
	}
}

void CNewTreeListCtrlView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CRect m_treeRect;
	m_tree.GetClientRect(&m_treeRect);
	
	// if horizontal scroll bar 
	if(pScrollBar == &m_horScrollBar)
	{
		int m_nCurPos = m_horScrollBar.GetScrollPos();
		int m_nPrevPos = m_nCurPos;
		// decide what to do for each diffrent scroll event
		switch(nSBCode)
		{
			case SB_LEFT:			m_nCurPos = 0;
									break;
			case SB_RIGHT:			m_nCurPos = m_horScrollBar.GetScrollLimit()-1;
									break;
			case SB_LINELEFT:		m_nCurPos = max(m_nCurPos-6, 0);
									break;
			case SB_LINERIGHT:		m_nCurPos = min(m_nCurPos+6, m_horScrollBar.GetScrollLimit()-1);
									break;
			case SB_PAGELEFT:		m_nCurPos = max(m_nCurPos-m_treeRect.Width(), 0);
									break;
			case SB_PAGERIGHT:		m_nCurPos = min(m_nCurPos+m_treeRect.Width(), m_horScrollBar.GetScrollLimit()-1);
									break;
			case SB_THUMBTRACK:
			case SB_THUMBPOSITION:  if(nPos==0)
									    m_nCurPos = 0;
								    else
									    m_nCurPos = min(StretchWidth(nPos, 6), m_horScrollBar.GetScrollLimit()-1);
								    break;
		}		

		m_horScrollBar.SetScrollPos(m_nCurPos);
		m_tree.m_nOffset = -m_nCurPos;

		// smoothly scroll the tree control
		{
			CRect m_scrollRect;
			m_tree.GetClientRect(&m_scrollRect);
			m_tree.ScrollWindow(m_nPrevPos - m_nCurPos, 0, &m_scrollRect, &m_scrollRect);
		}

		CRect m_headerRect;
		m_tree.m_wndHeader.GetWindowRect(&m_headerRect);
		CRect m_wndRect;
		GetClientRect(&m_wndRect);		

		m_tree.m_wndHeader.SetWindowPos(&wndTop, m_tree.m_nOffset, 0, max(StretchWidth(m_tree.GetColumnsWidth(),m_treeRect.Width()),m_wndRect.Width()), m_headerRect.Height(), SWP_SHOWWINDOW);
	}
	
	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CNewTreeListCtrlView::SortTree(int nCol, BOOL bAscending, HTREEITEM hParent)
{		
	HTREEITEM hChild=NULL;
	HTREEITEM hPa = hParent;
	
	while(hPa!=NULL)
	{
		if(m_tree.ItemHasChildren(hPa))
		{			
			m_tree.SortItems(nCol, bAscending, hPa);
			SortTree(nCol, bAscending, hChild);			
		}			

		hPa = m_tree.GetNextSiblingItem(hPa);
	}
	
	/*else
	{	
		HTREEITEM ChildOfParent=NULL;
		HTREEITEM ChildOfPChild=NULL;
		while(hPa!=NULL)
		{
			if(m_tree.ItemHasChildren(hPa))
			{
				ChildOfParent=m_tree.GetChildItem(hPa);
				while(ChildOfParent!=NULL)
				{						
					m_tree.SortItems(nCol,bAscending,ChildOfParent);
					ChildOfParent = m_tree.GetNextSiblingItem(ChildOfParent);
				}
				
			}
			hPa = m_tree.GetNextSiblingItem(hPa);
		}	
	}*/		
}

BOOL CNewTreeListCtrlView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	HD_NOTIFY *pHDN = (HD_NOTIFY*)lParam;	

	if((wParam == ID_TREE_LIST_HEADER) && (pHDN->hdr.code == HDN_ITEMCLICK))
	{
		int nCol = pHDN->iItem;

		BOOL bAscending = FALSE;

		if(m_tree.m_wndHeader.GetItemImage(nCol)==-1)
			bAscending = TRUE;
		else
		if(m_tree.m_wndHeader.GetItemImage(nCol)==1)
			bAscending = TRUE;

		for(int i=0;i<m_tree.GetColumnsNum();i++)
		{
			m_tree.m_wndHeader.SetItemImage(i, -1);
		}

		if(bAscending)
			m_tree.m_wndHeader.SetItemImage(nCol, 0);
		else
			m_tree.m_wndHeader.SetItemImage(nCol, 1);

		m_tree.SortItems(nCol, bAscending, NULL);
		SortTree(nCol, bAscending, m_tree.GetRootItem());

		m_tree.UpdateWindow();
	}
	else
	if((wParam == ID_TREE_LIST_HEADER) && (pHDN->hdr.code == HDN_ITEMCHANGED))
	{
		int m_nPrevColumnsWidth = m_tree.GetColumnsWidth();
		m_tree.RecalcColumnsWidth();
		ResetScrollBar();

		// in case we were at the scroll bar's end,
		// and some column's width was reduced,
		// update header's position (move to the right).
		CRect m_treeRect;
		m_tree.GetClientRect(&m_treeRect);

		CRect m_headerRect;
		m_tree.m_wndHeader.GetClientRect(&m_headerRect);

		if((m_nPrevColumnsWidth > m_tree.GetColumnsWidth()) &&
		   (m_horScrollBar.GetScrollPos() == m_horScrollBar.GetScrollLimit()-1) &&
		   (m_treeRect.Width() < m_tree.GetColumnsWidth()))
		{
			m_tree.m_nOffset = -m_tree.GetColumnsWidth()+m_treeRect.Width();
			m_tree.m_wndHeader.SetWindowPos(&wndTop, m_tree.m_nOffset, 0, 0, 0, SWP_NOSIZE);
		}

		m_tree.Invalidate();
	}
	else
		GetParent()->SendMessage(WM_NOTIFY, wParam, lParam);
	
	return CView::OnNotify(wParam, lParam, pResult);
}

void CNewTreeListCtrlView::OnContextMenu(CWnd* pWnd, CPoint point) 
{	
	GetParent()->SendMessage(WM_CONTEXTMENU, (WPARAM)pWnd, MAKELPARAM(point.x, point.y) ); 
}

void CNewTreeListCtrlView::OnSize(UINT nType, int cx, int cy) 
{
	SetScrollRange(SB_HORZ, 0, 0);
	SetScrollRange(SB_VERT, 0, 0);

	if(m_TLInitialized)
	{	
		// resize all the controls	
		CRect m_wndRect;
		GetClientRect(&m_wndRect);

		CRect m_headerRect;
		m_tree.m_wndHeader.GetClientRect(&m_headerRect);
		m_tree.m_wndHeader.SetWindowPos(&wndTop, 0, 0, m_wndRect.Width(), m_headerRect.Height(), SWP_NOMOVE);

		CRect m_scrollRect;
		m_horScrollBar.GetClientRect(&m_scrollRect);

		m_tree.SetWindowPos(&wndTop, 0, 0, m_wndRect.Width(), m_wndRect.Height() - m_scrollRect.Height(), SWP_NOMOVE);

		CRect m_treeRect;
		m_tree.GetClientRect(&m_treeRect);
		m_horScrollBar.MoveWindow(0, m_treeRect.bottom, m_wndRect.Width(), m_scrollRect.Height());

		if(m_tree.GetColumnsWidth() > m_treeRect.Width())
		{
			// show the horz scroll bar
			{
				CRect m_barRect;
				m_horScrollBar.GetClientRect(&m_barRect);			

				m_horScrollBar.EnableWindow(TRUE);

				m_horScrollBar.ShowWindow(SW_SHOW);

				// the tree becomes smaller
				m_tree.SetWindowPos(&wndTop, 0, 0, m_wndRect.Width(), m_wndRect.Height()-m_barRect.Height()-m_headerRect.Height(), SWP_NOMOVE);

				int tfdsfsd=m_wndRect.Width() - GetSystemMetrics(SM_CXVSCROLL);
				m_horScrollBar.SetWindowPos(&wndTop, 0, 0, m_wndRect.Width() - GetSystemMetrics(SM_CXVSCROLL), m_barRect.Height(), SWP_NOMOVE);
				
			}

			m_horScrollBar.SetScrollRange(0, m_tree.GetColumnsWidth()-m_treeRect.Width());

		}
		else
		{
			// hide the scroll bar
			{
				m_horScrollBar.EnableWindow(FALSE);

				m_horScrollBar.ShowWindow(SW_HIDE);

				// the tree becomes larger
				m_tree.SetWindowPos(&wndTop, 0, 0, m_wndRect.Width(), m_wndRect.Height()-m_headerRect.Height(), SWP_NOMOVE);				
			}

			m_horScrollBar.SetScrollRange(0, 0);
		}

		m_tree.ResetVertScrollBar();
		
		// Reset the header control position and scroll
		// the tree control as required.
		int m_nCurPos = m_horScrollBar.GetScrollPos();
		m_tree.m_nOffset = -m_nCurPos;
	
		// smoothly scroll the tree control
	    {
		CRect m_scrollRect;
		m_tree.GetClientRect(&m_scrollRect);
		m_tree.ScrollWindow(m_nCurPos, 0, &m_scrollRect, &m_scrollRect);
		}
		m_tree.m_wndHeader.GetWindowRect(&m_headerRect);	
		GetClientRect(&m_wndRect);

		m_tree.m_wndHeader.SetWindowPos(&wndTop, m_tree.m_nOffset, 0,max(StretchWidth(m_tree.GetColumnsWidth(),m_treeRect.Width()),m_wndRect.Width()), m_headerRect.Height(),SWP_SHOWWINDOW);			
	}	
}


BOOL CNewTreeListCtrlView::PreTranslateMessage(MSG* pMsg) 
{	
	if(pMsg->message == WM_MOUSEMOVE && pMsg->hwnd == m_tree.m_hWnd)
	{		
		CDC* pDC;
		pDC=GetDC();
		HDC hDC=pDC->GetSafeHdc();
		CDC HeadDC;
		HeadDC.Attach(hDC);
		// Save DC
		int nSavedDC = HeadDC.SaveDC();
		CFont font;
		font.CreateFont(14,0,                        // nWidth
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
		DEFAULT_PITCH,			   // nPitchAndFamily
		NULL);                     // lpszFacename

		HeadDC.SelectObject(font);
		
		TVHITTESTINFO hInfo;
		CPoint point(LOWORD(pMsg->lParam),HIWORD(pMsg->lParam));
		hInfo.pt = point;	
		m_tree.HitTest(&hInfo);				
		
		if (hInfo.hItem!=NULL)
		{
			CArray<RECT,RECT> ColumnRectArray;
			CArray<int,int>	ColumnWidthArray;
			CArray<CString,CString>	ItemTextArray;
			CArray<CSize,CSize>	ItemTextWidthArray;
			
			for (int i=0;i<m_tree.m_wndHeader.GetItemCount();i++)
			{
				RECT ColumnRect;					
				m_tree.m_wndHeader.GetItemRect(i,&ColumnRect);
				ColumnRectArray.Add(ColumnRect);
				int WidthOfColumn=ColumnRect.right-ColumnRect.left;				
				ColumnWidthArray.Add(WidthOfColumn);
				CString Text=m_tree.GetItemText(hInfo.hItem,i);	
				ItemTextArray.Add(Text);
				CSize Width=pDC->GetTextExtent(Text);				
				ItemTextWidthArray.Add(Width);
			}		
			
			for (int i=0;i<m_tree.m_wndHeader.GetItemCount();i++)
			{
				if (i==0)
				{					
					CPoint point=GetCurrentMessage()->pt;
					RECT TextItemRect;
					m_tree.GetItemRect(hInfo.hItem,&TextItemRect,TRUE);					
					point.x=TextItemRect.right-int((TextItemRect.right-TextItemRect.left)/2);					
					if ( (hInfo.flags & TVHT_ONITEMRIGHT) && (hInfo.pt.x<ColumnRectArray.GetAt(i).right) && (ItemTextWidthArray.GetAt(i).cx>(ColumnWidthArray.GetAt(i)-TextItemRect.right-4-int((TextItemRect.right-TextItemRect.left)/2))) )					
					{				
						CString colText=m_tree.GetItemText(hInfo.hItem,i);	
						DisplayToolTip(point,hInfo,colText);				
						m_tooltip.RelayEvent(pMsg);	
						break;
					}
					else
						m_tooltip.Pop();					
				}
				else
				{
					if ((hInfo.pt.x>ColumnRectArray.GetAt(i-1).right) && (hInfo.pt.x<ColumnRectArray.GetAt(i).right) && (ItemTextWidthArray.GetAt(i).cx>(ColumnWidthArray.GetAt(i)-8)) )
					{						
						CString colText=m_tree.GetItemText(hInfo.hItem,i);							
						point.x=ColumnRectArray.GetAt(i).left;						
						DisplayToolTip(point,hInfo,colText);				
						m_tooltip.RelayEvent(pMsg);	
						break;
					}
					else 
						m_tooltip.Pop();			
				}
			}		
			
			ColumnRectArray.RemoveAll();
			ColumnWidthArray.RemoveAll();
			ItemTextArray.RemoveAll();
			ItemTextWidthArray.RemoveAll();
		}
		else
			m_tooltip.Pop();					
		
		// Restore dc

		HeadDC.RestoreDC( nSavedDC );

		// Detach the dc before returning
		HeadDC.Detach();

	}
	return CView::PreTranslateMessage(pMsg);
}


void CNewTreeListCtrlView::DisplayToolTip(CPoint point, TVHITTESTINFO hInfo, CString colText)
{	
	RECT ItemRect;
	CToolInfo ToolInfo;

	if (m_tooltip.GetToolInfo(ToolInfo,GetDlgItem(ID_TREE_LIST_CTRL)))
	{					
		short height=m_tree.GetItemHeight();
		m_tree.GetItemRect(hInfo.hItem,&ItemRect,TRUE);	
		
		point.y=ItemRect.top+height+3;
		ClientToScreen(&point);

		m_tooltip.SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&ToolInfo);
		m_tooltip.SendMessage(TTM_TRACKPOSITION, 0, (LPARAM) (DWORD)MAKELONG(point.x, point.y));				
						
		ToolInfo.uFlags|=TTF_TRACK; 
		ToolInfo.uFlags|=TTF_ABSOLUTE; 

		m_tooltip.SetToolInfo(&ToolInfo);
							
		if (hInfo.hItem==m_tree.GetSelectedItem())
		{		
			m_tooltip.SetTipBkColor(RGB(128,128,95));
			m_tooltip.SetTipTextColor(RGB(255,255,255));			
		}
		else
		{
			m_tooltip.SetTipBkColor(RGB(250,254,218));
			m_tooltip.SetTipTextColor(RGB(0,0,0));
		}
		m_tooltip.UpdateTipText(colText,&m_tree);								
		m_tooltip.Update();
	}						
}


