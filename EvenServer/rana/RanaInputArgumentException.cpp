#include "RanaInputArgumentException.h"

RanaInputArgumentException::RanaInputArgumentException(RanaState L,const char *str)
	:m_str( str )
	,m_level(0)
	,m_array(false)
{
	m_str += create_tail_call_info(L);
}

RanaInputArgumentException::~RanaInputArgumentException() throw()
{
}

std::string RanaInputArgumentException::create_tail_call_info(RanaState L)
{
	lua_Debug ar;
	ar.currentline = -1000;
	if (lua_getstack(L, 1, &ar)) {  /* check function at level */
		char head[2048];
		lua_getinfo(L, "Sl", &ar);  /* get info about it */
		if (ar.currentline > 0) {
			sprintf(head,"(line %d of %s)", ar.currentline,ar.short_src);
			return head;
		}
	}
	return "";
}

const char *RanaInputArgumentException::what() const throw()
{
	return m_str.c_str();
}

void RanaInputArgumentException::appendPrefix(const char *name,bool array)
{
	if(m_level&&!m_array)
	{
		m_str=name+("."+m_str);
	}
	else
	{
		m_str=name+m_str;
	}
	m_array=array;
	m_level++;
}
