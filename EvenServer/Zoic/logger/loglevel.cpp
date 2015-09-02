#include "loglevel.h"

namespace Zoic
{
	const LogLevelValue OFF_LOG_LEVEL		=	6;
	const LogLevelValue FATAL_LOG_LEVEL		=	5;
	const LogLevelValue ERROR_LOG_LEVEL		=	4;
	const LogLevelValue WARN_LOG_LEVEL		=	3;
	const LogLevelValue INFO_LOG_LEVEL		=	2;
	const LogLevelValue DEBUG_LOG_LEVEL		=	1;
	const LogLevelValue TRACE_LOG_LEVEL		=	0;
	const LogLevelValue ALL_LOG_LEVEL		=	0;
	const LogLevelValue NOT_SET_LOG_LEVEL	=	-1;

	const char* OFF_LOG_LEVEL_NAME		= "OFF";
	const char* FATAL_LOG_LEVEL_NAME	= "FATAL";
	const char* ERROR_LOG_LEVEL_NAME	= "ERROR";
	const char* WARN_LOG_LEVEL_NAME		= "WARN";
	const char* INFO_LOG_LEVEL_NAME		= "INFO";
	const char* DEBUG_LOG_LEVEL_NAME	= "DEBUG";
	const char* TRACE_LOG_LEVEL_NAME	= "TRACE";
	const char* NOTSET_LOG_LEVEL_NAME	= "NOTSET";

	const char* LogLevelNames[] = {
		TRACE_LOG_LEVEL_NAME,
		DEBUG_LOG_LEVEL_NAME,
		INFO_LOG_LEVEL_NAME,
		WARN_LOG_LEVEL_NAME,
		ERROR_LOG_LEVEL_NAME,
		FATAL_LOG_LEVEL_NAME,
		OFF_LOG_LEVEL_NAME
	};
}

using namespace Zoic;

const char* LogLevel::toString(LogLevelValue level)
{
	if (level < TRACE_LOG_LEVEL || level > OFF_LOG_LEVEL)
	{
		return NOTSET_LOG_LEVEL_NAME;
	}
	return LogLevelNames[level]; 
}

LogLevelValue LogLevel::fromString(const std::string& str)
 {
	 for (int i=0;i<=OFF_LOG_LEVEL;++i)
	 {
		 const std::string cmpStr = LogLevelNames[i];
		 if (str == cmpStr)
		 {
			 return i;
		 }
	 }
	 return NOT_SET_LOG_LEVEL;
 }
