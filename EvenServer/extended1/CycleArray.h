#ifndef __CYCLEARRAY_H
#define __CYCLEARRAY_H

#include "BaseTypes.h"

template <class T> class CycleArrary;
template <class T,size_t LEN> class FixCycleArrary;

template <class T> class CycleArrary
{
public:
	CycleArrary(size_t maxSize);
	~CycleArrary();
	const T & operator[](size_t index) const;
	size_t size() const;
	size_t getSizeLimit();
	bool pushHead(const T& value);
	void pushHeadCycle(const T& value);
	bool pushTail(const T& value);
	void pushTailCycle(const T& value);
	bool popTail();
	bool popHead();
	bool adjustMaxSize(size_t maxSize);
	void clear();
private:
	T * m_array;
	size_t m_headIndex;
	size_t m_size;
	size_t m_maxIndex;
};

template<class T>
CycleArrary<T>::CycleArrary(size_t maxSize)
	:m_maxIndex(maxSize)
	,m_array(new T[maxSize])
	,m_headIndex(0)
	,m_size(0)
{
}

template<class T>
CycleArrary<T>::~CycleArrary()
{
	delete[] m_array;
}

template<class T>
bool CycleArrary<T>::adjustMaxSize(size_t maxSize)
{
	if( getSizeLimit() == maxSize )
	{
		return true;
	}
	if( size() > maxSize )
	{
		ASSERT(0);
		return false;
	}
	T* newArray = new T[maxSize];
	const size_t headCount = m_maxIndex - m_headIndex;
	const size_t headCopyCount = ( headCount > m_size ) ? m_size : headCount;
	size_t copiedCount = 0;
	size_t copyIndex;
	for( copyIndex=m_headIndex ;copiedCount<headCopyCount; ++copiedCount,++copyIndex )
	{
		newArray[copiedCount] = m_array[copyIndex];
	}
	for( copyIndex = 0; copiedCount<m_size; ++copiedCount,++copyIndex )
	{
		newArray[copiedCount] = m_array[copyIndex];
	}
	delete [] m_array;
	m_array = newArray;
	m_headIndex = 0;
	m_maxIndex = maxSize;
	return true;
}

template<class T>
inline size_t CycleArrary<T>::getSizeLimit()
{
	return m_maxIndex;
}

template<class T>
inline size_t CycleArrary<T>::size() const
{
	return m_size;
}

template<class T>
inline const T & CycleArrary<T>::operator[](size_t index) const
{
	ASSERT(m_size > index);
	return m_array[ ( m_headIndex + index ) % m_maxIndex ];
}

template<class T>
bool CycleArrary<T>::pushHead(const T& value)
{
	if(m_size >= m_maxIndex)
	{
		ASSERT(0);
		return false;
	}
	m_headIndex = (m_headIndex + m_maxIndex - 1) % m_maxIndex;
	m_array[m_headIndex] = value;
	++m_size;
	return true;
}

template<class T>
void CycleArrary<T>::pushHeadCycle(const T& value)
{
	m_headIndex = (m_headIndex + m_maxIndex - 1) % m_maxIndex;
	m_array[m_headIndex] = value;
	if(m_size < m_maxIndex)
	{
		++m_size;
	}
}

template<class T>
bool CycleArrary<T>::pushTail(const T& value)
{
	if(m_size >= m_maxIndex)
	{
		ASSERT(0);
		return false;
	}
	m_array[ ( m_headIndex + m_size ) % m_maxIndex ] = value;
	++m_size;
	return true;
}

template<class T>
void CycleArrary<T>::pushTailCycle(const T& value)
{
	m_array[ ( m_headIndex + m_size ) % m_maxIndex ] = value;
	if(m_size < m_maxIndex)
	{
		++m_size;
	}
	else
	{
		m_headIndex = (m_headIndex + 1) % m_maxIndex;
	}
}

template<class T>
bool CycleArrary<T>::popHead()
{
	if(!m_size)
	{
		ASSERT(0);
		return false;
	}
	m_headIndex = (m_headIndex + 1) % m_maxIndex;
	--m_size;
	return true;
}

template<class T>
bool CycleArrary<T>::popTail()
{
	if(!m_size)
	{
		ASSERT(0);
		return false;
	}
	--m_size;
	return true;
}

template<class T>
void CycleArrary<T>::clear()
{
	m_size = 0;
}

template <class T,size_t LEN> class FixCycleArrary
{
public:
	FixCycleArrary();
	const T & operator[](size_t index) const;
	T & at(size_t index);
	size_t size() const;
	size_t getSizeLimit();
	bool pushHead(const T& value);
	void pushHeadCycle(const T& value);
	bool pushTail(const T& value);
	void pushTailCycle(const T& value);
	bool popTail();
	bool popHead();
	void clear();
private:
	T m_array[LEN];
	size_t m_headIndex;
	size_t m_size;
};

template<class T,size_t LEN>
FixCycleArrary<T,LEN>::FixCycleArrary()
	:m_headIndex(0)
	,m_size(0)
{
}

template<class T,size_t LEN>
inline size_t FixCycleArrary<T,LEN>::getSizeLimit()
{
	return LEN;
}

template<class T,size_t LEN>
inline size_t FixCycleArrary<T,LEN>::size() const
{
	return m_size;
}

template<class T,size_t LEN>
inline const T & FixCycleArrary<T,LEN>::operator[](size_t index) const
{
	ASSERT(m_size > index);
	return m_array[ ( m_headIndex + index ) % LEN ];
}

template<class T,size_t LEN>
inline T & FixCycleArrary<T,LEN>::at(size_t index)
{
	ASSERT(m_size > index);
	return m_array[ ( m_headIndex + index ) % LEN ];
}

template<class T,size_t LEN>
bool FixCycleArrary<T,LEN>::pushHead(const T& value)
{
	if(m_size >= LEN)
	{
		ASSERT(0);
		return false;
	}
	m_headIndex = (m_headIndex + LEN - 1) % LEN;
	m_array[m_headIndex] = value;
	++m_size;
	return true;
}

template<class T,size_t LEN>
void FixCycleArrary<T,LEN>::pushHeadCycle(const T& value)
{
	m_headIndex = (m_headIndex + LEN - 1) % LEN;
	m_array[m_headIndex] = value;
	if(m_size < LEN)
	{
		++m_size;
	}
}

template<class T,size_t LEN>
bool FixCycleArrary<T,LEN>::pushTail(const T& value)
{
	if(m_size >= LEN)
	{
		ASSERT(0);
		return false;
	}
	m_array[ ( m_headIndex + m_size ) % LEN ] = value;
	++m_size;
	return true;
}

template<class T,size_t LEN>
void FixCycleArrary<T,LEN>::pushTailCycle(const T& value)
{
	m_array[ ( m_headIndex + m_size ) % LEN ] = value;
	if(m_size < LEN)
	{
		++m_size;
	}
	else
	{
		m_headIndex = (m_headIndex + 1) % LEN;
	}
}

template<class T,size_t LEN>
bool FixCycleArrary<T,LEN>::popHead()
{
	if(!m_size)
	{
		ASSERT(0);
		return false;
	}
	m_headIndex = (m_headIndex + 1) % LEN;
	--m_size;
	return true;
}

template<class T,size_t LEN>
bool FixCycleArrary<T,LEN>::popTail()
{
	if(!m_size)
	{
		ASSERT(0);
		return false;
	}
	--m_size;
	return true;
}

template<class T, size_t LEN>
void FixCycleArrary<T,LEN>::clear()
{
	m_size = 0;
}

#endif

