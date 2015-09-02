#include "include/SharedBufferReader.h"
#include "include/SharedBufferControl.h"
#include <errno.h>

#ifndef _MSC_VER
#include <unistd.h>
#endif

const static int HISTORY_SIZE = 8;

using namespace Zoic;

SharedBufferReader::SharedBufferReader():
m_sharedBufferCtl(*(new SharedBufferControl)),
m_isWait(false),
m_currentLoc(0),
m_historySize(HISTORY_SIZE)
{
	m_readLocHistory = new DWORD[m_historySize];
	m_writeLocHistory = new DWORD[m_historySize];
	m_readSizeHistory = new DWORD[m_historySize];
	m_readHeadHistory = new DWORD[m_historySize];
	for (int i=0;i<m_historySize;++i)
	{
		m_readLocHistory[i] = 0;
		m_writeLocHistory[i] = 0;
		m_readSizeHistory[i] = 0;
		m_readHeadHistory[i] = 0;
	}
}

SharedBufferReader::~SharedBufferReader()
{
	delete &m_sharedBufferCtl;

	delete []m_readLocHistory;
	delete []m_writeLocHistory;
	delete []m_readSizeHistory;
	delete []m_readHeadHistory;
}

bool SharedBufferReader::doRegister(const std::string& path,int code,DWORD blocksize,BYTE semcnt)
{
	return m_sharedBufferCtl.doRegister(path,code,blocksize,semcnt);
}

void SharedBufferReader::unregister()
{
	m_sharedBufferCtl.unregister();
}

bool SharedBufferReader::isRegister() const
{
	return m_sharedBufferCtl.isRegister();
}

DWORD SharedBufferReader::getBlockSize() const
{
	return m_sharedBufferCtl.getBlockSize();
}

BYTE SharedBufferReader::getSemaphoreCount() const
{
	return m_sharedBufferCtl.getSemaphoreCount();
}

void SharedBufferReader::setSemFlag(bool isLockNoWait,bool isUnlockNoWait,bool isLockUndo,bool isUnlockUndo)
{
	m_sharedBufferCtl.setSemFlag(isLockNoWait,isUnlockNoWait,isLockUndo,isUnlockUndo);
}

int SharedBufferReader::getErrorCode() const
{
	return m_sharedBufferCtl.getErrorCode();
}

short SharedBufferReader::getShmAttachCount()
{
	return m_sharedBufferCtl.getShmAttachCount();
}

int SharedBufferReader::readHead(BYTE* buf,DWORD& readSize,DWORD& totalSize)
{
	DWORD blockSize = m_sharedBufferCtl.getBlockSize();
	BYTE semCnt = m_sharedBufferCtl.getSemaphoreCount();

	DWORD readLoc = m_sharedBufferCtl.getReadLoc();
	QWORD readLoopCnt = m_sharedBufferCtl.getReadLoopCnt();
	while(semCnt > 2 &&
		(!SharedBufferControl::isMore(m_sharedBufferCtl.getWriteLoc(),m_sharedBufferCtl.getWriteLoopCnt(),readLoc,readLoopCnt,blockSize,semCnt))
		)
	{
		if (m_isWait)
		{
			if (!m_sharedBufferCtl.isExist())
			{
				//认为write进程已经死亡 或者 未开启
				//m_sharedBufferCtl.resetLoc();
				return -1;
			}
		#ifndef _MSC_VER
			usleep(1);
		#endif
			continue;
		}
		return 0;
	}
	DWORD writeLoc = m_sharedBufferCtl.getWriteLoc();

	BYTE readBlock = static_cast<BYTE>(readLoc/blockSize);
	m_sharedBufferCtl.lock(readBlock);
	if (m_sharedBufferCtl.getErrorCode() == EAGAIN)
	{
		return 0;
	}
	
	BYTE* sharedBuf = m_sharedBufferCtl.getBuffer(readLoc);
	ASSERT((readLoc+sizeof(DWORD))<=(blockSize*semCnt));
	memcpy(&totalSize,sharedBuf,sizeof(DWORD));
	ASSERT(totalSize <= blockSize*semCnt);
	readSize = (totalSize<(blockSize-readLoc%blockSize))?(totalSize):(blockSize-readLoc%blockSize);

	//读取前写入历史，便于调试
	m_readSizeHistory[m_currentLoc] = readSize;
	m_writeLocHistory[m_currentLoc] = writeLoc;
	m_readLocHistory[m_currentLoc] = readLoc;
	m_readHeadHistory[m_currentLoc] = totalSize;
	m_currentLoc = (m_currentLoc + 1)%m_historySize;

	ASSERT((readLoc+readSize)<=(blockSize*semCnt));
	ASSERT(readSize<=(blockSize-readLoc%blockSize));
	memcpy(buf,sharedBuf,readSize);
	m_sharedBufferCtl.increaseReadLoc(readSize);

	DWORD newReadLoc = m_sharedBufferCtl.getReadLoc();
	if (blockSize-newReadLoc%blockSize<sizeof(DWORD))
	{
		m_sharedBufferCtl.increaseReadLoc(blockSize-newReadLoc%blockSize);
	}
	m_sharedBufferCtl.unlock(readBlock);
	return 1;
}

int SharedBufferReader::readBuf(BYTE* buf,DWORD& readSize,DWORD size)
{
	DWORD blockSize = m_sharedBufferCtl.getBlockSize();
	BYTE semCnt = m_sharedBufferCtl.getSemaphoreCount();

	DWORD readLoc = m_sharedBufferCtl.getReadLoc();
	QWORD readLoopCnt = m_sharedBufferCtl.getReadLoopCnt();
	while(semCnt > 2 &&
		(!SharedBufferControl::isMore(m_sharedBufferCtl.getWriteLoc(),m_sharedBufferCtl.getWriteLoopCnt(),readLoc,readLoopCnt,blockSize,semCnt))
		)
	{
		if (m_isWait)
		{
			if (!m_sharedBufferCtl.isExist())
			{
				//认为write进程已经死亡 或者 未开启
				//m_sharedBufferCtl.resetLoc();
				return -1;
			}
		#ifndef _MSC_VER
			usleep(1);
		#endif
			continue;
		}
		return 0;
	}
	DWORD writeLoc = m_sharedBufferCtl.getWriteLoc();
	
	BYTE readBlock = static_cast<BYTE>(readLoc/blockSize);
	m_sharedBufferCtl.lock(readBlock);
	if (m_sharedBufferCtl.getErrorCode() == EAGAIN)
	{
		return 0;
	}

	BYTE* sharedBuf = m_sharedBufferCtl.getBuffer(readLoc);
	readSize = (size<(blockSize-readLoc%blockSize))?(size):(blockSize-readLoc%blockSize);
	ASSERT((readLoc+readSize)<=(blockSize*semCnt));
	ASSERT(readSize<=(blockSize-readLoc%blockSize));

	//读取前写入历史，便于调试
	m_readSizeHistory[m_currentLoc] = readSize;
	m_writeLocHistory[m_currentLoc] = writeLoc;
	m_readLocHistory[m_currentLoc] = readLoc;
	m_readHeadHistory[m_currentLoc] = 0;
	m_currentLoc = (m_currentLoc + 1)%m_historySize;

	memcpy(buf,sharedBuf,readSize);
	m_sharedBufferCtl.increaseReadLoc(readSize);

	DWORD newReadLoc = m_sharedBufferCtl.getReadLoc();
	if (blockSize-newReadLoc%blockSize < sizeof(DWORD))
	{
		m_sharedBufferCtl.increaseReadLoc(blockSize-newReadLoc%blockSize);
	}
	m_sharedBufferCtl.unlock(readBlock);
	return 1;
}

void SharedBufferReader::resetLoc()
{
	m_sharedBufferCtl.resetLoc();
}

bool SharedBufferReader::isExist()
{
	return m_sharedBufferCtl.isExist();
}

DWORD SharedBufferReader::getWriteLoc() const
{
	return m_sharedBufferCtl.getWriteLoc();
}

QWORD SharedBufferReader::getWriteLoopCnt() const
{
	return m_sharedBufferCtl.getWriteLoopCnt();
}

DWORD SharedBufferReader::getReadLoc() const
{
	return m_sharedBufferCtl.getReadLoc();
}

QWORD SharedBufferReader::getReadLoopCnt() const
{
	return m_sharedBufferCtl.getReadLoopCnt();
}