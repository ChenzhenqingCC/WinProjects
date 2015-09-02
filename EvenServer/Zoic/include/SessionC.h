/*! @file
	@ingroup network
	@brief 定义网络连接客户类
*/

#ifndef __ZOIC_SESSIONC_H
#define __ZOIC_SESSIONC_H

#include "Session.h"
#include "CommandFactory.h"
#include <string>


namespace Zoic
{
	/*! @brief 网络连接客户类
		@ingroup network


		代表发起网络连接的一端
	*/
	class SessionC:
		public Session,
		public CommandFactory
	{
	public:
		SessionC( MESSAGE_FUNC defaultFunc = CommandFactory::onDummyMessage );
		~SessionC();
		//!设置连接地址
		void setConnectAddress(
			const char *ip,	//!< 要连接的服务器IP地址
			WORD port		//!< 要连接的服务器端口号
			);

		//!设置本地地址
		void setLocalAddress(
			const char *ip,	//!< 本地IP地址
			WORD port		//!< 本地端口号
			);

		//!启动,连接到服务器
		int start();

		//!运行,收发数据
		int	 run();

		//!停止
		int	 stop();

		//!解析消息
		bool parseMessage(
			Message &msg	//!< 网络消息
			);

		/*! @brief 连接到服务器
			@retval 0 成功
			@retval 其它 失败
		*/
		int connect(
			DWORD ip,		//!< 要连接的服务器IP地址
			WORD port,		//!< 要连接的服务器端口号
			BYTE timeout=5	//!< 超时值(秒)
			);


		/*! @brief 是否连接成功
			@retval 0 成功
			@retval <0 失败
			@retval >0 等待中
		*/
		int isConnectSuccess(void);

		//!关闭连接
		void close();
	protected:
		void initForConnect();
		int stopConnecting(void);				//终止正在进行的连接
		bool				m_opened;			//!< 是否打开连接
	private:
		/*! @brief 获得连接状态
			@retval 0 成功
			@retval <0 失败
			@retval >0 等待中
		*/
		int					getConnectStatus();

		std::string			m_connectIP;		//!< 连接的IP地址
		WORD 				m_connectPort;		//!< 连接的端口号
		SOCKET				m_connectSocket;	//!< 连接套接字
		TIME_MSEL			m_connectTimeOut;	//!< 连接超时时刻
		std::string			m_localIP;			//!< 本地IP地址
		WORD				m_localPort;		//!< 本地端口号
		sockaddr_in			m_saServer;			//!< 远程服务器网络地址
	};
}

#endif
