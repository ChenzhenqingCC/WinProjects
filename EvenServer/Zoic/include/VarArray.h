/*! @file
	@ingroup utils
	@brief 定义数组模板
*/

#ifndef __ZOIC_VARARRAY_H
#define __ZOIC_VARARRAY_H
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
	template <class T,unsigned int LEN,class SIZE_TYPE = WORD>
	class VarArray
	{
	public:
		typedef T &			reference;
		typedef const T	&	const_reference;
		typedef SIZE_TYPE	size_type;

		//@{
		/*! @brief 下标操作符
		@return 元素引用
		@param pos 位置
		*/
		reference operator[](size_type pos);
		const_reference operator[](size_type pos) const;
		//@}

		//!当前长度
		size_type size() const;

		//!清除数据
		void clear();

		//!追加一个元素
		void push_back(
			const T & val	//!< 元素
			);

		//!从尾部删除一个元素
		void pop_back();

		//!从尾部删除size个元素
		void pop_back(
			size_type size	//!< 元素数量
			);

		//!删除一个元素
		void erase(
			size_type pos	//!< 位置
			);

		//!得到数组初始元素指针
		const T * getData() const;

		//!同时设置多个数组元素
		void setData(
			const T * buf,	//!< 初始元素指针
			size_type size	//!< 元素数量
			);

		//!相等比较操作符
		bool operator == (const VarArray & o) const;

		//!不等比较操作符
		bool operator != (const VarArray & o) const;

		static size_type getSizeLimit();
	private:
		size_type	m_size;		//!< 当前数组长度
		T		m_data[LEN];	//!< 数组数据
	};

	template <class T,unsigned int LEN,class SIZE_TYPE>
	inline typename VarArray<T,LEN,SIZE_TYPE>::size_type VarArray<T,LEN,SIZE_TYPE>::getSizeLimit( )
	{
		return LEN;
	}

	template <class T,unsigned int LEN,class SIZE_TYPE>
	inline typename VarArray<T,LEN,SIZE_TYPE>::reference VarArray<T,LEN,SIZE_TYPE>::operator[](size_type pos)
	{
		return m_data[pos];
	}

	template <class T,unsigned int LEN,class SIZE_TYPE>
	inline typename VarArray<T,LEN,SIZE_TYPE>::const_reference VarArray<T,LEN,SIZE_TYPE>::operator[](size_type pos) const
	{
		return m_data[pos];
	}
	
	template <class T,unsigned int LEN,class SIZE_TYPE>
	inline typename VarArray<T,LEN,SIZE_TYPE>::size_type VarArray<T,LEN,SIZE_TYPE>::size( ) const
	{
		return m_size;
	}
	
	template <class T,unsigned int LEN,class SIZE_TYPE>
	inline void VarArray<T,LEN,SIZE_TYPE>::clear()
	{
#ifdef _DEBUG
		size_type len = LEN - 1;
		ASSERT( static_cast<unsigned int>(len) == LEN-1 );
#endif
		m_size=0;
	}	
	
	template <class T,unsigned int LEN,class SIZE_TYPE>
	void VarArray<T,LEN,SIZE_TYPE>::push_back(const T & val)
	{
		if(m_size<LEN)
		{
			m_data[m_size++]=val;
		}
	}

	template <class T,unsigned int LEN,class SIZE_TYPE>
	void VarArray<T,LEN,SIZE_TYPE>::pop_back()
	{
		if(m_size)
		{
			m_size--;
		}
	}

	template <class T,unsigned int LEN,class SIZE_TYPE>
	void VarArray<T,LEN,SIZE_TYPE>::pop_back(size_type size)
	{
		if(m_size >= size)
		{
			m_size-=size;
		}
	}

	template <class T,unsigned int LEN,class SIZE_TYPE>
	void VarArray<T,LEN,SIZE_TYPE>::erase(size_type pos)
	{
		if( pos>=m_size)
		{
			return;
		}

		memmove( &m_data[pos], &m_data[pos+1], sizeof(T) * (m_size - (pos+1)) );
		--m_size;
	} 

	template <class T,unsigned int LEN,class SIZE_TYPE>
	const T * VarArray<T,LEN,SIZE_TYPE>::getData() const
	{
		return m_data;
	}

	template <class T,unsigned int LEN,class SIZE_TYPE>
	void VarArray<T,LEN,SIZE_TYPE>::setData(const T * buf,size_type size)
	{
		if(size>LEN)
		{
			size = LEN;
		}
		memcpy( m_data,buf, size * sizeof(T) );
		m_size = size;
	}

	template <class T,unsigned int LEN,class SIZE_TYPE>
	bool VarArray<T,LEN,SIZE_TYPE>::operator == (const VarArray & o) const
	{
		if(m_size!=o.m_size)
		{
			return false;
		}
		for(size_type i=0;i<m_size;i++)
		{
			if( m_data[i]!=o.m_data[i])
			{
				return false;
			}
		}
		return true;
	}

	template <class T,unsigned int LEN,class SIZE_TYPE>
	bool VarArray<T,LEN,SIZE_TYPE>::operator != (const VarArray & o) const
	{
		return !(*this==o);
	}

	class Message;
	namespace MessageImpl
	{
		void setString(Message & msg,const char *str,WORD buffSize);
		void getString(Message & msg,char *str,WORD buffSize);
	}

};

#endif
