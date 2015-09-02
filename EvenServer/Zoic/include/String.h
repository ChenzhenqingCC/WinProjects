/*! @file
	@ingroup utils
	@brief �����ַ���ģ��
*/

#ifndef __ZOIC_STRING_H
#define __ZOIC_STRING_H

#include "Types.h"

namespace Zoic
{
	/*! @brief �ַ���ģ��
		@ingroup utils
		@param LEN ����ַ�����
	*/
	template <const WORD LEN>
	class String
	{
	public:
		static const WORD LENGTH_LIMIT = LEN;
		static WORD getLengthLimit();
		//@{
		//!���캯��
		String();
		String(
			const char* str	//!< �ַ���
			);
		//@}

		//@{
		/*! @brief ��ֵ������
			@return ������������
			@param str �ַ���
		*/
		String & operator = (const char* str);
		String & operator = (const String & str);
		
		//@}

		//!�ַ���ת��������
		operator const char* () const;

		//@{
		/*! @brief ��ȱȽϲ�����
			@param str �ַ���
		*/
		bool operator == (const char* str) const;
		bool operator == (const String& str) const;
		//@}

		//@{
		/*! @brief ���ȱȽϲ�����
			@param str �ַ���
		*/
		bool operator != (const char* str) const;		
		bool operator != (const String& str) const;
		
		bool operator < (const String & str) const;
		//@}
	private:
		//!�����ַ���
		void setString(
			const char* str	//!< �ַ���
			);

		char m_str[LEN+1];	//!< �ַ�������
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
