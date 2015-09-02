/*! @file
	@ingroup buildin
	@brief ����ϵͳPING������
*/

#ifndef __SYSPING_H
#define __SYSPING_H

#include "Types.h"

namespace Zoic
{
	class Message;
	class Session;

	/*! @brief ����ϵͳPING����
		@ingroup buildin
	*/
	bool onSysPingReq(
		Session *pSession,	//!< ���Ӷ���ָ��
		Message &msg		//!< ������Ϣ
		);

	/*! @brief ����ϵͳPING��Ӧ
		@ingroup buildin
	*/
	bool onSysPingAck(
		Session *pSession,	//!< ���Ӷ���ָ��
		Message &msg		//!< ������Ϣ
		);

	class SysPingTime
	{
	public:
		SysPingTime();
		~SysPingTime();
		void init(BYTE maxCount,const DWORD *pingTimes);
		void clear();
		DWORD getNextPingTime(BYTE count) const;
		BYTE getMaxPingCount() const;
	private:
		DWORD * 	times;			//!< ϵͳPINGʱ�������ַ
		BYTE		timesSize;		//!< ϵͳPINGʱ�������С
	};

	inline SysPingTime::SysPingTime()
		:timesSize(0)
		,times(NULL)
	{
	}

	inline SysPingTime::~SysPingTime()
	{
		clear();
	}

	inline void SysPingTime::init(BYTE maxCount,const DWORD *pingTimes)
	{
		clear();
		if( !maxCount )
		{
			return;
		}
		timesSize = maxCount;
		times = new DWORD[maxCount];
		for(BYTE i = 0; i <maxCount; ++i )
		{
			times[i] = pingTimes[i];
		}
	}

	inline void SysPingTime::clear()
	{
		timesSize = 0;
		if(times)
		{
			delete [] times;
		}
	}

	inline DWORD SysPingTime::getNextPingTime(BYTE count) const
	{
		return times[count];
	}

	inline BYTE SysPingTime::getMaxPingCount() const
	{
		return timesSize;
	}

	extern SysPingTime g_sysPingTime;
}

#endif
