#ifndef __RANA_FEEDBACK_H__
#define __RANA_FEEDBACK_H__

class RanaEvent;
class RanaTrigger;
class RanaFeedback
{
public:
	virtual void onEventAdded(RanaEvent* pEvent);
	virtual void onEventRemoved(RanaEvent* pEvent);
	virtual void onTriggerAdded(RanaTrigger* pTrigger);
	virtual void onTriggerRemoved(RanaTrigger* pTrigger);
};

inline void RanaFeedback::onEventAdded(RanaEvent* pEvent)
{
}

inline void RanaFeedback::onEventRemoved(RanaEvent* pEvent)
{
}

inline void RanaFeedback::onTriggerAdded(RanaTrigger* pTrigger)
{
}

inline void RanaFeedback::onTriggerRemoved(RanaTrigger* pTrigger)
{
}

#endif //__RANA_SPACE_H__

