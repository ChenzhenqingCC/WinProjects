/*! @file
	@ingroup buildin
	@brief ������־��
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
	/*! @brief ��־��
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

		//!��¼��־��Ĭ���ļ�
		void log_f(
			LogLevelValue level,			//!< ��־�ȼ�	
			const char *msg,	//!< ��ʽ����
			...);


		//!��¼��־���ض��ļ�
		void flog_f(
			LogLevelValue level,			//!< ��־�ȼ�
			const char *prefix,	//!< �ļ�ǰ׺
			const char *suffix,	//!< �ļ���׺
			const char *msg,	//!< ��ʽ����
			...);

		void flognoput_f(
			LogLevelValue level,			//!< ��־�ȼ�
			const char *prefix,	//!< �ļ�ǰ׺
			const char *suffix,	//!< �ļ���׺
			const char *msg,	//!< ��ʽ����
			...);

		//!��ͬ��ͬʱִ��log_f��flog_f
		void flogex_f(
			LogLevelValue level,			//!< ��־�ȼ�
			const char *prefix,	//!< �ļ�ǰ׺
			const char *suffix,	//!< �ļ���׺
			const char *msg,	//!< ��ʽ����
			...);

		const char* getPrefix() const;
		const char* getSuffix() const;
		//!����Ĭ����־�ļ�ǰ��׺
		void setLogFix(
			const char *prefix,			//!< �ļ�ǰ׺
			const char *suffix=".log"	//!< �ļ���׺
			);
		
		void setRanaFix(
			const char* prefix			//!< �ļ�������ǰ׺
			);

		//! ��¼��־
		void _flog(
			LogLevelValue level,			//!< ��־����
			const char *prefix,	//!< �ļ�ǰ׺
			const char *suffix,	//!< �ļ���׺
			const char *msg,	//!< ��ʽ����
			va_list args,
			bool doput
			);

		//! ��¼��־
		void _fplog(
			size_t fileCount,
			FILE ** files,
			const char *msg,	//!< ��ʽ����
			va_list args,
			bool doput
			);

		FILE* _fopen(
			const char *prefix,	//!< �ļ�ǰ׺
			const char *suffix	//!< �ļ���׺
			);

		void closeLog();
		void openLog();
		void doPut();
		void undoPut();
		bool isDoPut() const;

		std::string getCurrentProcessName(const std::string& path) const;

		int getLogLevel(BYTE logStatusType) const;
		void setLogLevel(BYTE logStatusType,LogLevelValue logLevel);

		//!����ǰд���ļ�����·������Ϊ���̾���·��
		void resetFileAbsolutePath();
		
		void setFileAbsolutePath(const std::string& path);
		const std::string& getFileAbsolutePath() const;
		
		//! �ı��ļ���־
		void _textlogger(
			LogLevelValue level,					//!< ��־����
			bool doOutput,				//!< �Ƿ��ӡ
			const std::string& fname,	//!< �ļ�����
			const std::string& msg		//!< �ļ�����
			);

		//! NetLog���ı��ļ���־
		void _textnetlogger(
			LogLevelValue level,					//!< ��־����
			const std::string& fname,	//!< �ļ�����
			const std::string& msg		//!< �ļ�����
			);

		//! �������ļ���־
		void _binlogger(
			const std::string& fname,	//!< �ļ�����
			BYTE* buf,					//!< ���ݻ���
			DWORD size,					//!< ���ݳ���
			LogLevelValue logLevel		//!< ��־����
			);

		//! ������Ϣ��־
		void _netlog(
			LogLevelValue level,
			const std::string& fname,	//!< �ļ�����
			TIME_SEC time,				//!< ��¼ʱ��
			BYTE isRecv,				//!< ���ܻ��Ƿ���
			int len,					//!< ���ݳ���
			const BYTE* buf				//!< ���ݻ���
			);

		//! rana��־
		void _ranalogger(
			LogLevelValue level,					//!< ��־����
			bool doOutput,				//!< �Ƿ��ӡ
			const std::string& fname,	//!< �ļ�����
			const std::string& msg		//!< �ļ�����
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

		//!����ʱ��
		void updateTime();
		//!������г������ھ���·��
		void  initProcessAbsolutePath();

		char *m_prefix;		//!< ��־ǰ׺
		char *m_suffix;		//!< ��־��׺
		TIME_SEC	m_lasttime;		//!< ��һ�ε���flog��log��ʱ��
		char 		m_timebuf[20];	//!< ʱ���ַ���ֵ
		char		m_datebuf[20];	//!< �����ַ���ֵ
		bool		m_open;			//!< ȫ��log����
		bool		m_doput;		//!< log�Ƿ��ӡ����

		std::string m_processAbsolutePath;		//!< ���̾���·��
		std::string m_fileAbsolutePath;	//!< ��ǰд���ļ�����·��

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
