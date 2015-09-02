/*! @file
	@ingroup network
	@brief �����������ӻ���
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
	//�������ӽ��ܻ�����������
	class SessionUnpackMethod
	{
		friend class Session;
	public:
		SessionUnpackMethod():recommend_unpack_speed(0),pSession(NULL){};
		virtual ~SessionUnpackMethod(){};
		inline Session* getSession() const{return pSession;};
		//�������������ʵ�ʴ�pLockedBuff��ȡ�ߵ�buffer�Ĵ�С
		//next_unpack_req_buff_size�����֪Sessionֱ��ʣ�໺�����ѻ����ô�С֮ǰ���ٽ���unpack
		//(next_unpack_req_buff_sizeһ�����ڽ��������������δ�ۻ���һ�������İ����������ʾ��һ�������İ�����Ӧ�þ߱��Ĵ�С)
		virtual DWORD unpack(char* pLockedBuff,DWORD size,WORD& next_unpack_req_buff_size) = 0;
		//��ʾSession�رգ������������ź�
		virtual void open(){};
		virtual void close(){};
	public:
		//�ò���������Ϊ����ٶȵļ��ײ���ʹ�ã�����Session����������е��������ݼ̳е������ֵ������Ч,����ֵ������ʱ����
		//�ò�����һ������ÿ���Ե�ͬ�ڽ���������ͬƵ��ִ�еĻ���ÿ�δ�������ݰ����ֽ���,0��ʾ������
		DWORD recommend_unpack_speed;
	private:
		Session* pSession;
	};
	/*!
		@brief �������ӻ���
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

		//!�õ�Զ��IP��ַ
		const char * getIP() const;

		//!�õ�Զ�˶˿ں�
		WORD getPort() const;

		//!�õ�����IP��ַ
		const char * getLocalIP() const;

		//!�õ����ض˿ں�
		WORD getLocalPort() const;

		//!���ô����
		void setError(
			int error	/*!< �����Ϊ0�����ӶϿ�<br>
								<0 ϵͳ��������<br>
								>0 �Զ�������
							*/
			);
	
		//!�õ������
		int	getError(void);

		//!������
		virtual bool open(
			SOCKET sock,				//!< �׽���
			const sockaddr_in &addr		//!< Զ�̵�ַ
			);

		bool isSocketValid() const;

		/*! @brief ���շ�����Ϣ����
		*/
		virtual bool routine();

		//!�ر�����
		virtual void close();

		//!������Ϣ
		virtual bool parseMessage(
			Message &msg		//!< ������Ϣ
			);

		//!������Ϣ
		virtual bool sendMessage(
			const Message &msg	//!< Ҫ���͵���Ϣ
			);

		//!�õ������
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
		//!�õ�������Ϣ
		const DataBuffer& getSendStatus();
		//!�õ�������Ϣ
		const DataBuffer& getRecvStatus();

		//!�����ʼ��
		void netInitialize();

		//!����SYSPINGʱ��
		bool setSysPingTime(
			const SysPingTime* sysPingTime	//!< SYSPINGʱ��
			);

		SOCKET getSocket() const;

		//!���ͻ������������
		bool pushData(const BYTE *data,WORD size);
		/*! @brief �������

			�ӽ��ջ����н���߼�����������parseMessage����
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
			bool		m_dynamic;				//!< ���ͻ��治��ʱ�Ƿ�̬����
			bool		m_release_dynamic;		//!< ���ͻ����Ƿ�̬�ͷ�
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
		SessionUnpackMethod& m_unpackMethod;				//!< �������
		TIME_MSEL	m_lastReceivedTick;						//!< �����յ����ݵ�ʱ���
	private:
		//!
		static void clearBuffer(const BufferRule& rule,char*& raw,DataBuffer& buffer);
		static void initBuffer(BufferRule& rule,char*& raw,DataBuffer& buffer);

		//!������������ջ���
		void tryInitBuffer();
		//!��ԭbuffer
		void tryRevertBuffer();

		//!��������
		static bool createBuffer(
			DWORD & size,			//!< ���泤��,�������
			char * & raw,			//!< ԭʼ����,���(���������ڴ����)
			char* & data			//!< ʵ������,���(������������)
			);

		//!���ӷ��ͻ����С
		static bool increaseBuffer(const BufferRule& rule,char*& raw,DataBuffer& buffer);

		static SessionUnpackMethod& selectUnpackMethod(SessionUnpackMethod* unpack_method);

		//! ��������
		int recvData();

		//! ��������
		int sendData();

		BYTE	m_syspingCount;				//!< SYSPING̽�����

		const SysPingTime* m_syspingTimes;	//!< ϵͳPINGʱ���������

		std::string	m_ip;					//!< IP��ַ
		WORD		m_port;					//!< �˿�
		SOCKET		m_socket;				//!< �׽���
		std::string m_local_ip;				//!< ����IP��ַ
		WORD		m_local_port;			//!< ���ض˿�

		char * _m_send_buffer;			//!< ���ͻ���,Ԥ��Messageͷ
		DataBuffer& m_send_buffer;
		char * _m_recv_buffer;			//!< ���ջ���,Ԥ��Messageͷ
		DataBuffer& m_recv_buffer;

		WORD	m_next_unpack_min_length;			//!< ��һ�ν������С���ݳ���,�������������������ν��(0�ᱻ����Ϊ1)

		int	m_error;								//!< ����Ŷ���
		int m_systemError;							//!< ��ǰϵͳ�����
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
