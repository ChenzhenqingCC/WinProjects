/*! @file
	@ingroup utils
	@brief ����for_each�㷨
*/

#ifndef __ZOIC_FOR_EACH_H
#define __ZOIC_FOR_EACH_H

namespace Zoic
{
	/*!
		@brief for_each�㷨
		@ingroup utils

		���㷨��std::for_each���,�����������㷨����
	*/
	template<class _InIt,class _Fn1>
	inline void for_each(_InIt _First, _InIt _Last, _Fn1 & _Func)
	{
		for (; _First != _Last; ++_First)
		{
			_Func(*_First);
		}
	}
}

#endif
