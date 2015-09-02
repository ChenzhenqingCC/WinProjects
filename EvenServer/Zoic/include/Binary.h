/*! @file
	@ingroup utils
	@brief �������������ģ��
*/

#ifndef __ZOIC_BINARY_H
#define __ZOIC_BINARY_H
#include <string>
#include "VarArray.h"

namespace Zoic
{
	/*!
		@brief ����ģ��
		@ingroup utils
		@param LEN ������󳤶�
	*/
	template <WORD LEN,class SIZE_TYPE = WORD>
	class Binary:public VarArray<char,LEN,SIZE_TYPE>
	{
	};
};

#endif
