#ifndef __RANACOMMONSOLUTION_H
#define __RANACOMMONSOLUTION_H

#include "CommonTypes.h"
#include <vector>
#include <Zoic/include/Binary.h>
#include <Zoic/include/String.h>
#include <Zoic/include/FixArray.h>
#include <Zoic/include/VarArray.h>
#include <Zoic/include/UString.h>

#ifdef __RANAINPUTSTACK_H

//对CommonType.h定义的内容进行实现
template <class T>
void rana_push(RanaState L,const T_POS2D<T>& val)
{
	lua_newtable(L);

	rana_push(L,"x");
	rana_push(L,val.x);
	lua_settable(L,-3);

	rana_push(L,"y");
	rana_push(L,val.y);
	lua_settable(L,-3);
}

template <class T>
void rana_get(RanaState L,int index,T_POS2D<T> &val)
{
	rana_get(L,index,"x",val.x);
	rana_get(L,index,"y",val.y);
}

template <class T>
void rana_push(RanaState L,const T_GRADS<T>& val)
{
	lua_newtable(L);

	rana_push(L,"dx");
	rana_push(L,val.dx);
	lua_settable(L,-3);

	rana_push(L,"dy");
	rana_push(L,val.dy);
	lua_settable(L,-3);
}

template <class T>
void rana_get(RanaState L,int index,T_GRADS<T> &val)
{
	rana_get(L,index,"dx",val.dx);
	rana_get(L,index,"dy",val.dy);
}

template<typename HANDLE_T,typename SERIAL_T>
void rana_push(RanaState L,const T_SERIALHANDLE<HANDLE_T,SERIAL_T>& val)
{
	lua_newtable(L);

	rana_push(L,"handle");	
	rana_push(L,val.handle);
	lua_settable(L,-3);

	rana_push(L,"serial");
	rana_push(L,val.serial);
	lua_settable(L,-3);
}

template<typename HANDLE_T,typename SERIAL_T>
void rana_get(RanaState L,int index,T_SERIALHANDLE<HANDLE_T,SERIAL_T> &val)
{
	rana_get(L,index,"handle",val.handle);
	rana_get(L,index,"serial",val.serial);
}

template<class T,unsigned int LEN,class SIZE_TYPE>
void rana_push(RanaState L,const Zoic::VarArray<T,LEN,SIZE_TYPE> &val)
{
	lua_newtable(L);

	for(int i=0; i<val.size(); ++i)
	{
		const T &t = val[i];
		rana_push(L,t);
		lua_rawseti(L,-2,i+1);
	}
}

//对Zoic的基本类型进行定义
template <class T,unsigned int LEN,class SIZE_TYPE>
void rana_get(RanaState L,int index,Zoic::VarArray<T,LEN,SIZE_TYPE> &val)
{
	int i;
	val.clear();
	for (i = 1; i<=LEN ; i++)
	{
		lua_rawgeti(L, index, i);
		if (lua_isnil(L, -1))
		{
			lua_pop(L, 1);
			break;
		}
		try
		{
			RanaCheckType<T>::validateCheck(L,lua_gettop(L));
			T tmp;
			rana_get(L,lua_gettop(L),tmp);
			val.push_back(tmp);
		}catch(RanaInputArgumentException *e)
		{
			lua_pop(L,1);
			char buf[20];
			snprintf(buf,sizeof(buf),"[%d]",i);
			buf[sizeof(buf)-1]=0;
			e->appendPrefix(buf,true);
			throw e;
		}
		lua_pop(L, 1);
	}
#ifdef _DEBUG
	if(i > LEN)
	{
		lua_rawgeti(L, index, i);
		if (! lua_isnil(L, -1) )
		{
			char buf[512];
			sprintf(buf," VarArray larger than %d.",LEN);
			throw new RanaInputArgumentException(L, buf);
		}
		lua_pop(L, 1);
	}
#endif
}

template<class T>
void rana_get(RanaState L,int index,std::vector<T> &val)
{
	const int maxSize = 0x10000 - 1;
	int i;
	val.clear();
	for (i = 1; i<= maxSize ; i++)
	{
		lua_rawgeti(L, index, i);
		if (lua_isnil(L, -1))
		{
			lua_pop(L, 1);
			break;
		}
		try
		{
			RanaCheckType<T>::validateCheck(L,lua_gettop(L));
			T tmp;
			rana_get(L,lua_gettop(L),tmp);
			val.push_back(tmp);
		}catch(RanaInputArgumentException *e)
		{
			lua_pop(L,1);
			char buf[20];
			snprintf(buf,sizeof(buf),"[%d]",i);
			buf[sizeof(buf)-1]=0;
			e->appendPrefix(buf,true);
			throw e;
		}
		lua_pop(L, 1);
	}
#ifdef _DEBUG
	if(i > maxSize)
	{
		lua_rawgeti(L, index, i);
		if (! lua_isnil(L, -1) )
		{
			char buf[512];
			sprintf(buf," vector larger than %d.",maxSize);
			throw new RanaInputArgumentException(L, buf);
		}
		lua_pop(L, 1);
	}
#endif
}

template<class T,unsigned int LEN>
void rana_push(RanaState L,const Zoic::FixArray<T,LEN> &val)
{
	lua_newtable(L);

	for(int i=0; i<LEN; ++i)
	{
		const T &t = val[i];
		rana_push(L,t);
		lua_rawseti(L,-2,i+1);
	}
}

template<class T,unsigned int LEN>
void rana_get(RanaState L,int index,Zoic::FixArray<T,LEN> &val)
{
	int i;
	for (i = 0; i<LEN ; ++i)
	{
		lua_rawgeti(L, index, i+1);
		if (lua_isnil(L, -1))
		{
			lua_pop(L, 1);
			break;
		}
		try
		{
			RanaCheckType<T>::validateCheck(L,lua_gettop(L));
			rana_get(L,lua_gettop(L),val[i]);
		}catch(RanaInputArgumentException *e)
		{
			lua_pop(L,1);
			char buf[20];
			snprintf(buf,sizeof(buf),"[%d]",i+1);
			buf[sizeof(buf)-1]=0;
			e->appendPrefix(buf,true);
			throw e;
		}
		lua_pop(L, 1);
	}
#ifdef _DEBUG
	if(i >= LEN)
	{
		lua_rawgeti(L, index, i+1);
		if (! lua_isnil(L, -1) )
		{
			char buf[512];
			sprintf(buf," VarArray larger than %d.",LEN);
			throw new RanaInputArgumentException(L, buf);
		}
		lua_pop(L, 1);
	}
#endif
}

template <WORD LEN>
class RanaCheckType<Zoic::String<LEN> >
{
public:
	static void validateCheck(RanaState L,int index);
};
template <WORD LEN>
void RanaCheckType<Zoic::String<LEN> >::validateCheck(RanaState L,int index)
{
	RanaCheckType<std::string>::validateCheck(L,index);
}

template <WORD LEN>
void rana_get(RanaState L,int index,Zoic::String<LEN> &val)
{
	std::string str;
	rana_get(L,index,str);
	val=str.c_str();
}

template <WORD LEN>
class RanaCheckType<Zoic::Binary<LEN> >
{
public:
	static void validateCheck(RanaState L,int index);
};

template <WORD LEN>
void RanaCheckType<Zoic::Binary<LEN> >::validateCheck(RanaState L,int index)
{
	RanaCheckType<std::string>::validateCheck(L,index);
}

template <WORD LEN>
void rana_push(RanaState L,const Zoic::Binary<LEN> &val)
{
	lua_pushlstring(L, val.getData(),val.size());
}

template <WORD LEN>
void rana_get(RanaState L,int index,Zoic::Binary<LEN> &val)
{
	val.setData(lua_tostring(L,index),static_cast<typename Zoic::Binary<LEN>::size_type>(lua_strlen(L,index)));
}

template <WORD LEN>
class RanaCheckType<Zoic::UString<LEN> >
{
public:
	static void validateCheck(RanaState L,int index);
};
template <WORD LEN>
void RanaCheckType<Zoic::UString<LEN> >::validateCheck(RanaState L,int index)
{
	RanaCheckType<std::string>::validateCheck(L,index);
}

template <WORD LEN>
void rana_get(RanaState L,int index,Zoic::UString<LEN> &val)
{
	std::string str;
	rana_get(L,index,str);
	val=str.c_str();
}

#endif //end of "#ifdef __RANAINPUTSTACK_H"

#endif
