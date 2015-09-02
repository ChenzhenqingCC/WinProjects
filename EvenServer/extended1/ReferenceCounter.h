#ifndef __REFERENCECOUNTER_H
#define __REFERENCECOUNTER_H

#include "BaseTypes.h"

class ReferenceCounter
{
public:
	ReferenceCounter();
	void addRef();
	void releaseRef();
	unsigned int getRefCount() const;
protected:
	virtual void onOwnReference(){};
	virtual void onNoReference() = 0;
private:
	unsigned int m_reference;
};

inline ReferenceCounter::ReferenceCounter()
	:m_reference(0)
{
}

inline void ReferenceCounter::addRef()
{
	if( m_reference++ )
	{
		return;
	}
	onOwnReference();
}

inline void ReferenceCounter::releaseRef()
{
	ASSERT(m_reference);
	if( --m_reference )
	{
		return;
	}
	onNoReference();
}

inline unsigned int ReferenceCounter::getRefCount() const
{
	return m_reference;
}

#endif
