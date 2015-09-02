#ifndef __GAMESRV_GAMESERVICE_H
#define __GAMESRV_GAMESERVICE_H
#include "stdafx.h"
#include <string>
#include <Zoic/include/Module.h>
#include <Zoic/include/SessionBank.h>
#include "ServerDataDef.h"
#include "Zoic/include/QueryDefine.h"
#include "GameProto_Login.h"
#include <map>
using namespace GameDataDef;
using namespace GameProto;

//数据库提取类型
enum SelectType
{
	SELTYPE_USERACC = 0,	//用户账号数据	
	SELTYPE_PLAYERBASE = 1,	//玩家角色基本数据
};

typedef std::vector<PlayerDBBase> PlayerList;

class GameService:
	public Zoic::SessionBank
	, public Zoic::Module<GameService>
{
	friend class Zoic::Module<GameService>;
public:
	Zoic::SessionS *createSession();
	int start();
	int run();
	int stop();
	void init();
	void SelectAllDataFromDB(SelectType stype, Zoic::Query * query);
	ServerError IsUserCanLogin(UserAcc usrname, PassWord pass, UserID & uid);
	bool GetPlayerList(UserID uid, PlayerList & plist);
	ServerError CreateRole(UserID uid, P_TS_CreatePlayer_Req & cmd);
	bool CreateDBRole(UserID uid, P_TS_CreatePlayer_Req & cmd, PlayerID & pid);
	bool IsNickNameUsed(PlayerName & nickname);
	bool IsAccNameUsed(UserAcc & accname);
	ServerError CreateAccount(P_TS_CreateAcc_Req & cmd);
	bool CreateDBAcc(P_TS_CreateAcc_Req & cmd, UserID & uid);
	void CalculateRoleProps(PlayerBase & plb);
	bool FillPlayerBase(UserID uid, PlayerBase & plb);
	
	const HandleType & getClientHandle();
	const SerialType & getClientSerial();

	//触发计时系统
	void updateCurrentTime();
	GameService();
	~GameService();
private:
	bool m_clientEnable;				//Rana服务器的SessionBank是否工作
	HandleType	m_clientHandle;		//客户端名柄
	SerialType	m_clientSerial;		//客户端序列号
	std::map<UserAcc, UserAccount> allUsers;
	std::map<UserID, PlayerList> allPlayers;
};

inline const HandleType & GameService::getClientHandle()
{
	return m_clientHandle;
}

inline const SerialType & GameService::getClientSerial()
{
	return m_clientSerial;
}

#define g_GameService GameService::getInstance()
#endif