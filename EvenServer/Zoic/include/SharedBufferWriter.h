#ifndef __SHAREDBUFFERWRITER_H
#define __SHAREDBUFFERWRITER_H

#include "Types.h"
#include <string>
#include <deque>

namespace Zoic
{
	class SharedBufferControl;

	class SharedBufferWriter
	{
	public:
		SharedBufferWriter();
		virtual ~SharedBufferWriter();

		bool doRegister(const std::string& path,int code,DWORD blocksize,BYTE semcnt);
		void unregister();
		bool isRegister() const;

		void resetLoc();
		bool isExist();

		DWORD getBlockSize() const;
		BYTE getSemaphoreCount() const;
		
		void setSemFlag(bool isLockNoWait,bool isUnlockNoWait,bool isLockUndo,bool isUnlockUndo);

		bool isWaitFlag() const;
		void setWaitFlag(bool isWait);
	
		int writeBuf(DWORD size,BYTE* buf,DWORD& writeSize,bool isHead);
		bool readRemain(DWORD& size,BYTE* buf);

		int getErrorCode() const;
		short getShmAttachCount();

		void lockAll();
		void unlockAll();

		QWORD getWaitTimes() const;
		QWORD getTotalLoopTimes() const;

		void incLoopTimes();
		void incWaitTimes();

		void resetCurLoopTimes();
		void resetTotalLoopTimes();
		void resetWaitTimes();

		size_t getWirteBufSize() const;
	private:
		bool addWriteBufInfo(DWORD writeLoc,QWORD writeLoopCount,DWORD readLoc,QWORD readLoopCnt,DWORD totalSize);
		void cleanupWriteBufInfo(DWORD curReadLoc,QWORD curReadLoopCnt);
	
		struct WriteBufInfo
		{
			DWORD writeLoc;
			QWORD writeLoopCnt;
		};
		typedef std::deque<WriteBufInfo> WRITEBUFINFO_DEQUE;

	private:
		SharedBufferControl& m_sharedBufferCtl;
		bool m_isWait;
		QWORD m_curLoopTimes;
		QWORD m_waitTimes;
		QWORD m_totalLoopTimes;
		WRITEBUFINFO_DEQUE m_dequeLocInfo;
	};

	inline bool SharedBufferWriter::isWaitFlag() const
	{
		return m_isWait;
	}

	inline void SharedBufferWriter::setWaitFlag(bool isWait)
	{
		m_isWait = isWait;
	}
	
	inline QWORD SharedBufferWriter::getWaitTimes() const
	{
		return m_waitTimes;
	}

	inline QWORD SharedBufferWriter::getTotalLoopTimes() const
	{
		return m_totalLoopTimes;
	}

	inline void SharedBufferWriter::incLoopTimes()
	{
		++m_curLoopTimes;
		++m_totalLoopTimes;
	}

	inline void SharedBufferWriter::incWaitTimes()
	{
		if (m_curLoopTimes)
		{
			++m_waitTimes;
		}
	}
	
	inline void SharedBufferWriter::resetCurLoopTimes()
	{
		m_curLoopTimes = 0;
	}

	inline void SharedBufferWriter::resetWaitTimes()
	{
		m_waitTimes = 0;
	}

	inline void SharedBufferWriter::resetTotalLoopTimes()
	{
		m_totalLoopTimes = 0;
	}

	inline size_t SharedBufferWriter::getWirteBufSize() const
	{
		return m_dequeLocInfo.size();
	}
}

#endif