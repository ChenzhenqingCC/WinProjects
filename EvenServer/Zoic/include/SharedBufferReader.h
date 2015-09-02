#ifndef __SHAREDBUFFERREADER_H
#define __SHAREDBUFFERREADER_H

#include "Types.h"
#include <string>

namespace Zoic
{
	class SharedBufferControl;

	class SharedBufferReader
	{
	public:
		SharedBufferReader();
		virtual ~SharedBufferReader();
		bool doRegister(const std::string& path,int code,DWORD blocksize,BYTE semcnt);
		void unregister();
		bool isRegister() const;

		void resetLoc();
		bool isExist();

		DWORD getBlockSize() const;
		BYTE getSemaphoreCount() const;

		DWORD getWriteLoc() const;
		QWORD getWriteLoopCnt() const;
		DWORD getReadLoc() const;
		QWORD getReadLoopCnt() const;

		bool isWaitFlag() const;
		void setWaitFlag(bool isWait);

		void setSemFlag(bool isLockNoWait,bool isUnlockNoWait,bool isLockUndo,bool isUnlockUndo);
		int getErrorCode() const;
		short getShmAttachCount();

		int readHead(BYTE* buf,DWORD& readSize,DWORD& totalSize);
		int readBuf(BYTE* buf,DWORD& readSize,DWORD size);
	private:
		SharedBufferControl& m_sharedBufferCtl;
		bool m_isWait;

		const int m_historySize;
		int m_currentLoc;
		DWORD* m_readLocHistory;
		DWORD* m_writeLocHistory;
		DWORD* m_readSizeHistory;
		DWORD* m_readHeadHistory;
	};
	
	inline bool SharedBufferReader::isWaitFlag() const
	{
		return m_isWait;
	}

	inline void SharedBufferReader::setWaitFlag(bool isWait)
	{
		m_isWait = isWait;
	}
}

#endif