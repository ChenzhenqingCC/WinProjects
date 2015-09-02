/*! @file
	@ingroup mysql
	@brief 定义数据库参数值结构
*/

#ifndef __ZOID_PARAMETERVALUE_H
#define __ZOID_PARAMETERVALUE_H

#include "DateTime.h"

namespace Zoic
{
	/*! @brief 数据库参数值结构
		@ingroup mysql
	*/
	struct ParameterValue
	{
		~ParameterValue();
		enum TYPE
		{
			T_INTEGER,		//!< 整型
			T_INTEGERU,		//!< 无符号整型
			T_DOUBLE,		//!< 浮点型
			T_STRING,		//!< 字符串
			T_DATE,			//!< 日期型
			T_TIME,			//!< 时间型
			T_DATETIME		//!< 日期时间型
		} type;

		//!根据输入参数ParameterType中type的来同类型指向不同的值
		union
		{
			long long				_integer;						//!< 整      型：long long数据
			unsigned long long		_integerU;						//!< 无符号整型：unsigned long long数据
			double					_double;						//!< 浮  点  型：double数据
			char					_datetime[sizeof(DateTime)];	//!< 时  间  型：DateTime
			struct
			{
				const char *		buffer;			//!< 缓冲区
				bool				dynamicAlloced;	//!< 是否动态分配
			}						_string;		//!< 字符串：指向字符串的指针
		} data;
		unsigned long	length;					//!< 长度

		//@{
		/*! @brief 设置值
			@param val 值
		*/
		void setValue(const char &val);
		void setValue(const unsigned char &val);
		void setValue(const short &val);
		void setValue(const unsigned short &val);
		void setValue(const int &val);
		void setValue(const unsigned int &val);
		void setValue(const long &val);
		void setValue(const unsigned long &val);
		void setValue(const long long &val);
		void setValue(const unsigned long long &val);
		void setValue(const float &val);
		void setValue(const double &val);
		/*! @param val 字符串
			@param len 长度
			@param dynamic 是否动态分配
		*/
		void setValue(
			const char * val,	
			unsigned int len,	
			bool dynamic=false	
			);
		void setValue(const Date &val);
		void setValue(const Time &val);
		void setValue(const DateTime &val);
		//!设置整数
		void setInteger(const long long &val);
		//!设置无符号整数
		void setIntegerU(const unsigned long long &val);
		//!设置浮点数
		void setDouble(const double &val);
		//@}

		//@{
		/*! @brief 获得值
			@param val 值
		*/
		void getValue(char &val);
		void getValue(unsigned char &val);
		void getValue(short &val);
		void getValue(unsigned short &val);
		void getValue(int &val);
		void getValue(unsigned int &val);
		void getValue(long &val);
		void getValue(unsigned long &val);
		void getValue(long long &val);
		void getValue(unsigned long long &val);
		void getValue(float &val);
		void getValue(double &val);
		void getValue(std::string &val);
		void getValue(Date &val);
		void getValue(Time &val);
		void getValue(DateTime &val);
		//!获得整数
		void getInteger(long long &val);
		//!获得无符号整数
		void getIntegerU(unsigned long long &val);
		//!获得浮点数
		void getDouble(double &val);
		//@}

	private:
		//! 设置字符串值
		void setStringValue(
			const char * val,	//!< 字符串
			unsigned int len,	//!< 长度
			bool dynamic		//!< 是否动态分配
			);
	};

	inline ParameterValue::~ParameterValue()
	{
		//删除动态分配的字符
		if(type==ParameterValue::T_STRING && data._string.dynamicAlloced)
		{
			delete [] data._string.buffer;
		}
	}
	
	inline void ParameterValue::setValue(const char &val)
	{
		setInteger(val);
	}

	inline void ParameterValue::setValue(const unsigned char &val)
	{
		setIntegerU(val);
	}

	inline void ParameterValue::setValue(const short &val)
	{
		setInteger(val);
	}

	inline void ParameterValue::setValue(const unsigned short &val)
	{
		setIntegerU(val);
	}

	inline void ParameterValue::setValue(const int &val)
	{
		setInteger(val);
	}

	inline void ParameterValue::setValue(const unsigned int &val)
	{
		setIntegerU(val);
	}

	inline void ParameterValue::setValue(const long &val)
	{
		setInteger(val);
	}

	inline void ParameterValue::setValue(const unsigned long &val)
	{
		setIntegerU(val);
	}

	inline void ParameterValue::setValue(const long long &val)
	{
		setInteger(val);
	}

	inline void ParameterValue::setValue(const unsigned long long &val)
	{
		setIntegerU(val);
	}

	inline void ParameterValue::setValue(const float &val)
	{
		setDouble(val);
	}

	inline void ParameterValue::setValue(const double &val)
	{
		setDouble(val);
	}

	inline void ParameterValue::getValue(char &val)
	{
		long long tmp;
		getInteger(tmp);
		val=static_cast<char>(tmp);
	}

	inline void ParameterValue::getValue(unsigned char &val)
	{
		unsigned long long tmp;
		getIntegerU(tmp);
		val=static_cast<unsigned char>(tmp);
	}

	inline void ParameterValue::getValue(short &val)
	{
		long long tmp;
		getInteger(tmp);
		val=static_cast<short>(tmp);
	}

	inline void ParameterValue::getValue(unsigned short &val)
	{
		unsigned long long tmp;
		getIntegerU(tmp);
		val=static_cast<unsigned short>(tmp);
	}

	inline void ParameterValue::getValue(int &val)
	{
		long long tmp;
		getInteger(tmp);
		val=static_cast<int>(tmp);
	}

	inline void ParameterValue::getValue(unsigned int &val)
	{
		unsigned long long tmp;
		getIntegerU(tmp);
		val=static_cast<unsigned int>(tmp);
	}

	inline void ParameterValue::getValue(long &val)
	{
		long long tmp;
		getInteger(tmp);
		val=static_cast<long>(tmp);
	}

	inline void ParameterValue::getValue(unsigned long &val)
	{
		unsigned long long tmp;
		getIntegerU(tmp);
		val=static_cast<unsigned long>(tmp);
	}

	inline void ParameterValue::getValue(long long &val)
	{
		getInteger(val);
	}

	inline void ParameterValue::getValue(unsigned long long &val)
	{
		getIntegerU(val);
	}

	inline void ParameterValue::getValue(float &val)
	{
		double tmp;
		getDouble(tmp);
		val=static_cast<float>(tmp);
	}

	inline void ParameterValue::getValue(double &val)
	{
		getDouble(val);
	}
}

#endif
