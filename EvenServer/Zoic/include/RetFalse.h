/*! @file
	@ingroup buildin
	@brief 定义RET_FALSE宏
*/

#ifndef __ZOIC_RETFALSE_H
#define __ZOIC_RETFALSE_H
#ifdef _DEBUG
#include "Log.h"
#endif

/*! @def RET_FALSE(STR)
	@brief 返回FALSE
	@ingroup buildin
	@param STR 错误信息


	仅在DEBUG模式下记录错误日志
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
