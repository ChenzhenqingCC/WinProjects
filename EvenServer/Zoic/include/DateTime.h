/*! @file
	@ingroup utils
	@brief 定义日期时间类
*/

#ifndef __ZOIC_DATETIME_H
#define __ZOIC_DATETIME_H
#include <time.h>
#include <ostream>
#include "Types.h"

namespace Zoic
{
	class DateTime;
	/*! @brief 日期类
		@ingroup utils
	*/
	class Date
	{
	public:
		short int year;			//!< 年
		unsigned char month;	//!< 月,1-12
		unsigned char day;		//!< 日,1-31
		//@{
		//!构造函数
		Date();
		Date(
			short int y,		//!< 年
			unsigned char m,	//!< 月,1-12
			unsigned char d		//!< 日,1-31
			);
		Date(
			const DateTime& other	//!< 日期时间对象
			);
		Date(
			TIME_SEC t			//!< UNIX时间戳
			);
		//@}

		//!UNIX时间戳转换操作符
		operator TIME_SEC() const;
	};
	std::ostream& operator <<(std::ostream& os,const Date& d);
	
	/*! @brief 时间类
		@ingroup utils
	*/
	class Time
	{
	public:
		unsigned char hour;		//!< 时,0-23
		unsigned char minute;	//!< 分,0-59
		unsigned char second;	//!< 秒,0-59

		//@{
		//!构造函数
		Time();
		Time(
			unsigned char h,	//!< 时,0-23
			unsigned char m,	//!< 分,0-59
			unsigned char s		//!< 秒,0-59
			);
		Time(
			const DateTime& other	//!< 日期时间对象
			);
		Time(
			TIME_SEC t			//!< UNIX时间戳
			);
		//@}

		//!UNIX时间戳转换操作符
		operator TIME_SEC() const;
	};
	std::ostream& operator <<(std::ostream& os,const Time& t);

	/*! @brief 日期时间类
		@ingroup utils
	*/
	class DateTime
	{
	public:
		short int year;			//!< 年
		unsigned char month;	//!< 月,1-12
		unsigned char day;		//!< 日,1-31
		unsigned char hour;		//!< 时,0-23
		unsigned char minute;	//!< 分,0-59
		unsigned char second;	//!< 秒,0-59

		//@{
		//!构造函数
		DateTime();
		DateTime(
			short int y,		//!< 年
			unsigned char mon,	//!< 月,1-12
			unsigned char d,	//!< 日,1-31
			unsigned char h,	//!< 时,0-23
			unsigned char min,	//!< 分,0-59
			unsigned char s		//!< 秒,0-59
			);
		DateTime(
			const Date &d,		//!< 日期对象
			const Time &t		//!< 时间对象
			);
		DateTime(
			const Date &d		//!< 日期对象
			);
		DateTime(
			const Time &t		//!< 时间对象
			);
		DateTime(
			TIME_SEC t			//!< UNIX时间戳
			);
		//@}

		//!UNIX时间戳转换操作符
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
