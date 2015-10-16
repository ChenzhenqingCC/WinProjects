// TreeListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TreeListCtrl.h"
#include "NewTreeListCtrlView.h"
#include "windowsx.h"  // required for GET_X_LPARAM, GET_Y_LPARAM)
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_TREE_LIST_HEADER 370


/////////////////////////////////////////////////////////////////////////////
// CTLItem

CTLItem::CTLItem()
{
	m_cEnding = '?';
	m_itemString = "";
	m_Bold = FALSE;
	m_Color = ::GetSysColor(COLOR_WINDOWTEXT);
}

CTLItem::CTLItem(CTLItem &copyItem)
{
	m_cEnding = copyItem.m_cEnding;
	m_itemString = copyItem.GetItemString();
	m_Bold = copyItem.m_Bold;
	m_Color = copyItem.m_Color;
	itemData = copyItem.itemData;
}

CString CTLItem::GetSubstring(int m_nSub)
{
	CString m_tmpStr("");
	int i=0, nHits=0;
	int length = m_itemString.GetLength();

	while((i<length) && (nHits<=m_nSub))
	{
		if(m_itemString[i]==m_cEnding)
		{
			nHits++;
		}
		else
		if(nHits==m_nSub)
			m_tmpStr+=m_itemString[i];

		i++;
	}

	if((i>=length) && (nHits<m_nSub))
		return "";
	else
		return m_tmpStr;
}

void CTLItem::SetSubstring(int m_nSub, CString m_sText)
{	
	CString m_tmpStr("");
	int i=0, nHits=0, first=0;
	int length = m_itemString.GetLength();

	while((i<length) && (nHits<=m_nSub))
	{
		if(m_itemString[i]==m_cEnding)
		{
			if(nHits!=m_nSub)
				first = i;
			nHits++;
		}

		i++;
	}

	CString m_newStr("");
	if((nHits>m_nSub) || ((nHits==m_nSub) && (i>=length)))
	{
		// insert in the middle
		if(first!=0)
		{
			m_newStr = m_itemString.Left(first);
			m_newStr += m_cEnding; 
		}
		m_newStr += m_sText;
		if(i<length)
		{
			m_newStr += m_cEnding;
			m_newStr += m_itemString.Right(m_itemString.GetLength()-i);
		}

		m_itemString=m_newStr;
	}
	else
	{
		// insert at the end
		for(i=nHits;i<m_nSub;i++)
			m_itemString+=m_cEnding;
		m_itemString+=m_sText;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTreeListCtrl

CTreeListCtrl::CTreeListCtrl()
{
	m_nColumns = m_nColumnsWidth = 0;
	m_nOffset = 0;
	m_selectedItem=NULL;	
}

CTreeListCtrl::~CTreeListCtrl()
{		
}


BEGIN_MESSAGE_MAP(CTreeListCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CTreeListCtrl)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()	
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()	
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()	
	ON_MESSAGE(UWM_TV_CHECKBOX, OnTvCheckbox)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_WM_TIMER()		
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeListCtrl message handlers

int CTreeListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{	
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
} 

HTREEITEM CTreeListCtrl::GetTreeItem(int nItem)
{
	HTREEITEM m_ParentItem = GetRootItem();
	int m_nCount = 0;

	while((m_ParentItem!=NULL) && (m_nCount<nItem))
	{
		m_nCount ++ ;
		GetNextSiblingItem(m_ParentItem);
	}

	return m_ParentItem;
}

int CTreeListCtrl::GetListItem(HTREEITEM hItem)
{
	HTREEITEM m_ParentItem = GetRootItem();
	int m_nCount = 0;

	while((m_ParentItem!=NULL) && (m_ParentItem!=hItem))
	{
		m_nCount ++ ;
		GetNextSiblingItem(m_ParentItem);
	}

	return m_nCount;
}

int CTreeListCtrl::InsertColumn( int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, int nSubItem)
{
	HD_ITEM hdi;
	hdi.mask = HDI_TEXT | HDI_FORMAT;
	if(nWidth!=-1)
	{
		hdi.mask |= HDI_WIDTH;
		hdi.cxy = nWidth;
	}
	
	hdi.pszText = (LPTSTR)lpszColumnHeading;
	hdi.fmt = HDF_OWNERDRAW;

	if(nFormat == LVCFMT_RIGHT)
		hdi.fmt |= HDF_RIGHT;
	else
	if(nFormat == LVCFMT_CENTER)
		hdi.fmt |= HDF_CENTER;
	else
		hdi.fmt |= HDF_LEFT;

	m_nColumns ++ ;

	int m_nReturn = m_wndHeader.InsertItem(nCol, &hdi);

	if(m_nColumns==1)
	{
		m_wndHeader.SetItemImage(m_nReturn, 0);
	}

	RecalcColumnsWidth();
	SetBkColor(RGB(250,254,218));
	return m_nReturn;
}

int CTreeListCtrl::GetColumnWidth(int nCol)
{
	HD_ITEM hItem;
	hItem.mask = HDI_WIDTH;
	if(!m_wndHeader.GetItem(nCol, &hItem))
		return 0;

	return hItem.cxy;
}

int CTreeListCtrl::GetColumnAlign(int nCol)
{
	HD_ITEM hItem;
	hItem.mask = HDI_FORMAT;
	if(!m_wndHeader.GetItem(nCol, &hItem))
		return LVCFMT_LEFT;

	if(hItem.fmt & HDF_RIGHT)
		return LVCFMT_RIGHT;
	else
	if(hItem.fmt & HDF_CENTER)
		return LVCFMT_CENTER;
	else
		return LVCFMT_LEFT;
}

void CTreeListCtrl::RecalcColumnsWidth()
{
	m_nColumnsWidth = 0;
	for(int i=0;i<m_nColumns;i++)
		m_nColumnsWidth += GetColumnWidth(i);
}

void CTreeListCtrl::DrawItemText (CDC* pDC, CString text, CRect rect, int nWidth, int nFormat)
{
    //
    // Make sure the text will fit in the prescribed rectangle, and truncate
    // it if it won't.
    //
    BOOL bNeedDots = FALSE;
    int nMaxWidth = nWidth - 4;

    while ((text.GetLength()>0) && (pDC->GetTextExtent((LPCTSTR) text).cx > (nMaxWidth - 4))) {
        text = text.Left (text.GetLength () - 1);
        bNeedDots = TRUE;
    }

    if (bNeedDots) {
        if (text.GetLength () >= 1)
            text = text.Left (text.GetLength () - 1);
        text += "...";
    }

    //
    // Draw the text into the rectangle using MFC's handy CDC::DrawText
    // function.
    //
    rect.right = rect.left + nMaxWidth;

    UINT nStyle = DT_VCENTER | DT_SINGLELINE;
    if (nFormat == LVCFMT_LEFT)
        nStyle |= DT_LEFT;
    else if (nFormat == LVCFMT_CENTER)
        nStyle |= DT_CENTER;
    else 
        nStyle |= DT_RIGHT;

	if((text.GetLength()>0) && (rect.right>rect.left))
		pDC->DrawText (text, rect, nStyle);
}

void CTreeListCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rcClip, rcClient;
	dc.GetClipBox( &rcClip );
	GetClientRect(&rcClient);

	// Set clip region to be same as that in paint DC
	CRgn rgn;
	rgn.CreateRectRgnIndirect( &rcClip );
	dc.SelectClipRgn(&rgn);
	rgn.DeleteObject();
	
	COLORREF m_wndColor = RGB(250,254,218);

	dc.SetViewportOrg(m_nOffset, 0);

	dc.SetTextColor(m_wndColor);
	 
	// First let the control do its default drawing.
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)dc.m_hDC, 0 );

	HTREEITEM hItem = GetFirstVisibleItem();

	int n = GetVisibleCount(), m_nWidth;

	dc.FillSolidRect(GetColumnWidth(0),1,rcClient.Width(),rcClient.Height(),m_wndColor);

	CTLItem *pItem;

	// the most urgent thing is to erase the labels that were drawn by the tree
	while(hItem!=NULL && n>=0)
	{
		CRect rect;

		UINT selflag = TVIS_DROPHILITED | TVIS_SELECTED;
	
		CRect m_labelRect;
		GetItemRect( hItem, &m_labelRect, TRUE );
		GetItemRect( hItem, &rect, FALSE );
		if(GetColumnsNum()>1)
			rect.left = min(m_labelRect.left, GetColumnWidth(0));
		else
			rect.left = m_labelRect.left;
		rect.right = m_nColumnsWidth;
		dc.FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),m_wndColor);

		hItem = GetNextVisibleItem( hItem );
		n--;
	}

	// create the font
	CFont *pFontDC;
	CFont fontDC, boldFontDC;
	LOGFONT logfont;

	CFont *pFont = GetFont();
	pFont->GetLogFont( &logfont );

	fontDC.CreateFontIndirect( &logfont );
	pFontDC = dc.SelectObject( &fontDC );

	logfont.lfWeight = 700;
	boldFontDC.CreateFontIndirect( &logfont );

	// and now let's get to the painting itself

	hItem = GetFirstVisibleItem();
	n = GetVisibleCount();
	while(hItem!=NULL && n>=0)
	{
		CRect rect;

		UINT selflag = TVIS_DROPHILITED | TVIS_SELECTED;
	
		if ( !(GetItemState( hItem, selflag ) & selflag ))
		{
			dc.SetBkMode(TRANSPARENT);

			pItem = (CTLItem *)CTreeCtrl::GetItemData(hItem);
			CString sItem = pItem->GetItemText();


			CRect m_labelRect;
			GetItemRect( hItem, &m_labelRect, TRUE );
			GetItemRect( hItem, &rect, FALSE );
			if(GetColumnsNum()>1)
				rect.left = min(m_labelRect.left, GetColumnWidth(0));
			else
				rect.left = m_labelRect.left;
			rect.right = m_nColumnsWidth;


			dc.SetBkColor( m_wndColor );

			dc.SetTextColor( pItem->m_Color );

			if(pItem->m_Bold)
			{
				dc.SelectObject( &boldFontDC );
			}

			DrawItemText(&dc, sItem, CRect(rect.left+2, rect.top, GetColumnWidth(0), rect.bottom), GetColumnWidth(0)-rect.left-2, GetColumnAlign(0));

			m_nWidth = 0;
			for(int i=1;i<m_nColumns;i++)
			{
				m_nWidth += GetColumnWidth(i-1);
				DrawItemText(&dc, pItem->GetSubstring(i), CRect(m_nWidth, rect.top, m_nWidth+GetColumnWidth(i), rect.bottom), GetColumnWidth(i), GetColumnAlign(i));
			}
			
			dc.SetTextColor(::GetSysColor (COLOR_WINDOWTEXT ));

			if(pItem->m_Bold)
			{
				dc.SelectObject( &fontDC );
			}
		}
		else
		{

			CRect m_labelRect;
			GetItemRect( hItem, &m_labelRect, TRUE );
			GetItemRect( hItem, &rect, FALSE );
			if(GetColumnsNum()>1)
				rect.left = min(m_labelRect.left, GetColumnWidth(0));
			else
				rect.left = m_labelRect.left;
			rect.right = m_nColumnsWidth;


			// If the item is selected, paint the rectangle with the system color
			// COLOR_HIGHLIGHT

			COLORREF m_highlightColor = ::GetSysColor (COLOR_HIGHLIGHT);

			CBrush brush(m_highlightColor);
			dc.FillRect (rect, &brush);

			// draw a dotted focus rectangle

			dc.DrawFocusRect (rect);
			
			pItem = (CTLItem *)CTreeCtrl::GetItemData(hItem);
			CString sItem = pItem->GetItemText();

			dc.SetBkColor(m_highlightColor);

			dc.SetTextColor(::GetSysColor (COLOR_HIGHLIGHTTEXT));
			
			if(pItem->m_Bold)
			{
				dc.SelectObject( &boldFontDC );
			}

			DrawItemText(&dc, sItem, CRect(rect.left+2, rect.top, GetColumnWidth(0), rect.bottom), GetColumnWidth(0)-rect.left-2, GetColumnAlign(0));

			m_nWidth = 0;
			for(int i=1;i<m_nColumns;i++)
			{
				m_nWidth += GetColumnWidth(i-1);
				DrawItemText(&dc, pItem->GetSubstring(i), CRect(m_nWidth, rect.top, m_nWidth+GetColumnWidth(i), rect.bottom), GetColumnWidth(i), GetColumnAlign(i));
			}

			if(pItem->m_Bold)
			{
				dc.SelectObject( &fontDC );
			}
		}

		hItem = GetNextVisibleItem( hItem );
		n--;
	}

	dc.SelectObject( pFontDC );
}

void CTreeListCtrl::ResetVertScrollBar()
{
	CNewTreeListCtrlView *pFrame = (CNewTreeListCtrlView*)GetParent();

	CRect m_treeRect;
	GetClientRect(&m_treeRect);

	CRect m_wndRect;
	pFrame->GetClientRect(&m_wndRect);

	CRect m_headerRect;
	m_wndHeader.GetClientRect(&m_headerRect);	

	CRect m_barRect;
	pFrame->m_horScrollBar.GetClientRect(&m_barRect);

	if(!pFrame->HorizontalScrollVisible())
		SetWindowPos(&wndTop, 0, 0, m_wndRect.Width(), m_wndRect.Height()-m_headerRect.Height(), SWP_NOMOVE);
	else
		SetWindowPos(&wndTop, 0, 0, m_wndRect.Width(), m_wndRect.Height()-m_barRect.Height()-m_headerRect.Height(), SWP_NOMOVE);

	if(pFrame->HorizontalScrollVisible())
	{
		if(!pFrame->VerticalScrollVisible())
		{
			pFrame->m_horScrollBar.SetWindowPos(&wndTop, 0, 0, m_wndRect.Width(), m_barRect.Height(), SWP_NOMOVE);
			
			int nMin, nMax;
			pFrame->m_horScrollBar.GetScrollRange(&nMin, &nMax);
			if((nMax-nMin) == (GetColumnsWidth()-m_treeRect.Width()+GetSystemMetrics(SM_CXVSCROLL)))
				// i.e. it disappeared because of calling
				// SetWindowPos
			{
				if(nMax - GetSystemMetrics(SM_CXVSCROLL) > 0)
					pFrame->m_horScrollBar.SetScrollRange(nMin, nMax - GetSystemMetrics(SM_CXVSCROLL));
				else
					// hide the horz scroll bar and update the tree
				{
					pFrame->m_horScrollBar.EnableWindow(FALSE);

					// we no longer need it, so hide it!
					{
						pFrame->m_horScrollBar.ShowWindow(SW_HIDE);

						SetWindowPos(&wndTop, 0, 0, m_wndRect.Width(), m_wndRect.Height() - m_headerRect.Height(), SWP_NOMOVE);
						// the tree takes scroll's place
					}

					pFrame->m_horScrollBar.SetScrollRange(0, 0);

					// set scroll offset to zero
					{
						m_nOffset = 0;
						Invalidate();
						m_wndHeader.GetWindowRect(&m_headerRect);
						m_wndHeader.SetWindowPos(&wndTop, m_nOffset, 0, max(pFrame->StretchWidth(GetColumnsWidth(),m_wndRect.Width()),m_wndRect.Width()), m_headerRect.Height(), SWP_SHOWWINDOW);
					}
				}
			}
		}
		else
		{
			pFrame->m_horScrollBar.SetWindowPos(&wndTop, 0, 0, m_wndRect.Width() - GetSystemMetrics(SM_CXVSCROLL), m_barRect.Height(), SWP_NOMOVE);

			int nMin, nMax;
			pFrame->m_horScrollBar.GetScrollRange(&nMin, &nMax);
			if((nMax-nMin) == (GetColumnsWidth()-m_treeRect.Width()-GetSystemMetrics(SM_CXVSCROLL)))
				// i.e. it appeared because of calling
				// SetWindowPos
			{
				pFrame->m_horScrollBar.SetScrollRange(nMin, nMax + GetSystemMetrics(SM_CXVSCROLL));
			}
		}
	}
	else
	if(pFrame->VerticalScrollVisible())
	{
		if(GetColumnsWidth()>m_treeRect.Width())
			// the vertical scroll bar takes some place
			// and the columns are a bit bigger than the client
			// area but smaller than (client area + vertical scroll width)
		{
			// show the horz scroll bar
			{
				pFrame->m_horScrollBar.EnableWindow(TRUE);

				pFrame->m_horScrollBar.ShowWindow(SW_SHOW);

				// the tree becomes smaller
				SetWindowPos(&wndTop, 0, 0, m_wndRect.Width(), m_wndRect.Height()-m_barRect.Height()-m_headerRect.Height(), SWP_NOMOVE);

				pFrame->m_horScrollBar.SetWindowPos(&wndTop, 0, 0, m_wndRect.Width() - GetSystemMetrics(SM_CXVSCROLL), m_barRect.Height(), SWP_NOMOVE);
			}

			pFrame->m_horScrollBar.SetScrollRange(0, GetColumnsWidth()-m_treeRect.Width());
		}
	}
}

void CTreeListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	UINT flags;
	m_selectedItem = HitTest(point, &flags);

	if((flags & TVHT_ONITEMRIGHT) || (flags & TVHT_ONITEMINDENT) ||
	   (flags & TVHT_ONITEM))
	{
		SelectItem(m_selectedItem);
	}	

	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CTreeListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if((GetColumnsNum()==0) || (point.x<GetColumnWidth(0)))
	{
		CTreeCtrl::OnLButtonDblClk(nFlags, point);
		ResetVertScrollBar();
	}

	SetFocus();
}

void CTreeListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	ResetVertScrollBar();
	m_selectedItem=GetSelectedItem();	
}

BOOL CTreeListCtrl::SetItemData(HTREEITEM hItem, DWORD dwData)
{
	CTLItem *pItem = (CTLItem *)CTreeCtrl::GetItemData(hItem);
	if(!pItem)
		return FALSE;
	pItem->itemData = dwData;
	return CTreeCtrl::SetItemData(hItem, (LPARAM)pItem);
}

DWORD CTreeListCtrl::GetItemData(HTREEITEM hItem) const
{
	CTLItem *pItem = (CTLItem *)CTreeCtrl::GetItemData(hItem);
	if(!pItem)
		return NULL;
	return pItem->itemData;
}				

HTREEITEM CTreeListCtrl::InsertItem(LPCTSTR lpszItem, HTREEITEM hParent, HTREEITEM hInsertAfter)
{	
	CTLItem *pItem = new CTLItem;
	pItem->InsertItem(lpszItem);
	m_nItems++;
	((CNewTreeListCtrlView*)GetParent())->ResetScrollBar();
	
	return CTreeCtrl::InsertItem(TVIF_PARAM|TVIF_TEXT, "", 0, 0, 0, 0, (LPARAM)pItem, hParent, hInsertAfter);
}

HTREEITEM CTreeListCtrl::InsertItem( LPCTSTR lpszItem, int nImage, int nSelectedImage, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	CTLItem *pItem = new CTLItem;
	pItem->InsertItem(lpszItem);
	m_nItems++;
	((CNewTreeListCtrlView*)GetParent())->ResetScrollBar();
	
	return CTreeCtrl::InsertItem(TVIF_PARAM|TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE, "", nImage, nSelectedImage, 0, 0, (LPARAM)pItem, hParent, hInsertAfter);
}

HTREEITEM CTreeListCtrl::InsertItem(UINT nMask, LPCTSTR lpszItem, int nImage, int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam, HTREEITEM hParent, HTREEITEM hInsertAfter )
{
	CTLItem *pItem = new CTLItem;
	pItem->InsertItem(lpszItem);
	pItem->itemData = lParam;
	m_nItems++;
	((CNewTreeListCtrlView*)GetParent())->ResetScrollBar();	
	
	return CTreeCtrl::InsertItem(nMask, "", nImage, nSelectedImage, nState, nStateMask, (LPARAM)pItem, hParent, hInsertAfter); 
}


HTREEITEM CTreeListCtrl::CopyItem(HTREEITEM hItem, HTREEITEM hParent, HTREEITEM hInsertAfter)
{	
	if(ItemHasChildren(hItem))
		return NULL;
	
	TV_ITEM item;
	item.mask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_STATE | TVIF_TEXT;
	item.hItem = hItem;	
	GetItem(&item);
	CTLItem *pItem = (CTLItem *)CTreeCtrl::GetItemData(hItem);
	CTLItem *pNewItem = new CTLItem(*pItem);

	item.lParam = (LPARAM)pNewItem;

	TV_INSERTSTRUCT insStruct;
	insStruct.item = item;
	insStruct.hParent = hParent;
	insStruct.hInsertAfter = hInsertAfter;	
	
	return CTreeCtrl::InsertItem(&insStruct);
}

HTREEITEM CTreeListCtrl::MoveItem(HTREEITEM hItem, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	if(ItemHasChildren(hItem))
		return NULL;
		
	TV_ITEM item;
	item.mask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_STATE | TVIF_TEXT;
	item.hItem = hItem;	
	GetItem(&item);
	CTLItem *pItem = (CTLItem *)CTreeCtrl::GetItemData(hItem);
	CTLItem *pNewItem = new CTLItem(*pItem);
	DeleteItem(hItem);
	
	item.lParam = (LPARAM)pNewItem;

	TV_INSERTSTRUCT insStruct;	
	insStruct.item = item;
	insStruct.hParent = hParent;
	insStruct.hInsertAfter = hInsertAfter;
	

	return CTreeCtrl::InsertItem(&insStruct);
}

BOOL CTreeListCtrl::SetItemText( HTREEITEM hItem, int nCol ,LPCTSTR lpszItem )
{
	CTLItem *pItem = (CTLItem *)CTreeCtrl::GetItemData(hItem);
	if(!pItem)
		return FALSE;
	pItem->SetSubstring(nCol, lpszItem);	
	return CTreeCtrl::SetItemData(hItem, (LPARAM)pItem);
}

BOOL CTreeListCtrl::SetItemColor( HTREEITEM hItem, COLORREF m_newColor, BOOL m_bInvalidate )
{
	CTLItem *pItem = (CTLItem *)CTreeCtrl::GetItemData(hItem);
	if(!pItem)
		return FALSE;
	pItem->m_Color = m_newColor;
	if(!CTreeCtrl::SetItemData(hItem, (LPARAM)pItem))
		return FALSE;
	if(m_bInvalidate)
		Invalidate();
	return TRUE;	
}

BOOL CTreeListCtrl::SetItemBold( HTREEITEM hItem, BOOL m_Bold, BOOL m_bInvalidate )
{
	CTLItem *pItem = (CTLItem *)CTreeCtrl::GetItemData(hItem);
	if(!pItem)
		return FALSE;
	pItem->m_Bold = m_Bold;
	if(!CTreeCtrl::SetItemData(hItem, (LPARAM)pItem))
		return FALSE;
	if(m_bInvalidate)
		Invalidate();
	return TRUE;
}

CString CTreeListCtrl::GetItemText( HTREEITEM hItem, int nSubItem )
{
	CTLItem *pItem = (CTLItem *)CTreeCtrl::GetItemData(hItem);
		
	if(!pItem)
		return _T("");
	return pItem->GetSubstring(nSubItem);
}

CString CTreeListCtrl::GetItemText( int nItem, int nSubItem )
{
	return GetItemText(GetTreeItem(nItem), nSubItem);
}

BOOL CTreeListCtrl::DeleteItem( HTREEITEM hItem )
{
	CTLItem *pItem = (CTLItem *)CTreeCtrl::GetItemData(hItem);
	if(!pItem)
		return FALSE;
	
	m_nItems --;
	
	return CTreeCtrl::DeleteItem(hItem);
}

BOOL CTreeListCtrl::DeleteItem( int nItem )
{
	return DeleteItem(GetTreeItem(nItem));
}

int CALLBACK CTreeListCtrl::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CTLItem *pItem1 = (CTLItem *)lParam1;
	CTLItem *pItem2 = (CTLItem *)lParam2;
	
	SSortType *pSortType = (SSortType *)lParamSort;

	CString str1 = pItem1->GetSubstring(pSortType->nCol);
	CString str2 = pItem2->GetSubstring(pSortType->nCol);
	
	int m_nComp;
	// compare the two strings, but
	// notice:
	// in this case, "xxxx10" comes after "xxxx2"
	{
		CString tmpStr1, tmpStr2;
		int index = str1.FindOneOf("0123456789");
		if(index!=-1)
			tmpStr1 = str1.Right(str1.GetLength()-index);
		index = str2.FindOneOf("0123456789");
		if(index!=-1)
			tmpStr2 = str2.Right(str2.GetLength()-index);
		
		tmpStr1 = tmpStr1.SpanIncluding("0123456789");
		tmpStr2 = tmpStr2.SpanIncluding("0123456789");

		if((tmpStr1=="") && (tmpStr2==""))
			m_nComp = str1.CompareNoCase(str2);
		else
		{
			int num1 = atoi(tmpStr1);
			int num2 = atoi(tmpStr2);

			tmpStr1 = str1.SpanExcluding("0123456789");
			tmpStr2 = str2.SpanExcluding("0123456789");

			if(tmpStr1 == tmpStr2)
			{
				if(num1 > num2)
					m_nComp = 1;
				else
				if(num1 < num2)
					m_nComp = -1;
				else
					m_nComp = str1.CompareNoCase(str2);
			}
			else
				m_nComp = str1.CompareNoCase(str2);			
		}
	}

	if(!pSortType->bAscending)
	{
		if(m_nComp == 1)
			m_nComp = -1;
		else
		if(m_nComp == -1)
			m_nComp = 1;
	}

	return m_nComp;
}

BOOL CTreeListCtrl::SortItems( int nCol, BOOL bAscending, HTREEITEM low)
{
	TV_SORTCB tSort;

	tSort.hParent = low;	
	tSort.lpfnCompare = CompareFunc;

	SSortType *pSortType = new SSortType;
	pSortType->nCol = nCol;
	pSortType->bAscending = bAscending;
	tSort.lParam = (LPARAM)pSortType;
	
	BOOL m_bReturn = SortChildrenCB(&tSort);
	
	delete pSortType;	
		
	return m_bReturn;
}

void CTreeListCtrl::MemDeleteAllItems(HTREEITEM hParent)
{
	HTREEITEM hItem = hParent;	
	CTLItem *pItem;	
	
	while(hItem!=NULL)
	{			
		pItem = (CTLItem *)CTreeCtrl::GetItemData(hItem);
		MemItemArray.Add(pItem);
		
		if(ItemHasChildren(hItem))
			MemDeleteAllItems(GetChildItem(hItem));			
				
		hItem = GetNextSiblingItem(hItem);		
	}	
}

BOOL CTreeListCtrl::DeleteAllItems()
{
	LockWindowUpdate();
	BeginWaitCursor();
	
	MemDeleteAllItems(GetRootItem());		
	BOOL m_bReturn = CTreeCtrl::DeleteAllItems();
	
	for (int i=0;i<MemItemArray.GetSize();i++)
		delete MemItemArray.GetAt(i);
	MemItemArray.RemoveAll();	

	EndWaitCursor();
	UnlockWindowUpdate();
	return m_bReturn;
}

void CTreeListCtrl::OnDestroy()
{	
	MemDeleteAllItems(GetRootItem());		
	CTreeCtrl::DeleteAllItems();
	for (int i=0;i<MemItemArray.GetSize();i++)
		delete MemItemArray.GetAt(i);
	MemItemArray.RemoveAll();

	CTreeCtrl::OnDestroy();
}

BOOL CTreeListCtrl::DeleteSubItems( HTREEITEM hItem )
{
  HTREEITEM hChild(0);
  BOOL bRet(false), bErr(false);

  while(ItemHasChildren(hItem)){
    hChild = GetChildItem(hItem);
    if (ItemHasChildren(hChild)){
      DeleteSubItems(hChild);
    }
    bRet = DeleteItem(hChild);
    if (!bRet){
      bErr = true;
    }
  }
  return bErr;
}

void CTreeListCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{	
	// find what item is selected
	m_selectedItem = HitTest(point, &nFlags);
	
	if((nFlags & TVHT_ONITEMRIGHT) || (nFlags & TVHT_ONITEMINDENT) ||
	   (nFlags & TVHT_ONITEM))
	{
		SelectItem(m_selectedItem);
	}	

	CTreeCtrl::OnRButtonDown(nFlags, point);
}


	


void CTreeListCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{		
	m_selectedItem=HitTest(point);	
	SelectItem(m_selectedItem);

	CTreeCtrl::OnLButtonUp(nFlags, point);
}

void CTreeListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{	
	SetFocus();
	
	if ( m_nHoverTimerID )
	{
		KillTimer( m_nHoverTimerID );
		m_nHoverTimerID = 0;
	}	
		
	// sets counter to expand if hovered over item for a while
	m_nHoverTimerID = SetTimer(2, 750, NULL);
			
		
	CTreeCtrl::OnMouseMove(nFlags, point);
}


CString CTreeListCtrl::GetNextLine(CString &message)
{
CString result;
BOOL bOK = FALSE;
char k = 13;
char j;
int nSize = message.GetLength();
int nIndex = 0;
	
	//Search for the return character in the string
	while(nIndex < nSize && !bOK)
	{
		j = message.GetAt(nIndex);
		if(j == k)
		{
			result = message.Left(nIndex);
			//Drop the carriage return and line feed characters
			message = message.Right(nSize - (nIndex + 2));
			bOK = TRUE;
		}
		nIndex++;
	}
	return result;
}


void CTreeListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{

  DWORD dw = GetMessagePos();                   // retrieve mouse cursor position when msg was sent
  CPoint p(GET_X_LPARAM(dw), GET_Y_LPARAM(dw)); // ..and put into point structure
  ScreenToClient(&p);                    // make coords local to tree client area

  UINT htFlags = 0;
  HTREEITEM it = HitTest(p, &htFlags);   // See where the click was on

  if (it != NULL && htFlags==TVHT_ONITEMSTATEICON) {   
    // the check box was hit.
    // we just post a message
    PostMessage(UWM_TV_CHECKBOX, pNMHDR->idFrom, (LPARAM) it);
  }
	
  *pResult = 0;
}


// ----- Handle checkbox changed here! ------
LRESULT CTreeListCtrl::OnTvCheckbox(WPARAM wp, LPARAM lp)
{  
/*  HTREEITEM hParent;
  HTREEITEM CurrentChild;
 
  HTREEITEM hitem = (HTREEITEM) lp;
  // get what state the checkbox is in
  int checked = (GetItemState(hitem, TVIS_STATEIMAGEMASK) >> 12) - 1;
  
  
	 HTREEITEM TempSelected=hitem;
	  hParent=GetParentItem(hitem);
	  
	  if (checked==1)		
	  {
	  	  SetItemState(hitem,INDEXTOSTATEIMAGEMASK(3), TVIS_STATEIMAGEMASK);	  		  
	  }
*/
  
  return 0;
}


void CTreeListCtrl::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == m_nHoverTimerID )
	{
		KillTimer( m_nHoverTimerID );
		m_nHoverTimerID = 0;

		if ( m_selectedItem )
		{
			SelectItem( m_selectedItem );
			Expand(m_selectedItem,TVE_EXPAND);	
		}
	}
	else
	{
		CTreeCtrl::OnTimer(nIDEvent);
	}
}


