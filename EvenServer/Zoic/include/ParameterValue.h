/*! @file
	@ingroup mysql
	@brief �������ݿ����ֵ�ṹ
*/

#ifndef __ZOID_PARAMETERVALUE_H
#define __ZOID_PARAMETERVALUE_H

#include "DateTime.h"

namespace Zoic
{
	/*! @brief ���ݿ����ֵ�ṹ
		@ingroup mysql
	*/
	struct ParameterValue
	{
		~ParameterValue();
		enum TYPE
		{
			T_INTEGER,		//!< ����
			T_INTEGERU,		//!< �޷�������
			T_DOUBLE,		//!< ������
			T_STRING,		//!< �ַ���
			T_DATE,			//!< ������
			T_TIME,			//!< ʱ����
			T_DATETIME		//!< ����ʱ����
		} type;

		//!�����������ParameterType��type����ͬ����ָ��ͬ��ֵ
		union
		{
			long long				_integer;						//!< ��      �ͣ�long long����
			unsigned long long		_integerU;						//!< �޷������ͣ�unsigned long long����
			double					_double;						//!< ��  ��  �ͣ�double����
			char					_datetime[sizeof(DateTime)];	//!< ʱ  ��  �ͣ�DateTime
			struct
			{
				const char *		buffer;			//!< ������
				bool				dynamicAlloced;	//!< �Ƿ�̬����
			}						_string;		//!< �ַ�����ָ���ַ�����ָ��
		} data;
		unsigned long	length;					//!< ����

		//@{
		/*! @brief ����ֵ
			@param val ֵ
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
		/*! @param val �ַ���
			@param len ����
			@param dynamic �Ƿ�̬����
		*/
		void setValue(
			const char * val,	
			unsigned int len,	
			bool dynamic=false	
			);
		void setValue(const Date &val);
		void setValue(const Time &val);
		void setValue(const DateTime &val);
		//!��������
		void setInteger(const long long &val);
		//!�����޷�������
		void setIntegerU(const unsigned long long &val);
		//!���ø�����
		void setDouble(const double &val);
		//@}

		//@{
		/*! @brief ���ֵ
			@param val ֵ
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
		//!�������
		void getInteger(long long &val);
		//!����޷�������
		void getIntegerU(unsigned long long &val);
		//!��ø�����
		void getDouble(double &val);
		//@}

	private:
		//! �����ַ���ֵ
		void setStringValue(
			const char * val,	//!< �ַ���
			unsigned int len,	//!< ����
			bool dynamic		//!< �Ƿ�̬����
			);
	};

	inline ParameterValue::~ParameterValue()
	{
		//ɾ����̬������ַ�
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
