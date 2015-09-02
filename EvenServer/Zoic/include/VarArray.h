/*! @file
	@ingroup utils
	@brief ��������ģ��
*/

#ifndef __ZOIC_VARARRAY_H
#define __ZOIC_VARARRAY_H
#include "Types.h"

namespace Zoic
{
	/*!
		@brief ����ģ��
		@ingroup utils
		@param T Ԫ������
		@param LEN ������󳤶�


		��Ҫ���������С�Ϊ��ʵ�ִ�Ŀ��,���������ڵ�����ֻ����ƽ����������
	*/
	template <class T,unsigned int LEN,class SIZE_TYPE = WORD>
	class VarArray
	{
	public:
		typedef T &			reference;
		typedef const T	&	const_reference;
		typedef SIZE_TYPE	size_type;

		//@{
		/*! @brief �±������
		@return Ԫ������
		@param pos λ��
		*/
		reference operator[](size_type pos);
		const_reference operator[](size_type pos) const;
		//@}

		//!��ǰ����
		size_type size() const;

		//!�������
		void clear();

		//!׷��һ��Ԫ��
		void push_back(
			const T & val	//!< Ԫ��
			);

		//!��β��ɾ��һ��Ԫ��
		void pop_back();

		//!��β��ɾ��size��Ԫ��
		void pop_back(
			size_type size	//!< Ԫ������
			);

		//!ɾ��һ��Ԫ��
		void erase(
			size_type pos	//!< λ��
			);

		//!�õ������ʼԪ��ָ��
		const T * getData() const;

		//!ͬʱ���ö������Ԫ��
		void setData(
			const T * buf,	//!< ��ʼԪ��ָ��
			size_type size	//!< Ԫ������
			);

		//!��ȱȽϲ�����
		bool operator == (const VarArray & o) const;

		//!���ȱȽϲ�����
		bool operator != (const VarArray & o) const;

		static size_type getSizeLimit();
	private:
		size_type	m_size;		//!< ��ǰ���鳤��
		T		m_data[LEN];	//!< ��������
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
