#include "stdafx.h"
#include "CommandClient.h"
#include "Zoic/include/Message.h"
#include "Zoic/include/Log.h"
#include <fcntl.h>
#ifdef HAVE_MINGW32
#include <conio.h>
#endif

#include <iostream>

CommandClient::CommandClient()
{
	m_send_buffer_rule.m_dynamic = true;
}

bool CommandClient::parseMessage(Zoic::Message &msg)
try
{
	return SessionC::parseMessage(msg);
}
catch(std::exception * e)
{
	//CString s;
	//s.Format(_T("CommandClient exception:%s"), e->what());
	//MessageBox(NULL, s, L"", 0);
	loginfo_f("网络错误:%s", e->what());
	return false;
}

bool CommandClient::open(SOCKET sock,const sockaddr_in &addr)
{
	bool ret= SessionC::open(sock,addr);
	if(ret)
	{
		//printf("CommandClient connect to %s:%d\n",getIP(),getPort());
		loginfo_f("CommandClient connect to %s:%d\n", getIP(), getPort());
	}
	return ret;
}

void CommandClient::close()
{
	//printf("CommandClient disconenct from %s:%d\n",getIP(),getPort());
	loginfo_f("CommandClient disconenct from %s:%d\n", getIP(), getPort());
	SessionC::close();
}

int CommandClient::start()

{
	//puts("CommandClient::start");
	loginfo_f("CommandClient::start");
#ifndef HAVE_MINGW32
	if(fcntl(STDIN_FILENO, F_SETFL, FNDELAY)==-1)
	{
		return 1002;
	}
#endif

	std::string ip="127.0.0.1";
	WORD port=9000;

	//setConnectAddress(ip,port);
	int code = Zoic::SessionC::start();
	if(code)
	{
		//printf("Can't connect to %s:%d\n",ip,port);
		loginfo_f("Can't connect to %s:%d", ip, port);
	}
	return code;
}

int CommandClient::run()
{
	if(this->state == CLIENT_CONNECTED )	//连接成功
	{
	}
	if(this->state == CLIENT_NOT_CONNECTED )	//连接失败
	{
		stop();
	}
	return Zoic::SessionC::run();
}

int CommandClient::stop()
{
	//puts("CommandClient::stop");
	loginfo_f("CommandClient::stop");
	return Zoic::SessionC::stop();
}
