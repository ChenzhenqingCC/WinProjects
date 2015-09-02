#if defined(_MSC_VER)
#include "config-win.h"
#else
#include "config.h"
#endif
#include "RanaTriggerStat.h"
#include <time.h>

#ifdef HAVE_MINGW32
#include <winsock2.h>
#ifdef _MSC_VER
#pragma comment (lib, "winmm.lib")
#endif
#endif

static inline unsigned long getCurrentTime()
{
#ifdef HAVE_MINGW32
	return timeGetTime();
#else
	timespec ts;
	clock_gettime(CLOCK_MONOTONIC,&ts);
	return ts.tv_sec*1000+ts.tv_nsec/1000000;
#endif
}


RanaTriggerStat::RanaTriggerStat(void)
{
	clearEventCount();
}

RanaTriggerStat::~RanaTriggerStat(void)
{
}

void RanaTriggerStat::clearEventCount()
{
	m_M_Count = 0;
	m_MEC_Count = 0;
	m_MECA_Count = 0;
	m_LastConditionTime = 0;
	m_LastActionTime = 0;
	m_LastConditionStartTime = 0;
	m_LastActionStartTime = 0;
	m_ConditionStarting = false;
	m_ActionStarting = false;
}

bool RanaTriggerStat::ConditionStart()
{
	if(!m_ConditionStarting)
	{
		m_LastConditionStartTime = getCurrentTime();
		m_ConditionStarting = true;
		return true;
	}
	return false;
}

bool RanaTriggerStat::ActionStart()
{
	if(!m_ActionStarting)
	{
		m_LastActionStartTime = getCurrentTime();
		m_ActionStarting = true;
		return true;
	}
	return false;
}

bool RanaTriggerStat::ConditionOver()
{
	if(m_ConditionStarting)
	{
		unsigned long curtime = getCurrentTime();
		m_LastConditionTime = curtime-m_LastConditionStartTime;
		m_ConditionStarting = false;
		return true;
	}
	return false;
}

bool RanaTriggerStat::ActionOver()
{
	if(m_ActionStarting)
	{
		unsigned long curtime = getCurrentTime();
		m_LastActionTime = curtime-m_LastActionStartTime;
		m_ActionStarting = false;
		return true;
	}
	return false;
}
