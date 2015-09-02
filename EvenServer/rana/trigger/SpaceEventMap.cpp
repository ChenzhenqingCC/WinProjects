#include "SpaceEventMap.h"
#include "SpaceEvent.h"

namespace Trigger
{
	void * const* _Layer_Empty_Checker::getInitBuffer()
	{
		static void* staticBuffer[0x20];
		memset(staticBuffer,0,sizeof(staticBuffer) );
		return staticBuffer;
	}
	void * const* const _Layer_Empty_Checker::s_emptyBuffer = _Layer_Empty_Checker::getInitBuffer();

	SpaceEventMap::~SpaceEventMap()
	{
		TRIGGER_ASSERT( m_items.empty() );
	}

	bool SpaceEventMap::attachEvent(SpaceEvent* pEvent)
	{
		EventTypeItem* pItem = getListOfType( pEvent->getType() );
		if(!pItem)
		{
			pItem = createEmptyListOfType( pEvent->getType() );
		}
		if( pItem->lableIndex.find(pEvent) != pItem->lableIndex.end() ||
			pItem->noLableIndex.find(pEvent) != pItem->noLableIndex.end() )
		{
			return false;
		}
		const SortLable* pSortLable = pEvent->getSortLable();
		if( pSortLable )
		{
			pItem->lableIndex[pEvent] = pItem->sortLables.insert( std::make_pair(pSortLable,pEvent) );
		}
		else
		{
			pItem->noLableIndex[pEvent] = pItem->noLableEvents.insert(pItem->noLableEvents.end(),pEvent );
		}
		return true;
	}

	bool SpaceEventMap::detachEvent(SpaceEvent* pEvent)
	{
		EventTypeItem* pItem = getListOfType( pEvent->getType() );
		if(!pItem)
		{
			return false;
		}
		int successCount = 0;
		std::map< SpaceEvent*,EventTypeItem::LABLE_MMAP::iterator>::iterator itLable = pItem->lableIndex.find(pEvent);
		if( itLable != pItem->lableIndex.end() )
		{
			pItem->sortLables.erase(itLable->second);
			pItem->lableIndex.erase(itLable);
			++successCount;
		}
		std::map< SpaceEvent*,EventTypeItem::EVENT_LIST::iterator>::iterator itList = pItem->noLableIndex.find(pEvent);
		if( itList != pItem->noLableIndex.end() )
		{
			pItem->noLableEvents.erase(itList->second);
			pItem->noLableIndex.erase(itList);
			++successCount;
		}
		if( pItem->noLableEvents.empty() && pItem->sortLables.empty() )
		{
			removeEmptyListOfType( pEvent->getType() );
		}
		return successCount == 1;
	}

	EventTypeItem* SpaceEventMap::createEmptyListOfType(EventCategoryID categoryID)
	{
		if( m_items.getItem(categoryID) )
		{
			return NULL;
		}
		EventTypeItem* pItem = new EventTypeItem;
		m_items.addItem(categoryID,pItem);
		return pItem;
	}

	bool SpaceEventMap::removeEmptyListOfType(EventCategoryID categoryID)
	{
		EventTypeItem* pItem = m_items.getItem(categoryID);
		if( pItem->noLableEvents.size() || pItem->sortLables.size() )
		{
			return false;
		}
		delete m_items.removeItem(categoryID);
		return true;
	}

}
