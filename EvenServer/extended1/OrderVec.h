#ifndef __ORDERVEC_H
#define __ORDERVEC_H

#include <vector>
#include "BinaryHeap.h"

namespace Taxis
{
	template<class T,class _PrTaxis = std::less<T>,class _Alloc = std::allocator<T> >
	class OrderVec
		:private _PrTaxis
	{
	public:
		typedef T value_type;
		OrderVec(bool isUnique=true);
		OrderVec(const std::vector<T>& initData,bool isUnique=true);
		size_t reset(const std::vector<T>& initData);
		bool hasSameIndex(const OrderVec& o) const;
		bool hasDifferentIndex(const OrderVec& o) const;
		const std::vector<T,_Alloc>& getData() const;
	protected:
		void clear(bool isUnique);
	private:
		class _CmpOrder
		{
		public:
			inline _CmpOrder(){}
			inline bool operator()(T const * a, T const * b) const{return const_cast<_PrTaxis&>(m_order)(*a,*b);}
		private:
			_PrTaxis m_order;
		};
		class CmpLessPtr
		{
		public:
		};
		bool m_isUnique;
		std::vector<T,_Alloc> m_data;
	};

	template<class T,class _PrTaxis,class _Alloc>
	OrderVec<T,_PrTaxis,_Alloc>::OrderVec(bool isUnique)
		:m_isUnique(isUnique)
	{
	}

	template<class T,class _PrTaxis,class _Alloc>
	OrderVec<T,_PrTaxis,_Alloc>::OrderVec(const std::vector<T>& initData,bool isUnique)
		:m_isUnique(isUnique)
	{
		reset(initData);
	}

	template<class T,class _PrTaxis,class _Alloc>
	void OrderVec<T,_PrTaxis,_Alloc>::clear(bool isUnique)
	{
		m_data.clear();
		m_isUnique = isUnique;
	}

	template<class T,class _PrTaxis,class _Alloc>
	size_t OrderVec<T,_PrTaxis,_Alloc>::reset(const std::vector<T>& initData)
	{
		Taxis::BinaryHeap<T const *,_CmpOrder> initHeap(initData.size(),initData.size());
		for(size_t i = 0; i < initData.size(); ++ i)
		{
			initHeap.insert(&initData[i]);
		}
		m_data.clear();
		for(;initHeap.size();initHeap.pop_front())
		{
			if( m_isUnique &&m_data.size())
			{
				if( !(*this)(m_data[m_data.size()-1],*(initHeap.front() ) ) )
				{
					continue;
				}
			}
			m_data.push_back(*initHeap.front());
		}
		return m_data.size();
	}

	template<class T,class _PrTaxis,class _Alloc>
	bool OrderVec<T,_PrTaxis,_Alloc>::hasSameIndex(const OrderVec<T,_PrTaxis,_Alloc>& o) const
	{
		size_t i = 0;
		size_t j = 0;
		while( i<m_data.size() && j< o.m_data.size() )
		{
			const T& selfValue = m_data[i];
			const T& objectValue = o.m_data[j];
			if( (*this)(selfValue,objectValue ) )
			{
				++i;
				continue;
			}
			if( (*this)(objectValue,selfValue) )
			{
				++j;
				continue;
			}
			return true;
		}
		return false;
	}

	template<class T,class _PrTaxis,class _Alloc>
	bool OrderVec<T,_PrTaxis,_Alloc>::hasDifferentIndex(const OrderVec<T,_PrTaxis,_Alloc>& o) const
	{
		if( m_isUnique && o.m_isUnique && ( m_data.size() != o.m_data.size() ) )
		{
			return true;
		}
		size_t i = 0;
		size_t j = 0;
		for(;i<m_data.size() && j< o.m_data.size();++i,++j)
		{
			const T& selfValue = m_data[i];
			const T& objectValue = o.m_data[j];
			if( (*this)(selfValue,objectValue ) )
			{
				return true;
			}
			if( (*this)(objectValue,selfValue) )
			{
				return true;
			}
			if( !m_isUnique )
			{
				for(; i+1 < m_data.size();++i )
				{
					if( (*this)(selfValue,m_data[i+1]) )
					{
						break;
					}
				}
			}
			if( !o.m_isUnique )
			{
				for(; j+1 < o.m_data.size();++j )
				{
					if( (*this)(objectValue,o.m_data[j+1]) )
					{
						break;
					}
				}
			}
		}
		ASSERT( i==m_data.size() && j==o.m_data.size() );
		return false;
	}

	template<class T,class _PrTaxis,class _Alloc>
	inline const std::vector<T,_Alloc>& OrderVec<T,_PrTaxis,_Alloc>::getData() const
	{
		return m_data;
	}

	template<class T,class _PrTaxis = std::less<T>,class _Alloc = std::allocator<T> >
	class UniqueOrderVec
		:public OrderVec<T,_PrTaxis,_Alloc>
	{
	public:
		UniqueOrderVec();
		UniqueOrderVec(const std::vector<T,_Alloc>& initData);
		void clear();
	};

	template<class T,class _PrTaxis,class _Alloc>
	UniqueOrderVec<T,_PrTaxis,_Alloc>::UniqueOrderVec()
		:OrderVec<T,_PrTaxis,_Alloc>(true)
	{
	}

	template<class T,class _PrTaxis,class _Alloc>
	UniqueOrderVec<T,_PrTaxis,_Alloc>::UniqueOrderVec(const std::vector<T,_Alloc>& initData)
		:OrderVec<T,_PrTaxis,_Alloc>(initData,true)
	{
	}

	template<class T,class _PrTaxis,class _Alloc>
	void UniqueOrderVec<T,_PrTaxis,_Alloc>::clear()
	{
		OrderVec<T,_PrTaxis,_Alloc>::clear(true);
	}

	template<class T,class _PrTaxis = std::less<T>,class _Alloc = std::allocator<T> >
	class MultiOrderVec
		:public OrderVec<T,_PrTaxis,_Alloc>
	{
	public:
		MultiOrderVec();
		MultiOrderVec(const std::vector<T,_Alloc>& initData);
		void clear();
	};

	template<class T,class _PrTaxis,class _Alloc>
	MultiOrderVec<T,_PrTaxis,_Alloc>::MultiOrderVec()
		:OrderVec<T,_PrTaxis,_Alloc>(false)
	{
	}

	template<class T,class _PrTaxis,class _Alloc>
	MultiOrderVec<T,_PrTaxis,_Alloc>::MultiOrderVec(const std::vector<T,_Alloc>& initData)
		:OrderVec<T,_PrTaxis,_Alloc>(initData,false)
	{
	}

	template<class T,class _PrTaxis,class _Alloc>
	void MultiOrderVec<T,_PrTaxis,_Alloc>::clear()
	{
		OrderVec<T,_PrTaxis,_Alloc>::clear(false);
	}

};

#endif
