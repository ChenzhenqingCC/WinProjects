#include "loggerstrategy.h"
#include "logger.h"
#include "logcontent.h"
#include "appender.h"
#include "bufferappender.h"
#include "fileappender.h"
#include "consoleappender.h"
#include "extendfileappender.h"
#include "loggerbufferwriter.h"
#include "loggerappenderstorage.h"
#include "logstatusstrategy.h"
#include "Zoic/include/Log.h"

using namespace Zoic;

const static char* PREFIX_ARRAY[] = 
{
	"Warning_",
	"Error_",
	"Fatal_"
};

LoggerStrategy::LoggerStrategy(Logger& logger):
m_isWriteBack(true),
m_theLogger(logger),
m_prefixFileName("")
{

}

LoggerStrategy::~LoggerStrategy()
{

}

void LoggerStrategy::setFileName(const std::string& fname,const std::string& absolutePath /* = */ )
{
	BufferAppender* pBufferAppender = static_cast<BufferAppender*>(m_theLogger.findAppender(LAT_BUFFER));
	if (pBufferAppender)
	{
		pBufferAppender->setFileName(fname,absolutePath);
	}
	FileAppender* pFileAppender = static_cast<FileAppender*>(m_theLogger.findAppender(LAT_FILE));
	if (pFileAppender)
	{
		pFileAppender->setFileName(fname,absolutePath);
		pFileAppender->setPrefix(m_prefixFileName);
	}
	ExtendFileAppender* pExtendAppender = static_cast<ExtendFileAppender*>(m_theLogger.findAppender(LAT_EXTENDFILE));
	if (pExtendAppender)
	{
		pExtendAppender->setFileName(fname,absolutePath);
	}
}

Appender* LoggerStrategy::addAppender(unsigned char type)
{
	Appender* pAppender = m_theLogger.findAppender(type);
	if (!pAppender)
	{
		pAppender = g_LoggerAppanderStorage->findAppender(type);
		if (!pAppender)
		{
			return NULL;
		}
		m_theLogger.addAppender(pAppender);
	}
	pAppender->setActive(true);
	return pAppender;
}

bool LoggerStrategy::removeAppender(unsigned char type)
{
	return m_theLogger.removeAppender(type);
}

void LoggerStrategy::collectAppender(LogContent* pContent)
{
	if (g_LoggerBufferWriter->isUseBuffer() && g_LoggerBufferWriter->isSharedBufferRegister() && g_LoggerBufferWriter->isExist() && !g_LoggerBufferWriter->isSelfLog())
	{
		addAppender(LAT_BUFFER);
		removeAppender(LAT_FILE);
		removeAppender(LAT_EXTENDFILE);
		g_LoggerBufferWriter->setReadProcessExist(true);
	}
	else
	{
		removeAppender(LAT_EXTENDFILE);
		removeAppender(LAT_BUFFER);
		if (pContent->m_type == LCT_TEXT && m_theLogger.getStatusStrategy()->getLogType() != Log::LST_NETLOG)
		{
			if (pContent->m_logLevel >= WARN_LOG_LEVEL && pContent->m_logLevel <= FATAL_LOG_LEVEL)
			{
				ExtendFileAppender* pExtendAppender = static_cast<ExtendFileAppender*>(addAppender(LAT_EXTENDFILE));
				pExtendAppender->setPrefix(PREFIX_ARRAY[pContent->m_logLevel-WARN_LOG_LEVEL]);
			}
		}
		addAppender(LAT_FILE);
		
		if (g_LoggerBufferWriter->isReadProcessExist())
		{
			g_LoggerBufferWriter->writeRemain();
			g_LoggerBufferWriter->setReadProcessExist(false);
			g_LoggerBufferWriter->resetLoc();
		}
	}
}



