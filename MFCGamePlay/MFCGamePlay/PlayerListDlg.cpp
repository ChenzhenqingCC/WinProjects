// PlayerListDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCGamePlay.h"
#include "PlayerListDlg.h"
#include "afxdialogex.h"


// PlayerListDlg �Ի���

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


// PlayerListDlg ��Ϣ�������


void PlayerListDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
