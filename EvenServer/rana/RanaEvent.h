#ifndef __RANA_EVENT_H_
#define __RANA_EVENT_H_

#include <vector>
#include <string>
#include "RanaHeader.h"
#include "RanaMessage.h"
#include "RanaSortLable.h"
#include "trigger/SpaceEvent.h"

class RanaSpace;
class RanaTrigger;
class RanaEvent
	:public Trigger::SpaceEvent
{
	friend class RanaTriggerBank;
public:
	RanaEvent(RanaEventType _type);
	virtual ~RanaEvent();

	RanaEventType getType();
	const RanaEventName &getName();

	RanaTrigger* getTrigger() const;
public:
	virtual bool preValidate(RanaMessage *msg) const;
	void setOnOff(bool _isOn);
	bool getOnOff();

	const RanaWORD& getHandle();

	RanaSpace* getSelfSpace() const;

protected:
	virtual bool doValidate(Trigger::Message *msg) const;
private:
	RanaEventName m_name;
	RanaWORD m_handle;
	bool m_isOn;
};

inline const RanaWORD& RanaEvent::getHandle()
{
	return m_handle;
}

inline RanaEventType RanaEvent::getType()
{
	return Trigger::SpaceEvent::getType();
}

#endif //__RANA_EVENT_H_
