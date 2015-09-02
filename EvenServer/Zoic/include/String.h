/*! @file
	@ingroup utils
	@brief 定义字符串模板
*/

#ifndef __ZOIC_STRING_H
#define __ZOIC_STRING_H

#include "Types.h"

namespace Zoic
{
	/*! @brief 字符串模板
		@ingroup utils
		@param LEN 最大字符长度
	*/
	template <const WORD LEN>
	class String
	{
	public:
		static const WORD LENGTH_LIMIT = LEN;
		static WORD getLengthLimit();
		//@{
		//!构造函数
		String();
		String(
			const char* str	//!< 字符串
			);
		//@}

		//@{
		/*! @brief 赋值操作符
			@return 对象自身引用
			@param str 字符串
		*/
		String & operator = (const char* str);
		String & operator = (const String & str);
		
		//@}

		//!字符串转换操作符
		operator const char* () const;

		//@{
		/*! @brief 相等比较操作符
			@param str 字符串
		*/
		bool operator == (const char* str) const;
		bool operator == (const String& str) const;
		//@}

		//@{
		/*! @brief 不等比较操作符
			@param str 字符串
		*/
		bool operator != (const char* str) const;		
		bool operator != (const String& str) const;
		
		bool operator < (const String & str) const;
		//@}
	private:
		//!设置字符串
		void setString(
			const char* str	//!< 字符串
			);

		char m_str[LEN+1];	//!< 字符串缓存
	};
	
	template <const WORD LEN>
	String<LEN>::String()
	{
		m_str[0]=0;
	}

	template <const WORD LEN>
	String<LEN>::String(const char* str)
	{			
		setString(str);
	}

	template <const WORD LEN>
	inline WORD String<LEN>::getLengthLimit()
	{
		return LEN;
	}
	
	template <const WORD LEN>
	inline String<LEN> & String<LEN>::operator = (const char* str)
	{
		setString(str);
		return *this;
	}
	
	template <const WORD LEN>
	inline String<LEN> & String<LEN>::operator = (const String & str)
	{
		setString(str.m_str);
		return *this;
	}

	template <const WORD LEN>
	inline String<LEN>::operator const char* () const
	{
		return m_str;
	}
	
	template <const WORD LEN>
	inline bool String<LEN>::operator == (const char* str) const
	{
		return strcmp(m_str,str)==0;
	}
	
	template <const WORD LEN>
	inline bool String<LEN>::operator != (const char* str) const
	{
		return !(*this == str);
	}
	
	template <const WORD LEN>
	inline bool String<LEN>::operator == (const String& str) const
	{
		return (*this) == str.m_str;
	}
	
	template <const WORD LEN>
	inline bool String<LEN>::operator != (const String& str) const
	{
		return !(*this == str);
	}
	
	template <const WORD LEN>
	inline bool String<LEN>::operator < (const String & str) const
	{
		return strcmp(m_str,str.m_str)<0;
	}

	template <const WORD LEN>
	void String<LEN>::setString(const char* str)
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
	Message & operator << ( Message &msg,const String<LEN> & str)
	{
		MessageImpl::setString(msg,(const char *)str,LEN+1);
		return msg;
	}
	
	template <const WORD LEN>
	Message & operator >> ( Message &msg, String<LEN> &str)
	{
		MessageImpl::getString(msg,const_cast<char *>((const char *)str),LEN+1);
		return msg;
	}
}

#endif
