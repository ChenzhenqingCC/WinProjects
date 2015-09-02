/*! @file
	@ingroup network
	@brief 定义文件描述符集合类
*/

#ifndef __ZOIC_FDSET_H
#define __ZOIC_FDSET_H

#include "Types.h"
#include <list>

struct fd_set;

namespace Zoic
{
	/*!
		@brief 文件描述符集合
		@ingroup network

		封装操作系统 fd_set 结构
	*/
	class FdSet
	{
	public:
		//! 构造函数
		FdSet();

		//! 析构函数
		~FdSet();

		//! fd_set * 类型转换
		operator fd_set * () const;

		//! 设置套按字集合
		void setSockets(
			int size,				//!< 套接字数量(输入)
			SOCKET * sockets		//!< 套接字数组(输入)
			);

		//! 获得套字集合
		void getSockets(
			int * size,			//!< 套接字数量(输出)
			SOCKET * sockets	//!< 套接字数组(输出)
			) const;

		//! 返回文件描述符数量
		int getFdCount() const;

		//! 设置套接字
		void setSocket(
			SOCKET socket		//!< 套接字
			);

		//! 清除套接字
		void clearSocket(
			SOCKET socket		//!< 套接字
			);

		//! 复制操作
		void copyTo(
			FdSet & dest		//!< 复制目标
			);
	private:
#ifdef HAVE_MINGW32
		static const int INIT_ARRAY_SIZE=1;	//!< 初始数组大小
		struct _fd_set
		{
			unsigned int fd_count;
			SOCKET fd_array[INIT_ARRAY_SIZE];
		};
		unsigned int m_fdArraySize;			//!< 套接字数组大小
#else
		static const int INIT_ARRAY_SIZE=1;	//!< 初始数组大小
		int m_fdCount;						//!< 描述符数量
		typedef unsigned long _fd_mask;
		struct _fd_set
		{
			_fd_mask	bits[INIT_ARRAY_SIZE];
		};
		int m_bitsSize;					//!< 位数组大小
#endif
		_fd_set	*	m_fdSet;		//!< 描述符集合
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
