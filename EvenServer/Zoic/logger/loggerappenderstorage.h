#ifndef __LOGGERAPPENDERSTORAGE_H
#define __LOGGERAPPENDERSTORAGE_H

#include "Zoic/include/Module.h"
#include "appender.h"

namespace Zoic
{
	class LoggerAppenderStorage:
		public Zoic::Module<LoggerAppenderStorage>
	{
		friend class Zoic::Module<LoggerAppenderStorage>;
	public:
		Appender* findAppender(unsigned char type);
	private:
		LoggerAppenderStorage();
		~LoggerAppenderStorage();
	private:
		Appender* m_appenderStroage[LAT_COUNT];
	};
}

#define g_LoggerAppanderStorage LoggerAppenderStorage::getInstance()

#endif