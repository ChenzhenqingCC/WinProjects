#ifndef __LOGCONTENT_H
#define __LOGCONTENT_H

#include <string>
#include <vector>
#include "logtargetdescription.h"

namespace Zoic
{
	enum LogContentType
	{
		LCT_BASE,
		LCT_TEXT,
		LCT_BINARY
	};

	struct LogData
	{
		void outputTo(LogTargetDescription& logTarget) const;
		void extractFrom(LogTargetDescription& logTarget);
		std::string m_message;
		unsigned long m_size;
	};

	class TextLogContent;
	class BinaryLogContent;

	class LogContent
	{
	public:
		LogContent();
		virtual ~LogContent();
	
	public:
		LogData		m_logData;
		int m_logLevel;
		unsigned char m_type;
		static void toText(const LogContent& content,TextLogContent& textContent);
		static void toBinary(const LogContent& content,BinaryLogContent& binaryContent);
		
		virtual void formate();
		virtual const char* getMessage() const;
		virtual size_t getLength() const;

		const std::string& getDate() const;
		const std::string& getTime() const;
		void updateDateTime();
	private:
		std::string m_date;
		std::string m_time;
		long m_lastTimeLog;
	};

	class TextLogContent:
		public LogContent
	{
	public:
		TextLogContent(const char* buf,unsigned long size);
		virtual ~TextLogContent();

		virtual void formate();
		virtual const char* getMessage() const;
		virtual size_t getLength() const;
	private:
		std::string m_strOutput;
	};

	class BinaryLogContent:
		public LogContent
	{
	public:
		BinaryLogContent(const char* buf,unsigned long size);
		virtual ~BinaryLogContent();

		virtual void formate();
		virtual const char* getMessage() const;
		virtual size_t getLength() const;
	};

	inline const std::string& LogContent::getDate() const
	{
		return m_date;
	}

	inline const std::string& LogContent::getTime() const
	{
		return m_time;
	}

	inline LogTargetDescription& operator << (LogTargetDescription& logTarget,const LogData& data)
	{
		data.outputTo(logTarget);
		return logTarget;
	}

	inline LogTargetDescription& operator >> (LogTargetDescription& logTarget,LogData& data)
	{
		data.extractFrom(logTarget);
		return logTarget;
	}
}

#endif