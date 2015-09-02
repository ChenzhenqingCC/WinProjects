#ifndef __DATAIMAGE_H
#define __DATAIMAGE_H

template<class T>
class DataImage
{
private:	
	typedef void (*DUMMY_FUNC)();

public:
	DataImage(const T* item);

	//!转型以使含有DataImage<T>的逻辑表达式合法
	operator DUMMY_FUNC() const
	{
		return m_item?dummyFunc:NULL;
	}
	
	//!是否有效
	bool isValid() const;
	//! -> 操作符
	const T * operator ->() const;
	
	//!返回自身拷贝
	T duplicate() const;

private:	
	static void dummyFunc();

	//!禁止new
	static void * operator new(size_t size);

	//!禁止拷贝操作
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
