/*! @file
	@ingroup utils
	@brief 定义作为字符串使用的数组模板
*/

#ifndef __ZOIC_STRCHARARRAY_H
#define __ZOIC_STRCHARARRAY_H

#include "Types.h"

namespace Zoic
{
	/*! @brief 字符串模板
		@ingroup utils
		@param LEN 最大字符长度
	*/
	template <const WORD LEN>
	class StrCharArray
	{
	public:
		static WORD getLengthLimit();

		//@{
		/*! @brief 赋值操作符
		@return 对象自身引用
		@param str 字符串
		*/
		//StrCharArray & operator = (const char* str);
		//StrCharArray & operator = (const StrCharArray & str);
		//@}

		//!字符串转换操作符
		operator const char* () const;

		//@{
		/*! @brief 相等比较操作符
		@param str 字符串
		*/
		bool operator == (const char* str) const;
		bool operator == (const StrCharArray& str) const;
		//@}

		//@{
		/*! @brief 不等比较操作符
		@param str 字符串
		*/
		bool operator != (const char* str) const;		
		bool operator != (const StrCharArray& str) const;
		//@}
		//!设置字符串
		void setString(
			const char* str	//!< 字符串
			);
	private:

		char m_str[LEN+1];	//!< 字符串缓存
	};

	template <const WORD LEN>
	inline WORD StrCharArray<LEN>::getLengthLimit()
	{
		return LEN;
	}

	//template <const WORD LEN>
	//StrCharArray<LEN> & StrCharArray<LEN>::operator = (const char* str)
	//{
	//	setString(str);
	//	return *this;
	//}

	//template <const WORD LEN>
	//StrCharArray<LEN> & StrCharArray<LEN>::operator = (const StrCharArray & str)
	//{
	//	setString(str.m_str);
	//	return *this;
	//}

	template <const WORD LEN>
	StrCharArray<LEN>::operator const char* () const
	{
		return m_str;
	}

	template <const WORD LEN>
	bool StrCharArray<LEN>::operator == (const char* str) const
	{
		return strcmp(m_str,str)==0;
	}

	template <const WORD LEN>
	bool StrCharArray<LEN>::operator != (const char* str) const
	{
		return !(*this == str);
	}

	template <const WORD LEN>
	bool StrCharArray<LEN>::operator == (const StrCharArray& str) const
	{
		return (*this) == str.m_str;
	}

	template <const WORD LEN>
	bool StrCharArray<LEN>::operator != (const StrCharArray& str) const
	{
		return !(*this == str);
	}

	template <const WORD LEN>
	void StrCharArray<LEN>::setString(const char* str)
	{
		strlcpy(m_str,str,LEN+1);
		return;
	}

	class Message;
	namespace MessageImpl
	{
		void setString(Message & msg,const char *str,WORD buffSize);
		void getString(Message & msg,char *str,WORD buffSize);
	}

	template <const WORD LEN>
	Message & operator << ( Message &msg,const StrCharArray<LEN> & str)
	{
		MessageImpl::setString(msg,(const char *)str,LEN+1);
		return msg;
	}

	template <const WORD LEN>
	Message & operator >> ( Message &msg, StrCharArray<LEN> &str)
	{
		MessageImpl::getString(msg,const_cast<char *>((const char *)str),LEN+1);
		return msg;
	}
}

#endif
