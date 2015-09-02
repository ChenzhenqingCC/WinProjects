/*! @file
	@ingroup network
	@brief 定义网络连接管理类
*/

#ifndef __ZOIC_SESSIONBANK_H
#define __ZOIC_SESSIONBANK_H

#include "Types.h"
#include <list>
#include <string>
#include "CommandFactory.h"
#include "Selectable.h"

namespace Zoic
{
	class SessionS;
	typedef std::list<SessionS *> LIST_SESSIONS;
	/*!
		@brief 网络连接管理类
		@ingroup network
	*/
	class SessionBank:
		public CommandFactory,
		public Selectable
	{
	public:
		SessionBank( MESSAGE_FUNC defaultFunc = CommandFactory::onDummyMessage );
		virtual ~SessionBank();

		//!广播消息
		void broadcast(
			const Zoic::Message &msg	//!< 要广播的消息
			);

		//!设置监听地址
		void setListenAddress(
			const char *ip,	//!< 监听IP地址
			WORD port		//!< 监听端口号
			);

		//!获得监听IP
		const char* getListenIP() const;

		//!获得监听IP
		WORD getListenPort() const;

		//!设置可连接的最大数量
		bool setMaxSessions(
			HandleType maxSessions	//!< 可连接的最大数据
			);

		//!启动,读取配置信息,监听网络
		int  start();

		//!运行,管理连接的建立与删除，驱动活动连接收发数据
		int	 run();

		//!停止
		int	 stop();

		/*! @brief 根据句柄与序列号查找SessionS
			@return 如果成功,返回找到的SessionS指针，否则返回NULL
		*/
		SessionS * findSession(
			const HandleType & handle,	//!< 名柄
			const SerialType &serial	//!< 序列号
			);

		//!允许运行
		void allowRun();

		const LIST_SESSIONS & getActiveSessions() const;
	protected:
		/*! @brief创建要管理的SessionS对象
			@return 新创建的SessionS对象指针
		*/
		virtual SessionS * createSession()=0;
		HandleType getMaxSessionCount() const;
		/*! @brief 获得活动连接列表
			@return 活动连接列表
		*/
		SessionS * const * getSessionHandleTable() const;
		bool addSessionToActiveList(SessionS * pSession);
		bool addSessionToUnusedList(SessionS * pSession);
		bool				m_allowRun;			//!< 是否允许运行run函数
	private:
		void clearSessions();

		virtual void onSessionOpened(SessionS *pSession);
		virtual void onSessionClosed(SessionS *pSession);
		//!关闭连接
		void closeSession(
			SessionS *pSession	//!< 连接对象
			);

		//! 接收数据
		void recvData();
		
		LIST_SESSIONS		m_unusedSessions;	//!< 未用的Session
		LIST_SESSIONS		m_activeSessions;	//!< 活动的Session		
		SessionS **			m_sessions;			//!< 活动Session索引
		HandleType			m_maxSessions;		//!< 最大Session计数

		int					m_listenSocket;		//!< 监听套接字
		std::string			m_listenIP;			//!< 监听IP地址
		WORD 				m_listenPort;		//!< 监听端口号
		SerialType			m_nextSerial;		//!< 下一次的序列号
	};
	
	inline void SessionBank::allowRun()
	{
		m_allowRun=true;
	}

	inline const LIST_SESSIONS & SessionBank::getActiveSessions() const
	{
		return m_activeSessions;
	}

	inline const char* SessionBank::getListenIP() const
	{
		return m_listenIP.c_str();
	}

	inline WORD SessionBank::getListenPort() const
	{
		return m_listenPort;
	}

	inline HandleType SessionBank::getMaxSessionCount() const
	{
		return m_maxSessions;
	}

	inline SessionS * const * SessionBank::getSessionHandleTable() const
	{
		return m_sessions;
	}

}

#endif
