// GameLoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCGamePlay.h"
#include "GameLoginDlg.h"
#include "afxdialogex.h"
#include "CommandClient.h"
#include "GameProto_Login.h"
#include "StringConv.h"
#include "MFCGamePlayDlg.h"


// GameLoginDlg 对话框

IMPLEMENT_DYNAMIC(GameLoginDlg, CDialogEx)

GameLoginDlg::GameLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(GameLoginDlg::IDD, pParent)
{

}

GameLoginDlg::~GameLoginDlg()
{
}

void GameLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GameLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &GameLoginDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_REGIST, &GameLoginDlg::OnBnClickedRegist)
END_MESSAGE_MAP()


// GameLoginDlg 消息处理程序


void GameLoginDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	CEdit* pEditAcc = (CEdit*)(GetDlgItem(IDC_EDIT1));
	CEdit* pEditPass = (CEdit*)(GetDlgItem(IDC_EDIT2));
	CString username;
	CString userpass;
	GetDlgItemText(IDC_EDIT1, username);
	if (username.GetLength() == 0)
	{
		MessageBoxA(NULL, "请输入账号", "", 0);
		return;
	}

	GetDlgItemText(IDC_EDIT2, userpass);
	if (userpass.GetLength() == 0)
	{
		MessageBoxA(NULL, "请输入密码", "", 0);
		return;
	}
	GameProto::P_TS_Login_Req loginMsg;
	loginMsg.username = UniCodeToUTF8(username.GetString());
	loginMsg.password = UniCodeToUTF8(userpass.GetString());

	Zoic::Message msg;
	msg.ID(GameProto::TS_LOGIN_REQ) << loginMsg;
	g_CommandClient->sendMessage(msg);
}


void GameLoginDlg::OnBnClickedButton2()
{
	CDialog::OnCancel();
	// TODO:  在此添加控件通知处理程序代码
}


void GameLoginDlg::OnBnClickedRegist()
{
	// TODO:  在此添加控件通知处理程序代码
	CMFCGamePlayDlg * dlg = (CMFCGamePlayDlg*)(AfxGetApp()->m_pMainWnd);
	if (dlg)
	{
		dlg->SwitchStage(REGSITSTAGE);
	}
}
