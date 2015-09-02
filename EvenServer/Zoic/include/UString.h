/*! @file
	@ingroup utils
	@brief ¶¨Òåutf8×Ö·û´®Ä£°å
*/

#ifndef __ZOIC_USTRING_H
#define __ZOIC_USTRING_H

#include "String.h"
#include "Log.h"

const WORD UTF8_STRING_LENGTH	=	3; 

namespace Zoic
{
	/*! @brief ×Ö·û´®Ä£°å
		@ingroup utils
		@param LEN ×î´ó×Ö·û³¤¶È
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
			const char* str	//!< ×Ö·û´®
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
