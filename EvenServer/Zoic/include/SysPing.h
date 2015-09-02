/*! @file
	@ingroup buildin
	@brief 声明系统PING处理函数
*/

#ifndef __SYSPING_H
#define __SYSPING_H

#include "Types.h"

namespace Zoic
{
	class Message;
	class Session;

	/*! @brief 处理系统PING请求
		@ingroup buildin
	*/
	bool onSysPingReq(
		Session *pSession,	//!< 连接对象指针
		Message &msg		//!< 网络消息
		);

	/*! @brief 处理系统PING响应
		@ingroup buildin
	*/
	bool onSysPingAck(
		Session *pSession,	//!< 连接对象指针
		Message &msg		//!< 网络消息
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
		DWORD * 	times;			//!< 系统PING时间数组地址
		BYTE		timesSize;		//!< 系统PING时间数组大小
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
