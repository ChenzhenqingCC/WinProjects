/*! @file
	@ingroup utils
	@brief ��������ʱ����
*/

#ifndef __ZOIC_DATETIME_H
#define __ZOIC_DATETIME_H
#include <time.h>
#include <ostream>
#include "Types.h"

namespace Zoic
{
	class DateTime;
	/*! @brief ������
		@ingroup utils
	*/
	class Date
	{
	public:
		short int year;			//!< ��
		unsigned char month;	//!< ��,1-12
		unsigned char day;		//!< ��,1-31
		//@{
		//!���캯��
		Date();
		Date(
			short int y,		//!< ��
			unsigned char m,	//!< ��,1-12
			unsigned char d		//!< ��,1-31
			);
		Date(
			const DateTime& other	//!< ����ʱ�����
			);
		Date(
			TIME_SEC t			//!< UNIXʱ���
			);
		//@}

		//!UNIXʱ���ת��������
		operator TIME_SEC() const;
	};
	std::ostream& operator <<(std::ostream& os,const Date& d);
	
	/*! @brief ʱ����
		@ingroup utils
	*/
	class Time
	{
	public:
		unsigned char hour;		//!< ʱ,0-23
		unsigned char minute;	//!< ��,0-59
		unsigned char second;	//!< ��,0-59

		//@{
		//!���캯��
		Time();
		Time(
			unsigned char h,	//!< ʱ,0-23
			unsigned char m,	//!< ��,0-59
			unsigned char s		//!< ��,0-59
			);
		Time(
			const DateTime& other	//!< ����ʱ�����
			);
		Time(
			TIME_SEC t			//!< UNIXʱ���
			);
		//@}

		//!UNIXʱ���ת��������
		operator TIME_SEC() const;
	};
	std::ostream& operator <<(std::ostream& os,const Time& t);

	/*! @brief ����ʱ����
		@ingroup utils
	*/
	class DateTime
	{
	public:
		short int year;			//!< ��
		unsigned char month;	//!< ��,1-12
		unsigned char day;		//!< ��,1-31
		unsigned char hour;		//!< ʱ,0-23
		unsigned char minute;	//!< ��,0-59
		unsigned char second;	//!< ��,0-59

		//@{
		//!���캯��
		DateTime();
		DateTime(
			short int y,		//!< ��
			unsigned char mon,	//!< ��,1-12
			unsigned char d,	//!< ��,1-31
			unsigned char h,	//!< ʱ,0-23
			unsigned char min,	//!< ��,0-59
			unsigned char s		//!< ��,0-59
			);
		DateTime(
			const Date &d,		//!< ���ڶ���
			const Time &t		//!< ʱ�����
			);
		DateTime(
			const Date &d		//!< ���ڶ���
			);
		DateTime(
			const Time &t		//!< ʱ�����
			);
		DateTime(
			TIME_SEC t			//!< UNIXʱ���
			);
		//@}

		//!UNIXʱ���ת��������
		operator TIME_SEC() const;
	};
	std::ostream& operator <<(std::ostream& os,const DateTime& dt);
		
	inline Date::Date()
	{
	}
	
	inline Date::Date(short int y, unsigned char m, unsigned char d) :
		year(y),
		month(m),
		day(d)
	{			
	}
	
	inline Date::Date(const DateTime& other) :
		year(other.year),
		month(other.month),
		day(other.day)
	{
	}

	inline Time::Time()
	{
	}

	inline Time::Time(unsigned char h, unsigned char m, unsigned char s) :
		hour(h),minute(m),second(s)
	{
	}
	
	inline Time::Time(const DateTime& other) :
		hour(other.hour),
		minute(other.minute),
		second(other.second)
	{
	}
	
	inline DateTime::DateTime()
	{
	}
	
	inline DateTime::DateTime(short int y, unsigned char mon, unsigned char d,
		unsigned char h, unsigned char min, unsigned char s) :
		year(y),
		month(mon),
		day(d),
		hour(h),
		minute(min),
		second(s)
	{
	}
	
	inline DateTime::DateTime(const Date &d,const Time &t):
		year(d.year),
		month(d.month),
		day(d.day),
		hour(t.hour),
		minute(t.minute),
		second(t.second)
	{
	}
	
	inline DateTime::DateTime(const Date &d):
		year(d.year),
		month(d.month),
		day(d.day),
		hour(0),
		minute(0),
		second(0)
	{
	}
	
	inline DateTime::DateTime(const Time &t):
		year(1970),
		month(1),
		day(1),
		hour(t.hour),
		minute(t.minute),
		second(t.second)
	{
	}
}

#endif
