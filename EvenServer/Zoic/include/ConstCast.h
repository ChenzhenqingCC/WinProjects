/*! @file
	@ingroup utils
	@brief 定义解除常量修饰模板
*/

#ifndef __CONSTCAST_H
#define __CONSTCAST_H

/*! @brief 解除常量修饰模板
	@ingroup utils
	@param val 常量


	使用该模板可以不用显式的写常量类型，编译器会自动匹配。
*/
template <class T>
T & const_cast_(const T &val)
{
	return const_cast<T &>(val);
}

#endif
