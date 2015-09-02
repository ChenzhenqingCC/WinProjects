#pragma warning (disable : 4786)

#ifndef __TRIGGER_HEADER_H__
#define __TRIGGER_HEADER_H__

#include <time.h>
#include <stdio.h>
#include <string.h>


#ifdef _DEBUG
#define TRIGGER_ASSERT(x)	\
	if(!(x)) \
	{\
	*(int *)0=0;\
}
#else
#define TRIGGER_ASSERT(x);
#endif

namespace Trigger
{
	typedef unsigned char		BYTE;
	typedef unsigned short		WORD;
	typedef	unsigned long		DWORD;

	typedef	unsigned int		COUNT;

	typedef WORD				EventCategoryID;
}

#endif

