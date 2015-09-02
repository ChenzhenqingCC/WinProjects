#pragma warning(disable:4996)

#ifdef __RANAINPUTSTACK_H_FORBIDDEN
#error "RanaInputStack.h" is forbidden!!
#endif

#ifndef __RANAINPUTSTACK_H
#define __RANAINPUTSTACK_H
#include <string>
#include "RanaHeader.h"
#include "RanaInputArgumentException.h"

class RanaInputStack
{
public:
	RanaInputStack(RanaState L);
	const RanaState & getState();
	int getIndex();
	void incIndex();
private:
	int			m_index;
	RanaState	m_state;
};

//从当前表中得到子表
struct RanaSubTable
{
public:
	RanaSubTable(RanaState L,int table,const char *name);
	~RanaSubTable();
	int	index;
private:
	RanaState	m_state;
};

//检查参数模板
template <class T>
class RanaCheckType
{
public:
	static void validateCheck(RanaState L,int index);
};

template <class T>
void RanaCheckType<T>::validateCheck(RanaState L,int index)
{
	if(!lua_istable(L,index))
	{
		throw new RanaInputArgumentException(L, " is not a table.");
	}
}

template <>
inline void RanaCheckType<bool>::validateCheck(RanaState L,int index)
{
	if(!lua_isboolean(L,index))
	{
		throw new RanaInputArgumentException(L, " is not a boolean.");
	}
}

template <>
inline void RanaCheckType<std::string>::validateCheck(RanaState L,int index)
{
	if(!lua_isstring(L,index))
	{
		throw new RanaInputArgumentException(L, " is not a string.");
	}
}

template <>
inline void RanaCheckType<const char*>::validateCheck(RanaState L,int index)
{
	if(!lua_isstring(L,index))
	{
		throw new RanaInputArgumentException(L, " is not a string.");
	}
}

template <>
inline void RanaCheckType<char>::validateCheck(RanaState L,int index)
{
	if(!lua_isnumber(L,index))
	{
		throw new RanaInputArgumentException(L, " is not a number.");
	}
}

template <>
inline void RanaCheckType<unsigned char>::validateCheck(RanaState L,int index)
{
	if(!lua_isnumber(L,index))
	{
		throw new RanaInputArgumentException(L, " is not a number.");
	}
}

template <>
inline void RanaCheckType<short>::validateCheck(RanaState L,int index)
{
	if(!lua_isnumber(L,index))
	{
		throw new RanaInputArgumentException(L, " is not a number.");
	}
}

template <>
inline void RanaCheckType<unsigned short>::validateCheck(RanaState L,int index)
{
	if(!lua_isnumber(L,index))
	{
		throw new RanaInputArgumentException(L, " is not a number.");
	}
}

template <>
inline void RanaCheckType<int>::validateCheck(RanaState L,int index)
{
	if(!lua_isnumber(L,index))
	{
		throw new RanaInputArgumentException(L, " is not a number.");
	}
}

template <>
inline void RanaCheckType<unsigned int>::validateCheck(RanaState L,int index)
{
	if(!lua_isnumber(L,index))
	{
		throw new RanaInputArgumentException(L, " is not a number.");
	}
}

template <>
inline void RanaCheckType<long>::validateCheck(RanaState L,int index)
{
	if(!lua_isnumber(L,index))
	{
		throw new RanaInputArgumentException(L, " is not a number.");
	}
}

template <>
inline void RanaCheckType<unsigned long>::validateCheck(RanaState L,int index)
{
	if(!lua_isnumber(L,index))
	{
		throw new RanaInputArgumentException(L, " is not a number.");
	}
}

template <>
inline void RanaCheckType<float>::validateCheck(RanaState L,int index)
{
	if(!lua_isnumber(L,index))
	{
		throw new RanaInputArgumentException(L, " is not a number.");
	}
}

template <>
inline void RanaCheckType<double>::validateCheck(RanaState L,int index)
{
	if(!lua_isnumber(L,index))
	{
		throw new RanaInputArgumentException(L, " is not a number.");
	}
}

template <>
inline void RanaCheckType<RanaVariant>::validateCheck(RanaState L,int index)
{
}

//参数解析模板
template <class T>
RanaInputStack & operator >> (RanaInputStack &s,T &val)
{
	try
	{
		RanaCheckType<T>::validateCheck(s.getState(),s.getIndex());
		rana_get(s.getState(),s.getIndex(),val); 
	}catch(RanaInputArgumentException *e)
	{
		lua_pop(s.getState(),1);
		char buf[20];
		snprintf(buf,sizeof(buf),"arg%d",s.getIndex());
		buf[sizeof(buf)-1]=0;
		e->appendPrefix(buf);
		throw e;
	}
	s.incIndex();
	return s;
}

//表读取成员模板
template <class T>
void rana_get(RanaState L,int index,const char *name,T &val)
{
	if(!lua_istable(L,index))
	{
		RanaInputArgumentException *e = new RanaInputArgumentException(L, " is not a table.");
		e->appendPrefix(name);
		throw e;
	}
	lua_pushstring(L,name);
	lua_gettable(L,index);
	try
	{
		RanaCheckType<T>::validateCheck(L,lua_gettop(L));
		rana_get(L,lua_gettop(L),val);
	}catch(RanaInputArgumentException *e)
	{
		lua_pop(L,1);
		e->appendPrefix(name);
		throw e;
	}
	lua_pop(L,1);
}


inline const RanaState & RanaInputStack::getState()
{
	return m_state;
}

inline int RanaInputStack::getIndex()
{
	return m_index;
}

inline void RanaInputStack::incIndex()
{
	++m_index;
}

inline RanaInputStack::RanaInputStack(RanaState L):
	m_state(L),
	m_index(1)
{
}

inline RanaSubTable::RanaSubTable(RanaState L,int table,const char *name):
	m_state(L)
{
	lua_pushstring(L,name);
	lua_gettable(L,table);
	index=lua_gettop(L);
	if(!lua_istable(L,index))
	{
		lua_pop(L,1);
		throw new RanaInputArgumentException(L, " is not a table.");
	}
}

inline RanaSubTable::~RanaSubTable()
{
	lua_pop(m_state,1);
}

inline void rana_get(RanaState L,int index,std::string & val)
{
	val.assign(lua_tostring(L,index),lua_strlen(L,index));
}

inline void rana_get(RanaState L,int index,const char* &val)
{
	val=lua_tostring(L,index);
}

inline void rana_get(RanaState L,int index,bool &val)
{
	val=lua_toboolean(L,index)?true:false;
}

inline void rana_get(RanaState L,int index,char &val)
{
	val=static_cast<char>(lua_tonumber(L,index));
}

inline void rana_get(RanaState L,int index,unsigned char &val)
{
	val=static_cast<unsigned char>(lua_tonumber(L,index));
}

inline void rana_get(RanaState L,int index,short &val)
{
	val=static_cast<short>(lua_tonumber(L,index));
}

inline void rana_get(RanaState L,int index,unsigned short &val)
{
	val=static_cast<unsigned short>(lua_tonumber(L,index));
}

inline void rana_get(RanaState L,int index,int &val)
{
	val=static_cast<int>(lua_tonumber(L,index));
}

inline void rana_get(RanaState L,int index,unsigned int &val)
{
	val=static_cast<unsigned int>(lua_tonumber(L,index));
}

inline void rana_get(RanaState L,int index,long &val)
{
	val=static_cast<long>(lua_tonumber(L,index));
}

inline void rana_get(RanaState L,int index,unsigned long &val)
{
	val=static_cast<unsigned long>(lua_tonumber(L,index));
}

inline void rana_get(RanaState L,int index,float &val)
{
	val=static_cast<float>(lua_tonumber(L,index));
}

inline void rana_get(RanaState L,int index,double &val)
{
	val=static_cast<double>(lua_tonumber(L,index));
}

inline void rana_get(RanaState L,int index,RanaVariant& val)
{
	switch( lua_type(L,index) )
	{
	case LUA_TNIL:
		val.clear();
		break;
	case LUA_TNUMBER:
		val = static_cast<int>( lua_tonumber(L, index) );
		break;
	case LUA_TSTRING:
		val = lua_tostring(L, index);
		break;
	case LUA_TBOOLEAN:
		val = lua_toboolean(L, index);
		break;
	default:
		val.clear();
		throw new RanaInputArgumentException(L, " is not a simple value.");
		break;
	}
}

inline void rana_get(RanaState L,int index,unsigned long long & p)
{
	unsigned long low;
	unsigned long high;
	rana_get(L,index,"low",low);
	rana_get(L,index,"high",high);
	p = ( static_cast<long long>(high) << 32 ) | low;
}

//extern "C" {
//	LUAI_FUNC void luaG_runerror (lua_State *L, const char *fmt, ...);
//}
inline void onCatchRanaInputException(RanaState L, const char* inputName, RanaInputArgumentException * e)
{
	char errorMsg[1024];
	snprintf(errorMsg,1023,"[%s] %s",inputName,e->what());
	delete e;
	lua_pushstring(L,errorMsg);
	lua_error(L);
}

#define RANA_INPUT_BEGIN \
{ \
	RanaInputStack s(L); \
	try \
	{

#define RANA_INPUT_END(NAME) \
	}catch(RanaInputArgumentException * e) \
	{ \
		onCatchRanaInputException(L,#NAME,e); \
	} \
}

#endif
