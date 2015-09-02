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
		LS_NONE,		//δ��¼
		LS_LOGING,		//����¼
		LS_LOGED,		//�ѵ�¼
		LS_CREATEACC,	//�������˺�
		LS_CREATEROLE,	//��������ɫ
		LS_CHOOSEROLE,	//��ѡ���ɫ
		LS_ENTERED,	//�ѽ�����Ϸ
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
