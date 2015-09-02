#include "include/Clock.h"
#ifdef HAVE_MINGW32
#include <windows.h>
using namespace Zoic;

#ifdef ZOIC_NO_SERVICE
	Clock Zoic::theClock;
#endif

typedef long long LONGLONG;
#define FACTOR (0x19db1ded53e8000LL)
 
static inline LONGLONG
systime ()
{
	LARGE_INTEGER x;
	FILETIME ft;
	GetSystemTimeAsFileTime (&ft);
	x.HighPart = ft.dwHighDateTime;
	x.LowPart = ft.dwLowDateTime;
	x.QuadPart -= FACTOR;		/* Add conversion factor for UNIX vs. Windows base time */
	x.QuadPart /= 10;		/* Convert to microseconds */
	return x.QuadPart;
}

int Clock::gettimeofday (struct timeval *tv, struct timezone *tz)
{
	static bool tzflag;
	LONGLONG now = systime();
	
	tv->tv_sec = static_cast<long>(now / 1000000);
	tv->tv_usec = static_cast<long>(now % 1000000);
	return 0;
}

#endif
