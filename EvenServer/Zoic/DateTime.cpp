#include <string.h>
#include <iomanip>
#include "include/Types.h"
#include "include/DateTime.h"

using namespace Zoic;

Date::Date(TIME_SEC t)
{
	time_t timeValue = static_cast<time_t>( t );
	struct tm * tmstr=localtime(&timeValue);
	year=tmstr->tm_year+1900;
	month=tmstr->tm_mon+1;
	day=tmstr->tm_mday;
}

Date::operator TIME_SEC() const
{
	struct tm tmstr;
	memset(&tmstr,0,sizeof(tmstr));
	tmstr.tm_year=(int)year-1900;
	tmstr.tm_mon=(int)month-1;
	tmstr.tm_mday=day;
	return static_cast<TIME_SEC>( mktime(&tmstr) );
}

std::ostream& Zoic::operator <<(std::ostream& os,const Date& d)
{
	char fill = os.fill('0');
	std::ios::fmtflags flags = os.setf(std::ios::right);
	os		<< std::setw(4) << d.year << '-' 
			<< std::setw(2) << static_cast<int>(d.month) << '-'
			<< std::setw(2) << static_cast<int>(d.day);
	os.flags(flags);
	os.fill(fill);
	return os;
}

Time::Time(TIME_SEC t)
{
	time_t timeValue = static_cast<time_t>( t );
	struct tm * tmstr=localtime(&timeValue);
	hour=tmstr->tm_hour;
	minute=tmstr->tm_min;
	second=tmstr->tm_sec;
}

Time::operator TIME_SEC() const
{
	struct tm tmstr;
	memset(&tmstr,0,sizeof(tmstr));
	tmstr.tm_year=1970-1900;
	tmstr.tm_mday=2;
	tmstr.tm_hour=hour;
	tmstr.tm_min=minute;
	tmstr.tm_sec=second;
	return static_cast<TIME_SEC>( mktime(&tmstr) );
}

std::ostream&  Zoic::operator<<(std::ostream& os,const Time& t)
{
	char fill = os.fill('0');
	std::ios::fmtflags flags = os.setf(std::ios::right);
	os		<< std::setw(2) << static_cast<int>(t.hour) << ':' 
			<< std::setw(2) << static_cast<int>(t.minute) << ':'
			<< std::setw(2) << static_cast<int>(t.second);
	os.flags(flags);
	os.fill(fill);
	return os;
}

DateTime::DateTime(TIME_SEC t)
{
	struct tm * tmstr;
	if(t < 0)
	{
		tmstr = NULL;
	}
	else
	{
		time_t timeValue = static_cast<time_t>( t );
		tmstr=localtime(&timeValue);
	}
	if(tmstr)
	{
		year=tmstr->tm_year+1900;
		month=tmstr->tm_mon+1;
		day=tmstr->tm_mday;
		hour=tmstr->tm_hour;
		minute=tmstr->tm_min;
		second=tmstr->tm_sec;
	}
	else
	{//设置一个不被32位类型支持的时间
		year=2050;
		month=1;
		day=1;
		hour=0;
		minute=0;
		second=0;
	}
}

DateTime::operator TIME_SEC() const
{
	struct tm tmstr;
	memset(&tmstr,0,sizeof(tmstr));
	tmstr.tm_year=(int)year-1900;
	tmstr.tm_mon=(int)month-1;
	tmstr.tm_mday=day;
	tmstr.tm_hour=hour;
	tmstr.tm_min=minute;
	tmstr.tm_sec=second;
	return static_cast<TIME_SEC>( mktime(&tmstr) );
}


std::ostream& Zoic::operator <<(std::ostream& os, const DateTime& dt)
{
	char fill = os.fill('0');
	std::ios::fmtflags flags = os.setf(std::ios::right);
	os		<< std::setw(4) << dt.year << '-' 
			<< std::setw(2) << static_cast<int>(dt.month) << '-'
			<< std::setw(2) << static_cast<int>(dt.day) << ' '
			<< std::setw(2) << static_cast<int>(dt.hour) << ':' 
			<< std::setw(2) << static_cast<int>(dt.minute) << ':'
			<< std::setw(2) << static_cast<int>(dt.second);
	os.flags(flags);
	os.fill(fill);
	return os;
}
