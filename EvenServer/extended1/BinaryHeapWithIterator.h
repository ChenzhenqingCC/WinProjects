#ifndef __BINARYHEAPWITHITERATOR_H
#define __BINARYHEAPWITHITERATOR_H

#include "BinaryHeap.h"
#include <list>
#include <stack>

//含迭代器的二叉堆
//通过迭代器解决了儿叉堆每次只能删除最优元素,而不能随机删除的缺陷

namespace Taxis
{
	template<class T,class _PrTaxis = std::less<T> >
	class BinaryHeapForIteratorCore
	{
	public:
		class ValueType;
		struct _Core
		{
			ValueType const * pValue;
		};
		class _Iterator
		{
			friend class BinaryHeapForIteratorCore;
		public:
			_Iterator():position(NULL){}
			inline const T& operator *()
			{
				return position->pValue->data;
			}
		protected:
			_Core* position;
		};
		struct ValueType
		{
			T data;
			_Iterator selfIterator;
		};
		bool operator()(const ValueType& a,const ValueType& b) const
		{
			return const_cast<_PrTaxis&>(m_taxis)(a.data,b.data);
		}
		static void updateIterator(const ValueType& data,size_t currentIndex)
		{
			data.selfIterator.position->pValue = &data;
		}
	private:
		_PrTaxis m_taxis;
	};

	template<class T,class _PrTaxis = std::less<T>,bool _enableRandom = false >
	class BinaryHeapWithIterator
		:private BinaryHeap<typename BinaryHeapForIteratorCore<T,_PrTaxis>::ValueType,BinaryHeapForIteratorCore<T,_PrTaxis>,_enableRandom>
	{
	public:
		typedef BinaryHeapForIteratorCore<T,_PrTaxis> _core_type;
		class iterator
			:public _core_type::_Iterator
		{
			friend class BinaryHeapWithIterator;
		};
		typedef T value_type;
		BinaryHeapWithIterator(size_t minSize = 0x20,size_t maxSize = 0x1000000);
		~BinaryHeapWithIterator();
		void erase(const iterator& it);
		std::pair<iterator,bool> insert(T const&);
		T const& front() const;
		void pop_front();
		void clear();
		size_t size() const;
		void define_stock(size_t minSize,size_t maxSize);
	protected:
		bool _index_erase(size_t index);
		std::list<typename _core_type::_Core> m_totalIteratorStorage;
		std::stack<typename _core_type::_Core*> m_unused;
	private:
		typedef BinaryHeap<typename BinaryHeapForIteratorCore<T,_PrTaxis>::ValueType,BinaryHeapForIteratorCore<T,_PrTaxis>,_enableRandom> BASE_TYPE;
	};
}

namespace Taxis
{
	template<class T,class _PrTaxis,bool _enableRandom>
	BinaryHeapWithIterator<T,_PrTaxis,_enableRandom>::BinaryHeapWithIterator(size_t minSize,size_t maxSize)
		:BinaryHeap<typename BinaryHeapForIteratorCore<T,_PrTaxis>::ValueType,BinaryHeapForIteratorCore<T,_PrTaxis>,_enableRandom>(minSize,maxSize)
	{
		BASE_TYPE::_set_hook( _core_type::updateIterator );
	}

	template<class T,class _PrTaxis,bool _enableRandom>
	BinaryHeapWithIterator<T,_PrTaxis,_enableRandom>::~BinaryHeapWithIterator()
	{
	}

	template<class T,class _PrTaxis,bool _enableRandom>
	bool BinaryHeapWithIterator<T,_PrTaxis,_enableRandom>::_index_erase(size_t index)
	{
		if( index >= BASE_TYPE::size() )
		{
			return false;
		}
		typename _core_type::ValueType const& valueData = BASE_TYPE::_get_item(index);
		iterator itRelease;
		static_cast<typename _core_type::_Iterator&>(itRelease) = valueData.selfIterator;
		if( ! BASE_TYPE::_erase_byIndex(index) )
		{
			ASSERT(0);
			return false;
		}
		itRelease.position->pValue = NULL;
		m_unused.push(itRelease.position);
		return true;
	}

	template<class T,class _PrTaxis,bool _enableRandom>
	void BinaryHeapWithIterator<T,_PrTaxis,_enableRandom>::erase(const typename BinaryHeapWithIterator<T,_PrTaxis,_enableRandom>::iterator& it)
	{
		typename _core_type::ValueType const* pValueType = (*it.position).pValue;
		size_t index = pValueType - &BASE_TYPE::front();
		if( pValueType != &BASE_TYPE::_get_item(index) )
		{
			ASSERT(0);
			return;
		}
		if( !_index_erase(index) )
		{
			ASSERT(0);
		}
	}

	template<class T,class _PrTaxis,bool _enableRandom>
	inline std::pair<typename BinaryHeapWithIterator<T,_PrTaxis,_enableRandom>::iterator,bool> BinaryHeapWithIterator<T,_PrTaxis,_enableRandom>::insert(const T& data)
	{
		std::pair<typename BinaryHeapWithIterator<T,_PrTaxis,_enableRandom>::iterator,bool> retValue;
		if( m_unused.empty() )
		{
			typename _core_type::_Core emptyCore;
			emptyCore.pValue = NULL;
			m_totalIteratorStorage.push_back(emptyCore);
			retValue.first.position = &m_totalIteratorStorage.back();
		}
		else
		{
			retValue.first.position = m_unused.top();
			m_unused.pop();
		}
		typename _core_type::ValueType insertValue; 
		insertValue.data = data;
		insertValue.selfIterator = retValue.first;

		retValue.second = BASE_TYPE::insert(insertValue);
		if( ! retValue.second )
		{
			m_unused.push(retValue.first.position);
			retValue.first.position = NULL;
			return retValue;
		}
		return retValue;
	}

	template<class T,class _PrTaxis,bool _enableRandom>
	inline const T& BinaryHeapWithIterator<T,_PrTaxis,_enableRandom>::front() const
	{
		return BASE_TYPE::front().data;
	}

	template<class T,class _PrTaxis,bool _enableRandom>
	inline void BinaryHeapWithIterator<T,_PrTaxis,_enableRandom>::pop_front()
	{
		if( !_index_erase(0) )
		{
			ASSERT(0);
		}
	}

	template<class T,class _PrTaxis,bool _enableRandom>
	inline void BinaryHeapWithIterator<T,_PrTaxis,_enableRandom>::clear()
	{
		BASE_TYPE::clear();
		std::stack<typename _core_type::ValueType const**> emptyStack;
		m_unused = emptyStack;
		m_totalIteratorStorage.clear();
	}

	template<class T,class _PrTaxis,bool _enableRandom>
	inline size_t BinaryHeapWithIterator<T,_PrTaxis,_enableRandom>::size() const
	{
		return BASE_TYPE::size();
	}

	template<class T,class _PrTaxis,bool _enableRandom>
	inline void BinaryHeapWithIterator<T,_PrTaxis,_enableRandom>::define_stock(size_t minSize,size_t maxSize)
	{
		BASE_TYPE::define_stock(minSize,maxSize);
	}

}

#endif
