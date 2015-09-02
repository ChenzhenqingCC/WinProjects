#ifndef __TRIGGER_SPACE_EVENT_MAP_H__
#define __TRIGGER_SPACE_EVENT_MAP_H__

#include "SpaceEvent.h"
#include <memory>
#include <set>
#include <map>
#include <list>

namespace Trigger
{
	class SpaceEvent;
	class TriggerSpace;
	class SpaceEventMap;
	struct EventTypeItem
	{
		friend class SpaceEventMap;
		friend class TriggerSpace;
	protected:
		typedef std::multimap<const SortLable*,SpaceEvent*,CmpSortLable> LABLE_MMAP;
		typedef std::list<SpaceEvent*> EVENT_LIST;
		LABLE_MMAP sortLables;
		std::map< SpaceEvent*,LABLE_MMAP::iterator> lableIndex;
		EVENT_LIST noLableEvents;
		std::map< SpaceEvent*,EVENT_LIST::iterator> noLableIndex;
	};

	template <unsigned char LAYER> class _Layer_Item_Map;

	template<>
	class _Layer_Item_Map<0>
	{
		friend class SpaceEventMap;
		friend class _Layer_Item_Map<1>;
	public:
		typedef EventTypeItem Element;
		EventTypeItem* getItem(unsigned int index) const;
		bool addItem(unsigned int index,EventTypeItem* data);
		EventTypeItem* removeItem(unsigned int index);
		bool empty() const;
	protected:
		_Layer_Item_Map();
		~_Layer_Item_Map();
		static const unsigned int MAX_INDEX = 0x10;
		static const unsigned char BIT_COUNT = 4;
		static const unsigned int FULL_MASK = 0x0F;
	private:
		Element* m_items[0x10];
	};

	template<unsigned char LAYER>
	class _Layer_Item_Map
	{
		friend class SpaceEventMap;
		friend class _Layer_Item_Map<LAYER+1>;
	public:
		typedef _Layer_Item_Map<LAYER-1> Element;
		EventTypeItem* getItem(unsigned int index) const;
		bool addItem(unsigned int index,EventTypeItem* data);
		EventTypeItem* removeItem(unsigned int index);
		bool empty() const;
	protected:
		_Layer_Item_Map();
		~_Layer_Item_Map();
		static const unsigned int MAX_INDEX = Element::MAX_INDEX*0x10;
		static const unsigned char BIT_COUNT = 4+Element::BIT_COUNT;
		static const unsigned int FULL_MASK = MAX_INDEX-1;
	private:
		Element* m_items[0x10];
	};

	struct _Layer_Empty_Checker
	{
		template <unsigned char LAYER> friend class _Layer_Item_Map;
	private:
		static void * const* getInitBuffer();
		static void * const* const s_emptyBuffer;
	};

	//////////////////////////////////////////////////////////////////////////Exactly Started Here/////////////////////////////////////////
	class SpaceEventMap
	{
		friend class TriggerSpace;
	protected:
		~SpaceEventMap();
		const EventTypeItem* getEventsByType(EventCategoryID categoryID) const;
		bool attachEvent(SpaceEvent* pEvent);
		bool detachEvent(SpaceEvent* pEvent);
	private:
		EventTypeItem* getListOfType(EventCategoryID categoryID) const;
		EventTypeItem* createEmptyListOfType(EventCategoryID categoryID);
		bool removeEmptyListOfType(EventCategoryID categoryID);
		_Layer_Item_Map<3> m_items;
	};

	inline const EventTypeItem* SpaceEventMap::getEventsByType(EventCategoryID categoryID) const
	{
		return getListOfType(categoryID);
	}

	inline EventTypeItem* SpaceEventMap::getListOfType(EventCategoryID categoryID) const
	{
		return m_items.getItem(categoryID);
	}

	inline _Layer_Item_Map<(unsigned char)0>::_Layer_Item_Map()
	{
		memset(m_items,0,sizeof(m_items) );
	}
	template<unsigned char LAYER> inline _Layer_Item_Map<LAYER>::_Layer_Item_Map()
	{
		memset(m_items,0,sizeof(m_items) );
	}
	inline _Layer_Item_Map<(unsigned char)0>::~_Layer_Item_Map()
	{
	}
	template<unsigned char LAYER> inline _Layer_Item_Map<LAYER>::~_Layer_Item_Map()
	{
		for(size_t i = 0; i< 0x10; ++i)if(m_items[i])delete m_items[i];
	}
	inline EventTypeItem* _Layer_Item_Map<0>::getItem(unsigned int index) const
	{
		return m_items[index];
	}
	template<unsigned char LAYER> inline EventTypeItem* _Layer_Item_Map<LAYER>::getItem(unsigned int index) const
	{
		const unsigned int unit_index = index >> Element::BIT_COUNT;
		if( !m_items[unit_index] )return NULL;
		const unsigned int next_layer_index = index & Element::FULL_MASK;
		return m_items[unit_index]->getItem(next_layer_index);
	}
	inline bool _Layer_Item_Map<0>::addItem(unsigned int index,EventTypeItem* data)
	{
		if( m_items[index] )return false;
		m_items[index] = data;
		return true;
	}
	template<unsigned char LAYER> inline bool _Layer_Item_Map<LAYER>::addItem(unsigned int index,EventTypeItem* data)
	{
		const unsigned int unit_index = index >> Element::BIT_COUNT;
		if( !m_items[unit_index] )m_items[unit_index] = new Element;
		const unsigned int next_layer_index = index & Element::FULL_MASK;
		return m_items[unit_index]->addItem(next_layer_index,data);
	}
	inline EventTypeItem* _Layer_Item_Map<0>::removeItem(unsigned int index)
	{
		EventTypeItem* data = m_items[index];
		m_items[index] = NULL;
		return data;
	}
	template<unsigned char LAYER> inline EventTypeItem* _Layer_Item_Map<LAYER>::removeItem(unsigned int index)
	{
		const unsigned int unit_index = index >> Element::BIT_COUNT;
		if( !m_items[unit_index] )return NULL;
		const unsigned int next_layer_index = index & Element::FULL_MASK;
		EventTypeItem* data = m_items[unit_index]->removeItem(next_layer_index);
		if( !data )return NULL;
		if( m_items[unit_index]->empty() ){delete m_items[unit_index];m_items[unit_index]=NULL;};
		return data;
	}
	inline bool _Layer_Item_Map<0>::empty() const
	{
		return 0 == memcmp(m_items,_Layer_Empty_Checker::s_emptyBuffer,sizeof(m_items) );
	}
	template<unsigned char LAYER> inline bool _Layer_Item_Map<LAYER>::empty() const
	{
		return 0 == memcmp(m_items,_Layer_Empty_Checker::s_emptyBuffer,sizeof(m_items) );
	}
}


#endif //__TRIGGER_SPACE_EVENT_MAP_H__
