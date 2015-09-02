#ifndef __TRIGGER_MESSAGE_H_
#define __TRIGGER_MESSAGE_H_

#include "TriggerHeader.h"
#include "SortLable.h"

namespace Trigger
{
	class Message
	{
	public:
		Message(EventCategoryID eventType);
		virtual ~Message();

		EventCategoryID getEventType() const;

		virtual const SortLable* getSortLable() const;
	private:
		const EventCategoryID m_eventType;	// 触发事件的事件类型
	};

	inline Message::Message(EventCategoryID eventType)
		:m_eventType(eventType)
	{
	}

	inline Message::~Message()
	{
	}

	inline EventCategoryID Message::getEventType() const
	{
		return m_eventType;
	}

	inline const SortLable* Message::getSortLable() const
	{
		return NULL;
	}
}

#endif //__TRIGGER_MESSAGE_H_
