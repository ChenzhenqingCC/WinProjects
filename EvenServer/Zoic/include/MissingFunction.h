/*! @file
	@ingroup flat_define
	@brief ������ͬƽ̨δʵ�ֺ���
*/

#ifndef __MISSING_FUNCTION_H
#define __MISSING_FUNCTION_H
#include "config_flat.h"

#ifndef HAVE_STRLCPY
extern int strlcpy(char *dst, const char *src, int size);
#endif

#endif
