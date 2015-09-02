#ifndef __ZOIC_TINYBINARY_H
#define __ZOIC_TINYBINARY_H

#include <string>
#include "Message.h"
#include "OutOfBoundException.h"

namespace Zoic
{
	class TinyRefBinary
	{
	public:
		TinyRefBinary(std::string& string);
		void setData(WORD size,const BYTE* pData);
		const BYTE* getData() const;
		WORD getSize() const;
		void extractFrom(Zoic::Message& m);
		void ouputTo(Zoic::Message& m) const;
	protected:
		std::string& _get_ref();
	private:
		std::string& m_string;
	};

	class TinyBinary
		:public TinyRefBinary
	{
	public:
		TinyBinary();
		TinyBinary(const TinyBinary& o);
		TinyBinary& operator = (const TinyBinary& o);
		TinyBinary& operator = (const Zoic::Message& o);
		bool operator == (const Zoic::TinyBinary& o) const;
	private:
		std::string m_selfData;
	};

	inline Zoic::Message & operator << ( Zoic::Message &m,const TinyRefBinary & p )
	{
		p.ouputTo(m);
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,TinyRefBinary & p )
	{
		p.extractFrom(m);
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const TinyBinary & p )
	{
		p.ouputTo(m);
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,TinyBinary & p )
	{
		p.extractFrom(m);
		return m;
	}

	template<class T>
	inline void operator << ( TinyBinary &b,const T & p )
	{
		Zoic::Message tempMesg;
		tempMesg.ID(-1) << p;
		b.setData(tempMesg.getSize()-NETWORK_MSG_HEADER,tempMesg.getData()+NETWORK_MSG_HEADER);
	}

	template<class T>
	inline void operator >> ( const TinyBinary &b,T & p )
	{
		Zoic::Message tempMesg;
		tempMesg.ID(-1).writeData(b.getData(),b.getSize());
		tempMesg >> p;
		if( tempMesg.getReadPosition()!=tempMesg.getData()+tempMesg.getSize() )
		{
			throw new Zoic::OutOfBoundException("TinyBinary Message ReadData %d < %d (%s)!",
				( ((const BYTE*)tempMesg.getReadPosition())-tempMesg.getData() )-NETWORK_MSG_HEADER,tempMesg.getSize()-NETWORK_MSG_HEADER,
				__PRETTY_FUNCTION__ ); 
		}
	}

	//应对Zoic::Message>>const char*的问题
	template<>
	inline void operator >> ( const TinyBinary &b,const char* & p )
	{
		p = (const char*)((const void*)b.getData());
	}
}

#ifdef __RANAINPUTSTACK_H
template <>
inline void RanaCheckType<Zoic::TinyBinary>::validateCheck(RanaState L,int index)
{
	if(!lua_isstring(L,index))
	{
		throw new RanaInputArgumentException(L, " is not a string.");
	}
}

inline void rana_get(RanaState L,int index,Zoic::TinyBinary &val)
{
	val.setData( (WORD)(lua_strlen(L,index)),(const BYTE*)((const void *)(lua_tostring(L,index))) );
}

inline void rana_push(RanaState L,const Zoic::TinyBinary & val)
{
	lua_pushlstring(L, (const char*)((const void *)val.getData()),val.getSize());
}
#else
#define __RANAINPUTSTACK_H_FORBIDDEN
#endif

#endif //__ZOIC_TINYBINARY_H

