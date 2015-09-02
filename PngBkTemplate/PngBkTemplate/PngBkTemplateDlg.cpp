
// PngBkTemplateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PngBkTemplate.h"
#include "PngBkTemplateDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPngBkTemplateDlg �Ի���



CPngBkTemplateDlg::CPngBkTemplateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPngBkTemplateDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPngBkTemplateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPngBkTemplateDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOVE()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CPngBkTemplateDlg ��Ϣ�������

BOOL CPngBkTemplateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	//CMenu* pSysMenu = GetSystemMenu(FALSE);
	//if (pSysMenu != NULL)
	//{
	//	BOOL bNameValid;
	//	CString strAboutMenu;
	//	bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
	//	ASSERT(bNameValid);
	//	if (!strAboutMenu.IsEmpty())
	//	{
	//		pSysMenu->AppendMenu(MF_SEPARATOR);
	//		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	//	}
	//}

	//// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	////  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	COLORREF transColor = RGB(0, 255, 0);
	m_brush.CreateSolidBrush(transColor);

	DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	::SetWindowLong(m_hWnd, GWL_STYLE, dwExStyle | 0x80000);

	::SetLayeredWindowAttributes(m_hWnd, transColor, 0, 1);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPngBkTemplateDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPngBkTemplateDlg::OnPaint()
{
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
		pDC->BitBlt(rect.left, rect.top, m_nScreenX, m_nScreenY, m_pMemDC, rect.left, rect.top, SRCCOPY);
		m_pMemDC->SelectObject(oldBitmap);
		CDialogEx::OnPaint();
	}
}

HBRUSH CPngBkTemplateDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG)
	{
		return m_brush;
	}

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CPngBkTemplateDlg::OnDestroy()
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
}

void CPngBkTemplateDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	::SendMessage(parentDlg->GetSafeHwnd(), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
	CDialog::OnLButtonDown(nFlags, point);
}

void CPngBkTemplateDlg::DrawSomething()
{

}

BOOL CPngBkTemplateDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CPngBkTemplateDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
}

int CPngBkTemplateDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CDC* pDC = GetDC();
	m_pMemDC = new CDC();
	m_pBitmap = new CBitmap();
	m_nScreenX = 556;
	m_nScreenY = 397;
	m_pMemDC->CreateCompatibleDC(pDC);
	m_pBitmap->CreateCompatibleBitmap(pDC, m_nScreenX, m_nScreenY);
	CBitmap* oldBitmap = m_pMemDC->SelectObject(m_pBitmap);
	CBrush brush(RGB(255, 255, 255));
	CRect rect;
	GetClientRect(&rect);
	m_pMemDC->FillRect(CRect(rect.left, rect.top, m_nScreenX, m_nScreenY), &brush);
	m_pMemDC->SelectObject(oldBitmap);
	ReleaseDC(pDC);
	return 0;
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPngBkTemplateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

