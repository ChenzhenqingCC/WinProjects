#ifndef __TRIGGER_SPACE_ACTION_H__
#define __TRIGGER_SPACE_ACTION_H__

#include "TriggerHeader.h"
#include <set>

namespace Trigger
{
	class TriggerSpace;
	class SpaceEvent;
	class SpaceAction
	{
		friend class TriggerSpace;
	public:
		typedef void* ActionContext;
		ActionContext const& getCustomContext() const;
		const std::set<SpaceEvent*>& getBindedEvents() const;
		//运行放案
		virtual bool doAction(SpaceEvent* pTriggerEvent);
	protected:
		SpaceAction();
		virtual ~SpaceAction();
		TriggerSpace* getPossessingSpace();
		void setCustomContext(ActionContext const context);
	private:
		ActionContext m_context;
		TriggerSpace* m_pSelfSpace;
		std::set<SpaceEvent*> m_bindedEvents;
	};

	inline SpaceAction::ActionContext const& SpaceAction::getCustomContext() const
	{
		return m_context;
	}

	inline const std::set<SpaceEvent*>& SpaceAction::getBindedEvents() const
	{
		return m_bindedEvents;
	}

	inline TriggerSpace* SpaceAction::getPossessingSpace()
	{
		return m_pSelfSpace;
	}

	inline bool SpaceAction::doAction(SpaceEvent* pTriggerEvent)
	{
		return false;
	}
}

#endif

