/*! @file
	@ingroup network
	@brief 定义缓存区
*/

#ifndef __ZOIC_DATABUFFER_H
#define __ZOIC_DATABUFFER_H

#include "Types.h"
#include <string>
#include <vector>
#include "NoCopy.h"

namespace Zoic
{
	class DataBuffer
	{
	public:
		DataBuffer();
		~DataBuffer();
		bool replace(char * pBuffer,DWORD buffSize,char** pOld = NULL);//如果pOld为NULL，则自动执行删除操作，否则有外部管理
		bool cleanAllData();//清空所有数据

		DWORD getBufferSize() const;
		DWORD getDataSize() const;

		bool forceAdjustBuffer();

		bool isLocked() const;
		void forceUnlock();

		char * lockRead(DWORD datasize);
		bool unlockRead(DWORD realsize);

		char * lockWrite(DWORD datasize);
		bool unlockWrite(DWORD realsize);
	private:
		void _foreAdjust();
		char * m_buffer;
		DWORD m_buffer_size;
		DWORD m_start_positon;
		DWORD m_data_size;

		enum LockType
		{
			LT_NOLOCK,
			LT_PUSH,
			LT_POP,
		} m_locktype;
		DWORD m_lockedSize;
	};

	inline DataBuffer::DataBuffer()
		:m_buffer(NULL)
		,m_buffer_size(0)
		,m_start_positon(0)
		,m_data_size(0)
		,m_locktype(LT_NOLOCK)
		,m_lockedSize(0)
	{
	}

	inline DataBuffer::~DataBuffer()
	{
		if(m_buffer)
		{
			delete [] m_buffer;
		}
	}

	inline bool DataBuffer::replace(char * pBuffer,DWORD buffSize,char** pOld)
	{
		if(m_data_size > buffSize)
		{
			return false;
		}
		if( m_locktype != LT_NOLOCK )
		{
			return false;
		}
		if( m_data_size )
		{
			memmove( pBuffer,lockRead( m_data_size ), m_data_size );
			m_locktype = LT_NOLOCK;
		}
		char* tempOldBuff = m_buffer;
		m_buffer = pBuffer;
		m_buffer_size = pBuffer ? buffSize : 0;
		m_start_positon = 0;
		if(pOld)
		{
			*pOld = tempOldBuff;
		}
		else if(tempOldBuff)
		{
			delete [] tempOldBuff;
		}
		return true;
	}

	inline bool DataBuffer::cleanAllData()
	{
		if( m_locktype != LT_NOLOCK )
		{
			return false;
		}
		m_data_size = 0;
		m_start_positon = 0;
		return true;
	}

	inline DWORD DataBuffer::getBufferSize() const
	{
		return m_buffer_size;
	}

	inline DWORD DataBuffer::getDataSize() const
	{
		return m_data_size;
	}

	inline bool DataBuffer::forceAdjustBuffer()
	{
		if( m_locktype != LT_NOLOCK )
		{
			return false;
		}
		_foreAdjust();
		return true;
	}

	inline bool DataBuffer::isLocked() const
	{
		return (m_locktype != LT_NOLOCK);
	}

	inline void DataBuffer::forceUnlock()
	{
		m_locktype = LT_NOLOCK;
		m_lockedSize = 0;
	}

	inline void DataBuffer::_foreAdjust()
	{
		if( m_start_positon == 0 )
		{
			return;
		}
		memmove(m_buffer,m_buffer+m_start_positon,m_data_size);
		m_start_positon = 0;
	}

	inline char * DataBuffer::lockRead(DWORD datasize)
	{
		if(!datasize)
		{
			return NULL;
		}
		if( m_data_size < datasize )
		{
			return NULL;
		}
		if( m_locktype != LT_NOLOCK )
		{
			return NULL;
		}
		m_locktype = LT_POP;
		m_lockedSize = datasize;
		return m_buffer + m_start_positon;
	}

	inline bool DataBuffer::unlockRead(DWORD realsize)
	{
		if( m_lockedSize < realsize )
		{
			ASSERT(0);
			return false;
		}
		ASSERT(realsize <= m_data_size )
		if( m_locktype != LT_POP )
		{
			ASSERT(0);
			return false;
		}
		m_locktype = LT_NOLOCK;
		m_lockedSize = 0;
		m_data_size-=realsize;
		m_start_positon+=realsize;
		return true;
	}

	inline char * DataBuffer::lockWrite(DWORD datasize)
	{
		if(!datasize)
		{
			return NULL;
		}
		const DWORD nextDataSize = m_data_size + datasize;
		if( nextDataSize > m_buffer_size)
		{
			return NULL;
		}
		if( m_locktype != LT_NOLOCK )
		{
			ASSERT(0);
			return NULL;
		}
		m_locktype = LT_PUSH;
		m_lockedSize = datasize;
		if( m_start_positon + nextDataSize > m_buffer_size )
		{
			_foreAdjust();
		}
		return m_buffer+m_start_positon+m_data_size;
	}

	inline bool DataBuffer::unlockWrite(DWORD realsize)
	{
		if( m_lockedSize < realsize )
		{
			ASSERT(0);
			return false;
		}
		if( m_locktype != LT_PUSH )
		{
			ASSERT(0);
			return false;
		}
		m_locktype = LT_NOLOCK;
		m_lockedSize = 0;
		m_data_size+=realsize;
		return true;
	}

}

#endif
