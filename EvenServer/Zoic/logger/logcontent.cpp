#include "logcontent.h"
#include "appender.h"
#include "logtargetdescription.h"
#include "include/Clock.h"

using namespace Zoic;

LogContent::LogContent():
m_logLevel(0),
m_type(LCT_BASE),
m_date(""),
m_time(""),
m_lastTimeLog(0)
{
	m_logData.m_message = "";
	m_logData.m_size = 0;
}

LogContent::~LogContent()
{
	
}

void LogContent::toText(const LogContent& content,TextLogContent& textContent)
{
	textContent.m_logData.m_message = content.m_logData.m_message;
	textContent.m_logLevel = content.m_logLevel;
}

void LogContent::toBinary(const LogContent& content,BinaryLogContent& binaryContent)
{
	binaryContent.m_logData.m_message.assign(content.m_logData.m_message.c_str(),content.m_logData.m_size);
	binaryContent.m_logData.m_size = content.m_logData.m_size;
	binaryContent.m_logLevel = content.m_logLevel;
}

void LogContent::formate()
{

}

const char* LogContent::getMessage() const
{
	return m_logData.m_message.c_str();
}

size_t LogContent::getLength() const
{
	return m_logData.m_size;
}

void LogContent::updateDateTime()
{
	TIME_SEC now=g_Clock->getTime();
	if(m_lastTimeLog!=now)
	{
		time_t timeValue = static_cast<time_t>( now );
		struct tm * tm_now=localtime(&timeValue);
		char dateBuf[20];
		char timeBuf[20];
		strftime( timeBuf, sizeof(timeBuf) ,"[%H:%M:%S]: ", tm_now );
		strftime( dateBuf, sizeof(dateBuf) ,"%Y-%m-%d", tm_now);
		m_date = dateBuf;
		m_time = timeBuf;
		m_lastTimeLog=now;
	}
}

TextLogContent::TextLogContent(const char* buf,unsigned long size)
{
	m_type = LCT_TEXT;
	m_logData.m_message.assign(buf,size);
	m_logData.m_size = size;
}

TextLogContent::~TextLogContent()
{

}

void TextLogContent::formate()
{
	updateDateTime();
	m_strOutput = getTime() + "[" + LogLevel::toString(m_logLevel) + "] " + m_logData.m_message;
}

const char* TextLogContent::getMessage() const
{
	return m_strOutput.c_str();
}

size_t TextLogContent::getLength() const
{
	return m_strOutput.length();
}

BinaryLogContent::BinaryLogContent(const char* buf,unsigned long size)
{
	m_type = LCT_BINARY;
	m_logData.m_message.assign(buf,size);
	m_logData.m_size = size;
}

BinaryLogContent::~BinaryLogContent()
{

}

void BinaryLogContent::formate()
{

}

const char* BinaryLogContent::getMessage() const
{
	return m_logData.m_message.c_str();
}

size_t BinaryLogContent::getLength() const
{
	return m_logData.m_size;
}

void LogData::outputTo(LogTargetDescription& logTarget) const
{
	logTarget << m_size;
	logTarget.writeData(m_message.data(),m_size);
}

void LogData::extractFrom(LogTargetDescription& logTarget)
{
	logTarget >> m_size;
	unsigned char buf[0x10000];
	logTarget.readData(buf,m_size);
	m_message.assign((const char*)buf,m_size);
}
