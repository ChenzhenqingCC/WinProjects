#ifndef __TRIGGER_H__
#define __TRIGGER_H__

#include <string>
#include "RanaHeader.h"
#include "RanaTriggerStat.h"	//2006-09-19触发器统计类的头文件
#include "RanaEvent.h"
#include "trigger/SpaceAction.h"

class RanaSpace;
class RanaTrigger
	:public Trigger::SpaceAction
{
	friend class RanaTriggerBank;
public:
	RanaTrigger();
	virtual ~RanaTrigger();

	const RanaTriggerName &getName();
	void setOnOff(bool isOn);
	bool getOnOff();
	
	RanaWORD getHandle() const;
	RanaTriggerStat* getTriggerStat();
protected:
	RanaSpace* getSelfSpace() const;
	virtual bool doAction();
	// 条件检查
	bool checkCondition();
	// 执行动作
	void directDoAction();
protected:
	static int setRanaScriptCondition(RanaSpace* pSpace,const char* pTriggerName,const char* conditionDetail);
	static int setRanaScriptAction(RanaSpace* pSpace,const char* pTriggerName,const char* actionDetail);
	static int cleanRanaScriptCondition(RanaSpace* pSpace,const char* pTriggerName);
	static int cleanRanaScriptAction(RanaSpace* pSpace,const char* pTriggerName);
	static int deleteRanaScriptCondition(RanaSpace* pSpace,const char* pTriggerName);
	static int deleteRanaScriptAction(RanaSpace* pSpace,const char* pTriggerName);
private:
	void setName(const RanaTriggerName &_name);
	virtual bool doAction(Trigger::SpaceEvent* pTriggerEvent);

	RanaTriggerName m_name;
	bool m_isOn;

	RanaWORD m_handle;
	RanaTriggerStat* m_triggerStat;
};

inline RanaWORD RanaTrigger::getHandle() const
{
	return m_handle;
}

#endif //__TRIGGER_H__
