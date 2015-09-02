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
		const EventCategoryID m_eventType;	// �����¼����¼�����
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
