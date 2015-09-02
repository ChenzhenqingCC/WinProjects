#ifndef __LOGLEVEL_H
#define __LOGLEVEL_H

#include <string>

namespace Zoic
{
	typedef int LogLevelValue;

	extern const LogLevelValue OFF_LOG_LEVEL;
	extern const LogLevelValue FATAL_LOG_LEVEL;
	extern const LogLevelValue ERROR_LOG_LEVEL;
	extern const LogLevelValue WARN_LOG_LEVEL;
	extern const LogLevelValue INFO_LOG_LEVEL;
	extern const LogLevelValue DEBUG_LOG_LEVEL;
	extern const LogLevelValue TRACE_LOG_LEVEL;
	extern const LogLevelValue ALL_LOG_LEVEL;
	extern const LogLevelValue NOT_SET_LOG_LEVEL;

	extern const char* OFF_LOG_LEVEL_NAME;
	extern const char* FATAL_LOG_LEVEL_NAME;
	extern const char* ERROR_LOG_LEVEL_NAME;
	extern const char* WARN_LOG_LEVEL_NAME;
	extern const char* INFO_LOG_LEVEL_NAME;
	extern const char* DEBUG_LOG_LEVEL_NAME;
	extern const char* TRACE_LOG_LEVEL_NAME;
	extern const char* NOTSET_LOG_LEVEL_NAME;

	class LogLevel
	{
	public:	
		static const char* toString(LogLevelValue level);
		static LogLevelValue fromString(const std::string& str);
	};
}

#endif