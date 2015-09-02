#ifndef __RANA_TIMER_H__
#define __RANA_TIMER_H__

#include "RanaHeader.h"

/*
Desc: ��ʱ����
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
	void setPeriodTime(RanaTIME _periodTime); // periodTimeΪ�ٷ�֮һ��
	RanaTIME getPeriodTime();
	void setRepeat(bool _isRepeat);
	bool getRepeat();
	void setOnOff(bool _isOn);
	bool getOnOff();
	RanaTIME getRestTime();
	void reset();

	void setSpaceHandle(const RanaSpaceHandle& _spaceHandle);
	const RanaSpaceHandle& getSpaceHandle();
	void setExpireTime(RanaTIME _expiredTime); // periodTimeΪ�ٷ�֮һ��
	RanaTIME getExpireTime();

	void setRestTime(RanaTIME _restTime);

private:
	RanaSpaceName m_spaceName;	// ��ʱ�����ڵ�Rana�ռ���
	RanaSpaceHandle m_spaceHandle;	// ��ʱ�����ڵ�Rana�ռ�ľ��
	RanaTimerHandle m_handle;	// ��ʱ��Handle����1��ʼ��0δʹ��
	RanaTIME m_expireTime;	// ����ʱ��
	RanaTIME m_periodTime;	// �ܼ�ʱ���ȣ�΢�룩
	RanaTIME m_restTime;	// ʣ��ʱ�䳤�ȣ�΢�룩
	bool m_isRepeat;	// �Ƿ�ѭ����ʱ
	bool m_isOn;	// ��ʱ������
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
