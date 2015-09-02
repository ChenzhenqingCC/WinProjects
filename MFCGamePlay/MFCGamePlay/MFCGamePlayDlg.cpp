
// MFCGamePlayDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCGamePlay.h"
#include "MFCGamePlayDlg.h"
#include "afxdialogex.h"
#include "CommandClient.h"
#include "StringConv.h"

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


// CMFCGamePlayDlg 对话框



CMFCGamePlayDlg::CMFCGamePlayDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCGamePlayDlg::IDD, pParent)
	, nowShowDlg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCGamePlayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCGamePlayDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMFCGamePlayDlg 消息处理程序

BOOL CMFCGamePlayDlg::OnInitDialog()
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

	//GetDlgItem()
	g_LogMonitorSender->setSendMsgFunc(CMFCGamePlayDlg::SendLogToMe);

	ADDDLG_DEFINE(GameLoginDlg, LOGINSTAGE);
	ADDDLG_DEFINE(GameRegistDlg, REGSITSTAGE);
	ADDDLG_DEFINE(GameCreateRoleDlg, CREATESTAGE);
	ADDDLG_DEFINE(PlayerListDlg, CHOOSESTAGE);
	SwitchStage(LOGINSTAGE);
	//SwitchStage(CREATESTAGE);

	// TODO:  在此添加额外的初始化代码
	SetTimer(0, 10, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCGamePlayDlg::SetPlayerList(std::vector<PlayerDBBase> & plist)
{
	std::map<MyStage, CDialogEx *>::iterator it = myStageMap.find(CHOOSESTAGE);
	if (it != myStageMap.end())
	{
		CDialogEx * pDlg = it->second;
		if (pDlg)
		{
			CListBox* pBoxOne = (CListBox*)(pDlg->GetDlgItem(IDC_PLAYERLIST));
			if (pBoxOne)
			{
				static const wchar_t* WorkDescs[] = {
					L"无",
					L"战士",
					L"法师",
					L"射手",
				};
				for (size_t i = 0; i < plist.size(); ++i)
				{
					CString s;
					CString nname = UTF8ToUniCode(plist[i].nickname);
					s.Format(_T("%s  %s  %d级"), nname.GetString(), WorkDescs[plist[i].worktype], plist[i].level);
					pBoxOne->InsertString(pBoxOne->GetCount(), s);
				}
			}
		}
	}
}

void CMFCGamePlayDlg::SwitchStage(MyStage mystage)
{
	if (nowShowDlg)
		nowShowDlg->ShowWindow(SW_HIDE);
	std::map<MyStage, CDialogEx *>::iterator it = myStageMap.find(mystage);
	if (it != myStageMap.end())
	{
		CDialogEx * pDlg = it->second;
		if (pDlg)
		{
			pDlg->ShowWindow(SW_SHOW);
			pDlg->CenterWindow();
			nowShowDlg = pDlg;
		}
	}
}

void CMFCGamePlayDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (g_CommandClient->isSocketValid())
		g_CommandClient->run();
	SetTimer(0, 10, NULL);
	CDialog::OnTimer(nIDEvent);
}

void CMFCGamePlayDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCGamePlayDlg::OnPaint()
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

void CMFCGamePlayDlg::PostNcDestroy()
{
	for (std::map<MyStage, CDialogEx *>::iterator it = myStageMap.begin(); it != myStageMap.end(); ++it)
	{
		if (it->second)
		{
			delete it->second;
		}
	}
	CDialogEx::PostNcDestroy();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCGamePlayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CMFCGamePlayDlg::SendLogToMe(Zoic::LogContent* pLogContent)
{
	CMFCGamePlayDlg * dlg = (CMFCGamePlayDlg*)(AfxGetApp()->m_pMainWnd);
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
