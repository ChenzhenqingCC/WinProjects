// TranslaucentDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "TranslaucentDlg.h"
#include "afxdialogex.h"
#include "Public.h"


// CTranslaucentDlg 对话框

IMPLEMENT_DYNAMIC(CTranslaucentDlg, CDialog)

CTranslaucentDlg::CTranslaucentDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTranslaucentDlg::IDD, pParent)
, m_pMainDlg(NULL)
, m_pImage(NULL)
{

}

CTranslaucentDlg::~CTranslaucentDlg()
{
}

void CTranslaucentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTranslaucentDlg, CDialog)
	ON_WM_MOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTranslaucentDlg 消息处理程序

BOOL CTranslaucentDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	VERIFY((dwStyle & WS_POPUP) != 0);
	VERIFY((dwStyle & WS_CHILD) == 0);

	m_blend.BlendOp = 0;
	m_blend.BlendFlags = 0;
	m_blend.AlphaFormat = 1;
	m_blend.SourceConstantAlpha = 255;

	setImage();
	return TRUE;
}

void CTranslaucentDlg::setImage()
{
	GdiImageFromResource(&m_pImage, IDB_PNG_BK2, L"PNG");
	if (m_pImage)
	{
		MoveWindow(CRect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight()));
		this->CenterWindow();
		m_CenterArea = CRect(31, 24, m_pImage->GetWidth() - 27, m_pImage->GetHeight() - 23);
		ReSetChildDlg();
		m_pMainDlg->SetDrawRect(m_CenterArea);
		DrawTXBar();
	}	
}

void CTranslaucentDlg::DrawTXBar()
{
	RECT rct;
	GetWindowRect(&rct);

	HDC hdcTemp = GetDC()->m_hDC;
	HDC m_hdcMemory = CreateCompatibleDC(hdcTemp);
	HBITMAP hBitMap = CreateCompatibleBitmap(hdcTemp, rct.right, rct.bottom);
	SelectObject(m_hdcMemory, hBitMap);

	POINT ptWinPos = { rct.left, rct.top };

	Graphics graphics(m_hdcMemory);
	RECT rcClient;
	GetClientRect(&rcClient);

	graphics.DrawImage(m_pImage, Rect(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom));

	SIZE sizeWindow = { rct.right, rct.bottom };
	POINT ptSrc = { 0, 0 };

	DWORD dwExStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	if ((dwExStyle & 0x80000) != 0x80000)// 
		SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle ^ 0x80000);

	HDC hdcScreen = ::GetDC(m_hWnd);
	::UpdateLayeredWindow(m_hWnd, hdcScreen, &ptWinPos, &sizeWindow, m_hdcMemory, &ptSrc, 0, &m_blend, 2);
	graphics.ReleaseHDC(m_hdcMemory);
	::ReleaseDC(m_hWnd, hdcScreen);
	hdcScreen = NULL;
	::ReleaseDC(m_hWnd, hdcTemp);
	hdcTemp = NULL;
	DeleteObject(hBitMap);
	DeleteDC(m_hdcMemory);
	m_hdcMemory = NULL;

}

void CTranslaucentDlg::UpdateView()
{
	SIZE sizeWin = { m_pImage->GetWidth(), m_pImage->GetHeight() };
	DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	if ((dwExStyle & 0x80000) != 0x80000)
		::SetWindowLong(m_hWnd, GWL_STYLE, dwExStyle | 0x80000);
	HDC hDC = ::GetDC(m_hWnd);
	HDC hdcMemory = CreateCompatibleDC(hDC);

	HBITMAP hBitMap = CreateCompatibleBitmap(hDC, sizeWin.cx, sizeWin.cy);
	::SelectObject(hdcMemory, hBitMap);

	RECT rcWin;
	GetWindowRect(&rcWin);

	BITMAPINFOHEADER stBmpInfoHeader = { 0 };
	int nBytesPerLine = ((sizeWin.cx * 32 + 31) & (~31)) >> 3;
	stBmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	stBmpInfoHeader.biWidth = sizeWin.cx;
	stBmpInfoHeader.biHeight = sizeWin.cy;
	stBmpInfoHeader.biPlanes = 1;
	stBmpInfoHeader.biBitCount = 32;
	stBmpInfoHeader.biCompression = BI_RGB;
	stBmpInfoHeader.biClrUsed = 0;
	stBmpInfoHeader.biSizeImage = nBytesPerLine * sizeWin.cy;

	PVOID pvBits = NULL;
	HBITMAP hbmpMem = ::CreateDIBSection(NULL, (PBITMAPINFO)&stBmpInfoHeader, DIB_RGB_COLORS, &pvBits, NULL, 0);
	assert(hbmpMem != NULL);
	memset(pvBits, 0, sizeWin.cx * 4 * sizeWin.cy);
	if (hbmpMem)
	{
		HGDIOBJ hbmpOld = ::SelectObject(hdcMemory, hbmpMem);
		POINT ptWinPos = { rcWin.left, rcWin.top };
		Gdiplus::Graphics graph(hdcMemory);
		graph.SetSmoothingMode(Gdiplus::SmoothingModeNone);
		graph.DrawImage(m_pImage, 0, 0, sizeWin.cx, sizeWin.cy);

		POINT ptSrc = { 0, 0 };
		//UpdateLayeredWindowT(m_hWnd, hDC, &ptWinPos, &sizeWin, hdcMemory, &ptSrc, 0, &m_blend, 2);
		::UpdateLayeredWindow(m_hWnd, hDC, &ptWinPos, &sizeWin, hdcMemory, &ptSrc, 0, &m_blend, 2);

		graph.ReleaseHDC(hdcMemory);
		::SelectObject(hdcMemory, hbmpOld);
		::DeleteObject(hbmpMem);
	}
} 

void CTranslaucentDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	::SendMessage(GetSafeHwnd(), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
	CDialog::OnLButtonDown(nFlags, point);
}

int CTranslaucentDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_pMainDlg = new ChildDlg(this);
	if (m_pMainDlg)
	{
		m_pMainDlg->Create(IDD_CHILD_DIALOG, this);
		m_pMainDlg->ShowWindow(SW_SHOW);
		m_pMainDlg->SetParentDlg(this);
	}
	return 0;
}

void CTranslaucentDlg::ReSetChildDlg()
{
	CRect rcWin;
	GetWindowRect(&rcWin);
	rcWin.left += m_CenterArea.left;
	rcWin.top += m_CenterArea.top;
	rcWin.right = rcWin.left + m_CenterArea.Width();
	rcWin.bottom = rcWin.top + m_CenterArea.Height();
	if (m_pMainDlg)
	{
		m_pMainDlg->MoveWindow(rcWin);
	}
}

void CTranslaucentDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ReSetChildDlg();
}

LRESULT CTranslaucentDlg::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
	if (m_pMainDlg)
	{
		::PostMessage(m_pMainDlg->m_hWnd, WM_RESTART, 0, 0);
	}
	return TRUE;
}

void CTranslaucentDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_pMainDlg)
	{
		delete m_pMainDlg;
		m_pMainDlg = NULL;
	}

	if (m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
}
