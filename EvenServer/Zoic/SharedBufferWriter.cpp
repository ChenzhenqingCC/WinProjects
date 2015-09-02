#include "include/SharedBufferWriter.h"
#include "include/SharedBufferControl.h"
#include <errno.h>

#ifndef _MSC_VER
#include <unistd.h>
#endif

using namespace Zoic;

SharedBufferWriter::SharedBufferWriter():
m_sharedBufferCtl(*(new SharedBufferControl)),
m_isWait(false),
m_curLoopTimes(0),
m_totalLoopTimes(0),
m_waitTimes(0)
{
	
}

SharedBufferWriter::~SharedBufferWriter()
{
	m_dequeLocInfo.clear();
	delete &m_sharedBufferCtl;
}

bool SharedBufferWriter::doRegister(const std::string& path,int code,DWORD blocksize,BYTE semcnt)
{
	return m_sharedBufferCtl.doRegister(path,code,blocksize,semcnt);
}

void SharedBufferWriter::unregister()
{
	m_dequeLocInfo.clear();
	m_sharedBufferCtl.unregister();
}

int SharedBufferWriter::getErrorCode() const
{
	return m_sharedBufferCtl.getErrorCode();
}

short SharedBufferWriter::getShmAttachCount()
{
	return m_sharedBufferCtl.getShmAttachCount();
}

bool SharedBufferWriter::isRegister() const
{
	return m_sharedBufferCtl.isRegister();
}

DWORD SharedBufferWriter::getBlockSize() const
{
	return m_sharedBufferCtl.getBlockSize();
}

BYTE SharedBufferWriter::getSemaphoreCount() const
{
	return m_sharedBufferCtl.getSemaphoreCount();
}

void SharedBufferWriter::setSemFlag(bool isLockNoWait,bool isUnlockNoWait,bool isLockUndo,bool isUnlockUndo)
{
	m_sharedBufferCtl.setSemFlag(isLockNoWait,isUnlockNoWait,isLockUndo,isUnlockUndo);
}

int SharedBufferWriter::writeBuf(DWORD size,BYTE* buf,DWORD& writeSize,bool isHead)
{
	DWORD blockSize = m_sharedBufferCtl.getBlockSize();
	BYTE semCnt = m_sharedBufferCtl.getSemaphoreCount();

	DWORD writeLoc = m_sharedBufferCtl.getWriteLoc();
	QWORD writeLoopCnt = m_sharedBufferCtl.getWriteLoopCnt();
	writeSize = (size<(blockSize - writeLoc%blockSize))?(size):(blockSize - writeLoc%blockSize);

	DWORD curReadLoc = m_sharedBufferCtl.getReadLoc();
	QWORD curReadLoopCnt = m_sharedBufferCtl.getReadLoopCnt();

	while (semCnt > 2 &&
		(!SharedBufferControl::isMore(curReadLoc,curReadLoopCnt+1,writeLoc+writeSize,writeLoopCnt,blockSize,semCnt))
		)
	{
		if (m_isWait)
		{
			if (!m_sharedBufferCtl.isExist())
			{
				//认为read进程已经死亡 或者 未开启
				//m_sharedBufferCtl.resetLoc();
				return -1;
			}
		#ifndef _MSC_VER
			usleep(1);
		#endif
			incLoopTimes();
			curReadLoc = m_sharedBufferCtl.getReadLoc();
			curReadLoopCnt = m_sharedBufferCtl.getReadLoopCnt();
			continue;
		}
		return 0;
	}
	incWaitTimes();
	resetCurLoopTimes();

	BYTE writeBlock = static_cast<BYTE>(writeLoc/blockSize);
	m_sharedBufferCtl.lock(writeBlock);
	if (m_sharedBufferCtl.getErrorCode() == EAGAIN)
	{
		return 0;
	}
	
	if (isHead)
	{
 		addWriteBufInfo(writeLoc,writeLoopCnt,curReadLoc,curReadLoopCnt,size);
	}
	BYTE* sharedBuf = m_sharedBufferCtl.getBuffer(writeLoc);
	ASSERT((writeLoc + writeSize) <= (blockSize*semCnt));
	ASSERT(writeSize<=(blockSize-writeLoc%blockSize));
	memcpy(sharedBuf,buf,writeSize);
	m_sharedBufferCtl.increaseWriteLoc(writeSize);

	DWORD newWriteLoc = m_sharedBufferCtl.getWriteLoc();
	if ((blockSize - newWriteLoc%blockSize) < sizeof(DWORD))
	{
		m_sharedBufferCtl.increaseWriteLoc(blockSize - newWriteLoc%blockSize);
	}
	m_sharedBufferCtl.unlock(writeBlock);
	return 1;
}

bool SharedBufferWriter::readRemain(DWORD& size,BYTE* buf)
{
	DWORD curReadLoc = m_sharedBufferCtl.getReadLoc();
	QWORD curReadLoopCnt = m_sharedBufferCtl.getReadLoopCnt();
	cleanupWriteBufInfo(curReadLoc,curReadLoopCnt);
	if (!m_dequeLocInfo.empty())
	{
		DWORD blockSize = m_sharedBufferCtl.getBlockSize();
		BYTE semCount = m_sharedBufferCtl.getSemaphoreCount();

		WriteBufInfo& info = m_dequeLocInfo.back();
		BYTE* sharedBuf = m_sharedBufferCtl.getBuffer(info.writeLoc);
		ASSERT((info.writeLoc+sizeof(DWORD))<=(blockSize*semCount));
		memcpy(&size,sharedBuf,sizeof(DWORD));

		DWORD writeLoc = m_sharedBufferCtl.getWriteLoc();
		QWORD writeLoopCnt = m_sharedBufferCtl.getWriteLoopCnt();

		if (!SharedBufferControl::isMore(writeLoc,writeLoopCnt,info.writeLoc,info.writeLoopCnt,blockSize,semCount))
		{
			m_dequeLocInfo.clear();
			return false;
		}

		if (size <= blockSize-(info.writeLoc%blockSize))
		{
			ASSERT((info.writeLoc+size)<=(blockSize*semCount));
			memcpy(buf,sharedBuf,size);
		}
		else
		{
			DWORD readSize = (blockSize-(info.writeLoc%blockSize));
			ASSERT((info.writeLoc+readSize)<=(blockSize*semCount));
			memcpy(buf,sharedBuf,readSize);
			ASSERT((size-readSize)<=(blockSize*semCount));
			DWORD curLoc = (info.writeLoc + readSize)%(blockSize*semCount);
			memcpy(buf+readSize,m_sharedBufferCtl.getBuffer(curLoc),size-readSize);
		}

		m_sharedBufferCtl.increaseReadLoc(size);
		DWORD newReadLoc = m_sharedBufferCtl.getReadLoc();
		if ((blockSize - newReadLoc%blockSize) < sizeof(DWORD))
		{
			m_sharedBufferCtl.increaseReadLoc(blockSize - newReadLoc%blockSize);
		}
		return true;
	}
	return false;
}

bool SharedBufferWriter::addWriteBufInfo(DWORD writeLoc,QWORD writeLoopCount,DWORD curReadLoc,QWORD curReadLoopCnt,DWORD totalSize)
{
	cleanupWriteBufInfo(curReadLoc,curReadLoopCnt);
	WriteBufInfo info;
	info.writeLoc = writeLoc;
	info.writeLoopCnt = writeLoopCount;
	m_dequeLocInfo.push_front(info);
	return true;
}

void SharedBufferWriter::cleanupWriteBufInfo(DWORD curReadLoc,QWORD curReadLoopCnt)
{
	DWORD blockSize = m_sharedBufferCtl.getBlockSize();
	BYTE semCount = m_sharedBufferCtl.getSemaphoreCount();

	DWORD readLoc = curReadLoc;
	QWORD readLoopCnt = curReadLoopCnt;

	while(!m_dequeLocInfo.empty())
	{
		WriteBufInfo& info = m_dequeLocInfo.back();
		if (SharedBufferControl::isMore(readLoc,readLoopCnt,info.writeLoc,info.writeLoopCnt,blockSize,semCount))
		{
			m_dequeLocInfo.pop_back();
		}
		else
		{
			break;
		}
	}
}

void SharedBufferWriter::resetLoc()
{
	m_sharedBufferCtl.resetLoc();
}

bool SharedBufferWriter::isExist()
{
	return m_sharedBufferCtl.isExist();
}

void SharedBufferWriter::lockAll()
{
	m_sharedBufferCtl.lockAll();
}

void SharedBufferWriter::unlockAll()
{
	m_sharedBufferCtl.unlockAll();
}
