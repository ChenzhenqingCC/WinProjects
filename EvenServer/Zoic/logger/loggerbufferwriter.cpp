#include "loggerbufferwriter.h"
#include "include/SharedBufferWriter.h"
#include "logcontent.h"
#include "logtargetdescription.h"
#include "fileappender.h"


using namespace Zoic;

LoggerBufferWriter::LoggerBufferWriter():
m_isSelfLog(true),
m_isUseBuffer(true),
m_isReadProcessExist(false),
m_writer(*(new SharedBufferWriter))
{

}

LoggerBufferWriter::~LoggerBufferWriter()
{
	delete &m_writer;
}

bool LoggerBufferWriter::isSharedBufferRegister() const
{
	return m_writer.isRegister();
}

bool LoggerBufferWriter::openSharedBuffer(const std::string& path,int code,unsigned long blocksize,unsigned char semcnt)
{
	if (m_writer.isRegister())
	{
		return true;
	}

	bool result = m_writer.doRegister(path,code,blocksize,semcnt);
	if (result)
	{
		m_writer.setSemFlag(false,false,true,true);
		m_writer.setWaitFlag(true);
		return true;
	}
	return false;
}

short LoggerBufferWriter::getShmAttachCount()
{
	return m_writer.getShmAttachCount();
}

int LoggerBufferWriter::getErrorCode() const
{
	return m_writer.getErrorCode();
}

void LoggerBufferWriter::resetLoc()
{
	m_writer.resetLoc();
}

bool LoggerBufferWriter::isExist()
{
	return m_writer.isExist();
}

int LoggerBufferWriter::writeBuf(unsigned long size,unsigned char* buf,unsigned long& writeSize,bool isHead)
{
	return m_writer.writeBuf(size,buf,writeSize,isHead);
}

int LoggerBufferWriter::start()
{
	return 0;
}

int LoggerBufferWriter::stop()
{
	return 0;
}

bool LoggerBufferWriter::writeRemain()
{
	BYTE buf[8192];
	DWORD readSize = 0;

	m_writer.lockAll();		//共享内存区所有分块全部锁定
	bool lockResult = (m_writer.getErrorCode())?(false):(true);
	if (!lockResult)
	{
		return false;
	}
	while(m_writer.readRemain(readSize,buf))
	{
		LogTargetDescription logTarget(buf,readSize);
		std::string fname = "";
		std::string absolutePath = "";

		int logLevel = 0;
		FILE* fp = NULL;
		if (logTarget.getType() == LCT_BINARY)
		{
			LogData data;
			data.m_size = 0;
			logTarget >> fname >> absolutePath >> logLevel >> data;
			if (data.m_size > 0)
			{
				fp = FileAppender::openFile(fname,absolutePath,"ab+");
				if (!fp)
				{
					continue;
				}
				BinaryLogContent content((const char*)buf,data.m_size);
				content.formate();
				fwrite(content.getMessage(),1,content.getLength(),fp);
				fclose(fp);
			}
		}
		else
		{
			std::string strOutput = "";
			logTarget >> fname >> absolutePath >> logLevel >> strOutput;
			if (strOutput.length())
			{
				fp = FileAppender::openFile(fname,absolutePath,"a");
				if (!fp)
				{
					continue;
				}
				TextLogContent content(strOutput.c_str(),strOutput.length());
				content.formate();
				fwrite(content.getMessage(),1,content.getLength(),fp);
				fclose(fp);
			}
		}
	}
	m_writer.unlockAll();
	return true;
}

unsigned long long LoggerBufferWriter::getWaitTimes() const
{
	return m_writer.getWaitTimes();
}

unsigned long long LoggerBufferWriter::getTotalLoopTimes() const
{
	return m_writer.getTotalLoopTimes();
}

void LoggerBufferWriter::resetWaitTimes()
{
	m_writer.resetWaitTimes();
}

void LoggerBufferWriter::resetTotalLoopTimes()
{
	m_writer.resetTotalLoopTimes();
}

size_t LoggerBufferWriter::getWriteBufSize() const
{
	return m_writer.getWirteBufSize();
}