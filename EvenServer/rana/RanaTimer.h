#ifndef __RANA_TIMER_H__
#define __RANA_TIMER_H__

#include "RanaHeader.h"

/*
Desc: 计时器类
author: YYBear
*/
class RanaTimer
{
public:
	RanaTimer();
	virtual ~RanaTimer();

	void setHandle(RanaTimerHandle _handle);
	RanaTimerHandle getHandle();
	void setSpaceName(RanaSpaceName _spaceName);
	const RanaSpaceName& getSpaceName();
	void setPeriodTime(RanaTIME _periodTime); // periodTime为百分之一秒
	RanaTIME getPeriodTime();
	void setRepeat(bool _isRepeat);
	bool getRepeat();
	void setOnOff(bool _isOn);
	bool getOnOff();
	RanaTIME getRestTime();
	void reset();

	void setSpaceHandle(const RanaSpaceHandle& _spaceHandle);
	const RanaSpaceHandle& getSpaceHandle();
	void setExpireTime(RanaTIME _expiredTime); // periodTime为百分之一秒
	RanaTIME getExpireTime();

	void setRestTime(RanaTIME _restTime);

private:
	RanaSpaceName m_spaceName;	// 计时器所在的Rana空间名
	RanaSpaceHandle m_spaceHandle;	// 计时器所在的Rana空间的句柄
	RanaTimerHandle m_handle;	// 计时器Handle，从1开始，0未使用
	RanaTIME m_expireTime;	// 到期时间
	RanaTIME m_periodTime;	// 总计时长度（微秒）
	RanaTIME m_restTime;	// 剩余时间长度（微秒）
	bool m_isRepeat;	// 是否循环计时
	bool m_isOn;	// 计时器开关
};

class CompTriggerKey
{
public:
	const bool operator()(RanaTimer* a, RanaTimer* b) const
	{
		return a->getExpireTime() < b->getExpireTime();
	}
};

inline void RanaTimer::setSpaceHandle(const RanaSpaceHandle& _spaceHandle)
{
	m_spaceHandle = _spaceHandle;
}

inline const RanaSpaceHandle& RanaTimer::getSpaceHandle()
{
	return m_spaceHandle;
}

inline void RanaTimer::setExpireTime(RanaTIME _expiredTime)
{
	m_expireTime = _expiredTime;
}

inline RanaTIME RanaTimer::getExpireTime()
{
	return m_expireTime;
}

inline void RanaTimer::setRestTime(RanaTIME _restTime)
{
	m_restTime = _restTime;
}

#endif //__RANA_TIMER_H__
