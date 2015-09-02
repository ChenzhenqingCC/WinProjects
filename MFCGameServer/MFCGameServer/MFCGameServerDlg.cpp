
// MFCGameServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCGameServer.h"
#include "MFCGameServerDlg.h"
#include "afxdialogex.h"
#include "GameService.h"
#include "StringConv.h"
#include "GameDBConnection.h"
#include "Zoic/libxml2/include/libxml/parser.h"
#include "CCFileUtils.h"
#include "ServerConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMFCGameServerDlg 对话框



CMFCGameServerDlg::CMFCGameServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCGameServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCGameServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCGameServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCGameServerDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCGameServerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMFCGameServerDlg 消息处理程序

//wstring ANSIToUnicode(const string& str)
//{
//	int  len = 0;
//	len = str.length();
//	int  unicodeLen = ::MultiByteToWideChar(CP_ACP,
//		0,
//		str.c_str(),
//		-1,
//		NULL,
//		0);
//	wchar_t *  pUnicode;
//	pUnicode = new  wchar_t[unicodeLen + 1];
//	memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
//	::MultiByteToWideChar(CP_ACP,
//		0,
//		str.c_str(),
//		-1,
//		(LPWSTR)pUnicode,
//		unicodeLen);
//	wstring  rt;
//	rt = (wchar_t*)pUnicode;
//	delete  pUnicode;
//
//	return  rt;
//}


bool CMFCGameServerDlg::SendLogToMe(Zoic::LogContent* pLogContent)
{
	CMFCGameServerDlg * dlg = (CMFCGameServerDlg*)(AfxGetApp()->m_pMainWnd);
	if (dlg)
	{
		CListBox* pBoxOne;
		pBoxOne = (CListBox*)(dlg->GetDlgItem(IDC_LIST1));
		if (pBoxOne)
		{
			pBoxOne->InsertString(pBoxOne->GetCount(), ANSIToUniCode(pLogContent->getMessage()));
		}
	}
	return true;
}

BOOL CMFCGameServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	SetTimer(0, 10, NULL);
	g_LogMonitorSender->setSendMsgFunc(CMFCGameServerDlg::SendLogToMe);
	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}



void CMFCGameServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	g_GameService->run();
	SetTimer(0, 10, NULL);
	CDialog::OnTimer(nIDEvent);
}

void CMFCGameServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCGameServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCGameServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCGameServerDlg::OnBnClickedOk()
{
	const char *ip = "127.0.0.1";
	WORD port = 9000;
	g_GameService->setMaxSessions(5);
	g_GameService->setListenAddress(ip, port);
	g_GameService->start();
	g_DBConn->init();
	g_GameService->init();
	g_ServerConfig->init();
}


void CMFCGameServerDlg::OnBnClickedButton1()
{
	g_GameService->stop();
	g_DBConn->disconnect();
}
