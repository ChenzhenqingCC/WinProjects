/*! @file
	@ingroup utils
	@brief 定义for_each算法
*/

#ifndef __ZOIC_FOR_EACH_H
#define __ZOIC_FOR_EACH_H

namespace Zoic
{
	/*!
		@brief for_each算法
		@ingroup utils

		该算法与std::for_each相比,不返回最后的算法对象
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
