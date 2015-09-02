#ifndef __LOGSTATUSSTRATEGY_H
#define __LOGSTATUSSTRATEGY_H

#include "loglevel.h"

namespace Zoic
{
	class LogStatusStrategy
	{
	public:
		LogStatusStrategy(unsigned char type);
		~LogStatusStrategy();

		unsigned char getLogType() const;
		void setLogType(unsigned char type);

		int getLogLevel() const;
		void setLogLevel(int logLevel);
	private:
		unsigned char m_logType;
		int m_logLevel;
	};

	inline unsigned char LogStatusStrategy::getLogType() const
	{
		return m_logType;
	}

	inline void LogStatusStrategy::setLogType(unsigned char type)
	{
		m_logType = type;
	}

	inline int LogStatusStrategy::getLogLevel() const
	{
		return m_logLevel;
	}

	inline void LogStatusStrategy::setLogLevel(int logLevel)
	{
		m_logLevel = logLevel;
	}
}

#endif
