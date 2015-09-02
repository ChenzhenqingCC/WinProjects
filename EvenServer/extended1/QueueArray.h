#ifndef __QUEUEARRAY_H
#define __QUEUEARRAY_H

#include "BaseTypes.h"
#include "CycleArray.h"

template <class T, size_t LEN>
class FixQueueArrary
	:private FixCycleArrary<T,LEN>
{
private:
	typedef FixCycleArrary<T,LEN> BASIC_TYPE;
public:
	const T & operator[](size_t index) const;
	size_t size() const;
	static size_t getSizeLimit();
	bool pushTail(const T& value);
	bool popHead();
	void clear();
};

template<class T, size_t LEN>
inline size_t FixQueueArrary<T,LEN>::getSizeLimit()
{
	return LEN;
}

template<class T, size_t LEN>
inline size_t FixQueueArrary<T,LEN>::size() const
{
	return BASIC_TYPE::size();
}

template<class T, size_t LEN>
inline const T & FixQueueArrary<T,LEN>::operator[](size_t index) const
{
	return static_cast<BASIC_TYPE const&>(*this)[index];
}

template<class T, size_t LEN>
inline bool FixQueueArrary<T,LEN>::pushTail(const T& value)
{
	return BASIC_TYPE::pushTail(value);
}

template<class T, size_t LEN>
inline bool FixQueueArrary<T,LEN>::popHead()
{
	return BASIC_TYPE::popHead();
}

template<class T, size_t LEN>
inline void FixQueueArrary<T,LEN>::clear()
{
	BASIC_TYPE::clear();
}

#endif

