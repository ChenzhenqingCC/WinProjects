/*! @file
	@ingroup utils
	@brief �滻CONFIG_DECLARE,ʵ�������ļ����Զ���ȡ
*/
#ifdef CONFIG_DEFINE
#undef CONFIG_DEFINE
#endif

/*! @brief ����������
	@param TYPE ����������
	@param NAME ����������
*/

#define CONFIG_DEFINE_FUNC(TYPE,NAME,FUNCNAME) \
	private: \
		TYPE m_##NAME; \
	public: \
		typedef TYPE NAME##_TYPE; \
		const NAME##_TYPE & FUNCNAME() \
		{ \
			ASSERT( isLoaded() ) \
			return m_##NAME; \
		}

#define CONFIG_DEFINE(TYPE,NAME) CONFIG_DEFINE_FUNC(TYPE,NAME,get##NAME)

/*! @file
	@ingroup utils
	@brief �滻CONFIG_DECLARE,ʵ�������ļ����Զ���ȡ,���û���ҵ���������ʹ��Ĭ��ֵ
*/
#ifdef CONFIG_DEFINE_EX
#undef CONFIG_DEFINE_EX
#endif

/*! @brief ����������
	@param TYPE ����������
	@param NAME ����������
	@param VALUE ������Ĭ��ֵ
*/
#define CONFIG_DEFINE_EX(TYPE,NAME,VALUE) CONFIG_DEFINE(TYPE,NAME)

#ifndef __CONFIGDECLARE_H
#define __CONFIGDECLARE_H

#include "DateTime.h"

#endif
