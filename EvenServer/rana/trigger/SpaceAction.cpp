#include "SpaceAction.h"

namespace Trigger
{
	SpaceAction::SpaceAction()
		:m_pSelfSpace(NULL)
		,m_context(NULL)
	{
	}

	SpaceAction::~SpaceAction()
	{
	}

	void SpaceAction::setCustomContext(ActionContext const context)
	{
		m_context = context;
	}
}
