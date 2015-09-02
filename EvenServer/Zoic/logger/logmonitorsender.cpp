#include "logmonitorsender.h"

using namespace Zoic;

LogMonitorBank::LogMonitorBank():
m_pSendMsgFunc(NULL)
{

}

LogMonitorBank::~LogMonitorBank()
{

}

bool LogMonitorBank::sendToMonitor(LogContent* pContent)
{
	if (!m_pSendMsgFunc)
	{
		return false;
	}
	pContent->formate();
	return m_pSendMsgFunc(pContent);
}