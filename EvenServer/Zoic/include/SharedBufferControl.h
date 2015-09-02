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

		//ע�Ṳ���ڴ���źŵ�,���ϵͳ���Ѵ������,���򴴽�.������ErrorCode
		bool doRegister(
			const std::string& path,
			int code,
			DWORD size = 1024,
			BYTE semCnt = 4
			);
		//ע�������ڴ���źŵ�
		void unregister();

		bool isRegister() const;

		void lock(BYTE index);				//����index�ŷֿ飬������ErrorCode
		void unlock(BYTE index);			//����index�ŷֿ飬������ErrorCode

		void lockAll();						//�������зֿ飬������ErrorCode
		void unlockAll();					//�������зֿ飬������ErrorCode
		
		//�����źŵƿ��أ��Ƿ��������Ƿ����
		void setSemFlag(bool isLockNoWait,bool isUnlockNoWait,bool isLockUndo,bool isUnlockUndo);

		int getErrorCode();					//ÿ��ʹ�ú�����ErrorCodeΪ0
		short getShmAttachCount() const;	//�õ��빲���ڴ����ӵĽ�������
		
		BYTE* getBuffer(DWORD loc);			//�õ������ڴ���,locλ�õ�ָ��
		DWORD getBlockSize() const;			//�õ������ڴ����ֿ��С
		BYTE getSemaphoreCount() const;		//�õ������ڴ����ֿ�����

		DWORD getReadLoc() const;			//�õ���ָ��ƫ����
		DWORD getWriteLoc() const;			//�õ�дָ��ƫ����

		QWORD getReadLoopCnt() const;		//�õ���ָ��ѭ����
		QWORD getWriteLoopCnt() const;		//�õ�дָ��ѭ����

		void increaseReadLoc(DWORD size);	//��ָ��ƫ��������size
		void increaseWriteLoc(DWORD size);	//дָ��ƫ��������size

		bool isExist() const;				//�Է������Ƿ����

		void init(const std::string& keyPath,int keyCode);
		void destroy();								

		static bool isMore(DWORD aLoc,QWORD aCount,DWORD bLoc,QWORD bCount,DWORD blockSize,BYTE semCount);	//�Ƚ϶���дָ��ƫ����
	private:
		bool findSharedBuffer();		//Ѱ���Ѵ��ڵĹ����ڴ�����������ErrorCode
		bool findSemaphore();			//Ѱ���Ѵ��ڵ��źŵƣ�������ErrorCode

		bool createSharedBuffer(DWORD size,BYTE semCnt);		//�����µĹ����ڴ���,������ErrorCode
		bool createSemaphore(BYTE semCnt);						//�����µ��źŵ�,������ErrorCode

		void resetLoc();				//���ö���дָ��
		void formatBuffer();			//����дָ����Ϣ��ʽ��
	private:

		std::string m_keyPath;
		int m_keyCode;

		int m_errorCode;
		int m_shmID;					//�����ڴ���id
		int m_semID;					//�źŵ�id
		bool m_isRegister;				//�Ƿ��Ѿ�ע��
		unsigned short* m_semVal;

		BYTE* m_pSharedBufferHead;		//�����ڴ���ͷָ��
		DWORD* m_blockSize;				//�����ڴ����ֿ��С
		BYTE* m_semCount;				//�����ڴ����ֿ�����
		DWORD* m_writeLoc;				//дָ��ƫ����
		DWORD* m_readLoc;				//��ָ��ƫ����
		QWORD* m_writeLoopCnt;			//дָ��ѭ����
		QWORD* m_readLoopCnt;			//��ָ��ѭ����
		BYTE* m_pBuffer;				//�����ڴ���ʵ��ʹ�ò���ָ��ͷ
		
#ifndef _MSC_VER
	private:
		struct sembuf* m_waitSemBuf;	//����sembuf�ṹ
		struct sembuf* m_postSemBuf;	//����sembuf�ṹ
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