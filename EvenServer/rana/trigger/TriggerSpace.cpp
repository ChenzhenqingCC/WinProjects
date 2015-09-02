#include "TriggerSpace.h"

namespace Trigger
{
	TriggerSpace::TriggerSpace()
	{
	}

	TriggerSpace::~TriggerSpace()
	{
		TRIGGER_ASSERT(m_actions.empty() && m_events.empty() );
	}

	COUNT TriggerSpace::onMessage(Message* pMsg)
	{
		const EventCategoryID eventType = pMsg->getEventType();
		const EventTypeItem* pMaintainer = m_maintainersByCategory.getEventsByType(eventType);
		if(!pMaintainer)
		{
			return 0;
		}
		COUNT successCount = 0;
		if( pMaintainer->sortLables.size() )
		{
			const SortLable* pSortLable = pMsg->getSortLable();
			EventTypeItem::LABLE_MMAP::const_iterator itBegin = pSortLable?
				pMaintainer->sortLables.lower_bound(pSortLable):
				pMaintainer->sortLables.begin();
			const EventTypeItem::LABLE_MMAP::const_iterator itAllEnd = pMaintainer->sortLables.end();
			for(;itBegin != itAllEnd;++itBegin)
			{
				if(pSortLable)
				{
					const SortLable* pTargetSortLable = itBegin->first;
					if( pSortLable->sortLableFront(*pTargetSortLable) )
					{
						break;
					}
				}
				SpaceEvent *pEvent = itBegin->second;
				if( !triggerEvent(pEvent,pMsg) )
				{
					continue;
				}
				++successCount;
			}
		}
		if( pMaintainer->noLableEvents.size() )
		{
			EventTypeItem::EVENT_LIST::const_iterator itNoLable = pMaintainer->noLableEvents.begin();
			for(;itNoLable!=pMaintainer->noLableEvents.end(); ++itNoLable )
			{
				SpaceEvent *pEvent = *itNoLable;
				if( !triggerEvent(pEvent,pMsg) )
				{
					continue;
				}
				++successCount;
			}
		}
		return successCount;
	}
	
	bool TriggerSpace::triggerEvent(SpaceEvent* pEvent,Message* pMsg)
	{
		if( !pEvent->m_bindedAction )
		{
			return false;
		}
		if( !pEvent->doValidate(pMsg) )
		{
			return false;
		}
		return pEvent->m_bindedAction->doAction(pEvent);
	}

	bool TriggerSpace::possessAction(SpaceAction* pAction)
	{
		if( pAction->m_pSelfSpace )
		{
			return false;
		}
		if( !m_actions.insert(pAction).second )
		{
			TRIGGER_ASSERT(0);
		}
		pAction->m_pSelfSpace = this;
		return true;
	}

	bool TriggerSpace::possessEvent(SpaceEvent* pEvent)
	{
		if( pEvent->m_pSelfSpace )
		{
			return false;
		}
		if( !m_events.insert(pEvent).second )
		{
			TRIGGER_ASSERT(0);
		}
		pEvent->m_pSelfSpace = this;
		if( ! m_maintainersByCategory.attachEvent(pEvent) )
		{
			TRIGGER_ASSERT(0);
		}
		pEvent->m_bindedAction = NULL;
		return true;
	}

	bool TriggerSpace::bindEventToAction(SpaceEvent* pEvent,SpaceAction* pBindAction)
	{
		if( this != pEvent->m_pSelfSpace )
		{
			return false;
		}
		if( pBindAction && this != pBindAction->m_pSelfSpace )
		{
			return false;
		}
		if( pEvent->m_bindedAction == pBindAction )
		{
			return true;
		}
		if( pEvent->m_bindedAction )
		{
			if( ! pEvent->m_bindedAction->m_bindedEvents.erase(pEvent) )
			{
				TRIGGER_ASSERT(0);
			}
			pEvent->m_bindedAction = NULL;
		}
		if( pBindAction )
		{
			if( ! pBindAction->m_bindedEvents.insert(pEvent).second )
			{
				TRIGGER_ASSERT(0);
			}
			pEvent->m_bindedAction = pBindAction;
		}
		return true;
	}

	SpaceAction* TriggerSpace::releaseNonBindAction(SpaceAction* pAction)
	{
		if( this != pAction->m_pSelfSpace )
		{
			return NULL;
		}
		if( pAction->m_bindedEvents.size() )
		{
			return NULL;
		}
		if( !m_actions.erase(pAction) )
		{
			TRIGGER_ASSERT(0);
		}
		pAction->m_pSelfSpace = NULL;
		return pAction;
	}

	SpaceEvent* TriggerSpace::releaseNonBindEvent(SpaceEvent* pEvent)
	{
		if( this != pEvent->m_pSelfSpace )
		{
			return NULL;
		}
		if( pEvent->m_bindedAction )
		{
			return NULL;
		}
		if( !m_maintainersByCategory.detachEvent(pEvent) )
		{
			TRIGGER_ASSERT(0);
		}
		if( !m_events.erase(pEvent) )
		{
			TRIGGER_ASSERT(0);
		}
		pEvent->m_pSelfSpace = NULL;
		return pEvent;
	}

	void TriggerSpace::collectAllEventsByType(EventCategoryID eventType, std::vector<SpaceEvent*>& output)
	{
		const EventTypeItem* pMaintainer = m_maintainersByCategory.getEventsByType(eventType);
		if(!pMaintainer)
		{
			return;
		}
		EventTypeItem::LABLE_MMAP::const_iterator itLable = pMaintainer->sortLables.begin();
		for(;itLable!=pMaintainer->sortLables.end();++itLable)
		{
			output.push_back(itLable->second);
		}
		EventTypeItem::EVENT_LIST::const_iterator itNoLable = pMaintainer->noLableEvents.begin();
		for(;itNoLable!=pMaintainer->noLableEvents.end(); ++itNoLable )
		{
			output.push_back(*itNoLable);
		}
	}

}
