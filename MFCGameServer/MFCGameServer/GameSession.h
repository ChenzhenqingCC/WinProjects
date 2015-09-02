#ifndef __GAMESRV_GAMESESSION_H
#define __GAMESRV_GAMESESSION_H
#include "GameTypes.h"
#include <Zoic/include/SessionS.h>
#include "ServerMacros.h"

class GameSession:public Zoic::SessionS
{
	friend class GameService;
public:
	enum LOGIN_STATE
	{
		LS_NONE,		//未登录
		LS_LOGING,		//正登录
		LS_LOGED,		//已登录
		LS_CREATEACC,	//正创建账号
		LS_CREATEROLE,	//正创建角色
		LS_CHOOSEROLE,	//正选择角色
		LS_ENTERED,	//已进入游戏
	};
	ON_MESSAGE2
	template <const WORD CMD> bool onMessage(Zoic::Message &msg);
	bool parseMessage(Zoic::Message &msg);
	bool sendMessage(const Zoic::Message &msg);
	bool open(SOCKET sock,const sockaddr_in &addr);
	void close();
	CC_SYNTHESIZE(GameType::UserID, m_useraccid, UserAccID);
	CC_SYNTHESIZE(LOGIN_STATE, m_state, State);
protected:
	bool openClientDefault(SOCKET sock,const sockaddr_in &addr);
	void closeClientDefault();

private:
	GameSession();
	~GameSession();
};

#endif 
