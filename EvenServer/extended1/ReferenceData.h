#ifndef __REFERENCEDATA_H
#define __REFERENCEDATA_H

#include "ReferenceCounter.h"

//比std::sharedptr增加了限制，不能通过自由指针赋值和获得指针

template <class T>
class ReferencePointer
{
public:
	ReferencePointer();
	ReferencePointer(const ReferencePointer& org);
	ReferencePointer & operator=(const ReferencePointer & org);
	~ReferencePointer();
	operator const ReferencePointer*() const;
	//! -> 操作符
	const T * operator ->() const;
	static ReferencePointer create(const T& orgData);
protected:
	class Data
		:public ReferenceCounter
	{
	public:
		Data(const T& data);
		virtual void onNoReference();
		T m_data;
	};
	Data* getData() const;
	ReferencePointer(Data& org);
private:
	Data* m_pData;
};

template <class T>
inline ReferencePointer<T>::Data::Data(const T& data)
	:m_data(data)
{
}

template <class T>
inline void ReferencePointer<T>::Data::onNoReference()
{
	delete this;
}

template <class T>
inline ReferencePointer<T> ReferencePointer<T>::create(const T& orgData)
{
	Data* pData = new Data(orgData);
	return *pData;
}

template <class T>
inline ReferencePointer<T>::ReferencePointer()
	:m_pData(NULL)
{
}

template <class T>
inline ReferencePointer<T>::ReferencePointer(const ReferencePointer<T>& org)
	:m_pData(org.m_pData)
{
	if(m_pData)
	{
		m_pData->addRef();
	}
}

template <class T>
inline ReferencePointer<T>::ReferencePointer(typename ReferencePointer<T>::Data& orgData)
	:m_pData(&orgData)
{
	m_pData->addRef();
}

template <class T>
inline ReferencePointer<T> & ReferencePointer<T>::operator=(const ReferencePointer<T> & org)
{
	Data* pOld = m_pData;
	m_pData = org.m_pData;
	if(m_pData)
	{
		m_pData->addRef();
	}
	if(pOld)
	{
		pOld->releaseRef();
	}
	return *this;
}

template <class T>
inline ReferencePointer<T>::~ReferencePointer()
{
	if(m_pData)
	{
		m_pData->releaseRef();
	}
}

template <class T>
inline typename ReferencePointer<T>::Data* ReferencePointer<T>::getData() const
{
	return m_pData;
}

template <class T>
inline ReferencePointer<T>::operator const ReferencePointer<T>*() const
{
	return m_pData ? this : NULL;
}

template<class T>
inline const T * ReferencePointer<T>::operator ->() const
{
	return &m_pData->m_data;
}

#endif
