#ifndef __SHAREDBUFFER_H
#define __SHAREDBUFFER_H

#include "Types.h"
#include <string>

#ifdef _MSC_VER
#else
#include <sys/sem.h>
#include <sys/shm.h>
#endif

namespace Zoic
{
	extern const BYTE BUFFER_ERROR_LOC;
	class SharedBufferControl
	{
		friend class SharedBufferReader;
		friend class SharedBufferWriter;
	public:
		SharedBufferControl();
		virtual ~SharedBufferControl();

		//注册共享内存和信号灯,如果系统中已存在则打开,否则创建.可能有ErrorCode
		bool doRegister(
			const std::string& path,
			int code,
			DWORD size = 1024,
			BYTE semCnt = 4
			);
		//注销共享内存和信号灯
		void unregister();

		bool isRegister() const;

		void lock(BYTE index);				//锁定index号分块，可能有ErrorCode
		void unlock(BYTE index);			//解锁index号分块，可能有ErrorCode

		void lockAll();						//锁定所有分块，可能有ErrorCode
		void unlockAll();					//解锁所有分块，可能有ErrorCode
		
		//设置信号灯开关，是否阻塞，是否回退
		void setSemFlag(bool isLockNoWait,bool isUnlockNoWait,bool isLockUndo,bool isUnlockUndo);

		int getErrorCode();					//每次使用后重置ErrorCode为0
		short getShmAttachCount() const;	//得到与共享内存连接的进程数量
		
		BYTE* getBuffer(DWORD loc);			//得到共享内存区,loc位置的指针
		DWORD getBlockSize() const;			//得到共享内存区分块大小
		BYTE getSemaphoreCount() const;		//得到共享内存区分块数量

		DWORD getReadLoc() const;			//得到读指针偏移量
		DWORD getWriteLoc() const;			//得到写指针偏移量

		QWORD getReadLoopCnt() const;		//得到读指针循环数
		QWORD getWriteLoopCnt() const;		//得到写指针循环数

		void increaseReadLoc(DWORD size);	//读指针偏移量增加size
		void increaseWriteLoc(DWORD size);	//写指针偏移量增加size

		bool isExist() const;				//对方进程是否存在

		void init(const std::string& keyPath,int keyCode);
		void destroy();								

		static bool isMore(DWORD aLoc,QWORD aCount,DWORD bLoc,QWORD bCount,DWORD blockSize,BYTE semCount);	//比较读、写指针偏移量
	private:
		bool findSharedBuffer();		//寻找已存在的共享内存区，可能有ErrorCode
		bool findSemaphore();			//寻找已存在的信号灯，可能有ErrorCode

		bool createSharedBuffer(DWORD size,BYTE semCnt);		//创建新的共享内存区,可能有ErrorCode
		bool createSemaphore(BYTE semCnt);						//创建新的信号灯,可能有ErrorCode

		void resetLoc();				//重置读、写指针
		void formatBuffer();			//读、写指针信息格式化
	private:

		std::string m_keyPath;
		int m_keyCode;

		int m_errorCode;
		int m_shmID;					//共享内存区id
		int m_semID;					//信号灯id
		bool m_isRegister;				//是否已经注册
		unsigned short* m_semVal;

		BYTE* m_pSharedBufferHead;		//共享内存区头指针
		DWORD* m_blockSize;				//共享内存区分块大小
		BYTE* m_semCount;				//共享内存区分块数量
		DWORD* m_writeLoc;				//写指针偏移量
		DWORD* m_readLoc;				//读指针偏移量
		QWORD* m_writeLoopCnt;			//写指针循环数
		QWORD* m_readLoopCnt;			//读指针循环数
		BYTE* m_pBuffer;				//共享内存区实际使用部分指针头
		
#ifndef _MSC_VER
	private:
		struct sembuf* m_waitSemBuf;	//加锁sembuf结构
		struct sembuf* m_postSemBuf;	//解锁sembuf结构
		struct shmid_ds m_shmInfo;		
#endif
	};

	inline int SharedBufferControl::getErrorCode()
	{
		return m_errorCode;
	}

	inline bool SharedBufferControl::isRegister() const
	{
		return m_isRegister;
	}

	inline DWORD SharedBufferControl::getBlockSize() const
	{
		return (m_isRegister)?(*m_blockSize):(0);
	}

	inline BYTE SharedBufferControl::getSemaphoreCount() const
	{
		return (m_isRegister)?(*m_semCount):(0);
	}

	inline DWORD SharedBufferControl::getReadLoc() const
	{
		return (m_isRegister)?(*m_readLoc):(0);
	}

	inline DWORD SharedBufferControl::getWriteLoc() const
	{
		return (m_isRegister)?(*m_writeLoc):(0);
	}

	inline QWORD SharedBufferControl::getReadLoopCnt() const
	{
		return (m_isRegister)?(*m_readLoopCnt):(0);
	}

	inline QWORD SharedBufferControl::getWriteLoopCnt() const
	{
		return (m_isRegister)?(*m_writeLoopCnt):(0);
	}

	inline BYTE* SharedBufferControl::getBuffer(DWORD loc)
	{
		return (m_isRegister)?(m_pBuffer + loc):(NULL);
	}

	inline void SharedBufferControl::increaseReadLoc(DWORD size)
	{
		if (m_isRegister)
		{
			if ((*m_readLoc)+size >= (*m_blockSize)*(*m_semCount))
			{
				(*m_readLoopCnt) = (*m_readLoopCnt) + 1;
			}
			(*m_readLoc) = ((*m_readLoc)+size)%((*m_blockSize)*(*m_semCount));
		}
	}

	inline void SharedBufferControl::increaseWriteLoc(DWORD size)
	{
		if (m_isRegister)
		{
			if ((*m_writeLoc)+size >= (*m_blockSize)*(*m_semCount))
			{
				(*m_writeLoopCnt) = (*m_writeLoopCnt) + 1;
			}
			(*m_writeLoc) = ((*m_writeLoc)+size)%((*m_blockSize)*(*m_semCount));
		}
	}
}


#endif