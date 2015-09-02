#ifndef __APPENDER_H
#define __APPENDER_H

#include "loglevel.h"

namespace Zoic
{
	class LogContent;
	class BufferContent;
	class BufferAppender;
	class LogTargetDescription;
	
	enum LogAppenderType
	{
		LAT_CONSOLE,
		LAT_FILE,
		LAT_BUFFER,
		LAT_EXTENDFILE,
		LAT_COUNT
	};

	class Appender
	{
	public:
		Appender(unsigned char type);
		virtual ~Appender();

		unsigned char getType() const;
		void setType(unsigned char type);

		bool isActive() const;
		void setActive(bool b);

		virtual void init();

		virtual bool reopen() = 0;
		virtual bool close() = 0;
		virtual bool doAppend(LogContent* pContent) = 0;

	private:
		unsigned char m_type;
		bool m_isActive;
	};

	inline unsigned char Appender::getType() const
	{
		return m_type;
	}

	inline void Appender::setType(unsigned char type)
	{
		m_type = type;
	}

	inline bool Appender::isActive() const
	{
		return m_isActive;
	}

	inline void Appender::setActive(bool b)
	{
		m_isActive = b;
	}

}

#endif