// PlayerListDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCGamePlay.h"
#include "PlayerListDlg.h"
#include "afxdialogex.h"


// PlayerListDlg 对话框

IMPLEMENT_DYNAMIC(PlayerListDlg, CDialogEx)

PlayerListDlg::PlayerListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(PlayerListDlg::IDD, pParent)
{

}

PlayerListDlg::~PlayerListDlg()
{
}

void PlayerListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PlayerListDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &PlayerListDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// PlayerListDlg 消息处理程序


void PlayerListDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
}
