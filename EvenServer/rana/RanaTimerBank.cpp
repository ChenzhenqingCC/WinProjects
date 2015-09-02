#include "RanaTimerBank.h"
#include "RanaTimeMessage.h"
#include "RanaSpaceBank.h"

RanaTimerBank* RanaTimerBank::INSTANCE = NULL;

RanaTimerBank::RanaTimerBank()
{
}

RanaTimerBank::~RanaTimerBank()
{
	std::multiset<RanaTimer*,CompRanaTimerPoint>::iterator itActive;
	for (itActive = m_activeTimer.begin(); itActive!=m_activeTimer.end(); itActive++)
	{
		RanaTimer *timer = *itActive;
		if (timer)
		{
			delete timer;
		}
	}
	m_activeTimer.clear();
	std::list<RanaTimer*>::iterator itPaused;
	for (itPaused = m_pausedTimer.begin(); itPaused!=m_pausedTimer.end(); itPaused++)
	{
		RanaTimer *timer = *itPaused;
		if (timer)
		{
			delete timer;
		}
	}
	m_pausedTimer.clear();
	m_timerMap.clear();
}

RanaTimerBank* RanaTimerBank::getInstance()
{
	if (!INSTANCE)
	{
		INSTANCE = new RanaTimerBank;
	}
	return INSTANCE;
}

void RanaTimerBank::release()
{
	if (INSTANCE)
	{
		delete INSTANCE;
		INSTANCE = NULL;
	}
}

// 构造一个新的计时器，返回该计时器的Handle
RanaTimerHandle RanaTimerBank::createTimer(const RanaSpaceHandle& _spaceHandle)
{
	RanaSpace* pSpace = RanaSpaceBank::getInstance()->getLuaSpace(_spaceHandle);
	if(!pSpace)
	{
		return RANA_INVALID_TIMERHANDLE;
	}
	RanaTimer *timer = new RanaTimer;
	if (!timer)
	{
		debug_ranaLog("RanaTimerBank::createTimer : cannot new RanaTimer");
		return RANA_INVALID_TIMERHANDLE;
	}
	RanaTimerHandleManager *pHandleManager = &m_timerMap[_spaceHandle.handle];
	RanaTimerHandle handle = pHandleManager->nextHandle;
	pHandleManager->nextHandle++;

	timer->setHandle(handle);
	timer->setSpaceHandle(_spaceHandle);
	timer->setSpaceName( pSpace->getSpaceName() );
	timer->setOnOff(false);
 
	ListIterator* pListIterator =&(pHandleManager->handleMap)[handle];
	pListIterator->itType = ListIterator::E_PAUSED;
	pListIterator->pausedIt = m_pausedTimer.insert(m_pausedTimer.end(),timer);

	return handle;
}

// 删除一个RanaSpace对应的所有计时器
bool RanaTimerBank::delTimer(const RanaSpaceHandle& _spaceHandle)
{
	RanaSpace* pSpace = RanaSpaceBank::getInstance()->getLuaSpace(_spaceHandle);
	if(!pSpace)
	{
		if( m_timerMap.find(_spaceHandle.handle) != m_timerMap.end() )
		{
			debug_ranaLog("RanaTimerBank::deltimer : handle[%d] serial error!", _spaceHandle.handle);
		}
		return false;
	}
	RANA_SPACEITMAP::iterator itMap = m_timerMap.find(_spaceHandle.handle);
	if(itMap == m_timerMap.end())
	{
		//没有找到合适的spaceName
		debug_ranaLog("RanaTimerBank::delTimer : no timer for spaceHandle[%d]", 
			_spaceHandle.handle);
		return true;
	}
	RANA_HANDLEITMAP* pHandleMap = &((itMap->second).handleMap);
	RANA_HANDLEITMAP::iterator it;
	for (it = pHandleMap->begin(); it != pHandleMap->end(); it++)
	{
		ListIterator* pListIterator = &(it->second);
		RanaTimer *timer = NULL;
		switch(pListIterator->itType)
		{
		case ListIterator::E_ACTIVE:
			timer = *pListIterator->activeIt;
			m_activeTimer.erase(pListIterator->activeIt);
			break;
		case ListIterator::E_PAUSED:
			timer = *pListIterator->pausedIt;
			m_pausedTimer.erase(pListIterator->pausedIt);
			break;
		}
//		if (timer)
//		{
			delete timer;
//		}
	}
	pHandleMap->clear();
	m_timerMap.erase(itMap);
	return true;
}

// 计时器运行time毫秒
void RanaTimerBank::routine(RanaTIME _expiretime)
{
	m_curTime = _expiretime;
	std::multiset<RanaTimer*,CompRanaTimerPoint>::iterator timerIt;
	for (timerIt=m_activeTimer.begin(); timerIt!=m_activeTimer.end(); timerIt=m_activeTimer.begin() )
	{
		RanaTimer* timer = *timerIt;
		if(timer->getExpireTime() > _expiretime)
		{
			break;
		}
		m_activeTimer.erase(timerIt);
		bool bStop = true;
		if( timer->getRepeat() )
		{
			int periodTime = (int)timer->getPeriodTime();
			if( periodTime > 0)
			{
				bStop = false;
				timer->setExpireTime( timer->getExpireTime() + timer->getPeriodTime() );
				m_timerMap[timer->getSpaceHandle().handle][timer->getHandle()].activeIt = m_activeTimer.insert(timer);
			}
		}
		if(bStop)
		{
			timer->setOnOff(false);
			timer->setRestTime(0);
			m_timerMap[timer->getSpaceHandle().handle][timer->getHandle()].pausedIt = m_pausedTimer.insert(m_pausedTimer.end(),timer);
			m_timerMap[timer->getSpaceHandle().handle][timer->getHandle()].itType = ListIterator::E_PAUSED;
		}
		RanaSpace* pRanaSpace = RanaSpaceBank::getInstance()->getLuaSpace( timer->getSpaceHandle() );
		if(!pRanaSpace)
		{
			RANA_ASSERT(0);
			delTimer( timer->getSpaceHandle() );
			continue;
		}
		RanaTimeMessage msg;
//		msg.setSpaceName( );
		msg.setTimerHandle(timer->getHandle());
		pRanaSpace->onMessage(&msg);
	}
}

RanaTimer* RanaTimerBank::getTimer(const RanaSpaceHandle& _spaceHandle, 
									  RanaTimerHandle _timerHandle)
{
	RanaSpace* pSpace = RanaSpaceBank::getInstance()->getLuaSpace(_spaceHandle);
	if(!pSpace)
	{
		return NULL;
	}
	RANA_SPACEITMAP::iterator itMap = m_timerMap.find(_spaceHandle.handle);
	if(itMap == m_timerMap.end())
	{
		//没有找到合适的spaceName
		debug_ranaLog("RanaTimerBank::getTimer : no such spaceNameHandle[%u]", 
			_spaceHandle);
		return NULL;
	}
	RANA_HANDLEITMAP *pHandleMap = &(itMap->second).handleMap;
	RANA_HANDLEITMAP::iterator itHandle = pHandleMap->find(_timerHandle);
	if(itHandle == pHandleMap->end())
	{
		//没有找到合适的timerHandle
		//char str[sizeof(RanaSpaceName)+1];
		debug_ranaLog("RanaTimerBank::getTimer : no such timerHandle[%u : %u]",
			_spaceHandle, _timerHandle);
		return NULL;
	}
	ListIterator* pListIterator = &(itHandle->second);
	if( pListIterator->itType == ListIterator::E_PAUSED)
	{
		return *(pListIterator->pausedIt);
	}
	return *(pListIterator->activeIt);
}

void RanaTimerBank::setTimerPeriodTime(const RanaSpaceHandle& _spaceHandle, 
										  RanaTimerHandle _timerHandle, 
										  RanaTIME _periodTime)
{
	RanaTimer *timer = getTimer(_spaceHandle, _timerHandle);
	if (timer && _periodTime > 0.0)
	{
		timer->setPeriodTime(_periodTime);
	}
}

RanaTIME RanaTimerBank::getTimerPeriodTime(const RanaSpaceHandle& _spaceHandle, 
												 RanaTimerHandle _timerHandle)
{
	RanaTimer *timer = getTimer(_spaceHandle, _timerHandle);
	if (timer)
	{
		return timer->getPeriodTime();
	}

	return 0;
}

void RanaTimerBank::setTimerRepeat(const RanaSpaceHandle& _spaceHandle,
									  RanaTimerHandle _timerHandle, 
									  bool _isRepeat)
{
	RanaTimer *timer = getTimer(_spaceHandle, _timerHandle);
	if (timer)
	{
		timer->setRepeat(_isRepeat);
	}
}

bool RanaTimerBank::getTimerRepeat(const RanaSpaceHandle& _spaceHandle, 
										  RanaTimerHandle _timerHandle)
{
	RanaTimer *timer = getTimer(_spaceHandle, _timerHandle);
	if (timer)
	{
		return timer->getRepeat();
	}

	return false;
}

void RanaTimerBank::setTimerOnOff(const RanaSpaceHandle& _spaceHandle, 
									 RanaTimerHandle _timerHandle, 
									 bool _isOn)
{
	RanaTimer *timer = getTimer(_spaceHandle, _timerHandle);
	if (timer)
	{
		ListIterator* pListIterator = &(m_timerMap[_spaceHandle.handle][_timerHandle]);
		bool isCurOn = timer->getOnOff();
		if(isCurOn!=_isOn)
		{
			if(_isOn)
			{
				RanaTIME m_restTime = timer->getRestTime();
				if( m_restTime<=0 )
				{
					m_restTime = timer->getPeriodTime();
				}
				if(m_restTime>0)
				{
					timer->setOnOff(true);
					timer->setExpireTime(m_curTime + m_restTime);
					m_pausedTimer.erase(pListIterator->pausedIt);
					pListIterator->activeIt = m_activeTimer.insert(timer);
					pListIterator->itType = ListIterator::E_ACTIVE;
				}
			}
			else
			{
				RanaTIME m_restTime = timer->getExpireTime() - m_curTime;
				timer->setOnOff(false);
				timer->setRestTime(m_restTime);
				timer->setOnOff(false);
				timer->setRestTime(0);
				m_activeTimer.erase(pListIterator->activeIt);
				pListIterator->pausedIt = m_pausedTimer.insert(m_pausedTimer.end(),timer);
				pListIterator->itType = ListIterator::E_PAUSED;
			}
		}
	}
}

bool RanaTimerBank::getTimerOnOff(const RanaSpaceHandle& _spaceHandle, 
										 RanaTimerHandle _timerHandle)
{
	RanaTimer *timer = getTimer(_spaceHandle, _timerHandle);
	if (timer)
	{
		return timer->getOnOff();
	}

	return false;
}

RanaTIME RanaTimerBank::getTimerRestTime(const RanaSpaceHandle& _spaceHandle,
											   RanaTimerHandle _timerHandle)
{
	RanaTimer *timer = getTimer(_spaceHandle, _timerHandle);
	if (timer)
	{
		return timer->getRestTime();
	}

	return 0;
}

void RanaTimerBank::resetTimer(const RanaSpaceHandle& _spaceHandle,
								  RanaTimerHandle _timerHandle)
{
	RanaTimer *timer = getTimer(_spaceHandle, _timerHandle);
	if (timer)
	{
		ListIterator* pListIterator = &(m_timerMap[_spaceHandle.handle][_timerHandle]);
		bool isCurOn = timer->getOnOff();
		if(isCurOn)
		{
			m_activeTimer.erase(pListIterator->activeIt);
			timer->setExpireTime( m_curTime + timer->getPeriodTime() );
			pListIterator->activeIt = m_activeTimer.insert(timer);
		}
		else
		{
			timer->reset();
		}
		return;
	}
}

void RanaTimerBank::initCurrentTime(RanaTIME _time)
{
	m_curTime = _time;
}
