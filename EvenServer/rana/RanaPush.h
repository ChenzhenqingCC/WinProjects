#ifndef __RANA_PUSH_H
#define __RANA_PUSH_H

#include <set>
#include <vector>
#include <string>
#include "RanaHeader.h"

template <class K, class T, class A>
void rana_push(RanaState L,const std::set<K,T,A> &container);
template <class T, class A>
void rana_push(RanaState L,const std::vector<T,A> &container);
template <class T>
void rana_push(RanaState L,T *const & pobj);


//----------模板实现----------------
template <class K, class T, class A>
void rana_push(RanaState L,const std::set<K,T,A> &container)
{
	lua_newtable(L);
	int i;
	typename std::set<K,T,A>::const_iterator it;
	for(i=0,it=container.begin();it!=container.end();i++,++it)
	{
		const typename std::set<K,T,A>::value_type & t=*it;
		rana_push(L,t);
		lua_rawseti(L,-2,i+1);
	}
}

template <class T, class A>
void rana_push(RanaState L,const std::vector<T,A> &container)
{
	lua_newtable(L);

	int i;
	typename std::vector<T,A>::const_iterator it;
	for(i=0,it=container.begin();it!=container.end();i++,++it)
	{
		const typename std::vector<T,A>::value_type & t=*it;
		rana_push(L,t);
		lua_rawseti(L,-2,i+1);
	}
}

template <class T>
void rana_push(RanaState L,T *const & pobj)
{
	rana_push(L,*pobj);
}

//常用类型
inline void rana_push(RanaState L,const char* val)
{
	lua_pushstring(L, val);
}

inline void rana_push(RanaState L,const std::string & val)
{
	lua_pushlstring(L, val.c_str(),val.length());
}

inline void rana_push(RanaState L,bool val)
{
	lua_pushboolean(L,static_cast<int>(val));
}

inline void rana_push(RanaState L ,char val)
{
	lua_pushnumber(L,val);
}

inline void rana_push(RanaState L, unsigned char val)
{
	lua_pushnumber(L,val);
}

inline void rana_push(RanaState L,short val)
{
	lua_pushnumber(L,val);
}

inline void rana_push(RanaState L,unsigned short val)
{
	lua_pushnumber(L,val);
}

inline void rana_push(RanaState L,int val)
{
	lua_pushnumber(L,val);
}

inline void rana_push(RanaState L,unsigned int val)
{
	lua_pushnumber(L,val);
}

inline void rana_push(RanaState L,long val)
{
	lua_pushnumber(L,val);
}

inline void rana_push(RanaState L,unsigned long val)
{
	lua_pushnumber(L,val);
}

inline void rana_push(RanaState L,float val)
{
	lua_pushnumber(L,val);
}

inline void rana_push(RanaState L,double val)
{
	lua_pushnumber(L,val);
}

inline void rana_push(RanaState L,const RanaVariant &val)
{
	switch(val.getType())
	{
	case RanaVariant::VT_VOID:
		lua_pushnil(L);
		return;
	case RanaVariant::VT_BOOL:
		lua_pushboolean(L,bool(val));
		return;
	case RanaVariant::VT_INTEGER:
		lua_pushnumber(L,int(val));
		return;
	case RanaVariant::VT_STRING:
		lua_pushstring(L,val);
		return;
	case RanaVariant::VT_USERDATA:
		lua_pushlightuserdata(L,val);
		return;
	}
}

#endif
