#include "bufferappender.h"
#include "logcontent.h"
#include "logtargetdescription.h"
#include "loggerbufferwriter.h"
#include "include/SharedBufferWriter.h"

static const int MAX_LOGBUF_LEN = 1024*32;
static unsigned char LOG_BUFFER[MAX_LOGBUF_LEN]; 

using namespace Zoic;

BufferAppender::BufferAppender():
Appender(LAT_BUFFER),
m_fname(""),
m_absolutePath("")
{
	
}

BufferAppender::~BufferAppender()
{
	
}

void BufferAppender::init()
{
	Appender::init();
	m_fname = "";
}

bool BufferAppender::reopen()
{
	return true;
}

bool BufferAppender::close()
{
	return true;
}

bool BufferAppender::doAppend(LogContent* pContent)
{
	if (!g_LoggerBufferWriter->isSharedBufferRegister())
	{
		return false;
	}

	LogTargetDescription logTarget((BYTE*)LOG_BUFFER,MAX_LOGBUF_LEN);
	logTarget.resetSize();
	logTarget.setType(pContent->m_type);
	//pContent->formate();
	if (pContent->m_type == LCT_TEXT || pContent->m_type == LCT_BASE)
	{
		//std::string strOutput = pContent->getMessage();
		logTarget << m_fname << m_absolutePath << pContent->m_logLevel << pContent->m_logData.m_message;
	}
	else
	{
		logTarget << m_fname << m_absolutePath << pContent->m_logLevel << pContent->m_logData;
	}
	
	DWORD leftSize = logTarget.getSize();
	DWORD writeSize = 0;
	DWORD offset = 0;
	int code;
	bool isHead = true;
	do
	{
		code = g_LoggerBufferWriter->writeBuf(leftSize,LOG_BUFFER+offset,writeSize,isHead);
		if (code == -1)
		{
			//readÒÑ¾­ËÀÍö,Ð´Ê£ÓàµÄ
			g_LoggerBufferWriter->writeRemain();
			g_LoggerBufferWriter->setReadProcessExist(false);
			g_LoggerBufferWriter->resetLoc();
			return false;
		}
		else if (code == 0)
		{
			continue;
		}
		isHead = false;
		ASSERT(leftSize >= writeSize);
		leftSize -= writeSize;
		offset += writeSize;
	} while (leftSize > 0);
	return true;
}




