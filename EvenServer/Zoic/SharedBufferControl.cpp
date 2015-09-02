#include "include/SharedBufferControl.h"

#define  SVSEM_MODE (SEM_R | SEM_A | SEM_R >> 3 | SEM_R >> 6)
#define  SVSHM_MODE (SHM_R | SHM_W | SHM_R >> 3 | SHM_W >> 6)

#include <errno.h>

using namespace Zoic;

static const WORD ERROR_TOOMANY_PROCESS = 10000;

SharedBufferControl::SharedBufferControl():
m_keyPath(""),
m_keyCode(0),
m_errorCode(0),
m_shmID(0),
m_semID(0),
m_isRegister(false),
m_semVal(NULL),
m_pSharedBufferHead(NULL),
m_blockSize(NULL),
m_semCount(NULL),
m_writeLoc(NULL),
m_readLoc(NULL),
m_writeLoopCnt(NULL),
m_readLoopCnt(NULL),
m_pBuffer(NULL)
{
#ifndef _MSC_VER
	m_waitSemBuf = NULL;
	m_postSemBuf = NULL;
#endif
}


SharedBufferControl::~SharedBufferControl()
{
	destroy();
}

bool SharedBufferControl::doRegister(const std::string& path, int code, DWORD size /* = 1024 */, BYTE semCnt /* = 4 */)
{
	if (isRegister())
	{
		return true;
	}
	init(path,code);
	if (!findSharedBuffer())
	{
		if (getErrorCode() == ENOENT)
		{
			if (!createSharedBuffer(size,semCnt))
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	if (!findSemaphore())
	{
		if (getErrorCode() == ENOENT)
		{
			if (!createSemaphore(semCnt))
			{
				destroy();
				return false;
			}
		}
		else
		{
			destroy();
			return false;
		}
	}
	if (m_shmID <= 0 || m_semID <= 0 || !m_pSharedBufferHead || (int)(m_pSharedBufferHead) == -1)
	{
		return false;
	}
	m_isRegister = true;
	m_errorCode = 0;
	return true;
}

void SharedBufferControl::unregister()
{
	destroy();
}

void SharedBufferControl::init(const std::string& keyPath,int keyCode)
{
	m_keyPath = keyPath;
	m_keyCode = keyCode;
}

void SharedBufferControl::resetLoc()
{
	if (!m_isRegister)
	{
		return;
	}

	*m_writeLoc = 0;
	*m_readLoc = 0;
	*m_writeLoopCnt = 0;
	*m_readLoopCnt = 0;
}

void SharedBufferControl::formatBuffer()
{
	if (!m_pSharedBufferHead || (int)(m_pSharedBufferHead) == -1)
	{
		return;
	}

	m_blockSize = reinterpret_cast<DWORD*>(&m_pSharedBufferHead[0]);
	m_semCount = &m_pSharedBufferHead[sizeof(DWORD)];
	m_writeLoc = (DWORD*)(m_semCount + sizeof(BYTE));
	m_readLoc = (DWORD*)(m_writeLoc + sizeof(DWORD));
	m_writeLoopCnt = (QWORD*)(m_readLoc + sizeof(DWORD));
	m_readLoopCnt = (QWORD*)(m_writeLoopCnt + sizeof(QWORD));
	m_pBuffer = (BYTE*)(m_readLoopCnt + sizeof(QWORD));
}

bool SharedBufferControl::isMore(DWORD aLoc,QWORD aCount,DWORD bLoc,QWORD bCount,DWORD blockSize,BYTE semCount)
{
	DWORD totalSize = blockSize*semCount;
	if (aLoc >= totalSize)
	{
		aCount += (aLoc/totalSize);
		aLoc = aLoc%totalSize;
	}
	if (bLoc >= totalSize)
	{
		bCount += (bLoc/totalSize);
		bLoc = bLoc%totalSize;
	}

	if (aCount == bCount)
	{
		if (aLoc > bLoc)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (aCount > bCount)
	{
		return true;
	}
	else
	{
		return false;
	}
}

#ifdef _MSC_VER


void SharedBufferControl::lock(BYTE index)
{

}

void SharedBufferControl::unlock(BYTE index)
{

}

void SharedBufferControl::lockAll()
{

}

void SharedBufferControl::unlockAll()
{

}

bool SharedBufferControl::createSharedBuffer(DWORD size,BYTE semCnt)
{
	m_shmID = 0;
	return false;
}

bool SharedBufferControl::createSemaphore(BYTE semCnt)
{
	m_semID = 0;
	return false;
}

bool SharedBufferControl::findSharedBuffer()
{
	m_shmID = 0;
	return false;
}


bool SharedBufferControl::findSemaphore()
{
	m_semID = 0;
	return false;
}


void SharedBufferControl::destroy()
{
	m_isRegister = false;
	m_semID = 0;
	m_shmID = 0;
}

void SharedBufferControl::setSemFlag(bool isLockNoWait,bool isUnlockNoWait,bool isLockUndo,bool isUnlockUndo)
{

}

bool SharedBufferControl::isExist() const
{
	return false;
}

short SharedBufferControl::getShmAttachCount() const
{
	return -1;
}

#else

bool SharedBufferControl::createSharedBuffer(DWORD size,BYTE semCnt)
{
	key_t key = ftok(m_keyPath.c_str(),m_keyCode);
	int shmID = shmget(key,0,SVSHM_MODE);
	if (shmID != -1)
	{
		shmctl(shmID,IPC_RMID,NULL);
	}
	int totalSize = sizeof(DWORD)*3 + sizeof(BYTE) + sizeof(QWORD)*2 + size*semCnt;
 	m_shmID = shmget(key,totalSize,SVSHM_MODE | IPC_CREAT | IPC_EXCL);

	if (m_shmID == -1)
	{
		m_errorCode = errno;
		return false;
	}

	m_pSharedBufferHead = reinterpret_cast<BYTE*>(shmat(m_shmID,NULL,0));
	if (!m_pSharedBufferHead || (int)(m_pSharedBufferHead) == (-1))
	{
		m_errorCode = errno;
		return false;
	}
	formatBuffer();
	*m_blockSize = size;
	*m_semCount = semCnt;
	*m_writeLoc = 0;
	*m_readLoc = 0;
	*m_writeLoopCnt = 0;
	*m_readLoopCnt = 0;
	return true;
}

bool SharedBufferControl::createSemaphore(BYTE semCnt)
{
	if (m_shmID <= 0 || !m_pSharedBufferHead || (int)(m_pSharedBufferHead) == -1)
	{
		return false;
	}
	key_t key = ftok(m_keyPath.c_str(),m_keyCode);
	int semID = semget(key,0,0);
	if (semID != -1)
	{
		semctl(semID,0,IPC_RMID);
	}

	m_semID = semget(key,semCnt,SVSEM_MODE |  IPC_CREAT | IPC_EXCL);
	if (m_semID == -1)
	{
		m_errorCode = errno;
		return false;
	}
	
	union semun semUnion;
	semUnion.val = 1;
	for(int i=0;i<semCnt;++i)
	{
		semctl(m_semID,i,SETVAL,semUnion);
	}
	m_waitSemBuf = new sembuf[semCnt];
	m_postSemBuf = new sembuf[semCnt];
	m_semVal = new unsigned short[semCnt];
	setSemFlag(false,false,true,true);
	return true;
}

bool SharedBufferControl::findSharedBuffer()
{
	key_t key = ftok(m_keyPath.c_str(),m_keyCode);
	m_shmID = shmget(key,0,SVSHM_MODE);
	if (m_shmID == -1)
	{
		m_errorCode = errno;
		return false;
	}
	m_pSharedBufferHead = reinterpret_cast<BYTE*>(shmat(m_shmID,NULL,0));
	if (!m_pSharedBufferHead || (int)(m_pSharedBufferHead) == (-1))
	{
		m_errorCode = errno;
		return false;
	}

	if (getShmAttachCount() > 2)	//如果有超过2个进程存在,则detach,设置错误
	{
		shmdt(m_pSharedBufferHead);
		m_errorCode = ERROR_TOOMANY_PROCESS;
		return false;
	}

	formatBuffer();
	return true;
}

bool SharedBufferControl::findSemaphore()
{
	if (m_shmID <= 0 || !m_pSharedBufferHead || (int)(m_pSharedBufferHead) == -1)
	{
		return false;
	}
	key_t key = ftok(m_keyPath.c_str(),m_keyCode);
	m_semID = semget(key,0,SVSEM_MODE);
	if (m_semID == -1)
	{
		m_errorCode = errno;
		return false;
	}
	struct semid_ds semInfo;
	union semun semUnion;
	semUnion.buf = &semInfo;

	BYTE semCnt = *m_semCount;
	for(int i=0;i<semCnt;++i)
	{
		semctl(m_semID,i,IPC_STAT,semUnion);
	}
	
	m_waitSemBuf = new sembuf[semCnt];
	m_postSemBuf = new sembuf[semCnt];
	m_semVal = new unsigned short[semCnt];
	setSemFlag(false,false,true,true);
	return true;
}

void SharedBufferControl::setSemFlag(bool isLockNoWait,bool isUnlockNoWait,bool isLockUndo,bool isUnlockUndo)
{
	if (!m_isRegister)
	{
		return;
	}
	BYTE semCnt = *m_semCount;
	for(int i=0;i<semCnt;++i)
	{
		m_waitSemBuf[i].sem_num = i;
		m_waitSemBuf[i].sem_op = -1;
		m_waitSemBuf[i].sem_flg = ((isLockUndo)?(SEM_UNDO):(0)) | ((isLockNoWait)?(IPC_NOWAIT):(0));
		m_postSemBuf[i].sem_num = i;
		m_postSemBuf[i].sem_op = 1;
		m_postSemBuf[i].sem_flg =((isUnlockUndo)?(SEM_UNDO):(0)) | ((isUnlockNoWait)?(IPC_NOWAIT):(0));
	}
}

void SharedBufferControl::lock(BYTE index)
{
	int ret = semop(m_semID,&m_waitSemBuf[index],1);
	if (ret == -1)
	{
		m_errorCode = errno;
	}
}

void SharedBufferControl::unlock(BYTE index)
{
	int ret = semop(m_semID,&m_postSemBuf[index],1);
	if (ret == -1)
	{
		m_errorCode = errno;
	}
}

void SharedBufferControl::lockAll()
{
	if (!m_isRegister)
	{
		return;
	}
	BYTE semCnt = *m_semCount;
	int ret = semop(m_semID,m_waitSemBuf,semCnt);
	if (ret == -1)
	{
		m_errorCode = errno;
	}
}

void SharedBufferControl::unlockAll()
{
	if (!m_isRegister)
	{
		return;
	}
	BYTE semCnt = *m_semCount;
	int ret = semop(m_semID,m_postSemBuf,semCnt);
	if (ret == -1)
	{
		m_errorCode = errno;
	}
}

bool SharedBufferControl::isExist() const
{
	if (!m_isRegister || m_shmID <= 0 || m_semID <= 0)
	{
		return false;
	}

	short shmCnt = getShmAttachCount();
	if (shmCnt <= 1)
	{
		return false;
	}

	return true;
}

void SharedBufferControl::destroy()
{
	short shmCnt = getShmAttachCount();
	if (shmCnt > 1)
	{
 		shmdt(m_pSharedBufferHead);
	}
	else if(shmCnt == 1)
	{
		//shmdt(m_pSharedBufferHead);
		shmctl(m_shmID,IPC_RMID,NULL);
		semctl(m_semID,0,IPC_RMID);
	}

	if (m_waitSemBuf)
	{
		delete []m_waitSemBuf;
		m_waitSemBuf = NULL;
	}
	if (m_postSemBuf)
	{
		delete []m_postSemBuf;
		m_postSemBuf = NULL;
	}
	if (m_semVal)
	{
		delete []m_semVal;
		m_semVal = NULL;
	}

	m_isRegister = false;
	m_semID = 0;
	m_shmID = 0;
	m_pSharedBufferHead = NULL;
	m_blockSize = NULL;
	m_semCount = NULL;
	m_writeLoc = NULL;
	m_readLoc = NULL;
	m_writeLoopCnt = NULL;
	m_readLoopCnt = NULL;
	m_pBuffer = NULL;
}

short SharedBufferControl::getShmAttachCount() const
{
	if (m_shmID <= 0)
	{
		return -1;
	}
	shmid_ds shmInfo;
	shmctl(m_shmID,IPC_STAT,&shmInfo);
	return shmInfo.shm_nattch;
}

#endif
