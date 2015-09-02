#ifndef __RANACONTEXT_H__
#define __RANACONTEXT_H__

#include "RanaSpace.h"

class RanaContext
{
	friend class RanaSpaceBank;
	friend class RanaTrigger;
protected:
	virtual void onBeforeBurstTrigger(RanaSpace* pSpace, const RanaTrigger* pTrigger){};
	virtual bool onTriggerEventHit(RanaSpace* pSpace, const RanaTrigger* pTrigger){return true;};
	virtual bool onTriggerConditionSuccess(RanaSpace* pSpace, const RanaTrigger* pTrigger){return true;};
	virtual void onAfterBurstTrigger(RanaSpace* pSpace, const RanaTrigger* pTrigger){};
private:
	virtual RanaSpace::EXCEPTION_METHOD onRanaException(RanaSpace::EXCEPTION_CODE) = 0;
};

#endif
