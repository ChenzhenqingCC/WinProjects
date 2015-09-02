/*! @file
	@ingroup utils
	@brief ����utf8�ַ���ģ��
*/

#ifndef __ZOIC_USTRING_H
#define __ZOIC_USTRING_H

#include "String.h"
#include "Log.h"

const WORD UTF8_STRING_LENGTH	=	3; 

namespace Zoic
{
	/*! @brief �ַ���ģ��
		@ingroup utils
		@param LEN ����ַ�����
	*/

	template <const WORD LEN>
	class UString
		: public String<LEN*UTF8_STRING_LENGTH>
	{
	public:
		using String<LEN*UTF8_STRING_LENGTH>::operator=;
		using String<LEN*UTF8_STRING_LENGTH>::operator==;
		using String<LEN*UTF8_STRING_LENGTH>::operator!=;
		
		UString();
		UString(
			const char* str	//!< �ַ���
			);
		
	};
	
	template <const WORD LEN>
	UString<LEN>::UString()
		:String<LEN*UTF8_STRING_LENGTH>()
	{
	}

	template <const WORD LEN>
	UString<LEN>::UString(const char* str)
		:String<LEN*UTF8_STRING_LENGTH>(str)
	{			
	}
	
}

#endif
