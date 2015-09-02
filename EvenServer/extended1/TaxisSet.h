#ifndef __TAXISSET_H
#define __TAXISSET_H

#include <set>
#include <map>
#include <list>
#include <stdlib.h>

namespace Taxis
{
	template<class T,class _PrTaxis = std::less<T>,class _PrUnique = std::less<T>,class _Alloc = std::allocator<T> >
	class MultiSet
	{
	public:
		typedef typename std::multiset<T,_PrTaxis,_Alloc>::iterator iterator;
		typedef typename std::multiset<T,_PrTaxis,_Alloc>::const_iterator const_iterator;
		typedef T value_type;
		MultiSet();
		MultiSet(const MultiSet& o);
		MultiSet& operator=(const MultiSet& o);
		iterator insert(T const &);
		size_t erase(T const &);
		iterator erase(iterator it);
		const_iterator find(T const &) const;
		iterator find(T const &);
		iterator lower_bound(T const &);
		iterator upper_bound(T const &);
		const_iterator begin() const;
		iterator begin();
		const_iterator end() const;
		iterator end();
		void clear();
		size_t count(T const &);
		size_t size() const;
		bool empty() const;
		std::pair<iterator, iterator> equal_range(T const &);
		iterator random_insert(T const &);
	protected:
		class _CmpUnique
		{
		public:
			inline _CmpUnique(){}
			inline _CmpUnique(const _PrUnique& initTaxis):m_unique(initTaxis){}
			inline bool operator()(T const * a, T const * b) const{return const_cast<_PrUnique&>(m_unique)(*a,*b);}
		private:
			_PrUnique m_unique;
		};
	private:
		typedef std::map<T const *,iterator,_CmpUnique> UNIQUE_MAP;
		typedef std::multiset<T,_PrTaxis,_Alloc> MULTI_SET;
		UNIQUE_MAP setMap;
		MULTI_SET setData;
	};

}

namespace Taxis
{
	template<class T,class _PrTaxis,class _PrUnique,class _Alloc>
	inline MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::MultiSet()
	{
	}

	template<class T,class _PrTaxis,class _PrUnique,class _Alloc>
	MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::MultiSet(const MultiSet<T,_PrTaxis,_PrUnique,_Alloc>& o)
	{
		(*this)=o;
	}

	template<class T,class _PrTaxis,class _PrUnique,class _Alloc>
	MultiSet<T,_PrTaxis,_PrUnique,_Alloc>& MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::operator=(const MultiSet<T,_PrTaxis,_PrUnique,_Alloc>& o)
	{
		setData = o.setData;
		iterator it = setData.begin();
		while(it!=setData.end())
		{
			if( ! setMap.insert( std::make_pair(&(*it),it) ).second )
			{
				//error!
				setData.erase(it++);
				continue;
			}
			++it;
		}
		return *this;
	}

	template<class T,class _PrTaxis,class _PrUnique,class _Alloc>
	typename MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::iterator MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::insert(T const& data)
	{
		if( setMap.find(&data)!= setMap.end() )
		{
			return setData.end();
		}
		iterator it = setData.insert(data);
		setMap[&(*it)] = it;
		return it;
	}

	template<class T,class _PrTaxis,class _PrUnique,class _Alloc>
	size_t MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::erase(const T& data)
	{
		typename UNIQUE_MAP::iterator itMap = setMap.find(&data);
		if( itMap != setMap.end() )
		{
			iterator it = itMap->second;
			setMap.erase(itMap);
			setData.erase(it);
			return 1;
		}
		return 0;
	}

	template<class T,class _PrTaxis,class _PrUnique,class _Alloc>
	typename MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::iterator MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::erase(iterator it)
	{
		setMap.erase(&(*it));
		setData.erase(it++);
		return it;
	}

	template<class T,class _PrTaxis,class _PrUnique,class _Alloc>
		typename MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::const_iterator MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::find(T const & data) const
	{
		typename UNIQUE_MAP::const_iterator itMap = setMap.find(&data);
		if( itMap != setMap.end() )
		{
			return itMap->second;
		}
		return setData.end();
	}

	template<class T,class _PrTaxis,class _PrUnique,class _Alloc>
	typename MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::iterator MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::find(T const & data)
	{
		typename UNIQUE_MAP::iterator itMap = setMap.find(&data);
		if( itMap != setMap.end() )
		{
			return itMap->second;
		}
		return setData.end();
	}

	template<class T,class _PrTaxis,class _PrUnique,class _Alloc>
	inline typename MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::iterator MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::lower_bound(T const & data)
	{
		return setData.lower_bound(data);
	}

	template<class T,class _PrTaxis,class _PrUnique,class _Alloc>
	inline typename MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::iterator MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::upper_bound(T const & data)
	{
		return setData.upper_bound(data);
	}

	template<class T,class _PrTaxis,class _PrUnique,class _Alloc>
		inline typename MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::const_iterator MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::begin() const
	{
		return setData.begin();
	}

	template<class T,class _PrTaxis,class _PrUnique,class _Alloc>
	inline typename MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::iterator MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::begin()
	{
		return setData.begin();
	}

	template<class T,class _PrTaxis,class _PrUnique,class _Alloc>
		inline typename MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::const_iterator MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::end() const
	{
		return setData.end();
	}

	template<class T,class _PrTaxis,class _PrUnique,class _Alloc>
	inline typename MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::iterator MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::end()
	{
		return setData.end();
	}

	template<class T,class _PrTaxis,class _PrUnique,class _Alloc>
	inline void MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::clear()
	{
		setMap.clear();
		setData.clear();
	}

	template<class T,class _PrTaxis,class _PrUnique,class _Alloc>
	inline size_t MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::count(T const & data)
	{
		return setData.count(data);
	}

	template<class T,class _PrTaxis,class _PrUnique,class _Alloc>
	inline size_t MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::size() const
	{
		return setData.size();
	}

	template<class T,class _PrTaxis,class _PrUnique,class _Alloc>
	inline bool MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::empty() const
	{
		return size() == 0;
	}

	template<class T,class _PrTaxis,class _PrUnique,class _Alloc>
	inline typename std::pair<typename MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::iterator, typename MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::iterator> MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::equal_range(T const & data)
	{
		return setData.equal_range(data);
	}

	template<class T,class _PrTaxis,class _PrUnique,class _Alloc>
	typename MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::iterator MultiSet<T,_PrTaxis,_PrUnique,_Alloc>::random_insert(T const & data)
	{
		if( setMap.find(&data)!= setMap.end() )
		{
			return setData.end();
		}
		size_t dataSize = count(data);
		iterator it;
		if(!dataSize)
		{
			it = setData.insert(data);
		}
		else
		{
			++dataSize;
			size_t count = rand()%dataSize;
			iterator insertIt;
			if(count<=dataSize/2)
			{
				insertIt = lower_bound(data);
				for(;count != 0; --count)
				{
					++insertIt;
				}
			}
			else
			{
				insertIt = upper_bound(data);
				for(++count;count < dataSize; ++count)
				{
					--insertIt;
				}
			}
			it = setData.insert(insertIt,data);
		}
		setMap[&(*it)] = it;
		return it;
	}
}

/*
namespace Taxis
{

	template<class T,class _Pr = less<T>,class _Alloc = std::allocator<T> >
	class List
	{
	public:
		typedef typename std::list<T,_Alloc>::iterator iterator;
		iterator insert(iterator,T);
		size_t erase(T);
		iterator erase(iterator);
		const T& front() const;
		const T& back() const;
		void pop_back();
		void pop_front();
		void push_back(T);
		void push_front(T);
	private:
		std::map<T,iterator,_Pr,_Alloc> listMap;
		std::list<T,_Alloc> listData;
	};

	template<class T,class _Pr,class _Alloc>
	typename List<T,_Pr,_Alloc>::iterator typename List<T,_Pr,_Alloc>::insert(typename List<T,_Pr,_Alloc>::iterator itBase, T data)
	{
		if( listMap.find(data)!=listMap.end() )
		{
			return listData.end();
		}
		return listMap[data] = listData.insert(itBase,data);
	}

	template<class T,class _Pr,class _Alloc>
	size_t List<T,_Pr,_Alloc>::erase(T data)
	{
		iterator it = listMap.find(data);
		if( it!=listMap.end())
		{
			listData.erase(it->second);
			listMap.erase(it);
			return 1;
		}
		return 0;
	}

	template<class T,class _Pr,class _Alloc>
	typename List<T,_Pr,_Alloc>::iterator List<T,_Pr,_Alloc>::erase(typename List<T,_Pr,_Alloc>::iterator itErase)
	{
		listMap.erase(*itErase);
		return listData.erase(itErase);
	}

	template<class T,class _Pr,class _Alloc>
	inline const T& List<T,_Pr,_Alloc>::front() const
	{
		return listData.front();
	}

	template<class T,class _Pr,class _Alloc>
	inline const T& List<T,_Pr,_Alloc>::back() const
	{
		return listData.back();
	}

	template<class T,class _Pr,class _Alloc>
	inline void List<T,_Pr,_Alloc>::pop_back()
	{
		listData.pop_back();
	}

	template<class T,class _Pr,class _Alloc>
	inline void List<T,_Pr,_Alloc>::pop_front()
	{
		listData.pop_front();
	}

	template<class T,class _Pr,class _Alloc>
	inline void List<T,_Pr,_Alloc>::push_back(T data)
	{
		listData.push_back(data);
	}

	template<class T,class _Pr,class _Alloc>
	inline void List<T,_Pr,_Alloc>::push_front(T data)
	{
		listData.push_front(data);
	}

}
*/

#endif
