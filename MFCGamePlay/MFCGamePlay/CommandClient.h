#ifndef __COMMANDCLIENT_H
#define __COMMANDCLIENT_H
#include "stdafx.h"
#include <Zoic/include/SessionC.h>
#include <Zoic/include/Module.h>
enum
{
	CLIENT_CONNECTED,
	CLIENT_LOGIN_SUCCESS,
	CLIENT_LOGIN_FAILED,
	CLIENT_NOT_CONNECTED
};
class CommandClient:
	public Zoic::SessionC,
	public Zoic::Module<CommandClient>
{
	friend class Zoic::Module<CommandClient>;
public:
	template <const WORD cmd> static bool onMessage(Zoic::Session *pSession,Zoic::Message &msg);
	bool parseMessage(Zoic::Message &msg);
	bool open(SOCKET sock,const sockaddr_in &addr);
	void close();
	int state;
	int start();
	int run();
	int stop();
private:
	CommandClient();
};

#define g_CommandClient CommandClient::getInstance()

#endif
