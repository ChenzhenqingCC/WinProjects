#ifndef __RANA_TIMEEVENT_H__
#define __RANA_TIMEEVENT_H__

#include "RanaEvent.h"

class RanaTimeEvent : public RanaEvent
{
public:
	RanaTimeEvent();
	virtual ~RanaTimeEvent();

	virtual bool preValidate(RanaMessage *_msg);
	virtual void getArgument(std::vector<RanaVariant> & args);
	void setTimerHandle(RanaTimerHandle _handle);
	RanaTimerHandle getTimerHandle(RanaMessage *_msg);

private:
	RanaTimerHandle m_timerHandle;
};
#endif //__RANA_TIMEEVENT_H__
