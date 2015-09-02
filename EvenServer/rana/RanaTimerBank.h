#ifndef __RANA_TIMER_BANK_H__
#define __RANA_TIMER_BANK_H__

#pragma warning (disable : 4786)

#include <map>
#include <set>
#include <list>
#include "RanaTimer.h"

/*
Desc: ��ʱ��������
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

	// ����һ���µļ�ʱ�������ظü�ʱ����Handle
	RanaTimerHandle createTimer(const RanaSpaceHandle& _spaceHandle);
	// ɾ��һ��RanaSpace��Ӧ�����м�ʱ��
	bool delTimer(const RanaSpaceHandle& _spaceHandle);
	// ��ʱ������time΢��
	void routine(RanaTIME _expiretime); 

	// ����/��ü�ʱ����ʱ����
	void setTimerPeriodTime( const RanaSpaceHandle& _spaceHandle, RanaTimerHandle _timerHandle, 
		RanaTIME _periodTime);
	RanaTIME getTimerPeriodTime( const RanaSpaceHandle& _spaceHandle, RanaTimerHandle _timerHandle);
	// ����/��ü�ʱ�����ظ�ѡ��
	void setTimerRepeat( const RanaSpaceHandle& _spaceHandle, RanaTimerHandle _timerHandle, 
		bool _isRepeat);
	bool getTimerRepeat( const RanaSpaceHandle& _spaceHandle, RanaTimerHandle _timerHandle);
	// ����/��ü�ʱ������״̬
    void setTimerOnOff(	const RanaSpaceHandle& _spaceHandle, RanaTimerHandle _timerHandle, 
		bool _isOn);
	bool getTimerOnOff(	const RanaSpaceHandle& _spaceHandle, RanaTimerHandle _timerHandle);
	// ��ü�ʱ��ʣ��ʱ�䣨��λ�����룩
	RanaTIME getTimerRestTime( const RanaSpaceHandle& _spaceHandle, RanaTimerHandle _timerHandle);
	// ���ü�ʱ��
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
//	std::map<RanaSpaceName, std::map<RanaTimerHandle, RanaTimer*> > m_timerMap; // ��ʱ��ӳ��
};

#endif //__RANA_TIMER_BANK_H__
