/*! @file
	@ingroup network
	@brief 定义选择器类
*/

#ifndef __ZOIC_SELECTOR_H
#define __ZOIC_SELECTOR_H

#include <set>
#include "Types.h"
#include "Module.h"
#include <time.h>
#include "FdSet.h"
#ifdef HAVE_MINGW32
#include <winsock2.h>
#ifdef _MSC_VER
#pragma comment (lib, "winmm.lib")
#endif
#endif

namespace Zoic
{
	class Selectable;

	/*! @brief 选择器类
		@ingroup buildin
	*/
	class Selector
#ifndef ZOIC_NO_SERVICE
		:public Module<Selector>
#else
		:public NoCopy
#endif
	{
#ifndef ZOIC_NO_SERVICE
		friend class Module<Selector>;
#endif
	public :
		//! 加入套接字
		bool insert(
			Selectable * socket		//!< 套接字
			);

		//! 删除套接字
		bool remove(
			Selectable * socket		//!< 套接字
			);

		//! 析构函数
		~Selector();

#ifndef ZOIC_NO_SERVICE
	private:
#else
	public:
#endif
		//! 构造函数
		Selector();

		//!运行例程，更新时间
		int run();
private:
		class CmpSocket
		{
		public:
			bool operator()(Selectable *a ,Selectable * b) const;
		};
		typedef std::set<Selectable *,CmpSocket> SOCKETS;


		/*! @brief获得相关联的可选择对象

			调用方应确保传入有效值,在该函数内假定总是会找到相关对象
		*/
		Selectable * getSelectable(
			SOCKET socket
			);

		SOCKETS			m_sockets;			//!< 套接字集合
		FdSet			m_baseFdSet;		//!< 基本描述符
		FdSet			m_recvFdSet;		//!< 接收描述符集合
		FdSet			m_sendFdSet;		//!< 发送描述符集合
		Selectable **	m_socketArray;		//!< 套接字数组，以套接字为下标
		unsigned int	m_socketArraySize;	//!< 套接字数组大小		
		SOCKET *		m_activeSockets;	//!< 活动套接字数组
		unsigned int	m_activeSocketsSize;//!< 活动套接字数组大小
	};

#ifndef ZOIC_NO_SERVICE
	#define g_Selector Zoic::Selector::getInstance()
#else
	extern Selector theSelector;
	#define g_Selector (&Zoic::theSelector)
#endif

	inline Selectable * Selector::getSelectable(SOCKET socket)
	{
		return m_socketArray[socket];
	}

}
#endif
