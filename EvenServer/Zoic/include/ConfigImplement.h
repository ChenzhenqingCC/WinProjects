/*! @file
	@ingroup utils
	@brief 替换CONFIG_DECLARE,实现配置文件的自动读取
*/

#ifdef CONFIG_DEFINE_FUNC
#undef CONFIG_DEFINE_FUNC
#endif
#define CONFIG_DEFINE_FUNC(TYPE,NAME,FUNCNAME) \
	Zoic::_config_it=m_vars.find(#NAME); \
	if(Zoic::_config_it!=m_vars.end()) \
	{ \
		Zoic::ConfigItem<const char*> item(Zoic::_config_it->second.c_str()); \
		m_##NAME=item; \
		logtrace_f("conf:%s=%s",#NAME,Zoic::_config_it->second.c_str()); \
	} \
	else \
	{ \
		logerror_f("conf:not find %s=<%s>\n",#NAME,#TYPE); \
		return false; \
	}
#ifdef CONFIG_DEFINE
#undef CONFIG_DEFINE
#endif
#define CONFIG_DEFINE(TYPE,NAME) CONFIG_DEFINE_FUNC(TYPE,NAME,get##NAME)

/*! @file
	@ingroup utils
	@brief 替换CONFIG_DECLARE,实现配置文件的自动读取
*/

#ifdef CONFIG_DEFINE_EX
#undef CONFIG_DEFINE_EX
#endif
#define CONFIG_DEFINE_EX(TYPE,NAME,VALUE) \
	Zoic::_config_it=m_vars.find(#NAME); \
	if(Zoic::_config_it!=m_vars.end()) \
	{ \
		Zoic::ConfigItem<const char*> item(Zoic::_config_it->second.c_str()); \
		m_##NAME=item; \
		logtrace_f("conf:%s=%s",#NAME,Zoic::_config_it->second.c_str()); \
	} \
	else \
	{ \
		m_##NAME=Zoic::makeItem(VALUE); \
		loginfo_f("conf:%s=%s (<%s>defaulted)",#NAME,#VALUE,#TYPE); \
	}

#ifndef __CONFIGIMPLEMENT_H
#define __CONFIGIMPLEMENT_H

#include <Zoic/include/Log.h>

namespace Zoic
{
	static std::map<std::string,std::string>::iterator _config_it;
//! @cond 
	template <class T>
	class ConfigItem
	{
	public:
		ConfigItem(T t);
		template <class U>
		operator U (void) const;
		T m_t;
	};

	template <>
	class ConfigItem<const char*>
	{
	public:
		ConfigItem(const char *str);
		operator const char * (void) const;
		operator char (void) const;
		operator unsigned char (void) const;
		operator short (void) const;
		operator unsigned short (void) const;
		operator int (void) const;
		operator unsigned int (void) const;
		operator long (void) const;
		operator unsigned long (void) const;
		operator long long (void) const;
		operator unsigned long long (void) const;
		operator float (void) const;
		operator double (void) const;
		operator Zoic::DateTime (void) const;
		operator Zoic::Date (void) const;
		operator Zoic::Time (void) const;	
	private:
		long long getInteger() const;
		unsigned long long getIntegerU() const;
		double getDouble() const;
		const char * m_str;	
	};

	template <class T>
	inline ConfigItem<T>::ConfigItem(T t)
	{
		m_t=t;
	}

	template <class T>
	template <class U>
	inline ConfigItem<T>::operator U (void) const 
	{ 
		return static_cast<U>(m_t); 
	}

	inline ConfigItem<const char*>::ConfigItem(const char *str)
	{
		m_str=str;
	}

	inline ConfigItem<const char*>::operator const char * (void) const
	{
		return m_str;
	}

	inline ConfigItem<const char*>::operator char (void) const
	{
		return static_cast<char>(getInteger());
	}

	inline ConfigItem<const char*>::operator unsigned char (void) const
	{
		return static_cast<unsigned char>(getIntegerU());
	}

	inline ConfigItem<const char*>::operator short (void) const
	{
		return static_cast<short>(getInteger());
	}

	inline ConfigItem<const char*>::operator unsigned short (void) const
	{
		return static_cast<unsigned short>(getIntegerU());
	}

	inline ConfigItem<const char*>::operator int (void) const
	{
		return static_cast<int>(getInteger());
	}

	inline ConfigItem<const char*>::operator unsigned int (void) const
	{
		return static_cast<unsigned int>(getIntegerU());
	}

	inline ConfigItem<const char*>::operator long (void) const
	{
		return static_cast<long>(getInteger());
	}

	inline ConfigItem<const char*>::operator unsigned long (void) const
	{
		return static_cast<unsigned long>(getIntegerU());
	}

	inline ConfigItem<const char*>::operator long long (void) const
	{
		return getInteger();
	}

	inline ConfigItem<const char*>::operator unsigned long long (void) const
	{
		return getIntegerU();
	}

	inline ConfigItem<const char*>::operator float (void) const
	{
		return static_cast<float>(getDouble());
	}

	inline ConfigItem<const char*>::operator double (void) const
	{
		return getDouble();
	}

	inline long long ConfigItem<const char*>::getInteger() const
	{
		return strtoll(m_str,NULL,10);
	}

	inline unsigned long long ConfigItem<const char*>::getIntegerU() const
	{
		return strtoull(m_str,NULL,10);
	}

	inline double ConfigItem<const char*>::getDouble() const
	{
		return strtod(m_str,NULL);
	}

	inline ConfigItem<const char*>::operator Zoic::DateTime (void) const
	{
		int year=0,month=0,day=0;
		int hour=0,minute=0,second=0;
		sscanf(m_str,"%d%*c%d%*c%d%*c%d%*c%d%*c%d",&year,&month,&day,&hour,&minute,&second);
		return Zoic::DateTime(year,month,day,hour,minute,second);
	}

	inline ConfigItem<const char*>::operator Zoic::Date (void) const
	{
		int year=0,month=0,day=0;
		sscanf(m_str,"%d%*c%d%*c%d",&year,&month,&day);
		return Zoic::Date(year,month,day);
	}

	inline ConfigItem<const char*>::operator Zoic::Time (void) const
	{
		int hour=0,minute=0,second=0;
		sscanf(m_str,"%d%*c%d%*c%d",&hour,&minute,&second);
		return Zoic::Time(hour,minute,second);
	}

	template <class T>
	ConfigItem<T> makeItem(T t)
	{
		return ConfigItem<T>(t);
	}

}

//! @endcond 
#endif
