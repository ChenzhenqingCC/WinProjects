/*! @file
	@ingroup flat_define
	@brief ѡ��ͬƽ̨�������ļ�
*/

#ifndef __ZOIC_CONFIG_FLAT_H
#define __ZOIC_CONFIG_FLAT_H


#if defined(_MSC_VER)//VS����
#if _MSC_VER >= 1400	//VC8
#pragma warning(disable:4996)
#endif
#include "config-win.h"
#else	//UNIX����
#include <config.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#endif


#endif	//__ZOIC_CONFIG_FLAT_H
