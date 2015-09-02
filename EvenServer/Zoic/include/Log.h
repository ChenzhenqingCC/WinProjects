/*! @file
	@ingroup buildin
	@brief 定义日志类
*/

#ifndef __ZOIC_LOG_H
#define __ZOIC_LOG_H

#include <stdarg.h>
#include "Zoic/logger/loglevel.h"
#include "Module.h"
#include "Types.h"


class ModuleInit;

namespace Zoic
{
	class Logger;
	class LogStatusStrategy;
	/*! @brief 日志类
		@ingroup buildin
	*/
	class Log
#ifndef ZOIC_NO_SERVICE
		:public Module<Log>
#else
		:public NoCopy
#endif
	{
#ifndef ZOIC_NO_SERVICE
		friend class Module<Log>;
		friend class ModuleInit;
#endif
	public :
		enum LOGGER_TYPE
		{
			TEXT_LOGGER,
			NETLOGTEXT_LOGGER,
			BINARY_LOGGER,
			RANA_LOGGER,
			COUNT_LOGGER
		};

		enum LOG_STATUS_TYPE
		{
			LST_LOG,
			LST_NETLOG,
			LST_RANA,
			LST_COUNT
		};

		//!记录日志到默认文件
		void log_f(
			LogLevelValue level,			//!< 日志等级	
			const char *msg,	//!< 格式控制
			...);


		//!记录日志到特定文件
		void flog_f(
			LogLevelValue level,			//!< 日志等级
			const char *prefix,	//!< 文件前缀
			const char *suffix,	//!< 文件后缀
			const char *msg,	//!< 格式控制
			...);

		void flognoput_f(
			LogLevelValue level,			//!< 日志等级
			const char *prefix,	//!< 文件前缀
			const char *suffix,	//!< 文件后缀
			const char *msg,	//!< 格式控制
			...);

		//!等同于同时执行log_f和flog_f
		void flogex_f(
			LogLevelValue level,			//!< 日志等级
			const char *prefix,	//!< 文件前缀
			const char *suffix,	//!< 文件后缀
			const char *msg,	//!< 格式控制
			...);

		const char* getPrefix() const;
		const char* getSuffix() const;
		//!设置默认日志文件前后缀
		void setLogFix(
			const char *prefix,			//!< 文件前缀
			const char *suffix=".log"	//!< 文件后缀
			);
		
		void setRanaFix(
			const char* prefix			//!< 文件名附加前缀
			);

		//! 记录日志
		void _flog(
			LogLevelValue level,			//!< 日志级别
			const char *prefix,	//!< 文件前缀
			const char *suffix,	//!< 文件后缀
			const char *msg,	//!< 格式控制
			va_list args,
			bool doput
			);

		//! 记录日志
		void _fplog(
			size_t fileCount,
			FILE ** files,
			const char *msg,	//!< 格式控制
			va_list args,
			bool doput
			);

		FILE* _fopen(
			const char *prefix,	//!< 文件前缀
			const char *suffix	//!< 文件后缀
			);

		void closeLog();
		void openLog();
		void doPut();
		void undoPut();
		bool isDoPut() const;

		std::string getCurrentProcessName(const std::string& path) const;

		int getLogLevel(BYTE logStatusType) const;
		void setLogLevel(BYTE logStatusType,LogLevelValue logLevel);

		//!将当前写入文件绝对路径重置为进程绝对路径
		void resetFileAbsolutePath();
		
		void setFileAbsolutePath(const std::string& path);
		const std::string& getFileAbsolutePath() const;
		
		//! 文本文件日志
		void _textlogger(
			LogLevelValue level,					//!< 日志级别
			bool doOutput,				//!< 是否打印
			const std::string& fname,	//!< 文件名称
			const std::string& msg		//!< 文件内容
			);

		//! NetLog的文本文件日志
		void _textnetlogger(
			LogLevelValue level,					//!< 日志级别
			const std::string& fname,	//!< 文件名称
			const std::string& msg		//!< 文件内容
			);

		//! 二进制文件日志
		void _binlogger(
			const std::string& fname,	//!< 文件名称
			BYTE* buf,					//!< 数据缓存
			DWORD size,					//!< 数据长度
			LogLevelValue logLevel		//!< 日志级别
			);

		//! 网络消息日志
		void _netlog(
			LogLevelValue level,
			const std::string& fname,	//!< 文件名称
			TIME_SEC time,				//!< 记录时间
			BYTE isRecv,				//!< 接受还是发送
			int len,					//!< 数据长度
			const BYTE* buf				//!< 数据缓存
			);

		//! rana日志
		void _ranalogger(
			LogLevelValue level,					//!< 日志级别
			bool doOutput,				//!< 是否打印
			const std::string& fname,	//!< 文件名称
			const std::string& msg		//!< 文件内容
			);

		void initLogPath();
		
		void logfatal_f(const char* msg,...);
		void logerror_f(const char* msg,...);
		void logwarning_f(const char* msg,...);
		void loginfo_f(const char* msg,...);
		void logdebug_f(const char* msg,...);
		void logtrace_f(const char* msg,...);
		void lograna_f(LogLevelValue level,bool doOutput,const char* msg,...);
		void flograna_f(LogLevelValue level,bool doOutput,const char* prefix,const char* suffix,const char* msg,...);

#ifndef ZOIC_NO_SERVICE
	private:
#else
	public:
#endif
		Log();
		~Log();

		//!更新时间
		void updateTime();
		//!获得运行程序所在绝对路径
		void  initProcessAbsolutePath();

		char *m_prefix;		//!< 日志前缀
		char *m_suffix;		//!< 日志后缀
		TIME_SEC	m_lasttime;		//!< 上一次调用flog与log的时间
		char 		m_timebuf[20];	//!< 时间字符串值
		char		m_datebuf[20];	//!< 日期字符串值
		bool		m_open;			//!< 全局log开关
		bool		m_doput;		//!< log是否打印开关

		std::string m_processAbsolutePath;		//!< 进程绝对路径
		std::string m_fileAbsolutePath;	//!< 当前写入文件绝对路径

		Logger*				m_loggerStorage[COUNT_LOGGER];
		LogStatusStrategy*	m_statusStrategyStorage[LST_COUNT];
	};

	extern const std::string LOGGER_TYPE_NAME[Log::COUNT_LOGGER];
	extern const std::string LOG_STATUS_TYPE_NAMES[Log::LST_COUNT];

#ifdef ZOIC_NO_SERVICE
	extern Log theLog;
#endif
}

namespace Zoic
{
	inline const char* Log::getPrefix() const
	{
		return m_prefix;
	}

	inline const char* Log::getSuffix() const
	{
		return m_suffix;
	}

	inline void Log::closeLog()
	{
		m_open  = false;
	}

	inline void Log::openLog()
	{
		m_open = true;
	}

	inline void Log::doPut()
	{
		m_doput = true;
	}

	inline void Log::undoPut()
	{
		m_doput = false;
	}

	inline bool Log::isDoPut() const
	{
		return m_doput;
	}
}

#ifndef ZOIC_NO_SERVICE

#define log_f Zoic::Log::getInstance()->log_f
#define flog_f Zoic::Log::getInstance()->flog_f
#define flogex_f Zoic::Log::getInstance()->flogex_f
#define logfatal_f Zoic::Log::getInstance()->logfatal_f
#define logerror_f Zoic::Log::getInstance()->logerror_f
#define logwarning_f Zoic::Log::getInstance()->logwarning_f
#define loginfo_f Zoic::Log::getInstance()->loginfo_f
#define logdebug_f Zoic::Log::getInstance()->logdebug_f
#define logtrace_f Zoic::Log::getInstance()->logtrace_f
#define lograna_f Zoic::Log::getInstance()->lograna_f
#define flograna_f Zoic::Log::getInstance()->flograna_f
#define flognoput_f Zoic::Log::getInstance()->flognoput_f
#define g_Log Zoic::Log::getInstance()

#ifdef _DEBUG
#define dlog_f           log_f
#define dlogwarning_f    logwarning_f
#else
#define dlog_f           dprintf
#define dlogwarning_f    dprintf
#endif

#else

#define log_f (Zoic::theLog.log_f)
#define flog_f (Zoic::theLog.flog_f)
#define flogex_f (Zoic::theLog.flogex_f)
#define logfatal_f (Zoic::theLog.logfatal_f)
#define logerror_f (Zoic::theLog.logerror_f)
#define logwarning_f (Zoic::theLog.logwaring_f)
#define loginfo_f (Zoic::theLog.loginfo_f)
#define logdebug_f (Zoic::theLog.logdebug_f)
#define logtrace_f (Zoic::theLog.logtrace_f)
#define lograna_f (Zoic::theLog.lograna_f)
#define flograna_f (Zoic::theLog.flograna_f)
#define flognoput_f (Zoic::theLog.flognoput_f)
#define g_Log (&Zoic::theLog)

#endif

#endif
