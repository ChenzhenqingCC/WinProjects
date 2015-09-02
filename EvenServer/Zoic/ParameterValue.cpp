#include <sstream>
#include <iomanip>
#include "include/Types.h"
#include "include/ParameterValue.h"

using namespace Zoic;

template <class T> static inline void setPrecision(std::ostringstream &outs,T val)
{
}

template <> 
inline void setPrecision(std::ostringstream &outs,const float & val)
{
	outs.precision(7);	// max digits in IEEE 754 single-prec float
}

template <> 
inline void setPrecision(std::ostringstream &outs,const double & val)
{
	outs.precision(16);	// max digits in IEEE 754 double-prec float
}

#define SET_CONVERT(DT)													\
	switch(type) 														\
	{																	\
		case T_INTEGER:													\
			data._integer=static_cast<long long>(val);					\
			break;														\
		case T_INTEGERU:												\
			data._integerU=static_cast<unsigned long long>(val);		\
			break;														\
		case T_DOUBLE:													\
			data._double=static_cast<double>(val);						\
			break;														\
		case T_STRING:													\
		{																\
			std::ostringstream outs;									\
			setPrecision(outs,val);										\
			outs << val;												\
			std::string str=outs.str();									\
			setValue(str.c_str(),static_cast<unsigned int>(str.length()),true);	\
			break;														\
		}																\
		case T_DATE:													\
		case T_TIME:													\
		case T_DATETIME:												\
		{																\
			new(data._datetime) DateTime(static_cast<TIME_SEC>(DT));	\
			break;														\
		}																\
	}

#define GET_CONVERT(TP,TO)												\
	switch(type)														\
	{																	\
		case T_INTEGER:													\
			val = static_cast<TP>(data._integer);						\
			break;														\
		case T_INTEGERU:												\
			val = static_cast<TP>(data._integerU);						\
			break;														\
		case T_DOUBLE:													\
			val = static_cast<TP>(data._double);						\
			break;														\
		case T_STRING:													\
			val = TO;													\
			break;														\
		case T_DATE:													\
		case T_TIME:													\
		case T_DATETIME:												\
		{																\
			val = *reinterpret_cast<DateTime *>(data._datetime);		\
			break;														\
		}																\
	}

static inline Date strtoDate(const char *val)
{
	int year=0,month=0,day=0;
	sscanf(val,"%d%*c%d%*c%d",&year,&month,&day);
	return Date(year,month,day);
}

static inline Time strtoTime(const char *val)
{
	int hour=0,minute=0,second=0;
	sscanf(val,"%d%*c%d%*c%d",&hour,&minute,&second);
	return Time(hour,minute,second);
}

static inline DateTime strtoDateTime(const char *val)
{
	int year=0,month=0,day=0;
	int hour=0,minute=0,second=0;
	sscanf(val,"%d%*c%d%*c%d%*c%d%*c%d%*c%d",&year,&month,&day,&hour,&minute,&second);
	return DateTime(year,month,day,hour,minute,second);
}

void ParameterValue::setInteger(const long long &val)
{
	SET_CONVERT(static_cast<time_t>(val))
}

void ParameterValue::setIntegerU(const unsigned long long &val)
{
	SET_CONVERT(static_cast<time_t>(val))
}

void ParameterValue::setDouble(const double &val)
{
	SET_CONVERT(static_cast<time_t>(val))
}

void ParameterValue::setValue(const Date &val)
{
	SET_CONVERT(val)
}

void ParameterValue::setValue(const Time &val)
{
	SET_CONVERT(val)
}

void ParameterValue::setValue(const DateTime &val)
{
	SET_CONVERT(val)
}

void ParameterValue::setValue(const char * val,unsigned int len,bool dynamic)
{
	switch(type)
	{
		case T_INTEGER:
			data._integer=strtoll(val,NULL,10);
			break;
		case T_INTEGERU:
			data._integerU=strtoull(val,NULL,10);
			break;
		case T_DOUBLE:
			data._double=strtod(val,NULL);
			break;
		case T_STRING:
		{
			setStringValue(val,len,dynamic);
			break;
		}
		case T_DATE:
		{
			new(&data._datetime) DateTime(strtoDate(val));
			break;
		}
		case T_TIME:
		{
			new(&data._datetime) DateTime(strtoTime(val));
			break;
		}
		case T_DATETIME:
		{
			new(&data._datetime) DateTime(strtoDateTime(val));
			break;
		}
	}
}
	
void ParameterValue::getValue(std::string &val)
{
	std::ostringstream outs;
	switch(type)
	{
		case T_INTEGER:
			outs << data._integer;
			break;
		case T_INTEGERU:
			outs << data._integerU;
			break;
		case T_DOUBLE:
			outs << data._double;
			break;
		case T_STRING:
		{
			val.assign(data._string.buffer,length);
			return;
		}
		case T_DATE:
		{
			outs << Date(*reinterpret_cast<DateTime *>(data._datetime));
			break;
		}
		case T_TIME:
		{
			outs << Time(*reinterpret_cast<DateTime *>(data._datetime));
			break;
		}
		case T_DATETIME:
		{
			outs << *reinterpret_cast<DateTime *>(data._datetime);
			break;
		}
	}
	val=outs.str();
}

void ParameterValue::getInteger(long long &val)
{
	GET_CONVERT(long long,strtoll(data._string.buffer,NULL,10));
}

void ParameterValue::getIntegerU(unsigned long long &val)
{
	GET_CONVERT(unsigned long long,strtoull(data._string.buffer,NULL,10));
}

void ParameterValue::getDouble(double &val)
{
	GET_CONVERT(double,strtod(data._string.buffer,NULL));
}

void ParameterValue::getValue(Date &val)
{
	GET_CONVERT(TIME_SEC,strtoDate(data._string.buffer));
}

void ParameterValue::getValue(Time &val)
{
	GET_CONVERT(TIME_SEC,strtoTime(data._string.buffer));
}

void ParameterValue::getValue(DateTime &val)
{
	GET_CONVERT(TIME_SEC,strtoDateTime(data._string.buffer));
}

void ParameterValue::setStringValue(const char * val,unsigned int len,bool dynamic)
{
	if(data._string.dynamicAlloced)
	{
		delete [] data._string.buffer;
	}
	data._string.dynamicAlloced=dynamic;
	if(dynamic)
	{
		data._string.buffer=new char[len+1];
		strcpy(const_cast<char *>(data._string.buffer),val);
	}
	else
	{
		data._string.buffer=val;
	}
	length=len;
}
