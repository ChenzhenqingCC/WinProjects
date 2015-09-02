#include "RanaEvent.h"
#include "RanaTrigger.h"
#include "RanaSpace.h"

RanaEvent::RanaEvent(RanaEventType _type)
	:Trigger::SpaceEvent(_type)
{
	m_isOn = false;
}

RanaEvent::~RanaEvent()
{
}

void RanaEvent::setOnOff(bool _isOn)
{
	m_isOn = _isOn;
}

bool RanaEvent::getOnOff()
{
	return m_isOn;
}

RanaSpace* RanaEvent::getSelfSpace() const
{
	RanaSpace* const * ppSpace = (RanaSpace*const*)getCustomContext();
	return ppSpace ? *ppSpace : NULL;
}

const RanaEventName &RanaEvent::getName()
{
	return m_name;
}

RanaTrigger* RanaEvent::getTrigger() const
{
	Trigger::SpaceAction* pAction = getBindedAction();
	if( pAction->getCustomContext() != getCustomContext() )
	{
		return NULL;
	}
	return static_cast<RanaTrigger*>(pAction);
}

bool RanaEvent::doValidate(Trigger::Message *msg) const
{
	RanaSpace* pSpace = getSelfSpace();
	RanaTrigger* pTrigger = getTrigger();
	if(pSpace && pTrigger)
	{
		pSpace->getTriggerBank()->tagMessageCount( pTrigger->getHandle() );
	}
	return preValidate( static_cast<RanaMessage*>(msg) );
}

bool RanaEvent::preValidate(RanaMessage *msg) const
{
	return m_isOn;
}
