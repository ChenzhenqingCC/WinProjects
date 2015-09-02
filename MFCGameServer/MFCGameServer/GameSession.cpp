#include "stdafx.h"
#ifndef ZOIC_NO_SERVICE
#include <Zoic/include/NetLog.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <Zoic/include/SysPing.h>
#include <Zoic/include/Log.h>
#include "GameSession.h"
#include "GameService.h"
using namespace Zoic;



GameSession::GameSession():
m_useraccid(0)
, m_state(LS_NONE)
{
	{//把ping时间改改长
		const BYTE timeSize = 255;//SYSPING_KEEPCNT+1;
		DWORD timeData[timeSize];
		DWORD tm=0;

		const DWORD maxClientSysPingTime = 100000;
		const DWORD clientSysPingKeepIdle = maxClientSysPingTime/2;
		const DWORD clientSysPingKeepIntvl = clientSysPingKeepIdle/SYSPING_KEEPCNT;
		tm = clientSysPingKeepIdle;
		for(int i=0;i<timeSize;++i)
		{
			timeData[i]=tm;
			tm+=clientSysPingKeepIntvl;
		}

		static Zoic::SysPingTime ranaClientSysPingTime;
		ranaClientSysPingTime.init(timeSize,timeData);
		setSysPingTime(&ranaClientSysPingTime);
	}
	
	m_send_buffer_rule.m_dynamic = true;
	m_send_buffer_rule.m_release_dynamic = true;
	m_send_buffer_rule.m_max_dynamic_buff_size = 512 * 1024 * 1024;		//将发送缓冲区改为512M，以防爆掉
}

GameSession::~GameSession()
{
}

bool GameSession::parseMessage(Zoic::Message &msg)
try
{
	return SessionS::parseMessage(msg);
}
catch (std::exception * e)
{
	//CString s;
	//s.Format(_T("CommandClient exception:%s"), e->what());
	//MessageBox(NULL,s,L"",0);
	loginfo_f("网络错误:%s", e->what());
	return false;
}

bool GameSession::sendMessage(const Zoic::Message &msg)
{
	return SessionS::sendMessage(msg);
}

bool GameSession::open(SOCKET sock, const sockaddr_in &addr)
{
	bool ret = SessionS::open(sock, addr);
	if(ret)
	{
		logdebug_f("GameSession[%d,%d] open on %s:%d\n",getHandle(),getSerial(),getIP(),getPort());
	}
	return ret;
}

void GameSession::close()
{
	logdebug_f("GameSession[%d,%d] close on %s:%d\n",getHandle(),getSerial(),getIP(),getPort());
	SessionS::close();
}
