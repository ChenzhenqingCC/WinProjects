#ifndef __LOGGER_H
#define __LOGGER_H

#include <set>
#include <string>


namespace Zoic
{
	class Appender;
	class LogContent;
	class BufferContent;
	class LoggerStrategy;
	class LogStatusStrategy;

	class Logger
	{
	public:
		Logger(unsigned char loggerType);
		virtual ~Logger();

		bool isLogWithBuffer() const;
		void setLogWithBuffer(bool b);

		bool isOpenSession() const;
		void setOpenSession(bool b);

		bool addAppender(Appender* pAppender);
		Appender* findAppender(unsigned char type);
		bool removeAppender(unsigned char type);

		void setLoggerStrategy(LoggerStrategy* pStrategy);
		LoggerStrategy* getLoggerStrategy();

		LogStatusStrategy* getStatusStrategy();
		void setStatusStrategy(LogStatusStrategy* pStatusStrategy);

		unsigned char getLoggerType() const;

		void destroy();

		virtual bool reopenAll();
		virtual bool closeAll();
		virtual bool doAppendAll(LogContent* pContent);
	private:
		class AppenderCmp
		{
		public:
			const bool operator()(const Appender* a,const Appender* b) const;
		};
		typedef std::set<Appender*,AppenderCmp> APPENDER_SET;
		
	private:
		APPENDER_SET m_setAppender;
		LoggerStrategy* m_strategy;
		LogStatusStrategy* m_levelStrategy;
		unsigned char m_loggerType;
	};

	extern const std::string LOGGER_TYPE_NAME[];

	inline void Logger::setLoggerStrategy(LoggerStrategy* pStrategy)
	{
		m_strategy = pStrategy;
	}

	inline LoggerStrategy* Logger::getLoggerStrategy()
	{
		return m_strategy;
	}

	inline LogStatusStrategy* Logger::getStatusStrategy()
	{
		return m_levelStrategy;
	}

	inline void Logger::setStatusStrategy(LogStatusStrategy* pStatusStrategy)
	{
		m_levelStrategy = pStatusStrategy;
	}

	inline unsigned char Logger::getLoggerType() const
	{
		return m_loggerType;
	}
}


#endif