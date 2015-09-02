#ifndef __MASKARRAY_H
#define __MASKARRAY_H

#include "MaskCounter.h"
#include "MaskValue.h"
#include <string.h>

#pragma pack(1)

typedef unsigned char MaskArrayIndex;

template<class T,typename MASK_TYPE = unsigned char>
class MaskArray
{
public:
	MaskArray();
	~MaskArray();
	static const MaskArrayIndex MASK_INDEX_BOUNT = sizeof(MASK_TYPE) * BYTE_BIT_COUNT;
	static const MASK_TYPE FULL_MASK = ~0;
	struct DataField
	{
		MaskArrayIndex count;
		T* const * pBuf;
	};
	MASK_TYPE getMask() const;
	bool haveData(MaskArrayIndex index) const;
	T * getData(MaskArrayIndex index) const;
	bool setData(MaskArrayIndex index, T* data);
	bool eraseData(MaskArrayIndex index);
	DataField getDataInRange(MaskArrayIndex beginIndex,MaskArrayIndex endIndex) const;
	DataField getDataField() const;
	void clear();
	static void swap(MaskArray& a,MaskArray& b);
	MaskArray& operator=(MaskArray& o);
private:
	T** m_data;
	MASK_TYPE	m_mask;
};

#pragma pack()

template<class T,typename MASK_TYPE>
inline MaskArray<T,MASK_TYPE>::MaskArray()
	:m_data(0)
	,m_mask(0)
{
}

template<class T,typename MASK_TYPE>
inline MaskArray<T,MASK_TYPE>::~MaskArray()
{
	clear();
}

template<class T,typename MASK_TYPE>
inline void MaskArray<T,MASK_TYPE>::clear()
{
	if(m_data)
	{
		delete [] m_data;
	}
	m_data = 0;
	m_mask = 0;
}

template<class T,typename MASK_TYPE>
inline MASK_TYPE MaskArray<T,MASK_TYPE>::getMask() const
{
	return m_mask;
}

template<class T,typename MASK_TYPE>
inline bool MaskArray<T,MASK_TYPE>::haveData(MaskArrayIndex index) const
{
	return ( m_mask & MASK_VALUE(index) ) ? true : false;
}

template<class T,typename MASK_TYPE>
inline T * MaskArray<T,MASK_TYPE>::getData(MaskArrayIndex index) const
{
	const MASK_TYPE maskValue =  MASK_VALUE(index);
	const MASK_TYPE maskLower =  maskValue - 1;
	if( index >= MASK_INDEX_BOUNT || !( m_mask & maskValue ) )
	{
		return 0;
	}
	return m_data[ MaskCounter::getInstance()->getBitCount<MASK_TYPE>( m_mask & maskLower ) ];
}

template<class T,typename MASK_TYPE>
bool MaskArray<T,MASK_TYPE>::setData(MaskArrayIndex index,T* data)
{
	if( index >= MASK_INDEX_BOUNT )
	{
		return false;
	}
	const MASK_TYPE maskValue =  MASK_VALUE(index);
	const MASK_TYPE maskLower =  maskValue - 1;
	if( m_mask & maskValue )
	{
		m_data[ MaskCounter::getInstance()->getBitCount<MASK_TYPE>( m_mask & maskLower ) ] = data;
		return true;
	}
	const unsigned char frontCount = MaskCounter::getInstance()->getBitCount<MASK_TYPE>( m_mask & maskLower );
	const unsigned char backCount = MaskCounter::getInstance()->getBitCount<MASK_TYPE>( m_mask ) - frontCount;
	T **	tempData = new T*[frontCount+backCount+1];
	if(frontCount)
	{
		memcpy(tempData,m_data,frontCount*sizeof(T*) );
	}
	if(backCount)
	{
		memcpy(&tempData[frontCount+1],&m_data[frontCount],backCount*sizeof(T*) );
	}
	tempData[frontCount] = data;
	if(m_data)
	{
		delete [] m_data;
	}
	m_data = tempData;
	m_mask |= maskValue;
	return true;
}

template<class T,typename MASK_TYPE>
bool MaskArray<T,MASK_TYPE>::eraseData(MaskArrayIndex index)
{
	if( index >= MASK_INDEX_BOUNT )
	{
		return false;
	}
	const MASK_TYPE maskValue =  MASK_VALUE(index);
	const MASK_TYPE maskLower =  maskValue - 1;
	if( !( m_mask & maskValue ) )
	{
		return false;
	}
	unsigned char frontCount = MaskCounter::getInstance()->getBitCount<MASK_TYPE>( m_mask & maskLower );
	unsigned char backCount = MaskCounter::getInstance()->getBitCount<MASK_TYPE>( m_mask ) - frontCount - 1;
	T **	tempData = NULL;
	if( frontCount || backCount)
	{
		tempData = new T*[frontCount+backCount];
	}
	if(frontCount)
	{
		memcpy(tempData,m_data,frontCount*sizeof(T*) );
	}
	if(backCount)
	{
		memcpy(&tempData[frontCount],&m_data[frontCount+1],backCount*sizeof(T*));
	}
	delete [] m_data;
	m_data = tempData;
	m_mask &= ~(maskValue);
	return true;
}

template<class T,typename MASK_TYPE>
typename MaskArray<T,MASK_TYPE>::DataField MaskArray<T,MASK_TYPE>::getDataInRange(MaskArrayIndex beginIndex,MaskArrayIndex endIndex) const
{
	DataField output = {0,0};
	if(beginIndex >= MASK_INDEX_BOUNT || endIndex <= beginIndex)
	{
		return output;
	}
	unsigned char frontCount = 0;
	if(beginIndex)
	{
		const MASK_TYPE startMaskVLower =  MASK_VALUE(beginIndex) - 1;
		frontCount = MaskCounter::getInstance()->getBitCount<MASK_TYPE>( m_mask & startMaskVLower );
	}
	{
		const MASK_TYPE endMaskLower =  endIndex >= MASK_INDEX_BOUNT ? FULL_MASK : MASK_VALUE( endIndex ) - 1;
		output.count = MaskCounter::getInstance()->getBitCount<MASK_TYPE>( m_mask & endMaskLower ) - frontCount;
	}
	if(output.count)
	{
		output.pBuf = m_data + frontCount;
	}
	return output;
}

template<class T,typename MASK_TYPE>
typename MaskArray<T,MASK_TYPE>::DataField MaskArray<T,MASK_TYPE>::getDataField() const
{
	if(!m_data)
	{
		DataField output = {0,0};
		return output;
	}
	DataField output = {MaskCounter::getInstance()->getBitCount<MASK_TYPE>( m_mask ),m_data };
	return output;
}

template<class T,typename MASK_TYPE>
void MaskArray<T,MASK_TYPE>::swap(MaskArray<T,MASK_TYPE>& a,MaskArray<T,MASK_TYPE>& b)
{
	std::swap(a.m_data,b.m_data);
	std::swap(a.m_mask,b.m_mask);
}

/*Disabled
template<class T,typename MASK_TYPE>
MaskArray<T,MASK_TYPE>& MaskArray<T,MASK_TYPE>::operator=(MaskArray<T,MASK_TYPE>& o)
{

}
*/

#endif
