#include "RanaTimer.h"

RanaTimer::RanaTimer()
{
	m_handle = RANA_INVALID_TIMERHANDLE;
	m_periodTime = 0;
	m_restTime = 0;
	m_isRepeat = false;
	m_isOn = false;
}

RanaTimer::~RanaTimer()
{
}

void RanaTimer::setHandle(RanaTimerHandle _handle)
{
	m_handle = _handle;
}

RanaTimerHandle RanaTimer::getHandle()
{
	return m_handle;
}

void RanaTimer::setSpaceName(RanaSpaceName _spaceName)
{
	m_spaceName = _spaceName;
}

const RanaSpaceName& RanaTimer::getSpaceName()
{
	return m_spaceName;
}

void RanaTimer::setPeriodTime(RanaTIME _periodTime) // _periodTime为百分之一秒单位
{
	m_periodTime = _periodTime * 10; //将_periodTime转换为毫秒单位（千分之一秒）
	m_restTime = m_periodTime;
}

RanaTIME RanaTimer::getPeriodTime()
{
	return m_periodTime;
}

void RanaTimer::setRepeat(bool _isRepeat)
{
	m_isRepeat = _isRepeat;
}

bool RanaTimer::getRepeat()
{
	return m_isRepeat;
}

void RanaTimer::setOnOff(bool _isOn)
{
	m_isOn = _isOn;
}

bool RanaTimer::getOnOff()
{
	return m_isOn;
}

RanaTIME RanaTimer::getRestTime()
{
	return m_restTime;
}

void RanaTimer::reset()
{
	m_restTime = m_periodTime;
}
