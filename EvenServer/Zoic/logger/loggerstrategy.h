#ifndef __LOGGERSTRATEGY_H
#define __LOGGERSTRATEGY_H

#include <string>

namespace Zoic
{
	class Logger;
	class LogContent;
	class Appender;

	class LoggerStrategy
	{
	public:
		LoggerStrategy(Logger& logger);
		virtual ~LoggerStrategy();

		virtual void collectAppender(LogContent* pContent);

		void setFileName(const std::string& fname,const std::string& absolutePath);

		Appender* addAppender(unsigned char type);
		bool removeAppender(unsigned char type);

		bool isWriteBack() const;
		void setWriteBack(bool b);

		const std::string& getPrefix() const;
		void setPrefix(const std::string& strPrefix);

	private:
		Logger& m_theLogger;
		bool m_isWriteBack;
		std::string m_prefixFileName;
		std::string m_absolutePath;
	};

	inline bool LoggerStrategy::isWriteBack() const
	{
		return m_isWriteBack;
	}

	inline void LoggerStrategy::setWriteBack(bool b)
	{
		m_isWriteBack = b;
	}

	inline const std::string& LoggerStrategy::getPrefix() const
	{
		return m_prefixFileName;
	}

	inline void LoggerStrategy::setPrefix(const std::string& strPrefix)
	{
		m_prefixFileName = strPrefix;
	}
}



#endif