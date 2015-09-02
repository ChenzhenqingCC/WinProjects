#ifndef __TAXISCOMMON_H
#define __TAXISCOMMON_H

namespace Taxis
{
	template<typename T> inline bool lessCompareZeroAsMinus(T a,T b)
	{
		return static_cast<T>(a - 1) < static_cast<T>(b-1);
	}
}

#endif
