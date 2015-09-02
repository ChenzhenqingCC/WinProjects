#include "include/TinyBinary.h"

namespace Zoic
{
	TinyRefBinary::TinyRefBinary(std::string& string)
		:m_string(string)
	{
	}

	std::string& TinyRefBinary::_get_ref()
	{
		return m_string;
	}

	void TinyRefBinary::setData(WORD size,const BYTE* pData)
	{
		m_string.assign((char*)((void*)pData) ,size);
	}

	WORD TinyRefBinary::getSize() const
	{
		return static_cast<WORD>( m_string.size() );
	}

	const BYTE* TinyRefBinary::getData() const
	{
		return (const BYTE*)((const void*)m_string.c_str());
	}

	void TinyRefBinary::extractFrom(Zoic::Message& m)
	{
		WORD size;
		BYTE buff[0x10000];
		m >> size;
		m.readData(buff,size);
		setData(size,buff);
	}

	void TinyRefBinary::ouputTo(Zoic::Message& m) const
	{
		const WORD size = getSize();
		m << size;
		m.writeData(getData(),size );
	}


	TinyBinary::TinyBinary()
		:TinyRefBinary(m_selfData)
	{
	}

	TinyBinary::TinyBinary(const TinyBinary& o)
		:TinyRefBinary(m_selfData)
	{
		setData( o.getSize(),o.getData() );
	}

	TinyBinary& TinyBinary::operator = (const TinyBinary& o)
	{
		setData( o.getSize(),o.getData() );
		return *this;
	}

	bool TinyBinary::operator == (const TinyBinary& o) const
	{
		const WORD size = getSize();
		if (size != o.getSize())
		{
			return false;
		}
		
		if (memcmp(getData(), o.getData(), size))
		{
			return false;
		}
		return true;
	}
}
