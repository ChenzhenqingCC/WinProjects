/*! @file
	@ingroup network
	@brief 定义网络连接基类
*/

#ifndef __ZOIC_SESSION_H
#define __ZOIC_SESSION_H

#include "Types.h"
#include <string>
#include <list>
#ifdef HAVE_MINGW32
#include <winsock2.h>
#ifdef _MSC_VER
#pragma comment (lib, "ws2_32.lib")
#endif
#else
#include <netinet/in.h>
#endif
#include "NoCopy.h"
#include "Selectable.h"

namespace Zoic
{
	class Message;
	class SysPingTime;
	class DataBuffer;
	class SessionUnpackMethod;

	class Session;
	//网络连接接受缓冲区处理方案
	class SessionUnpackMethod
	{
		friend class Session;
	public:
		SessionUnpackMethod():recommend_unpack_speed(0),pSession(NULL){};
		virtual ~SessionUnpackMethod(){};
		inline Session* getSession() const{return pSession;};
		//解包函数：返回实际从pLockedBuff内取走的buffer的大小
		//next_unpack_req_buff_size将会告知Session直到剩余缓冲区堆积到该大小之前不再进行unpack
		//(next_unpack_req_buff_size一般用于解决缓冲区内数据未累积到一个完整的包的情况，表示下一个完整的包至少应该具备的大小)
		virtual DWORD unpack(char* pLockedBuff,DWORD size,WORD& next_unpack_req_buff_size) = 0;
		//表示Session关闭，或者启动的信号
		virtual void open(){};
		virtual void close(){};
	public:
		//该参数用于作为解包速度的简易参数使用，便于Session根据需求进行调整，根据继承的情况该值可能无效,该数值可以随时调整
		//该参数是一个基于每次以等同于接收数据相同频率执行的话，每次处理的数据包的字节数,0表示无限制
		DWORD recommend_unpack_speed;
	private:
		Session* pSession;
	};
	/*!
		@brief 网络连接基类
		@ingroup network
	*/
	class Session:
		public NoCopy,
		public Selectable

	{
		friend class SessionBank;
	public:
		Session(SessionUnpackMethod* unpack_method = NULL);
		virtual ~Session();

		//!得到远端IP地址
		const char * getIP() const;

		//!得到远端端口号
		WORD getPort() const;

		//!得到本地IP地址
		const char * getLocalIP() const;

		//!得到本地端口号
		WORD getLocalPort() const;

		//!设置错误号
		void setError(
			int error	/*!< 如果不为0则连接断开<br>
								<0 系统定义错误号<br>
								>0 自定义错误号
							*/
			);
	
		//!得到错误号
		int	getError(void);

		//!打开连接
		virtual bool open(
			SOCKET sock,				//!< 套接字
			const sockaddr_in &addr		//!< 远程地址
			);

		bool isSocketValid() const;

		/*! @brief 接收发送消息例程
		*/
		virtual bool routine();

		//!关闭连接
		virtual void close();

		//!解析消息
		virtual bool parseMessage(
			Message &msg		//!< 网络消息
			);

		//!发送消息
		virtual bool sendMessage(
			const Message &msg	//!< 要发送的消息
			);

		//!得到错误号
		int	getSystemError(void);

		class DefaultUnpackModule
		{
		public:
			DefaultUnpackModule();
			void open(SessionUnpackMethod& method);
			void close();
			DWORD unpack(char* pLockedBuff,DWORD size,WORD& next_unpack_req_buff_size);
		private:
			WORD m_last_imcomplete_packet_len;
			SessionUnpackMethod* m_method;
		};
		SessionUnpackMethod& getUnpackMethod() const;
	protected:
		//!得到发送信息
		const DataBuffer& getSendStatus();
		//!得到接收信息
		const DataBuffer& getRecvStatus();

		//!网络初始化
		void netInitialize();

		//!设置SYSPING时间
		bool setSysPingTime(
			const SysPingTime* sysPingTime	//!< SYSPING时间
			);

		SOCKET getSocket() const;

		//!向发送缓冲区添加数据
		bool pushData(const BYTE *data,WORD size);
		/*! @brief 解包处理

			从接收缓存中解出逻缉包，并调用parseMessage函数
		*/
		void unpack();
		int tryRecvData();
		bool pingCheck();
		int trySendData();

		bool needUnpack() const;

		struct BufferRule
		{
			BufferRule(DWORD init_size,bool dynamic, DWORD m_dynamic_max,bool release_dynamic = false)
				:m_init_buff_size(init_size),m_dynamic(dynamic),m_release_dynamic(release_dynamic),m_max_dynamic_buff_size(m_dynamic_max){};
			DWORD		m_init_buff_size;
			bool		m_dynamic;				//!< 发送缓存不足时是否动态分配
			bool		m_release_dynamic;		//!< 发送缓存是否动态释放
			DWORD		m_max_dynamic_buff_size;
		};
		struct SocketRule
		{
			SocketRule(bool close_no_linger)
				:m_close_no_linger(close_no_linger){};
			bool		m_close_no_linger;
		};
		BufferRule	m_send_buffer_rule;
		BufferRule	m_recv_buffer_rule;
		SocketRule	m_socket_rule;
	
		class DefaultUnpackMethod
			:public SessionUnpackMethod
		{
		public:
			DefaultUnpackMethod();
			virtual void open();
			virtual void close();
			virtual DWORD unpack(char* pLockedBuff,DWORD size,WORD& next_unpack_req_buff_size);
		private:
			DefaultUnpackModule m_module;
		};
		SessionUnpackMethod& m_unpackMethod;				//!< 解包方案
		TIME_MSEL	m_lastReceivedTick;						//!< 最后接收到数据的时间戳
	private:
		//!
		static void clearBuffer(const BufferRule& rule,char*& raw,DataBuffer& buffer);
		static void initBuffer(BufferRule& rule,char*& raw,DataBuffer& buffer);

		//!创建发送与接收缓存
		void tryInitBuffer();
		//!还原buffer
		void tryRevertBuffer();

		//!创建缓存
		static bool createBuffer(
			DWORD & size,			//!< 缓存长度,输入输出
			char * & raw,			//!< 原始数据,输出(用来管理内存分配)
			char* & data			//!< 实际数据,输出(用来管理数据)
			);

		//!增加发送缓存大小
		static bool increaseBuffer(const BufferRule& rule,char*& raw,DataBuffer& buffer);

		static SessionUnpackMethod& selectUnpackMethod(SessionUnpackMethod* unpack_method);

		//! 接收数据
		int recvData();

		//! 发送数据
		int sendData();

		BYTE	m_syspingCount;				//!< SYSPING探测计数

		const SysPingTime* m_syspingTimes;	//!< 系统PING时间和最大次数

		std::string	m_ip;					//!< IP地址
		WORD		m_port;					//!< 端口
		SOCKET		m_socket;				//!< 套接字
		std::string m_local_ip;				//!< 本地IP地址
		WORD		m_local_port;			//!< 本地端口

		char * _m_send_buffer;			//!< 发送缓存,预留Message头
		DataBuffer& m_send_buffer;
		char * _m_recv_buffer;			//!< 接收缓存,预留Message头
		DataBuffer& m_recv_buffer;

		WORD	m_next_unpack_min_length;			//!< 下一次解包的最小数据长度,如果数量不足则放弃本次解包(0会被视作为1)

		int	m_error;								//!< 错误号定义
		int m_systemError;							//!< 当前系统错误号
	};
	
	inline const char * Session::getIP() const
	{
		return m_ip.c_str();
	}
	
	inline WORD Session::getPort() const
	{
		return m_port;
	}

	inline const char * Session::getLocalIP() const
	{
		return m_local_ip.c_str();
	}

	inline WORD Session::getLocalPort() const
	{
		return m_local_port;
	}
	
	inline void Session::setError(int error)
	{
		if(!m_error)
		{
			m_error=error;
		}
	}
	
	inline int Session::getError(void)
	{
		return m_error;
	}

	inline int Session::getSystemError(void)
	{
		return m_systemError;
	}

	inline const DataBuffer& Session::getSendStatus()
	{
		return m_send_buffer;
	}

	inline const DataBuffer& Session::getRecvStatus()
	{
		return m_recv_buffer;
	}

	inline SOCKET Session::getSocket() const
	{
		return m_socket;
	}

	inline SessionUnpackMethod& Session::getUnpackMethod() const
	{
		return m_unpackMethod;
	}

	inline bool Session::isSocketValid() const
	{
		return m_socket!=INVALID_SOCKET;
	}
}

#ifndef ON_MESSAGE2
#define ON_MESSAGE2 template <const WORD cmd> static bool onMessage(Zoic::Session *pSession,Zoic::Message &msg);
#endif

#endif
