// GameCreateRoleDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCGamePlay.h"
#include "GameCreateRoleDlg.h"
#include "afxdialogex.h"
#include "GameTypes.h"
#include "GameProto_Login.h"
#include "StringConv.h"
#include "CommandClient.h"
using namespace GameEnum;
using namespace GameProto;


// GameCreateRoleDlg �Ի���

IMPLEMENT_DYNAMIC(GameCreateRoleDlg, CDialogEx)

GameCreateRoleDlg::GameCreateRoleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(GameCreateRoleDlg::IDD, pParent)
{

	
}

GameCreateRoleDlg::~GameCreateRoleDlg()
{
}

BOOL GameCreateRoleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	((CButton *)GetDlgItem(IDC_WORK_WARR))->SetCheck(TRUE);
	return TRUE;
}

void GameCreateRoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GameCreateRoleDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &GameCreateRoleDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// GameCreateRoleDlg ��Ϣ�������


void GameCreateRoleDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int idx = GetCheckedRadioButton(IDC_WORK_WARR, IDC_WORK_BOW);
	if (idx > 0)
	{
		DWORD worktype = idx - IDC_WORK_WARR + PLAYERWORK_WARR;
		CString nickname;
		GetDlgItemText(IDC_NICKNAME, nickname);
		if (nickname.GetLength() == 0)
		{
			MessageBoxA(NULL, "�������ɫ����", "", 0);
			return;
		}

		P_TS_CreatePlayer_Req cmd;
		cmd.type = worktype;
		cmd.name = UniCodeToUTF8(nickname.GetString());

		Zoic::Message msg;
		msg.ID(GameProto::TS_CREATEPLAYER_REQ) << cmd;
		g_CommandClient->sendMessage(msg);
	}
	else
	{
		MessageBoxA(NULL, "��ѡ��ְҵ", "", 0);
	}

}
