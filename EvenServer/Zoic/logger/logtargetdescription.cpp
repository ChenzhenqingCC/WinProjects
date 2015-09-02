#include "logtargetdescription.h"

using namespace Zoic;

LogTargetDescription::LogTargetDescription(unsigned char* buf,unsigned long limit):
m_pBuf(buf),
m_limit(limit)
{
	clear();
}

LogTargetDescription::LogTargetDescription(const LogTargetDescription& rhd)
{
	*this = rhd;
}

void LogTargetDescription::clear()
{
	m_pSize = (unsigned long*)&m_pBuf[0];
	m_pType = (unsigned char*)&m_pBuf[sizeof(unsigned long)];
	m_pRead = m_pWrite = &m_pBuf[sizeof(unsigned long) + sizeof(unsigned char)];
}

void LogTargetDescription::readData(void* pData,int n)
{
	if (m_pRead + n > m_pBuf + m_limit)
	{
		puts("NetLogContentBuffer readData > m_limit!");
		return;
	}
	if (m_pRead + n > m_pBuf + getSize())
	{
		puts("NetLogContentBuffer readData > getSize()!");
		return;
	}
	memcpy(pData,m_pRead,n);
	m_pRead += n;
}

void LogTargetDescription::skipData(int n)
{
	if (m_pRead + n > m_pBuf + m_limit)
	{
		puts("NetLogContentBuffer skipData > m_limit!");
		return;
	}
	if (m_pRead + n > m_pBuf + getSize())
	{
		puts("NetLogContentBuffer skipData > getSize()!");
		return;
	}
	m_pRead += n;
}

void LogTargetDescription::writeData(const void* pData,int n)
{
	if (m_pWrite + n > m_pBuf + m_limit)
	{
		puts("NetLogContentBuffer writeData > m_limit!");
		return;
	}
	memcpy(m_pWrite,pData,n);
	m_pWrite += n;
	*m_pSize += n;
}

static inline int z_strlen( const char * p, int nMaxLen)
{
	int i;
	for ( i = 0 ; i < nMaxLen ; i++ )
	{
		if ( p[i] == '\0' )
			break;
	}
	if ( i == nMaxLen )
	{
		return -1;
	}
	return i;
}

LogTargetDescription & LogTargetDescription::operator >> ( std::string &val )
{
	int nLen = z_strlen( reinterpret_cast<const char *>(m_pRead), static_cast<int>( m_pBuf + getSize() - m_pRead ));
	if ( nLen != -1)
	{
		nLen++;
		val=reinterpret_cast<const char *>(m_pRead);
		m_pRead+=nLen;
	}
	else
	{
		val = "(null)";
		//throw new OutOfBoundException("[ID:%d] NetLogContentBuffer>>string",ID());
		puts("NetLogContentBuffer >> string failed!");
	}
	return * this;
}

LogTargetDescription & LogTargetDescription::operator >> ( const char* &val )
{
	int nLen = z_strlen( reinterpret_cast<const char *>(m_pRead), static_cast<int>( m_pBuf + getSize() - m_pRead ));
	if ( nLen != -1)
	{
		nLen++;
		val=reinterpret_cast<const char *>(m_pRead);
		m_pRead+=nLen;
	}
	else
	{
		val = "(null)";
		puts("NetLogContentBuffer >> PCSTR failed!");
	}
	return * this;
}