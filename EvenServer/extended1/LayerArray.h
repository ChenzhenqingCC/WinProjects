#ifndef __LAYERARRAY_H
#define __LAYERARRAY_H

#include <vector>
#include <memory.h>
#include "NumericExtend.h"

//多层数组(时间换空间方案)
//以下标的形式访问每个元素,但是开销预计为普通数组下标访问的2*(LAYER+1)倍
//每层子Layer下标域没用被用到的部分分配较少的内存，比完全分配内存的数组方式大大节省内存
//适用于解决以下问题:1.减少低有效值率的广范围下标数组的内存开销;2.提高连续区间的数值键字典的访问速度
//注意:RemoveData函数并不释放元素的内存,只是将其返回,所以每个元素的内存需自行管理
template<class T,unsigned char LAYER = 0,unsigned char UNIT_SIZE_EXPONENT_BASE_2 = 4> class LayerArray;


template<unsigned char UNIT_SIZE_EXPONENT_BASE_2> class _Empty_Layer_Array
{
	template <class,unsigned char,unsigned char> friend class LayerArray;
private:
	static void * const* _get_initialed_buffer();
	static const size_t UNIT_SIZE = 1 << UNIT_SIZE_EXPONENT_BASE_2;
	static void * const* const s_empty_buffer;
};

template<class T,unsigned char UNIT_SIZE_EXPONENT_BASE_2>
class LayerArray<T,0,UNIT_SIZE_EXPONENT_BASE_2>
{
	typedef _Empty_Layer_Array<UNIT_SIZE_EXPONENT_BASE_2> UNIT_DEFINE;
public:
	typedef T UNIT_TYPE;
	static const size_t SIZE_LIMIT = UNIT_DEFINE::UNIT_COUNT;
	typedef MaskBoundChecker<(unsigned int)SIZE_LIMIT> BOUND;

	LayerArray();
	~LayerArray();
	T* operator[](unsigned int index) const;
	bool addData(unsigned int index,T* data);
	T* removeData(unsigned int index);
	bool empty() const;
private:
	UNIT_TYPE* m_items[UNIT_DEFINE::UNIT_SIZE];
};

template<class T,unsigned char LAYER,unsigned char UNIT_SIZE_EXPONENT_BASE_2>
class LayerArray
{
	typedef _Empty_Layer_Array<UNIT_SIZE_EXPONENT_BASE_2> UNIT_DEFINE;
public:
	typedef LayerArray<T,LAYER-1,UNIT_SIZE_EXPONENT_BASE_2> UNIT_TYPE;
	static const size_t SIZE_LIMIT = UNIT_TYPE::SIZE_LIMIT * UNIT_DEFINE::UNIT_COUNT;
	typedef MaskBoundChecker<(unsigned int)SIZE_LIMIT> BOUND;

	LayerArray();
	~LayerArray();
	T* operator[](unsigned int index) const;
	bool addData(unsigned int index,T* data);
	T* removeData(unsigned int index);
	bool empty() const;
private:
	UNIT_TYPE* m_items[UNIT_DEFINE::UNIT_SIZE];
};

template<unsigned char UNIT_SIZE_EXPONENT_BASE_2>
void * const* _Empty_Layer_Array<UNIT_SIZE_EXPONENT_BASE_2>::_get_initialed_buffer()
{
	static void* staticBuffer[size_t(2)<<UNIT_SIZE_EXPONENT_BASE_2];
	memset(staticBuffer,0,sizeof(staticBuffer) );
	return staticBuffer;
}
template<unsigned char UNIT_SIZE_EXPONENT_BASE_2>
void * const* const _Empty_Layer_Array<UNIT_SIZE_EXPONENT_BASE_2>::s_empty_buffer = _Empty_Layer_Array<UNIT_SIZE_EXPONENT_BASE_2>::_get_initialed_buffer();

template<class T,unsigned char UNIT_SIZE_EXPONENT_BASE_2>
inline LayerArray<T,0,UNIT_SIZE_EXPONENT_BASE_2>::LayerArray()
{
	memset( m_items,0,sizeof(m_items) );
}

template<class T,unsigned char LAYER,unsigned char UNIT_SIZE_EXPONENT_BASE_2>
inline LayerArray<T,LAYER,UNIT_SIZE_EXPONENT_BASE_2>::LayerArray()
{
	memset( m_items,0,sizeof(m_items) );
}

template<class T,unsigned char UNIT_SIZE_EXPONENT_BASE_2>
inline LayerArray<T,0,UNIT_SIZE_EXPONENT_BASE_2>::~LayerArray()
{
}

template<class T,unsigned char LAYER,unsigned char UNIT_SIZE_EXPONENT_BASE_2>
inline LayerArray<T,LAYER,UNIT_SIZE_EXPONENT_BASE_2>::~LayerArray()
{
	for(size_t i=0;i<UNIT_COUNT; ++i )
	{
		if(m_items[i])
		{
			delete m_items[i];
		}
	}
}

template<class T,unsigned char UNIT_SIZE_EXPONENT_BASE_2>
inline T* LayerArray<T,0,UNIT_SIZE_EXPONENT_BASE_2>::operator [](unsigned int index) const
{
	return m_items[index];
}

template<class T,unsigned char LAYER,unsigned char UNIT_SIZE_EXPONENT_BASE_2>
inline T* LayerArray<T,LAYER,UNIT_SIZE_EXPONENT_BASE_2>::operator [](unsigned int index) const
{
	const unsigned int unit_index = index >> UNIT_TYPE::BOUND::VALID_BIT_COUNT;
	if(!m_items[unit_index])
	{
		return NULL;
	}
	const unsigned int next_layer_index = index & UNIT_TYPE::BOUND::FULL_MASK_VALUE;
	return (*m_items[unit_index])[next_layer_index];
}

template<class T,unsigned char UNIT_SIZE_EXPONENT_BASE_2>
inline bool LayerArray<T,0,UNIT_SIZE_EXPONENT_BASE_2>::addData(unsigned int index,T* data)
{
	if( !data )
	{
		return false;
	}
	if( m_items[index] )
	{
		return false;
	}
	m_items[index] = data;
	return true;
}

template<class T,unsigned char LAYER,unsigned char UNIT_SIZE_EXPONENT_BASE_2>
bool LayerArray<T,LAYER,UNIT_SIZE_EXPONENT_BASE_2>::addData(unsigned int index,T* data)
{
	if( !data )
	{
		return false;
	}
	const unsigned int unit_index = index >> UNIT_TYPE::BOUND::VALID_BIT_COUNT;
	if( !m_items[unit_index] )
	{
		m_items[unit_index] = new UNIT_TYPE;
	}
	const unsigned int next_layer_index = index & UNIT_TYPE::BOUND::FULL_MASK_VALUE;
	return m_items[unit_index]->addData(next_layer_index,data);
}

template<class T,unsigned char UNIT_SIZE_EXPONENT_BASE_2>
inline T* LayerArray<T,0,UNIT_SIZE_EXPONENT_BASE_2>::removeData(unsigned int index)
{
	T* data = m_items[index];
	m_items[index] = NULL;
	return data;
}

template<class T,unsigned char LAYER,unsigned char UNIT_SIZE_EXPONENT_BASE_2>
T* LayerArray<T,LAYER,UNIT_SIZE_EXPONENT_BASE_2>::removeData(unsigned int index)
{
	const unsigned int unit_index = index >> UNIT_TYPE::BOUND::VALID_BIT_COUNT;
	if(!m_items[unit_index])
	{
		return NULL;
	}
	const unsigned int next_layer_index = index & UNIT_TYPE::BOUND::FULL_MASK_VALUE;
	T* data = m_items[unit_index]->removeData(next_layer_index);
	if(!data)
	{
		return NULL;
	}
	if( m_items[unit_index]->empty() )
	{
		delete m_items[unit_index];
		m_items[unit_index]=NULL;
	}
	return data;
}

template<class T,unsigned char UNIT_SIZE_EXPONENT_BASE_2>
inline bool LayerArray<T,0,UNIT_SIZE_EXPONENT_BASE_2>::empty() const
{
	return 0 == memcmp(m_items,UNIT_DEFINE::s_empty_buffer,sizeof(m_items) );
}

template<class T,unsigned char LAYER,unsigned char UNIT_SIZE_EXPONENT_BASE_2>
inline bool LayerArray<T,LAYER,UNIT_SIZE_EXPONENT_BASE_2>::empty() const
{
	return 0 == memcmp(m_items,UNIT_DEFINE::s_empty_buffer,sizeof(m_items) );
}

#endif
