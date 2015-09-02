#include "loggerappenderstorage.h"
#include "consoleappender.h"
#include "bufferappender.h"
#include "fileappender.h"
#include "extendfileappender.h"
#include <extended/CompileTimeExtension.h>

using namespace Zoic;

LoggerAppenderStorage::LoggerAppenderStorage()
{
	for (int i=0;i<LAT_COUNT;++i)
	{
		m_appenderStroage[i] = NULL;
	}
	Appender* tempStorage[] = {
		new ConsoleAppender,
		new FileAppender,
		new BufferAppender,
		new ExtendFileAppender
	};
	COMPILETIME_ASSERT(sizeof(tempStorage)==sizeof(m_appenderStroage),__LoggerAppenderStorage__ );
	memcpy( m_appenderStroage,tempStorage,sizeof(m_appenderStroage) );
}

LoggerAppenderStorage::~LoggerAppenderStorage()
{
	for (int i=0;i<LAT_COUNT;++i)
	{
		Appender* pAppender = m_appenderStroage[i];
		if (pAppender)
		{
			delete pAppender;
			m_appenderStroage[i] = NULL;
		}
	}
}

Appender* LoggerAppenderStorage::findAppender(unsigned char type)
{
	if (type >= LAT_COUNT)
	{
		return NULL;
	}
	m_appenderStroage[type]->init();
	return m_appenderStroage[type];
}

