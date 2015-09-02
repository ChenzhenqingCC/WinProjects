#include "include/SysPing.h"
#include "include/Session.h"
#include "include/Message.h"

using namespace Zoic;

SysPingTime Zoic::g_sysPingTime;

namespace
{
	class init_s_syspingTimes
	{
	public:
		init_s_syspingTimes()
		{
			const BYTE timeSize = SYSPING_KEEPCNT+1;
			DWORD timeData[timeSize]; 
			DWORD tm=SYSPING_KEEPIDLE;
			for(int i=0;i<timeSize;++i)
			{
				timeData[i]=tm;
				tm+=SYSPING_KEEPINTVL;
			}
			g_sysPingTime.init(timeSize,timeData);
		}
	} _init;
}
	
bool Zoic::onSysPingReq(Session *pSession,Message &msg)
{
	Message msgSend;
	msgSend.ID(SYSPING_ACK);
	pSession->sendMessage(msgSend);
	return true;
}

bool Zoic::onSysPingAck(Session *pSession,Message &msg)
{
	return true;
}
