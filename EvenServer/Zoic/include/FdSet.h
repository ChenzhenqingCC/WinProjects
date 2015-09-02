/*! @file
	@ingroup network
	@brief �����ļ�������������
*/

#ifndef __ZOIC_FDSET_H
#define __ZOIC_FDSET_H

#include "Types.h"
#include <list>

struct fd_set;

namespace Zoic
{
	/*!
		@brief �ļ�����������
		@ingroup network

		��װ����ϵͳ fd_set �ṹ
	*/
	class FdSet
	{
	public:
		//! ���캯��
		FdSet();

		//! ��������
		~FdSet();

		//! fd_set * ����ת��
		operator fd_set * () const;

		//! �����װ��ּ���
		void setSockets(
			int size,				//!< �׽�������(����)
			SOCKET * sockets		//!< �׽�������(����)
			);

		//! ������ּ���
		void getSockets(
			int * size,			//!< �׽�������(���)
			SOCKET * sockets	//!< �׽�������(���)
			) const;

		//! �����ļ�����������
		int getFdCount() const;

		//! �����׽���
		void setSocket(
			SOCKET socket		//!< �׽���
			);

		//! ����׽���
		void clearSocket(
			SOCKET socket		//!< �׽���
			);

		//! ���Ʋ���
		void copyTo(
			FdSet & dest		//!< ����Ŀ��
			);
	private:
#ifdef HAVE_MINGW32
		static const int INIT_ARRAY_SIZE=1;	//!< ��ʼ�����С
		struct _fd_set
		{
			unsigned int fd_count;
			SOCKET fd_array[INIT_ARRAY_SIZE];
		};
		unsigned int m_fdArraySize;			//!< �׽��������С
#else
		static const int INIT_ARRAY_SIZE=1;	//!< ��ʼ�����С
		int m_fdCount;						//!< ����������
		typedef unsigned long _fd_mask;
		struct _fd_set
		{
			_fd_mask	bits[INIT_ARRAY_SIZE];
		};
		int m_bitsSize;					//!< λ�����С
#endif
		_fd_set	*	m_fdSet;		//!< ����������
	};

	inline FdSet::operator fd_set * () const
	{
		return reinterpret_cast<fd_set *>(m_fdSet);
	}

	inline int FdSet::getFdCount() const
	{
#ifdef HAVE_MINGW32
		return 0;
#else
		return m_fdCount;
#endif
	}
}

#endif
