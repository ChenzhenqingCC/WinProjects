/*! @file
	@ingroup network
	@brief 定义网络连接服务类
*/

#ifndef __ZOIC_SESSIONS_H
#define __ZOIC_SESSIONS_H

#include "Session.h"
#include <list>

namespace Zoic
{
	class CommandFactory;
	class SessionS;
	typedef std::list<SessionS *> LIST_SESSIONS;
	/*! @brief 网络连接服务类
		@ingroup network


		代表监听网络连接的一端
	*/
	class SessionS
		:public Session
	{
		friend class SessionBank;
	public:
		SessionS(SessionUnpackMethod* unpack_method = NULL);
		//!获得连接句柄
		const HandleType & getHandle();
		//!获得连接序列号
		const SerialType & getSerial();
		//!是否准备好
		bool isReady();		
	protected:
		//!解析消息
		bool parseMessage(
			Message &msg	//!< 网络消息
			);
		
		bool					m_ready;			//!< 客户端装备完毕标志
		CommandFactory *		m_commandFactory;	//!< 网络命令处理工厂
	private:
		HandleType				m_handle;			//!< 句柄
		SerialType				m_serial;			//!< 序列号
		LIST_SESSIONS::iterator	m_itList;			//!< 在List中的位置
	};
	
	inline const HandleType & SessionS::getHandle()
	{
		return m_handle;
	}
	
	inline const SerialType & SessionS::getSerial()
	{
		return m_serial;
	}
	
	inline bool SessionS::isReady()
	{
		return m_ready;
	}
}

#endif
