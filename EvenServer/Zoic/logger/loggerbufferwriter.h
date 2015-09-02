#ifndef __LOGGERBUFFERWRITER_H
#define __LOGGERBUFFERWRITER_H

#include <string>
#include "Zoic/include/Module.h"

namespace Zoic
{
	class SharedBufferWriter;

	class LoggerBufferWriter:
		public Zoic::Module<LoggerBufferWriter>
	{
		friend class Zoic::Module<LoggerBufferWriter>;
	public:
		LoggerBufferWriter();
		~LoggerBufferWriter();
		bool isSharedBufferRegister() const;
		bool openSharedBuffer(const std::string& path,int code,unsigned long blocksize,unsigned char semcnt);
		int writeBuf(unsigned long size,unsigned char* buf,unsigned long& writeSize,bool isHead);
		short getShmAttachCount();
		int getErrorCode() const;
		bool isExist();
		void resetLoc();
		bool writeRemain();

		int start();
		int stop();

		bool isSelfLog() const;
		void setSelfLog(bool b);
		bool isUseBuffer() const;
		void setUseBuffer(bool b);
		bool isReadProcessExist() const;
		void setReadProcessExist(bool b);
		bool isWritingRemain() const;

		unsigned long long getWaitTimes() const;
		unsigned long long getTotalLoopTimes() const;
		void resetWaitTimes();
		void resetTotalLoopTimes();
		size_t getWriteBufSize() const;
	private:
		bool		m_isSelfLog;
		bool		m_isUseBuffer;
		bool		m_isReadProcessExist;
		SharedBufferWriter& m_writer;
	};

	inline bool LoggerBufferWriter::isSelfLog() const
	{
		return m_isSelfLog;
	}

	inline void LoggerBufferWriter::setSelfLog(bool b)
	{
		m_isSelfLog = b;
	}

	inline bool LoggerBufferWriter::isUseBuffer() const
	{
		return m_isUseBuffer;
	}

	inline void LoggerBufferWriter::setUseBuffer(bool b)
	{
		m_isUseBuffer = b;
	}

	inline bool LoggerBufferWriter::isReadProcessExist() const
	{
		return m_isReadProcessExist;
	}

	inline void LoggerBufferWriter::setReadProcessExist(bool b)
	{
		m_isReadProcessExist = b;
	}
}

#define g_LoggerBufferWriter Zoic::LoggerBufferWriter::getInstance()

#endif