/*! @file
	@ingroup utils
	@brief ��������������ģ��
*/

#ifndef __CONSTCAST_H
#define __CONSTCAST_H

/*! @brief �����������ģ��
	@ingroup utils
	@param val ����


	ʹ�ø�ģ����Բ�����ʽ��д�������ͣ����������Զ�ƥ�䡣
*/
template <class T>
T & const_cast_(const T &val)
{
	return const_cast<T &>(val);
}

#endif
