#ifndef __MASKCOUNTER_H
#define __MASKCOUNTER_H

#include <string>
#include "BaseTypes.h"

const unsigned char BYTE_BIT_COUNT = 8;
typedef unsigned short COUNTER_MASK_TYPE;
const unsigned char COUNTER_BYTE_COUNT = sizeof(COUNTER_MASK_TYPE);
const COUNTER_MASK_TYPE FULL_COUNTER_MASK = ~0;
const unsigned char COUNTER_MASK_BIT_COUNT = COUNTER_BYTE_COUNT*BYTE_BIT_COUNT;

class MaskCounter
{
public:
	MaskCounter();
	template <typename T> unsigned char getBitCount(T mask) const;
#ifdef _DEBUG
	template <typename T> unsigned char getMaskWORDCount() const;
#endif
	static MaskCounter* getInstance();
private:
	unsigned char m_bitCount[static_cast<size_t>( FULL_COUNTER_MASK ) + 1 ];
	static MaskCounter m_instance;
};

#ifdef _DEBUG
template <typename T>
inline unsigned char MaskCounter::getMaskWORDCount() const
{
	return ( sizeof(T)/COUNTER_BYTE_COUNT + ( ( sizeof(T) % COUNTER_BYTE_COUNT ) ? 1:0 ) );
}
#endif

template <typename T>
inline unsigned char MaskCounter::getBitCount(T mask) const
{
#ifdef _DEBUG
	int tempMask = mask;
	if( tempMask < 0)
	{
		ASSERT(0);
	}
#endif
	const unsigned char maskCount = ( sizeof(T)/COUNTER_BYTE_COUNT + ( ( sizeof(T) % COUNTER_BYTE_COUNT ) ? 1:0 ) );
	if(maskCount <= 1)
	{
		return m_bitCount[mask];
	}
	
	unsigned char bitCount = 0;
	T curMask = mask;
	for(unsigned char i = 0; i < maskCount;++i)
	{
		bitCount += m_bitCount[curMask & FULL_COUNTER_MASK];
		curMask >>= COUNTER_MASK_BIT_COUNT;
	}
	return bitCount;
}

inline MaskCounter* MaskCounter::getInstance()
{
	return &m_instance;
}

#endif
