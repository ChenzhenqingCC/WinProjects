/*! @file
	@ingroup utils
	@brief ������Ϊ�ַ���ʹ�õ�����ģ��
*/

#ifndef __ZOIC_STRCHARARRAY_H
#define __ZOIC_STRCHARARRAY_H

#include "Types.h"

namespace Zoic
{
	/*! @brief �ַ���ģ��
		@ingroup utils
		@param LEN ����ַ�����
	*/
	template <const WORD LEN>
	class StrCharArray
	{
	public:
		static WORD getLengthLimit();

		//@{
		/*! @brief ��ֵ������
		@return ������������
		@param str �ַ���
		*/
		//StrCharArray & operator = (const char* str);
		//StrCharArray & operator = (const StrCharArray & str);
		//@}

		//!�ַ���ת��������
		operator const char* () const;

		//@{
		/*! @brief ��ȱȽϲ�����
		@param str �ַ���
		*/
		bool operator == (const char* str) const;
		bool operator == (const StrCharArray& str) const;
		//@}

		//@{
		/*! @brief ���ȱȽϲ�����
		@param str �ַ���
		*/
		bool operator != (const char* str) const;		
		bool operator != (const StrCharArray& str) const;
		//@}
		//!�����ַ���
		void setString(
			const char* str	//!< �ַ���
			);
	private:

		char m_str[LEN+1];	//!< �ַ�������
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
