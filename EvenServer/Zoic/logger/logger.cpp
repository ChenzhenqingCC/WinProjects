#include "logger.h"
#include "appender.h"
#include "logcontent.h"
#include "bufferappender.h"
#include "fileappender.h"
#include "loggerstrategy.h"

using namespace Zoic;


const bool Logger::AppenderCmp::operator () (const Appender* a,const Appender* b) const
{
	return a->getType() < b->getType();
}

Logger::Logger(unsigned char loggerType):
m_strategy(NULL),
m_levelStrategy(NULL),
m_loggerType(loggerType)
{
	m_strategy = new LoggerStrategy(*this);
}

Logger::~Logger()
{
	destroy();
	if (m_strategy)
	{
		delete m_strategy;
	}
}

bool Logger::reopenAll()
{
	APPENDER_SET::iterator it = m_setAppender.begin();
	for (;it != m_setAppender.end();++it)
	{
		(*it)->reopen();
	}
	return true;
}

bool Logger::closeAll()
{
	APPENDER_SET::iterator it = m_setAppender.begin();
	for(;it != m_setAppender.end();++it)
	{
		(*it)->close();
	}
	return true;
}

bool Logger::doAppendAll(LogContent* pContent)
{
	APPENDER_SET::iterator it = m_setAppender.begin();
	for(;it != m_setAppender.end();++it)
	{
		Appender* pAppender = (*it);
		if (pAppender && pAppender->isActive())
		{
			(*it)->doAppend(pContent);
		}
	}
	return true;
}

bool Logger::addAppender(Appender* pAppender)
{
	APPENDER_SET::iterator it = m_setAppender.find(pAppender);
	if (it != m_setAppender.end())
	{
		return false;
	}
	m_setAppender.insert(pAppender);
	return true;
}

Appender* Logger::findAppender(unsigned char type)
{
	char tempBuf[sizeof(Appender)];
	Appender* pFind = reinterpret_cast<Appender*>(tempBuf);
	pFind->setType(type);
	APPENDER_SET::iterator it = m_setAppender.find(pFind);
	if (it != m_setAppender.end())
	{
		return (*it);
	}
	return NULL;
}

bool Logger::removeAppender(unsigned char type)
{
	char tempBuf[sizeof(Appender)];
	Appender* pFind = reinterpret_cast<Appender*>(tempBuf);
	pFind->setType(type);
	APPENDER_SET::iterator it = m_setAppender.find(pFind);
	if (it != m_setAppender.end())
	{
		m_setAppender.erase(it);
		return true;
	}
	return false;
}

void Logger::destroy()
{
	m_setAppender.clear();
}



