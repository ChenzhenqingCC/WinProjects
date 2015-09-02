#include "include/Log.h"
#include "include/Clock.h"
#include <stdarg.h>
#include "include/NetLog.h"

#include "logger/logger.h"
#include "logger/loggerstrategy.h"
#include "logger/consoleappender.h"
#include "logger/fileappender.h"
#include "logger/bufferappender.h"
#include "logger/logcontent.h"
#include "logger/logmonitorsender.h"
#include "logger/logstatusstrategy.h"
#include <extended/CompileTimeExtension.h>

#ifdef _MSC_VER
#include <process.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

using namespace Zoic;

#ifdef ZOIC_NO_SERVICE
	Log Zoic::theLog;
#endif


#undef log_f
#undef flog_f
#undef flogex_f
#undef logfatal_f
#undef logerror_f
#undef logwarning_f
#undef loginfo_f
#undef logdebug_f
#undef logtrace_f
#undef lograna_f
#undef flograna_f
#undef flognoput_f
static const int TIMEBUF_LENGTH=12;
static const int LOGBUF_MAXLEN=1024*1024;
static char LOGBUF[LOGBUF_MAXLEN+1] = { 0 };
static const int MAX_CALL_LEN = 1024;


#ifdef _DEBUG
const static bool s_doput = true;
#else
const static bool s_doput = false;
#endif

const std::string Zoic::LOGGER_TYPE_NAME[] = 
{
	"Text",
	"NetLogText",
	"Binary",
	"Rana"
};

const std::string Zoic::LOG_STATUS_TYPE_NAMES[] = 
{
	"Log",
	"NetLog",
	"Rana"
};


Log::Log():
	m_lasttime( 0 ),
	m_prefix(NULL),
	m_suffix(NULL),
	m_open(true),
	m_doput(true),
	m_processAbsolutePath(""),
	m_fileAbsolutePath("")
{
	setLogFix("_Undefined_Implement_",".log");

	for (int i=0;i<COUNT_LOGGER;++i)
	{
		m_loggerStorage[i] = new Logger(i);
	}

	for (int i=0;i<LST_COUNT;++i)
	{
		m_statusStrategyStorage[i] = new LogStatusStrategy(i);
	}

	m_loggerStorage[TEXT_LOGGER]->setStatusStrategy(m_statusStrategyStorage[LST_LOG]);
	m_loggerStorage[NETLOGTEXT_LOGGER]->setStatusStrategy(m_statusStrategyStorage[LST_NETLOG]);
	m_loggerStorage[BINARY_LOGGER]->setStatusStrategy(m_statusStrategyStorage[LST_NETLOG]);
	m_loggerStorage[RANA_LOGGER]->setStatusStrategy(m_statusStrategyStorage[LST_RANA]);

	COMPILETIME_ASSERT(sizeof(LOGGER_TYPE_NAME)==((COUNT_LOGGER) * sizeof(std::string)),__LoggerTypeNames__);
	COMPILETIME_ASSERT(sizeof(LOG_STATUS_TYPE_NAMES) == ((LST_COUNT) * sizeof(std::string)),__LogStatusTypeNames__);

	initProcessAbsolutePath();
	resetFileAbsolutePath();
}

Log::~Log()
{
	if(m_prefix)
	{
		delete[]m_prefix;
		m_prefix = NULL;
	}
	if(m_suffix)
	{
		delete[]m_suffix;
		m_suffix = NULL;
	}

	for (int i=0;i<COUNT_LOGGER;++i)
	{
		delete m_loggerStorage[i];
		m_loggerStorage[i] = NULL;
	}

	for (int i=0;i<LST_COUNT;++i)
	{
		delete m_statusStrategyStorage[i];
		m_statusStrategyStorage[i] = NULL;
	}
}

static inline void createFileName(char* fname,int size,const char *prefix,const char *suffix,const char *date)
{
	snprintf(fname,size,"%s%s%s",prefix,date,suffix);
	fname[strlen(prefix)+strlen(suffix)+strlen(date)] = '\0';
}

static inline FILE * open(const char *prefix,const char *suffix,const char *date)
{
	char fname[640];
	snprintf(fname,sizeof(fname),"%s%s%s",prefix,date,suffix);
	fname[sizeof(fname)-1]='\0';
	return fopen(fname, "a");
}

void Log::_flog(LogLevelValue level, const char *prefix, const char *suffix, const char *msg, va_list args,bool doput)
{
	if (getLogLevel(LST_LOG) > level)		//判断日志级别
	{
		return;
	}
	if (!m_open)
	{
		return;
	}
	if (!m_doput)
	{
		doput = false;
	}

	int len = vsnprintf(LOGBUF,LOGBUF_MAXLEN,msg,args);
	char* tail = LOGBUF + len;
	*tail++ = '\n';
	*tail = '\0';
	++len;

	updateTime();
	char fname[640];
	createFileName(fname,640,prefix,suffix,m_datebuf);
	_textlogger(level,doput,fname,LOGBUF);
}


void Log::_fplog(size_t fileCount, FILE ** files, const char *msg, va_list args,bool doput)
{
	updateTime();

	LOGBUF[TIMEBUF_LENGTH] = 0;
	strncpy(LOGBUF, m_timebuf, TIMEBUF_LENGTH);
	int len = vsnprintf(LOGBUF+TIMEBUF_LENGTH, LOGBUF_MAXLEN-TIMEBUF_LENGTH, msg, args);

	if(doput)
	{
		puts(LOGBUF);
	}

	if(!fileCount)
	{
		return;
	}
	char *tail = LOGBUF+TIMEBUF_LENGTH+len;
	*tail ++= '\n';
	*tail = 0;
	for(size_t i = 0; i< fileCount; ++i)
	{
		fputs(LOGBUF, files[i]);
	}
}


void Log::log_f(LogLevelValue level,const char *msg,...)
{
	va_list args;
	va_start(args, msg);
	_flog(level,m_prefix, m_suffix, msg, args, s_doput);
	va_end(args);
}

void Log::flog_f(LogLevelValue level,const char *prefix,const char *suffix,const char *msg,...)
{
	va_list args;
	va_start(args, msg);
	_flog(level,prefix, suffix, msg, args, s_doput);
	va_end(args);
}

void Log::flognoput_f(LogLevelValue level,const char *prefix,const char *suffix,const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	g_Log->_flog(level,prefix, suffix, msg, args, false);
	va_end(args);
}

void Log::flogex_f(LogLevelValue level,const char *prefix,const char *suffix,const char *msg,...)
{
	updateTime();
	char fname[640];
	va_list args;
	va_start(args,msg);
	
	int len = vsnprintf(LOGBUF,LOGBUF_MAXLEN,msg,args);
	char* tail = LOGBUF + len;
	*tail++ = '\n';
	*tail = '\0';
	++len;

	createFileName(fname,640,m_prefix,m_suffix,m_datebuf);
	_textlogger(level,false,fname,LOGBUF);
	
	memset(fname,0,640);
	createFileName(fname,640,prefix,suffix,m_datebuf);
	_textlogger(level,false,fname,LOGBUF);
}

void Log::setLogFix(const char *prefix,const char *suffix)
{
	if(m_prefix)
	{
		delete[]m_prefix;
		m_prefix = NULL;
	}
	if(m_suffix)
	{
		delete[]m_suffix;
		m_suffix = NULL;
	}

	int len = strlen(prefix) + 1;
	m_prefix = new char[len];
	sprintf(m_prefix, "%s", prefix);

	len = strlen(suffix) + 1;
	m_suffix = new char[len];
	sprintf(m_suffix, "%s", suffix);

	//m_prefix=prefix;
	//m_suffix=suffix;
}

void Log::setRanaFix( const char* prefix )
{
	m_loggerStorage[RANA_LOGGER]->getLoggerStrategy()->setPrefix(prefix);
}

void Log::updateTime()
{
	TIME_SEC now=g_Clock->getTime();
	if(m_lasttime!=now)
	{
		time_t timeValue = static_cast<time_t>( now );
		struct tm * tm_now=localtime(&timeValue);
		strftime( m_timebuf, sizeof(m_timebuf) ,"[%H:%M:%S]: ", tm_now );
		strftime( m_datebuf, sizeof(m_datebuf) ,"%Y-%m-%d", tm_now);
		m_lasttime=now;
	}
}

void Log::_textlogger(LogLevelValue level,bool doOutput,const std::string& fname,const std::string& msg)
{
	if (getLogLevel(LST_LOG) > level)		//判断日志级别
	{
		return;
	}
	if (doOutput)
	{
		m_loggerStorage[TEXT_LOGGER]->getLoggerStrategy()->addAppender(LAT_CONSOLE);
	}
	else
	{
		m_loggerStorage[TEXT_LOGGER]->getLoggerStrategy()->removeAppender(LAT_CONSOLE);
	}
	TextLogContent content(msg.c_str(),msg.length());
	content.m_logLevel = level;
	m_loggerStorage[TEXT_LOGGER]->getLoggerStrategy()->collectAppender(&content);
	m_loggerStorage[TEXT_LOGGER]->getLoggerStrategy()->setFileName(fname,m_fileAbsolutePath);
	m_loggerStorage[TEXT_LOGGER]->doAppendAll(&content);
	if (doOutput && m_loggerStorage[TEXT_LOGGER]->getStatusStrategy()->getLogLevel() <= content.m_logLevel)
	{
		g_LogMonitorSender->sendToMonitor(&content);
	}
}

void Log::_textnetlogger( LogLevelValue level, const std::string& fname,const std::string& msg )
{
	if (getLogLevel(LST_NETLOG) > level)
	{
		return;
	}
	m_loggerStorage[NETLOGTEXT_LOGGER]->getLoggerStrategy()->removeAppender(LAT_CONSOLE);
	TextLogContent content(msg.c_str(),msg.length());
	content.m_logLevel = level;
	m_loggerStorage[NETLOGTEXT_LOGGER]->getLoggerStrategy()->collectAppender(&content);
	m_loggerStorage[NETLOGTEXT_LOGGER]->getLoggerStrategy()->setFileName(fname,m_fileAbsolutePath);
	m_loggerStorage[NETLOGTEXT_LOGGER]->doAppendAll(&content);
}

void Log::_ranalogger(LogLevelValue level,bool doOutput,const std::string& fname,const std::string& msg)
{
	if (getLogLevel(LST_RANA) > level)		//判断日志级别
	{
		return;
	}
	if (doOutput)
	{
		m_loggerStorage[RANA_LOGGER]->getLoggerStrategy()->addAppender(LAT_CONSOLE);
	}
	else
	{
		m_loggerStorage[RANA_LOGGER]->getLoggerStrategy()->removeAppender(LAT_CONSOLE);
	}
	TextLogContent content(msg.c_str(),msg.length());
	content.m_logLevel = level;
	m_loggerStorage[RANA_LOGGER]->getLoggerStrategy()->collectAppender(&content);
	m_loggerStorage[RANA_LOGGER]->getLoggerStrategy()->setFileName(fname,m_fileAbsolutePath);
	m_loggerStorage[RANA_LOGGER]->doAppendAll(&content);
	if (doOutput && m_loggerStorage[RANA_LOGGER]->getStatusStrategy()->getLogLevel() <= content.m_logLevel)
	{
		g_LogMonitorSender->sendToMonitor(&content);
	}
}

void Log::_binlogger( const std::string& fname,BYTE* buf, DWORD size, LogLevelValue logLevel )
{
	if (getLogLevel(LST_NETLOG) > logLevel)		//判断日志级别
	{
		return;
	}
	m_loggerStorage[BINARY_LOGGER]->getLoggerStrategy()->removeAppender(LAT_CONSOLE);
	BinaryLogContent content((const char*)buf,size);
	content.m_logLevel = logLevel;
	m_loggerStorage[BINARY_LOGGER]->getLoggerStrategy()->collectAppender(&content);
	m_loggerStorage[BINARY_LOGGER]->getLoggerStrategy()->setFileName(fname,m_fileAbsolutePath);
	m_loggerStorage[BINARY_LOGGER]->doAppendAll(&content);
}

void Log::_netlog(LogLevelValue level,const std::string& fname,TIME_SEC time,BYTE isRecv,int len,const BYTE* buf)
{
	if (getLogLevel(LST_NETLOG) > level )		//判断日志级别,现在bin文件日志级别暂时设为DEBUG_LOG_LEVEL
	{
		return;
	}
	BYTE* dst = (BYTE*)LOGBUF;
	*(TIME_SEC *)dst=time;
	dst+=sizeof(TIME_SEC);
	*(BYTE *)dst=isRecv;
	dst+=sizeof(BYTE);
	*(int *)dst=len;
	dst+=sizeof(int);
	
	memcpy(dst,buf,len);
	_binlogger(fname,(BYTE*)LOGBUF,len+sizeof(TIME_SEC)+sizeof(BYTE)+sizeof(int),level);
}

void Log::loginfo_f(const char* msg,...)
{
	va_list args;
	va_start(args, msg);
	_flog(INFO_LOG_LEVEL,m_prefix, m_suffix, msg, args, s_doput);
	va_end(args);
}

void Log::logdebug_f(const char* msg,...)
{
	va_list args;
	va_start(args, msg);
	_flog(DEBUG_LOG_LEVEL,m_prefix, m_suffix, msg, args, s_doput);
	va_end(args);
}

void Log::logtrace_f(const char* msg,...)
{
	va_list args;
	va_start(args, msg);
	_flog(TRACE_LOG_LEVEL,m_prefix, m_suffix, msg, args, s_doput);
	va_end(args);
}

void Log::lograna_f(LogLevelValue level,bool doOutput,const char* msg,...)
{
	if (getLogLevel(LST_RANA) > level)
	{
		return;
	}
	va_list args;
	va_start(args,msg);

	int len = vsnprintf(LOGBUF,LOGBUF_MAXLEN,msg,args);
	char* tail = LOGBUF + len;
	*tail++ = '\n';
	*tail = '\0';
	++len;

	char fname[640];
	updateTime();
	createFileName(fname,640,m_prefix,m_suffix,m_datebuf);
	_ranalogger(level,doOutput&&s_doput&&m_doput,fname,LOGBUF);
	va_end(args);
}

void Log::flograna_f(LogLevelValue level,bool doOutput,const char* prefix,const char* suffix,const char* msg,...)
{
	if (getLogLevel(LST_RANA) > level)
	{
		return;
	}
	va_list args;
	va_start(args,msg);

	int len = vsnprintf(LOGBUF,LOGBUF_MAXLEN,msg,args);
	char* tail = LOGBUF + len;
	*tail++ = '\n';
	*tail = '\0';
	++len;

	char fname[640];
	updateTime();
	createFileName(fname,640,prefix,suffix,m_datebuf);
	_ranalogger(level,doOutput&&s_doput&&m_doput,fname,LOGBUF);
	va_end(args);
}

void Log::logwarning_f(const char* msg,...)
{
	va_list args;
	va_start(args, msg);
	_flog(WARN_LOG_LEVEL,m_prefix, m_suffix, msg, args, s_doput);
	va_end(args);
}

void Log::logerror_f(const char* msg,...)
{
	va_list args;
	va_start(args, msg);
	_flog(ERROR_LOG_LEVEL,m_prefix, m_suffix, msg, args, s_doput);
	va_end(args);
}

void Log::logfatal_f(const char* msg,...)
{
	va_list args;
	va_start(args, msg);
	_flog(FATAL_LOG_LEVEL,m_prefix, m_suffix, msg, args, s_doput);
	va_end(args);
}

int Log::getLogLevel(BYTE logStatusType) const
{
	if (logStatusType >= LST_COUNT)
	{
		return NOT_SET_LOG_LEVEL;
	}
	return m_statusStrategyStorage[logStatusType]->getLogLevel();
}

void Log::setLogLevel(BYTE logStatusType,LogLevelValue logLevel)
{
	if (logStatusType >= LST_COUNT)
	{
		return;
	}
	if (logLevel > OFF_LOG_LEVEL)
	{
		logLevel = OFF_LOG_LEVEL;
	}
	else if (logLevel < ALL_LOG_LEVEL)
	{
		logLevel = ALL_LOG_LEVEL;
	}
	m_statusStrategyStorage[logStatusType]->setLogLevel(logLevel);
}

std::string Log::getCurrentProcessName(const std::string& path) const
{
	std::string name = "Server";
	int len = path.length();
	for (size_t i=len-1;i>=0;--i)
	{
		if (path[i] == '/')
		{
			name = path.substr(i+1,len);
			break;
		}
	}
	return name;
}

void Log::initLogPath()
{
	std::string arg_0 = g_Service->getArgv()[0];
	std::string processName = getCurrentProcessName(arg_0);
	std::string strName = processName + "_";

	setLogFix(strName.c_str());
	strName = "NetLog/" + processName;
	Zoic::NetLog::getInstance()->config(strName.c_str(),"net",".log");
}

void Log::setFileAbsolutePath(const std::string& path)
{
	m_fileAbsolutePath = path;
}

const std::string& Log::getFileAbsolutePath() const
{
	return m_fileAbsolutePath;
}

void Log::resetFileAbsolutePath()
{
	m_fileAbsolutePath = m_processAbsolutePath;
}

#ifdef _MSC_VER

void Log::initProcessAbsolutePath()
{
	m_processAbsolutePath = "";
}

#else

void Log::initProcessAbsolutePath()
{
	char line[MAX_CALL_LEN] = {};
	std::string call = "pwd";
	FILE* fp = popen(call.c_str(), "r");
	if (!fp)
	{
		return;
	}
	if (!fgets(line, sizeof(line)/sizeof(*line), fp))
	{
		return;
	}
	pclose(fp);
	int len = strlen(line);
	if (len > 0)
	{
		line[len-1] = '\0';
	}
	m_processAbsolutePath = line;
}

#endif
