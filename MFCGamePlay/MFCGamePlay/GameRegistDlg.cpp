// GameRegistDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCGamePlay.h"
#include "GameRegistDlg.h"
#include "afxdialogex.h"
#include "GameProto_Login.h"
#include "StringConv.h"
#include "CommandClient.h"

using namespace GameProto;


// GameRegistDlg 对话框

IMPLEMENT_DYNAMIC(GameRegistDlg, CDialogEx)

GameRegistDlg::GameRegistDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(GameRegistDlg::IDD, pParent)
{

}

GameRegistDlg::~GameRegistDlg()
{
}

void GameRegistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GameRegistDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &GameRegistDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// GameRegistDlg 消息处理程序


void GameRegistDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	CString accname;
	GetDlgItemText(IDC_REGNAME, accname);
	if (accname.GetLength() == 0)
	{
		MessageBoxA(NULL, "请输入账号名称", "", 0);
		return;
	}

	CString pass;
	GetDlgItemText(IDC_REGPASS, pass);
	if (pass.GetLength() == 0)
	{
		MessageBoxA(NULL, "请输入密码", "", 0);
		return;
	}
	
	CString passc;
	GetDlgItemText(IDC_REGPASSC, passc);
	if (passc.GetLength() == 0)
	{
		MessageBoxA(NULL, "请确认密码", "", 0);
		return;
	}

	if (pass == passc)
	{
		P_TS_CreateAcc_Req cmd;
		cmd.username = UniCodeToUTF8(accname.GetString());
		cmd.password = UniCodeToUTF8(pass.GetString());
		Zoic::Message msg;
		msg.ID(GameProto::TS_CREATEACC_REQ) << cmd;
		g_CommandClient->sendMessage(msg);
	}
	else
	{
		MessageBoxA(NULL, "密码不匹配", "", 0);
	}
}
