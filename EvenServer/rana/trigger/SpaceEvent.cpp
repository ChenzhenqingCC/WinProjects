#include "SpaceEvent.h"

namespace Trigger
{
	SpaceEvent::SpaceEvent(EventCategoryID eventType)
		:m_eventType(eventType)
		,m_pSelfSpace(NULL)
		,m_context(NULL)
		,m_bindedAction(NULL)
	{
	}

	SpaceEvent::~SpaceEvent()
	{
	}

	void SpaceEvent::setCustomContext(EventContext const context)
	{
		m_context = context;
	}

}
