#ifndef __DATAIMAGE_H
#define __DATAIMAGE_H

template<class T>
class DataImage
{
private:	
	typedef void (*DUMMY_FUNC)();

public:
	DataImage(const T* item);

	//!ת����ʹ����DataImage<T>���߼����ʽ�Ϸ�
	operator DUMMY_FUNC() const
	{
		return m_item?dummyFunc:NULL;
	}
	
	//!�Ƿ���Ч
	bool isValid() const;
	//! -> ������
	const T * operator ->() const;
	
	//!����������
	T duplicate() const;

private:	
	static void dummyFunc();

	//!��ֹnew
	static void * operator new(size_t size);

	//!��ֹ��������
	DataImage & operator=(const DataImage &);

	const T* m_item;
};

template<class T>
inline DataImage<T>::DataImage(const T* item)
:m_item(item)
{
}

template<class T>
inline const T * DataImage<T>::operator ->() const
{
	return m_item;
}

//template<class T>
//inline DataImage<T>::operator DataImage<T>::DUMMY_FUNC() const
//{	
//	return m_item?dummyFunc:NULL;
//}

template<class T>
bool DataImage<T>::isValid() const
{
	return (m_item)?true:false;
}

template<class T>
inline T DataImage<T>::duplicate() const
{
	return *m_item;
}

template<class T>
inline void DataImage<T>::dummyFunc()
{
}

#endif
