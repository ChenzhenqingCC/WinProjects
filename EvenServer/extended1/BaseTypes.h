#ifndef __BASETYPES_H
#define __BASETYPES_H

#include <assert.h>

#ifndef ASSERT
	#ifdef _DEBUG
		#define ASSERT(x) assert(x)
	#else
		#define ASSERT(x)
	#endif
#endif

#endif
