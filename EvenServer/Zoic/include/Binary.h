/*! @file
	@ingroup utils
	@brief 定义二进制数组模板
*/

#ifndef __ZOIC_BINARY_H
#define __ZOIC_BINARY_H
#include <string>
#include "VarArray.h"

namespace Zoic
{
	/*!
		@brief 数组模板
		@ingroup utils
		@param LEN 数组最大长度
	*/
	template <WORD LEN,class SIZE_TYPE = WORD>
	class Binary:public VarArray<char,LEN,SIZE_TYPE>
	{
	};
};

#endif
