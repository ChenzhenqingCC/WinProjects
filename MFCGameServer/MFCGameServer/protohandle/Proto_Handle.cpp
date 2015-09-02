#include "stdafx.h"
#include <Zoic/include/VectorMessage.h>
#include <Zoic/include/Message.h>
#include <Zoic/include/SessionCommand.h>
#include "GameProto_Login.h"
#include "GameService.h"
#include "GameSession.h"
#include <Zoic/include/Log.h>

using namespace GameProto;
using namespace Zoic;
using namespace GameType;

SESSION_COMMAND(GameSession, TS_LOGIN_REQ)
{
	GameSession * self = static_cast<GameSession *>(pSession);
	P_TS_Login_Req pkt;
	msg >> pkt;
	UserID uid = 0;
	ErrorType error = g_GameService->IsUserCanLogin(pkt.username, pkt.password, uid);
	if (uid > 0)
	{
		loginfo_f("用户[%s]登入成功!", std::string(pkt.username).c_str());
		P_TS_PlayerList_Ack plMsg;
		if (g_GameService->GetPlayerList(uid, plMsg.playerlist))
		{
			Zoic::Message msg;
			msg.ID(TS_PLAYERLIST_ACK) << plMsg;
			self->sendMessage(msg);
			self->setState(LS_CHOOSEROLE);
		}
		else
		{
			P_TS_CreatePlayer_Req createMsg;
			Zoic::Message msg;
			msg.ID(TS_CREATEPLAYER_REQ) << createMsg;
			self->sendMessage(msg);
			self->setState(LS_CREATEROLE);
		}
	}

	if (error > 0)
	{
		loginfo_f("用户[%s]登入失败!", std::string(pkt.username).c_str());
		P_ST_Usual_Ack logMsg;
		logMsg.MsgType = TS_CREATEPLAYER_REQ;
		logMsg.code = error;
		Zoic::Message msg;
		msg.ID(ST_USUAL_ACK) << logMsg;
		self->sendMessage(msg);
		self->setState(LS_NONE);
	}
	else
	{
		if (self)
			self->setUserAccID(uid);
	}
	return true;
}

SESSION_COMMAND(GameSession, TS_CREATEPLAYER_REQ)
{
	GameSession * self = static_cast<GameSession *>(pSession);
	P_TS_CreatePlayer_Req pkt;
	msg >> pkt;
	UserID uid = self->getUserAccID();
	if (uid > 0)
	{
		ServerError error = g_GameService->CreateRole(uid, pkt);
		if (error == ERROR_OK)
		{
			P_TS_PlayerList_Ack plMsg;
			if (g_GameService->GetPlayerList(uid, plMsg.playerlist))
			{
				Zoic::Message msg;
				msg.ID(TS_PLAYERLIST_ACK) << plMsg;
				self->sendMessage(msg);
				self->setState(LS_CHOOSEROLE);
			}
		}
		else
		{
			P_ST_Usual_Ack ccMsg;
			ccMsg.MsgType = TS_CREATEPLAYER_REQ;
			ccMsg.code = error;
			Zoic::Message msg;
			msg.ID(ST_USUAL_ACK) << ccMsg;
			self->sendMessage(msg);
		}
	}
	return true;
}

SESSION_COMMAND(GameSession, TS_CREATEACC_REQ)
{
	GameSession * self = static_cast<GameSession *>(pSession);
	P_TS_CreateAcc_Req pkt;
	msg >> pkt;

	ServerError error = g_GameService->CreateAccount(pkt);
	P_ST_Usual_Ack ccMsg;
	ccMsg.MsgType = TS_CREATEACC_REQ;
	ccMsg.code = error;

	Zoic::Message msgAck;
	msgAck.ID(ST_USUAL_ACK) << ccMsg;
	self->sendMessage(msgAck);
	self->setState(LS_NONE);
	return true;
}

SESSION_COMMAND(GameSession, TS_CHOOSEROLE_REQ)
{
	GameSession * self = static_cast<GameSession *>(pSession);
	UserID uid = self->getUserAccID();
	P_TS_ChooseRole_Req pkt;
	msg >> pkt;

	P_TS_MainRoleData_Ack mainMsg;
	if (g_GameService->FillPlayerBase(uid, mainMsg.player))
	{
		g_GameService->CalculateRoleProps(mainMsg.player);
		Zoic::Message msg;
		msg.ID(TS_MAINROLEDATA_ACK) << mainMsg;
		self->sendMessage(msg);
		self->setState(LS_ENTERED);
	}
	else
	{
		P_ST_Usual_Ack ccMsg;
		ccMsg.MsgType = TS_CHOOSEROLE_REQ;
		ccMsg.code = CHOOSEROLE_FAILED;

		Zoic::Message msgAck;
		msgAck.ID(ST_USUAL_ACK) << ccMsg;
		self->sendMessage(msgAck);
	}
	return true;
}
