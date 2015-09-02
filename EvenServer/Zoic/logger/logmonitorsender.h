#ifndef __LOGMONITORSENDER_H
#define __LOGMONITORSENDER_H

#include <string>
#include "Zoic/include/Types.h"
#include "Zoic/include/Module.h"
#include "logcontent.h"

namespace Zoic
{
	class LogMonitorBank:
		public Zoic::Module<LogMonitorBank>
	{
		friend class Zoic::Module<LogMonitorBank>;
	public:
		typedef bool (*SEND_TO_MONITOR)(LogContent* pLogContent);
		void setSendMsgFunc(SEND_TO_MONITOR pFunc);

		bool sendToMonitor(LogContent* pContent);
	private:
		LogMonitorBank();
		~LogMonitorBank();

		SEND_TO_MONITOR m_pSendMsgFunc;
	};

	inline void LogMonitorBank::setSendMsgFunc(SEND_TO_MONITOR pFunc)
	{
		m_pSendMsgFunc = pFunc;
	}
}

#define g_LogMonitorSender Zoic::LogMonitorBank::getInstance()

#endif