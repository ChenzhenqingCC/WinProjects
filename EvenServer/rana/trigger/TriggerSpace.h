#ifndef __TRIGGER_SPACE_H__
#define __TRIGGER_SPACE_H__

#include "TriggerHeader.h"
#include "SpaceEvent.h"
#include "SpaceEventMap.h"
#include "SpaceAction.h"
#include "Message.h"

namespace Trigger
{
	class TriggerSpace
	{
	public:
		COUNT onMessage(Message* pMsg);
	public:
		typedef std::set<SpaceAction*> ACTOIN_LIST;
		typedef std::set<SpaceEvent*> EVENT_LIST;
	public:
		TriggerSpace();
		virtual ~TriggerSpace();
		bool possessAction(SpaceAction* pAction);
		bool possessEvent(SpaceEvent* pEvent);
		bool bindEventToAction(SpaceEvent* pEvent,SpaceAction* pBindAction);
		SpaceAction* releaseNonBindAction(SpaceAction* pAction);
		SpaceEvent* releaseNonBindEvent(SpaceEvent* pEvent);
		const ACTOIN_LIST& listActions() const;
		const EVENT_LIST& listEvents() const;
	public:
		void collectAllEventsByType(EventCategoryID eventType, std::vector<SpaceEvent*>& output);
	private:
		bool triggerEvent(SpaceEvent* pEvent,Message* pMsg);
		ACTOIN_LIST m_actions;
		EVENT_LIST m_events;
		SpaceEventMap m_maintainersByCategory;
	};

	inline const TriggerSpace::ACTOIN_LIST& TriggerSpace::listActions() const
	{
		return m_actions;
	}

	inline const TriggerSpace::EVENT_LIST& TriggerSpace::listEvents() const
	{
		return m_events;
	}
}

#endif

