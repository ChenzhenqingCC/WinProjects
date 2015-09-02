/*! @file
	@ingroup utils
	@brief ÉùÃ÷²ð·Ö×Ö·û´®º¯Êý
*/

#ifndef __ZOIC_SPLITSTRING_H
#define __ZOIC_SPLITSTRING_H

#include <vector>
#include <string>

namespace Zoic
{
	/*! @brief ²ð·Ö×Ö·û´®
		@ingroup utils
	*/
	void splitString(
		const char *src,					//!< Ô­×Ö·û´®
		std::vector<std::string> &dst,		//!< Éú³ÉµÄ×Ö·û´®ÐòÁÐ
		const char *separator="\t \r\n"		//!< ·Ö¸ô·û
		);
}

#endif
