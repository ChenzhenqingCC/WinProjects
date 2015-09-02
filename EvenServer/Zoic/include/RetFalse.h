/*! @file
	@ingroup buildin
	@brief ����RET_FALSE��
*/

#ifndef __ZOIC_RETFALSE_H
#define __ZOIC_RETFALSE_H
#ifdef _DEBUG
#include "Log.h"
#endif

/*! @def RET_FALSE(STR)
	@brief ����FALSE
	@ingroup buildin
	@param STR ������Ϣ


	����DEBUGģʽ�¼�¼������־
*/

#ifdef _DEBUG
#define RET_FALSE(STR) \
	log("RET_FALSE:%s:%d\n\t%s",__FILE__,__LINE__,STR); \
	return false;
#else
#define RET_FALSE(STR)	\
	return false;
#endif

#endif
