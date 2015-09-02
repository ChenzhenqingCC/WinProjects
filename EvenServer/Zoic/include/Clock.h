/*! @file
	@ingroup buildin
	@brief ����ʱ����
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
	/*! @brief ����Ϣ�洢ģ��
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
		/*! @brief ��ȡʱ��
			@return UNIXʱ���	
		*/
		TIME_SEC getTime();

		/*! @brief ��ȡʱ��
			@return timeval�ṹ
		*/
		const timeval & getTimeVal();

		//!ȡ����ȥ������
		DWORD getSeconds();

		//!ȡ����ȥ�ĺ�����
		const TIME_MSEL & getMilliSeconds();
		
		//!ȡ�ó���������������ȥ�ĺ�����
		const TIME_MSEL getPastSeconds();

		//!����ʱ��,����ʱ��״̬
		void setTime();
#ifndef ZOIC_NO_SERVICE
	private:
#else
	public:
#endif
		Clock();
	private:
		//!�������̣�����ʱ��
		int run();

		timeval			m_current;			//!< ��ǰʱ��
		DWORD			m_second;			//!< ��ȥ������
		TIME_MSEL 	m_millisecond;		//!< ��ȥ�ĺ�����
		TIME_MSEL   m_startsecond;		//!< ����ʼ����ʱ�ĺ�����
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
