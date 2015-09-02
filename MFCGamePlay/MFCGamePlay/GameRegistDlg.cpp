// GameRegistDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCGamePlay.h"
#include "GameRegistDlg.h"
#include "afxdialogex.h"
#include "GameProto_Login.h"
#include "StringConv.h"
#include "CommandClient.h"

using namespace GameProto;


// GameRegistDlg �Ի���

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


// GameRegistDlg ��Ϣ�������


void GameRegistDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString accname;
	GetDlgItemText(IDC_REGNAME, accname);
	if (accname.GetLength() == 0)
	{
		MessageBoxA(NULL, "�������˺�����", "", 0);
		return;
	}

	CString pass;
	GetDlgItemText(IDC_REGPASS, pass);
	if (pass.GetLength() == 0)
	{
		MessageBoxA(NULL, "����������", "", 0);
		return;
	}
	
	CString passc;
	GetDlgItemText(IDC_REGPASSC, passc);
	if (passc.GetLength() == 0)
	{
		MessageBoxA(NULL, "��ȷ������", "", 0);
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
		MessageBoxA(NULL, "���벻ƥ��", "", 0);
	}
}
