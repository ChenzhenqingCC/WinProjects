#include "include/WorkFlow.h"
#include "include/Clock.h"

namespace Zoic
{
	WorkFlow::WorkFlow() :m_nextActionTime(-1)
	{
	}

	void WorkFlow::delay(WorkUnit* unit,DWORD delayMs)
	{
		insert(unit,g_Clock->getMilliSeconds()+delayMs);
	}

	void WorkFlow::insert(WorkUnit* unit,TIME_MSEL workTime)
	{
		m_workFlows.insert( std::make_pair(workTime,unit) );
		if( m_nextActionTime<0 || m_nextActionTime>workTime )
		{
			m_nextActionTime = workTime;
		}
	}

	void WorkFlow::clear()
	{
		std::multimap<TIME_MSEL,WorkUnit*>::iterator itMMap = m_workFlows.begin();
		for(;itMMap!=m_workFlows.end();++itMMap)
		{
			itMMap->second->release();
		}
		m_workFlows.clear();
		m_nextActionTime = -1;
	}

	int WorkFlow::routine()
	{
		TIME_MSEL curTime = g_Clock->getMilliSeconds();
		if( m_nextActionTime<0 || m_nextActionTime > curTime)
		{
			return 0;
		}
		std::multimap<TIME_MSEL,WorkUnit*>::iterator itMMapEnd = m_workFlows.upper_bound(curTime);
		std::vector<WorkUnit *> working;
		for(std::multimap<TIME_MSEL,WorkUnit*>::iterator itMMap = m_workFlows.begin();itMMap!=itMMapEnd;++itMMap)
		{
			working.push_back(itMMap->second);
		}
		if( itMMapEnd!=m_workFlows.end() )
		{
			m_nextActionTime = itMMapEnd->first;
		}
		else
		{
			m_nextActionTime = -1;
		}

		m_workFlows.erase(m_workFlows.begin(),itMMapEnd);
		for(std::vector<WorkUnit *>::iterator workingIt = working.begin();workingIt!=working.end();++workingIt)
		{
			WorkUnit * unit = *workingIt;
			unit->action();
			unit->release();
		}
		return 0;
	}
}
