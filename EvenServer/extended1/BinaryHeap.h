#ifndef __BINARYHEAP_H
#define __BINARYHEAP_H

#include <set>
#include <stdlib.h>
#include "BaseTypes.h"

//����������-�����
//����:
//1.����,ȡ����Ϊlog2��������,��std::multisetҪ��10������
//2.��̬�ڴ��ȡ,�������ڴ���Ƭ,��֤�������е��ȶ���
//ȱ��:
//1.ÿ��ֻ�ܻ�������е�����,���޷���̬�������˳��
//2.�޷�����Ψһ������,�޷�ģ��std::set,ͬʱҲû�в��ҹ���
//3.ÿ��ֻ��ɾ����ǰ����Ԫ��,���������ɾ��

namespace Taxis
{
	template<class T,class _PrTaxis = std::less<T>,bool _enableRandom = false >
	class BinaryHeap
	{
	public:
		typedef T value_type;
		BinaryHeap(size_t minSize = 0x20,size_t maxSize = 0x1000000);
		~BinaryHeap();
		bool insert(const T&);
		const T& front() const;
		void pop_front();
		void clear();
		size_t size() const;
		void define_stock(size_t minSize,size_t maxSize);
	protected:
		typedef void(*INDEX_FUNC)(T const & data,size_t index);
		void _set_hook(INDEX_FUNC _indexHook);
		bool _erase_byIndex(size_t eraseIndex);
		const T& _get_item(size_t index) const;
	private:
		int _adjust_forEmptyIndex(size_t startEmptyIndex,const T& fillingData,INDEX_FUNC _indexHook);//���ؽ�������
		void setStock(size_t stockSize,INDEX_FUNC _indexHook);
		T* m_data;
		size_t m_size;
		size_t m_stock;
		size_t m_maxStock;
		_PrTaxis m_taxisRule;
		INDEX_FUNC m_indexHook;
	};
}

namespace Taxis
{
	template<class T,class _PrTaxis,bool _enableRandom>
	BinaryHeap<T,_PrTaxis,_enableRandom>::BinaryHeap(size_t minSize,size_t maxSize)
		:m_size(0)
		,m_maxStock( maxSize ? maxSize : 1 )
		,m_indexHook(NULL)
	{
		m_stock = minSize ? minSize : 1;
		if(m_stock > m_maxStock)
		{
			m_maxStock = m_stock;
		}
		m_data = new T[m_stock];
	}

	template<class T,class _PrTaxis,bool _enableRandom>
	BinaryHeap<T,_PrTaxis,_enableRandom>::~BinaryHeap()
	{
		delete [] m_data;
	}

	template<class T,class _PrTaxis,bool _enableRandom>
	void BinaryHeap<T,_PrTaxis,_enableRandom>::setStock(size_t stockSize,typename BinaryHeap<T,_PrTaxis,_enableRandom>::INDEX_FUNC _indexHook)
	{
		if( m_size > stockSize )
		{
			return;
		}
		T* oldData = m_data;
		m_data = new T[stockSize];
		if(_indexHook)
		{
			for(size_t curPos=0;curPos<m_size;++curPos)
			{
				m_data[curPos] = oldData[curPos];
				_indexHook(m_data[curPos],curPos);
			}
		}
		else
		{
			for(size_t curPos=0;curPos<m_size;++curPos)
			{
				m_data[curPos] = oldData[curPos];
			}
		}
		delete [] oldData;
		m_stock = stockSize;
	}

	template<class T,class _PrTaxis,bool _enableRandom>
	int BinaryHeap<T,_PrTaxis,_enableRandom>::_adjust_forEmptyIndex(size_t startEmptyIndex,const T& fillingData,typename BinaryHeap<T,_PrTaxis,_enableRandom>::INDEX_FUNC _indexHook)
	{
		if( startEmptyIndex >= m_size )
		{
			ASSERT(0);
			return 0;
		}
		int swapCount = 0;
		size_t curPos = startEmptyIndex;
		{//���ϵ���
			while(curPos > 0)
			{
				size_t fatherPos = ( curPos + 1 ) / 2 - 1;
				if( m_taxisRule(fillingData,m_data[fatherPos])  )
				{
					//��������������ɴ��봦��
				}
				else if( _enableRandom && rand() % 2 && !m_taxisRule(m_data[fatherPos],fillingData) )
				{
					//��������������ɴ��봦��
				}
				else
				{
					break;//��ǰλ�þ���fillingData��λ��
				}
				m_data[curPos] = m_data[fatherPos];
				if(_indexHook)_indexHook(m_data[curPos],curPos);
				curPos = fatherPos;
				++swapCount;
			}
		}
		if(curPos == startEmptyIndex)
		{//���µ���
			while(curPos < m_size)
			{
				size_t sonSecond = ( curPos + 1 ) * 2;
				size_t sonFirst = sonSecond - 1;
				if(sonFirst >= m_size )
				{
					break;
				}
				size_t selectSon = sonFirst;
				if(sonSecond < m_size )
				{
					if( m_taxisRule(m_data[sonSecond],m_data[sonFirst]) )
					{
						selectSon = sonSecond;
					}
					else if( m_taxisRule(m_data[sonFirst],m_data[sonSecond]) )
					{
					}
					else if( _enableRandom && rand() % 2 )
					{
						selectSon = sonSecond;
					}
				}
				if( m_taxisRule(fillingData,m_data[selectSon])  )
				{
					break;//��ǰλ�þ���fillingData��λ��
				}
				else if( _enableRandom && rand() % 2 && !m_taxisRule(m_data[selectSon],fillingData) )
				{
					break;//��ǰλ�þ���fillingData��λ��
				}
				m_data[curPos] = m_data[selectSon];
				if(_indexHook)_indexHook(m_data[curPos],curPos);
				curPos = selectSon;
				++swapCount;
			}
		}
		m_data[curPos] = fillingData;
		if(_indexHook)_indexHook(m_data[curPos],curPos);
		return swapCount;
	}

	template<class T,class _PrTaxis,bool _enableRandom>
	inline bool BinaryHeap<T,_PrTaxis,_enableRandom>::insert(const T& data)
	{
		if(m_size>=m_stock)
		{
			if(m_stock >= m_maxStock)
			{
				ASSERT(0);
				return false;
			}
			size_t nextStock = m_stock * 2;
			if(nextStock > m_maxStock)
			{
				nextStock = m_maxStock;
			}
			setStock(nextStock,m_indexHook);
		}
		const size_t startPos = m_size++;
		_adjust_forEmptyIndex(startPos,data,m_indexHook);
		return true;
	}

	template<class T,class _PrTaxis,bool _enableRandom>
	void BinaryHeap<T,_PrTaxis,_enableRandom>::_set_hook(INDEX_FUNC _indexHook)
	{
		m_indexHook = _indexHook;
	}

	template<class T,class _PrTaxis,bool _enableRandom>
	inline const T& BinaryHeap<T,_PrTaxis,_enableRandom>::_get_item(size_t index) const
	{
		ASSERT(m_size > index);
		return m_data[index];
	}

	template<class T,class _PrTaxis,bool _enableRandom>
	inline const T& BinaryHeap<T,_PrTaxis,_enableRandom>::front() const
	{
		return _get_item(0);
	}

	template<class T,class _PrTaxis,bool _enableRandom>
	inline void BinaryHeap<T,_PrTaxis,_enableRandom>::pop_front()
	{
		if( !_erase_byIndex(0) )
		{
			ASSERT(0);
		}
	}

	template<class T,class _PrTaxis,bool _enableRandom>
	bool BinaryHeap<T,_PrTaxis,_enableRandom>::_erase_byIndex(size_t eraseIndex)
	{
		if( m_size <= eraseIndex )
		{
			return false;
		}
		T& tailData = m_data[--m_size];
		if(m_size == eraseIndex)
		{
			return true;
		}
		_adjust_forEmptyIndex(eraseIndex,tailData,m_indexHook);
		return true;
	}


	template<class T,class _PrTaxis,bool _enableRandom>
	void BinaryHeap<T,_PrTaxis,_enableRandom>::clear()
	{
		m_size = 0;
	}

	template<class T,class _PrTaxis,bool _enableRandom>
	inline size_t BinaryHeap<T,_PrTaxis,_enableRandom>::size() const
	{
		return m_size;
	}

	template<class T,class _PrTaxis,bool _enableRandom>
	void BinaryHeap<T,_PrTaxis,_enableRandom>::define_stock(size_t minSize,size_t maxSize)
	{
		if( !minSize )
		{
			minSize = 1;
		}
		m_maxStock = maxSize ? maxSize : 1;
		if(minSize > m_maxStock)
		{
			m_maxStock = minSize;
		}
		if(m_stock < minSize)
		{
			setStock(minSize,m_indexHook);
		}
	}
}

#endif
