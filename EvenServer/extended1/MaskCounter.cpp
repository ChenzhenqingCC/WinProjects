#include <string.h>
#include "MaskCounter.h"

MaskCounter MaskCounter::m_instance;

MaskCounter::MaskCounter()
{
	memset(m_bitCount,0,sizeof(m_bitCount) );
	for(size_t i = 0; i< static_cast<size_t>( FULL_COUNTER_MASK ) + 1; i++)
	{
		unsigned char j=0;
		unsigned long mask = 0x01;
		while(mask <= FULL_COUNTER_MASK)
		{
			if(i & mask )
			{
				++m_bitCount[i];
			}
			++j;
			mask = 0x01 << j;
		}
	}
}
