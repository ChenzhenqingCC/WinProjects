#include "RanaTimeMessage.h"
#include "RanaTimeEvent.h"

RanaTimeMessage::RanaTimeMessage()
	:RanaMessage(RANA_TIME_EVENT)
{
	m_timerHandle = RANA_INVALID_TIMERHANDLE;
}

RanaTimeMessage::~RanaTimeMessage()
{
}

void RanaTimeMessage::setTimerHandle(RanaTimerHandle _handle)
{
	m_timerHandle = _handle;
}

RanaTimerHandle RanaTimeMessage::getTimerHandle()
{
	return m_timerHandle;
}

