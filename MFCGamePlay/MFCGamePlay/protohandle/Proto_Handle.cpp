#include "stdafx.h"
#include "resource.h"
#include <Zoic/include/VectorMessage.h>
#include <Zoic/include/Message.h>
#include <Zoic/include/SessionCommand.h>
#include "GameProto_Login.h"
#include "CommandClient.h"
#include <Zoic/include/Log.h>
#include "MFCGamePlayDlg.h"

using namespace GameProto;
using namespace Zoic;
using namespace GameType;

SESSION_COMMAND(CommandClient, TS_CREATEPLAYER_REQ)
{
	//CommandClient * self = static_cast<CommandClient *>(pSession);
	CMFCGamePlayDlg * dlg = (CMFCGamePlayDlg*)(AfxGetApp()->m_pMainWnd);
	if (dlg)
	{
		dlg->SwitchStage(CREATESTAGE);
	}
	return true;
}

SESSION_COMMAND(CommandClient, TS_PLAYERLIST_ACK)
{
	P_TS_PlayerList_Ack pkt;
	msg >> pkt;
	CMFCGamePlayDlg * dlg = (CMFCGamePlayDlg*)(AfxGetApp()->m_pMainWnd);
	if (dlg)
	{
		dlg->SetPlayerList(pkt.playerlist);
		dlg->SwitchStage(CHOOSESTAGE);
	}
	return true;
}

static const char* MsgDescs[] = {
	"",
	"",
	"��¼",
	"����ɫ",
	"��ɫ�б�",
	"ע��",
};

static const char* ErrorDescs[] = {
	"�ɹ�",
	"���ݿ����",
	"�˺Ų�����",
	"�������",
	"����������",
	"������δ����",
	"�û��˺��ظ�",
	"ְҵ����",
	"�ǳ�Ϊ��",
	"�ǳ��Ѿ�����",
	"��ɫ����",
	"�˺Ŵ���",
};

SESSION_COMMAND(CommandClient, ST_USUAL_ACK)
{
	P_ST_Usual_Ack pkt;
	msg >> pkt;
	loginfo_f("%s: %s", MsgDescs[pkt.MsgType], ErrorDescs[pkt.code]);
	CMFCGamePlayDlg * dlg = (CMFCGamePlayDlg*)(AfxGetApp()->m_pMainWnd);
	switch (pkt.MsgType)
	{
	case TS_CREATEACC_REQ:
		{
							 if (pkt.code == ERROR_OK)
							 {
								 if (dlg)
								 {
									 dlg->SwitchStage(LOGINSTAGE);
								 }
							 }
		}
		break;
	case TS_CREATEPLAYER_REQ:
		{
								if (pkt.code == ERROR_OK)
								{
									if (dlg)
									{
										//dlg->SwitchStage(FIGHTSTAGE);
									}
								}
		}
	default:
		break;
	}
	return true;
}
