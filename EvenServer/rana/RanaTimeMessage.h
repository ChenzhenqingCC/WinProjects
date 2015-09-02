#ifndef __RANA_TIMEMESSAGE_H__
#define __RANA_TIMEMESSAGE_H__

#include "RanaMessage.h"

class RanaTimeMessage : public RanaMessage
{
public:
	RanaTimeMessage();
	virtual ~RanaTimeMessage();

	void setTimerHandle(RanaTimerHandle _handle);
	RanaTimerHandle getTimerHandle();

private:
	RanaTimerHandle m_timerHandle;
};

#endif //__RANA_TIMEMESSAGE_H__
