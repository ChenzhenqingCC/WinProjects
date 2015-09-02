#ifndef __RANA_TIMER_BANK_H__
#define __RANA_TIMER_BANK_H__

#pragma warning (disable : 4786)

#include <map>
#include <set>
#include <list>
#include "RanaTimer.h"

/*
Desc: 计时器管理类
author: YYBear
*/
class RanaTimerBank
{
public:
	class CompRanaTimerPoint
	{
	public:
		const bool operator()(RanaTimer* a, RanaTimer* b) const
		{
			return a->getExpireTime() < b->getExpireTime();
		}
	};
	struct ListIterator
	{
		enum
		{
			E_ACTIVE,
			E_PAUSED
		} itType;
		std::multiset<RanaTimer*,CompRanaTimerPoint>::iterator activeIt;
		std::list<RanaTimer*>::iterator pausedIt;
	};
	typedef std::map<RanaTimerHandle, ListIterator> RANA_HANDLEITMAP;
	struct RanaTimerHandleManager
	{
		int nextHandle;
		RANA_HANDLEITMAP handleMap;
//		operator RANA_HANDLEITMAP&()
//		{
//			return handleMap;
//		}
		ListIterator& operator [](RanaTimerHandle handle)
		{
			return handleMap[handle];
		}
		RanaTimerHandleManager():nextHandle(1)
		{
		}
	};
	typedef std::map<RanaWORD, RanaTimerHandleManager> RANA_SPACEITMAP;

	static RanaTimerBank* getInstance();
	static void release();

	// 构造一个新的计时器，返回该计时器的Handle
	RanaTimerHandle createTimer(const RanaSpaceHandle& _spaceHandle);
	// 删除一个RanaSpace对应的所有计时器
	bool delTimer(const RanaSpaceHandle& _spaceHandle);
	// 计时器运行time微秒
	void routine(RanaTIME _expiretime); 

	// 设置/获得计时器计时周期
	void setTimerPeriodTime( const RanaSpaceHandle& _spaceHandle, RanaTimerHandle _timerHandle, 
		RanaTIME _periodTime);
	RanaTIME getTimerPeriodTime( const RanaSpaceHandle& _spaceHandle, RanaTimerHandle _timerHandle);
	// 设置/获得计时器可重复选项
	void setTimerRepeat( const RanaSpaceHandle& _spaceHandle, RanaTimerHandle _timerHandle, 
		bool _isRepeat);
	bool getTimerRepeat( const RanaSpaceHandle& _spaceHandle, RanaTimerHandle _timerHandle);
	// 设置/获得计时器开关状态
    void setTimerOnOff(	const RanaSpaceHandle& _spaceHandle, RanaTimerHandle _timerHandle, 
		bool _isOn);
	bool getTimerOnOff(	const RanaSpaceHandle& _spaceHandle, RanaTimerHandle _timerHandle);
	// 获得计时器剩余时间（单位：毫秒）
	RanaTIME getTimerRestTime( const RanaSpaceHandle& _spaceHandle, RanaTimerHandle _timerHandle);
	// 重置计时器
	void resetTimer( const RanaSpaceHandle& _spaceHandle, RanaTimerHandle _timerHandle);

	void initCurrentTime(RanaTIME _time);

protected:
	RanaTimer* getTimer( const RanaSpaceHandle& _spaceHandle, 
		RanaTimerHandle _timerHandle);

private:
	RanaTimerBank();
	~RanaTimerBank();

	static RanaTimerBank *INSTANCE;

	std::multiset<RanaTimer*,CompRanaTimerPoint> m_activeTimer;
	std::list<RanaTimer*> m_pausedTimer;
	RANA_SPACEITMAP m_timerMap;
	RanaTIME m_curTime;
//	std::map<RanaSpaceName, std::map<RanaTimerHandle, RanaTimer*> > m_timerMap; // 计时器映射
};

#endif //__RANA_TIMER_BANK_H__
