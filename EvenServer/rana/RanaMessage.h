#ifndef __RANA_MESSAGE_H__
#define __RANA_MESSAGE_H__

#include "RanaHeader.h"
#include "trigger/Message.h"

class RanaMessage
	:public Trigger::Message
{
public:
	RanaMessage(RanaEventType eventType);
};

#endif //__RANA_MESSAGE_H__
