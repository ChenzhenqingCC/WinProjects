#include "include/MissingFunction.h"

#ifndef HAVE_STRLCPY
int strlcpy(char *dst, const char *src, int size)
{
	--size;
	for(int i=0;i<size;++i)
	{
		*dst=*src++;
		if(*dst++ == '\0')
		{
			return i;
		}
	}
	*dst = '\0';
	return size;
}

#endif
