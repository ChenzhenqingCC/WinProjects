/*! @file
	@ingroup utils
	@brief 定义数组模板
*/

#ifndef __ZOIC_FIXARRAY_H
#define __ZOIC_FIXARRAY_H
#include "Types.h"

namespace Zoic
{
	/*!
		@brief 数组模板
		@ingroup utils
		@param T 元素类型
		@param LEN 数组最大长度


		主要用于联合中。为了实现此目标,存在数组内的类型只能是平凡数据类型
	*/
	template <class T,unsigned int LEN>
	class FixArray
	{
	public:
		typedef T			ElementType;
		typedef T &			reference;
		typedef const T	&	const_reference;
		typedef unsigned int	size_type;
		static const size_type SIZE_LIMIT = LEN;
		
		//@}
		reference operator[](size_type pos);
		const_reference operator[](size_type pos) const;
		//!得到数组初始元素指针
		T * getData();
		const T * getData() const;

		//!相等比较操作符
		bool operator == (const FixArray & o) const;

		//!不等比较操作符
		bool operator != (const FixArray & o) const;

		static size_type getSizeLimit();


		template<class T2>
		FixArray& operator=(const FixArray<T2,LEN>& s);
	private:
		T		m_data[LEN];	//!< 数组数据
	};
	
	template <class T,unsigned int LEN>
	inline typename FixArray<T,LEN>::size_type FixArray<T,LEN>::getSizeLimit( )
	{
		return LEN;
	}

	template <class T,unsigned int LEN>
	inline typename FixArray<T,LEN>::reference FixArray<T,LEN>::operator[](size_type pos)
	{
		ASSERT(pos<LEN);
		return m_data[pos];
	}

	template <class T,unsigned int LEN>
	inline typename FixArray<T,LEN>::const_reference FixArray<T,LEN>::operator[](size_type pos) const
	{
		ASSERT(pos<LEN);
		return m_data[pos];
	}
	
	template <class T,unsigned int LEN>
	inline T * FixArray<T,LEN>::getData()
	{
		return m_data;
	}	

	template <class T,unsigned int LEN>
	inline const T * FixArray<T,LEN>::getData() const
	{
		return m_data;
	}	

	template <class T,unsigned int LEN>
	bool FixArray<T,LEN>::operator == (const FixArray & o) const
	{
		for(size_type i=0;i<LEN;i++)
		{
			if( m_data[i]!=o.m_data[i])
			{
				return false;
			}
		}
		return true;
	}

	template <class T,unsigned int LEN>
	template <class T2>
	FixArray<T,LEN>& FixArray<T,LEN>::operator=(const FixArray<T2,LEN>& s)
	{
		for(unsigned int i = 0;i<LEN;++i)
		{
			m_data[i] = s[i];
		}
		return *this;
	}

	template <class T,unsigned int LEN>
	bool FixArray<T,LEN>::operator != (const FixArray & o) const
	{
		return !(*this==o);
	}
};

#endif
