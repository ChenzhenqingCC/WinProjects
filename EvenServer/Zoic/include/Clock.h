/*! @file
	@ingroup buildin
	@brief 定义时钟类
*/

#ifndef __ZOIC_CLOCK_H
#define __ZOIC_CLOCK_H

#include "Types.h"
#include "Module.h"
#include <time.h>
#ifdef HAVE_MINGW32
#include <winsock2.h>
#ifdef _MSC_VER
#pragma comment (lib, "winmm.lib")
#endif
#endif

namespace Zoic
{
	/*! @brief 类信息存储模板
		@ingroup buildin
	*/
	class Clock
#ifndef ZOIC_NO_SERVICE
		:public Module<Clock>
#else
		:public NoCopy
#endif
	{
#ifndef ZOIC_NO_SERVICE
		friend class Module<Clock>;
#endif
	public :
		/*! @brief 获取时间
			@return UNIX时间戳	
		*/
		TIME_SEC getTime();

		/*! @brief 获取时间
			@return timeval结构
		*/
		const timeval & getTimeVal();

		//!取得逝去的秒数
		DWORD getSeconds();

		//!取得逝去的毫秒数
		const TIME_MSEL & getMilliSeconds();
		
		//!取得程序启动到现在逝去的毫秒数
		const TIME_MSEL getPastSeconds();

		//!设置时间,更新时钟状态
		void setTime();
#ifndef ZOIC_NO_SERVICE
	private:
#else
	public:
#endif
		Clock();
	private:
		//!运行例程，更新时间
		int run();

		timeval			m_current;			//!< 当前时间
		DWORD			m_second;			//!< 逝去的秒数
		TIME_MSEL 	m_millisecond;		//!< 逝去的毫秒数
		TIME_MSEL   m_startsecond;		//!< 程序开始运行时的毫秒数
	#ifdef HAVE_MINGW32
		static int gettimeofday(struct timeval *tp, struct timezone *tzp);
		DWORD			m_tailMillisecond;
	#endif
	};

	inline Clock::Clock()
	{
	#ifdef HAVE_MINGW32
		m_startsecond=m_millisecond=m_tailMillisecond=timeGetTime();
	#endif
		setTime();
	}

	inline int Clock::run()
	{
		setTime();
		return 0;
	}
	
	inline void Clock::setTime()
	{
		gettimeofday(&m_current, NULL);
	#ifdef HAVE_MINGW32
		DWORD now = timeGetTime();
		DWORD up = now - m_tailMillisecond;
		m_tailMillisecond = now;
		m_millisecond +=up;
		m_second = static_cast<DWORD>(m_millisecond / 1000);
	#else
		timespec ts;
		clock_gettime(CLOCK_MONOTONIC,&ts);
		m_second=ts.tv_sec;
		m_millisecond=(TIME_MSEL)ts.tv_sec*1000+ts.tv_nsec/1000000;
	#endif
	}

	inline TIME_SEC Clock::getTime()
	{
		return m_current.tv_sec;
	}

	inline const timeval & Clock::getTimeVal()
	{
		return m_current;
	}

	inline unsigned long Clock::getSeconds()
	{
		return m_second;
	}

	inline const TIME_MSEL & Clock::getMilliSeconds()
	{
		return m_millisecond;
	}

	inline const TIME_MSEL Clock::getPastSeconds()
	{
		return m_millisecond - m_startsecond;
	}

#ifndef ZOIC_NO_SERVICE
	#define g_Clock Zoic::Clock::getInstance()
#else
	extern Clock theClock;
	#define g_Clock (&Zoic::theClock)
#endif
}
#endif
