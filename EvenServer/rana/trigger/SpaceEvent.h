#ifndef __TRIGGER_SPACE_EVENT_H_
#define __TRIGGER_SPACE_EVENT_H_

#include <vector>
#include <string>
#include "TriggerHeader.h"
#include "SortLable.h"
#include "Message.h"

namespace Trigger
{
	class TriggerSpace;
	class SpaceAction;
	//SpaceEvent在进行触发匹配时按照顺序进行一下检验
	//1.EventCategoryID一致匹配;2.SortLable排序匹配(如果存在);3.PreValidate遍历校验
	//三者全部检查通过之后其绑定的动作才会被触发
	//不匹配的Message尽量在越早的步骤被检查出来,效率越得到道保证

	class SpaceEvent
	{
		friend class TriggerSpace;
		friend class SpaceEventMap;
	public:
		typedef void* EventContext;
		EventContext const& getCustomContext() const;
		SpaceAction* getBindedAction() const;
		// 预检查用方法
		EventCategoryID getType() const;
		virtual const SortLable* getSortLable() const;
		virtual bool doValidate(Message *msg) const;
	protected:
		SpaceEvent(EventCategoryID eventType);
		virtual ~SpaceEvent();
		//归属获得
		TriggerSpace* getPossessingSpace();
		void setCustomContext(EventContext const context);
	private:
		EventContext m_context;
		TriggerSpace* m_pSelfSpace;
		SpaceAction* m_bindedAction;
		const EventCategoryID m_eventType;
	};

	inline SpaceEvent::EventContext const& SpaceEvent::getCustomContext() const
	{
		return m_context;
	}

	inline EventCategoryID SpaceEvent::getType() const
	{
		return m_eventType;
	}

	inline SpaceAction* SpaceEvent::getBindedAction() const
	{
		return m_bindedAction;
	}

	inline bool SpaceEvent::doValidate(Message *msg) const
	{
		return true;
	}

	inline const SortLable* SpaceEvent::getSortLable() const
	{
		return NULL;
	}

	inline TriggerSpace* SpaceEvent::getPossessingSpace()
	{
		return m_pSelfSpace;
	}
}

#endif //__TRIGGER_SPACE_EVENT_H_
