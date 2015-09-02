// ChildDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PngBkTemplate.h"
#include "ChildDlg.h"
#include "afxdialogex.h"


// ChildDlg �Ի���

IMPLEMENT_DYNAMIC(ChildDlg, CDialog)

ChildDlg::ChildDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ChildDlg::IDD, pParent)
{

}

ChildDlg::~ChildDlg()
{
}

void ChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ChildDlg, CDialog)
	ON_WM_CREATE()
	//ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// ChildDlg ��Ϣ�������


int ChildDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//if (CDialog::OnCreate(lpCreateStruct) == -1)
		//return -1;

	// TODO:  �ڴ������ר�õĴ�������
	CDC* pDC = GetDC();
	m_pMemDC = new CDC();
	m_pBitmap = new CBitmap();
	m_nScreenX = 1024;
	m_nScreenY = 640;
	m_pMemDC->CreateCompatibleDC(pDC);
	m_pBitmap->CreateCompatibleBitmap(pDC, m_nScreenX, m_nScreenY);
	CBitmap* oldBitmap = m_pMemDC->SelectObject(m_pBitmap);
	CBrush brush(RGB(255, 255, 255));
	CRect rect;
	GetClientRect(&rect);
	m_pMemDC->FillRect(CRect(rect.left, rect.top, m_nScreenX, m_nScreenY), &brush);
	m_pMemDC->SelectObject(oldBitmap);
	ReleaseDC(pDC);

	m_BK.SetImage(IDB_PNG_BK2, L"PNG");
	m_Img1.SetImage(IDB_PNG_IMG, L"PNG");

	return CDialog::OnCreate(lpCreateStruct);
}


HBRUSH ChildDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG)
	{
		return m_brush;
	}

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void ChildDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_pMemDC)
	{
		delete m_pMemDC;
		m_pMemDC = NULL;
	}

	if (m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
	// TODO:  �ڴ˴������Ϣ����������
}


void ChildDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDC* pDC = GetDC();
		CRect rect;
		GetClientRect(rect);
		CBitmap* oldBitmap = m_pMemDC->SelectObject(m_pBitmap);
		DrawSomething();
		pDC->BitBlt(rect.left, rect.top, m_DrawRect.Width(), m_DrawRect.Height(), m_pMemDC, m_DrawRect.left, m_DrawRect.top, SRCCOPY);
		m_pMemDC->SelectObject(oldBitmap);
	}
}


BOOL ChildDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return TRUE;
}


BOOL ChildDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//COLORREF transColor = RGB(0, 255, 0);
	//m_brush.CreateSolidBrush(transColor);

	//DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	//::SetWindowLong(m_hWnd, GWL_STYLE, dwExStyle | 0x80000);

	//::SetLayeredWindowAttributes(m_hWnd, transColor, 0, 1);
	SetTimer(2000, 10, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void ChildDlg::DrawSomething()
{
	m_BK.Render(m_pMemDC->GetSafeHdc(), PointF(0, 0));
	m_Img1.Render(m_pMemDC->GetSafeHdc(), PointF(100, 100));
}


void ChildDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	::SendMessage(parentDlg->GetSafeHwnd(), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
	CDialog::OnLButtonDown(nFlags, point);
}


void ChildDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
		case 2000:
		{
			Invalidate();
		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}
