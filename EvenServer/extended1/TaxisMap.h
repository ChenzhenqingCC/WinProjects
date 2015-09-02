#ifndef __TAXISMAP_H
#define __TAXISMAP_H

#include <set>
#include <map>
#include <list>

namespace Taxis
{

	template<class KEY,class T,class _PrTaxis = std::less<KEY>,class _PrUnique = std::less<T>,class _Alloc = std::allocator<pair<const KEY, T> > >
	class MultiMap
	{
	public:
		typedef typename std::multimap<KEY,T,_PrTaxis,_Alloc>::const_iterator const_iterator;
		typedef T value_type;
		iterator insert( const std::pair<KEY,T>& );
		size_t erase(const T&);
		iterator erase(iterator);
		const_iterator find(const T&) const;
		iterator find(const T&);
		iterator lower_bound(const KEY&);
		iterator upper_bound(const KEY&);
		const_iterator begin() const;
		iterator begin();
		const_iterator end() const;
		iterator end();
		void clear();
		size_t count(const KEY&);
		size_t size() const;
		bool empty() const;
		std::pair<const_iterator, const_iterator> equal_range(const KEY&);
		iterator random_insert(const std::pair<KEY,T>&);
	private:
		std::map<const T*,iterator,_PrUnique,_Alloc> mapMap;
		std::multimap<KEY,T,_PrTaxis,_Alloc> mapData;
	};

}

namespace Taxis
{
	template<class KEY,class T,class _PrTaxis,class _PrUnique,class _Alloc>
	typename MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::iterator MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::insert(const std::pair<KEY,T>& dataPair)
	{
		if( mapMap.find(dataPair.second)!= mapMap.end() )
		{
			return mapData.end();
		}
		iterator it = mapData.insert(dataPair);
		mapMap[dataPair.second] = it;
		return it;
	}

	template<class KEY,class T,class _PrTaxis,class _PrUnique,class _Alloc>
	size_t MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::erase(const T& data)
	{
		typename std::map<T,iterator,_PrUnique,_Alloc>::iterator it = mapMap.find(data);
		if( it != mapMap.end() )
		{
			mapData.erase(it->second);
			mapMap.erase(it);
			return 1;
		}
		return 0;
	}

	template<class KEY,class T,class _PrTaxis,class _PrUnique,class _Alloc>
	typename MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::iterator MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::erase(iterator it)
	{
		mapMap.erase(it->second);
		mapData.erase(it++);
		return it;
	}

	template<class KEY,class T,class _PrTaxis,class _PrUnique,class _Alloc>
	typename MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::const_iterator MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::find(const T& data) const
	{
		typename std::map<T,iterator,_PrUnique,_Alloc>::const_iterator it = mapMap.find(data);
		if( it != mapMap.end() )
		{
			return it->second;
		}
		return mapData.end();
	}

	template<class KEY,class T,class _PrTaxis,class _PrUnique,class _Alloc>
	typename MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::iterator MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::find(const T& data)
	{
		typename std::map<T,iterator,_PrUnique,_Alloc>::iterator it = mapMap.find(data);
		if( it != mapMap.end() )
		{
			return it->second;
		}
		return mapData.end();
	}

	template<class KEY,class T,class _PrTaxis,class _PrUnique,class _Alloc>
	inline typename MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::iterator MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::lower_bound(const KEY& key)
	{
		return mapData.lower_bound(key);
	}

	template<class KEY,class T,class _PrTaxis,class _PrUnique,class _Alloc>
	inline typename MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::iterator MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::upper_bound(const KEY& key)
	{
		return mapData.upper_bound(key);
	}

	template<class KEY,class T,class _PrTaxis,class _PrUnique,class _Alloc>
		inline typename MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::const_iterator MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::begin() const
	{
		return mapData.begin();
	}

	template<class KEY,class T,class _PrTaxis,class _PrUnique,class _Alloc>
	inline typename MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::iterator MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::begin()
	{
		return mapData.begin();
	}

	template<class KEY,class T,class _PrTaxis,class _PrUnique,class _Alloc>
	inline typename MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::const_iterator MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::end() const
	{
		return mapData.end();
	}

	template<class KEY,class T,class _PrTaxis,class _PrUnique,class _Alloc>
	inline typename MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::iterator MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::end()
	{
		return mapData.end();
	}

	template<class KEY,class T,class _PrTaxis,class _PrUnique,class _Alloc>
	inline void MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::clear()
	{
		mapMap.clear();
		mapData.clear();
	}

	template<class KEY,class T,class _PrTaxis,class _PrUnique,class _Alloc>
	inline size_t MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::count(const KEY& key)
	{
		return mapData.count(key);
	}

	template<class KEY,class T,class _PrTaxis,class _PrUnique,class _Alloc>
	inline size_t MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::size() const
	{
		return mapData.size();
	}

	template<class KEY,class T,class _PrTaxis,class _PrUnique,class _Alloc>
	inline bool MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::empty() const
	{
		return size() == 0;
	}

	template<class KEY,class T,class _PrTaxis,class _PrUnique,class _Alloc>
	inline typename std::pair<typename MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::iterator, typename MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::iterator> MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::equal_range(const KEY& key)
	{
		return mapData.equal_range(key);
	}

	template<class KEY,class T,class _PrTaxis,class _PrUnique,class _Alloc>
	typename MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::iterator MultiMap<KEY,T,_PrTaxis,_PrUnique,_Alloc>::random_insert(const std::pair<KEY,T>& dataPair)
	{
		if( mapMap.find(dataPair.second)!= mapMap.end() )
		{
			return mapData.end();
		}
		size_t dataSize = mapData.count(dataPair.first);
		iterator it;
		if(!dataSize)
		{
			it = mapData.insert(dataPair);
		}
		else
		{
			++dataSize;
			size_t count = rand()%dataSize;
			iterator insertIt;
			if(count<=dataSize/2)
			{
				insertIt = lower_bound(dataPair.first);
				for(;count != 0; --count)
				{
					++insertIt;
				}
			}
			else
			{
				insertIt = upper_bound(dataPair.first);
				for(++count;count < dataSize; ++count)
				{
					--insertIt;
				}
			}
			it = mapData.insert(insertIt,dataPair);
		}
		mapMap[dataPair.second] = it;
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
