#include "include/FdSet.h"

using namespace Zoic;

FdSet::FdSet()
{
#ifdef HAVE_MINGW32
	m_fdSet=reinterpret_cast<_fd_set *>(new char[sizeof(_fd_set)]);
	m_fdSet->fd_count = 0;
	m_fdArraySize = INIT_ARRAY_SIZE;
#else
	m_fdSet=reinterpret_cast<_fd_set *>(new char[sizeof(_fd_set)]);
	m_fdCount = 0;
	m_bitsSize = INIT_ARRAY_SIZE;
	memset(m_fdSet->bits,0,sizeof(_fd_mask)*m_bitsSize);
#endif
}

FdSet::~FdSet()
{
	delete [] reinterpret_cast<char *>(m_fdSet);
}

#define	zoic_howmany(x, y)	(((x) + ((y) - 1)) / (y))

#define	ZOIC_NFDBITS	(sizeof(_fd_mask) * 8)	/* bits per mask */
#define	zoic_fdset_mask(n)	((_fd_mask)1 << ((n) % ZOIC_NFDBITS))
#define	ZOIC_FD_ISSET(n, p)	(((p)->bits[(n)/ZOIC_NFDBITS] & zoic_fdset_mask(n)) != 0)
#define	ZOIC_FD_SET(n, p)	((p)->bits[(n)/ZOIC_NFDBITS] |= zoic_fdset_mask(n))
#define	ZOIC_FD_CLR(n, p)	((p)->bits[(n)/ZOIC_NFDBITS] &= ~zoic_fdset_mask(n))

void FdSet::getSockets(int * size,SOCKET * sockets) const
{
#ifdef HAVE_MINGW32
	* size = m_fdSet->fd_count;
	memcpy(sockets,m_fdSet->fd_array,sizeof(SOCKET)*m_fdSet->fd_count);
#else
	* size = 0;
	for(SOCKET s = 0; s< m_fdCount;++s)
	{
		if(ZOIC_FD_ISSET(s,m_fdSet))
		{
			sockets[(*size)++]=s;
		}
	}
#endif
}


void FdSet::setSocket(SOCKET socket)
{
#ifdef HAVE_MINGW32
	unsigned int i;
	for (i = 0; i < m_fdSet->fd_count; i++)
	{
		if ( m_fdSet->fd_array[i] == socket)
		{
			break;
		}
	}
    if (i == m_fdSet->fd_count)
	{
		if ( m_fdSet->fd_count < m_fdArraySize)
		{
			m_fdSet->fd_array[m_fdSet->fd_count++] = socket;
		}
		else
		{
			unsigned int newSize = m_fdSet->fd_count+1;
			_fd_set * newFdSet = reinterpret_cast<_fd_set *>(new char[sizeof(_fd_set)+sizeof(SOCKET)*(newSize-INIT_ARRAY_SIZE)]);
			newFdSet->fd_count = m_fdSet->fd_count;
			memcpy(newFdSet->fd_array,m_fdSet->fd_array,sizeof(SOCKET)*m_fdSet->fd_count);
			delete [] reinterpret_cast<char *>(m_fdSet);
			newFdSet->fd_array[newFdSet->fd_count++] = socket;
			m_fdSet = newFdSet;
			m_fdArraySize = newSize;
		}
	}
#else
	if( socket >= m_fdCount)
	{
		m_fdCount = socket + 1;
		int bitsSize = zoic_howmany(m_fdCount,ZOIC_NFDBITS);
		if(bitsSize > m_bitsSize)
		{
			_fd_set * newFdSet = reinterpret_cast<_fd_set *>(new char[sizeof(_fd_set)+sizeof(_fd_mask)*(bitsSize-INIT_ARRAY_SIZE)]);
			memcpy(newFdSet->bits,m_fdSet->bits,sizeof(_fd_mask)*m_bitsSize);
			memset(newFdSet->bits+m_bitsSize,0,sizeof(_fd_mask)*(bitsSize-m_bitsSize));
			delete [] reinterpret_cast<char *>(m_fdSet);
			m_fdSet = newFdSet;
			m_bitsSize = bitsSize;
		}
	}
	ZOIC_FD_SET(socket,m_fdSet);
#endif
}

void FdSet::clearSocket(SOCKET socket)
{
#ifdef HAVE_MINGW32
	unsigned int i;
	for (i = 0; i <	m_fdSet->fd_count ; i++)
	{
		if ( m_fdSet->fd_array[i] == socket)
		{
			while (i < m_fdSet->fd_count-1)
			{
				m_fdSet->fd_array[i] = m_fdSet->fd_array[i+1];
				i++;
			}
			m_fdSet->fd_count--;
			break;
		}
	}
#else
	if(socket >= m_fdCount)
	{
		return;
	}
	if(ZOIC_FD_ISSET(socket,m_fdSet))
	{
		ZOIC_FD_CLR(socket,m_fdSet);
		if(socket==m_fdCount-1)
		{//¸üÐÂfdCount
			int s;
			for(s = m_fdCount-2;s>=0;--s)
			{
				if(ZOIC_FD_ISSET(s,m_fdSet))
				{
					break;
				}
			}
			m_fdCount = s+1;
		}
	}
#endif
}

void FdSet::copyTo(FdSet & dest)
{
#ifdef HAVE_MINGW32
	if(m_fdSet->fd_count > dest.m_fdArraySize)
	{
		_fd_set * newFdSet = reinterpret_cast<_fd_set *>(new char[sizeof(_fd_set)+sizeof(SOCKET)*(m_fdSet->fd_count-INIT_ARRAY_SIZE)]);
		delete [] reinterpret_cast<char *>(dest.m_fdSet);
		dest.m_fdSet = newFdSet;
		dest.m_fdArraySize = m_fdSet->fd_count;
	}
	dest.m_fdSet->fd_count = m_fdSet->fd_count;
	memcpy(dest.m_fdSet->fd_array,m_fdSet->fd_array,sizeof(SOCKET)*m_fdSet->fd_count);
#else
	int bitsSize = zoic_howmany(m_fdCount,ZOIC_NFDBITS);
	if(bitsSize > dest.m_bitsSize)
	{
		_fd_set * newFdSet = reinterpret_cast<_fd_set *>(new char[sizeof(_fd_set)+sizeof(_fd_mask)*(bitsSize-INIT_ARRAY_SIZE)]);
		delete [] reinterpret_cast<char *>(dest.m_fdSet);
		dest.m_fdSet = newFdSet;
		dest.m_bitsSize = bitsSize;
	}
	dest.m_fdCount = m_fdCount;
	memcpy(dest.m_fdSet->bits,m_fdSet->bits,sizeof(_fd_mask)*bitsSize);
#endif
}
