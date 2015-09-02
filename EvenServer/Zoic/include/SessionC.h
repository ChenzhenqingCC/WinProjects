/*! @file
	@ingroup network
	@brief �����������ӿͻ���
*/

#ifndef __ZOIC_SESSIONC_H
#define __ZOIC_SESSIONC_H

#include "Session.h"
#include "CommandFactory.h"
#include <string>


namespace Zoic
{
	/*! @brief �������ӿͻ���
		@ingroup network


		�������������ӵ�һ��
	*/
	class SessionC:
		public Session,
		public CommandFactory
	{
	public:
		SessionC( MESSAGE_FUNC defaultFunc = CommandFactory::onDummyMessage );
		~SessionC();
		//!�������ӵ�ַ
		void setConnectAddress(
			const char *ip,	//!< Ҫ���ӵķ�����IP��ַ
			WORD port		//!< Ҫ���ӵķ������˿ں�
			);

		//!���ñ��ص�ַ
		void setLocalAddress(
			const char *ip,	//!< ����IP��ַ
			WORD port		//!< ���ض˿ں�
			);

		//!����,���ӵ�������
		int start();

		//!����,�շ�����
		int	 run();

		//!ֹͣ
		int	 stop();

		//!������Ϣ
		bool parseMessage(
			Message &msg	//!< ������Ϣ
			);

		/*! @brief ���ӵ�������
			@retval 0 �ɹ�
			@retval ���� ʧ��
		*/
		int connect(
			DWORD ip,		//!< Ҫ���ӵķ�����IP��ַ
			WORD port,		//!< Ҫ���ӵķ������˿ں�
			BYTE timeout=5	//!< ��ʱֵ(��)
			);


		/*! @brief �Ƿ����ӳɹ�
			@retval 0 �ɹ�
			@retval <0 ʧ��
			@retval >0 �ȴ���
		*/
		int isConnectSuccess(void);

		//!�ر�����
		void close();
	protected:
		void initForConnect();
		int stopConnecting(void);				//��ֹ���ڽ��е�����
		bool				m_opened;			//!< �Ƿ������
	private:
		/*! @brief �������״̬
			@retval 0 �ɹ�
			@retval <0 ʧ��
			@retval >0 �ȴ���
		*/
		int					getConnectStatus();

		std::string			m_connectIP;		//!< ���ӵ�IP��ַ
		WORD 				m_connectPort;		//!< ���ӵĶ˿ں�
		SOCKET				m_connectSocket;	//!< �����׽���
		TIME_MSEL			m_connectTimeOut;	//!< ���ӳ�ʱʱ��
		std::string			m_localIP;			//!< ����IP��ַ
		WORD				m_localPort;		//!< ���ض˿ں�
		sockaddr_in			m_saServer;			//!< Զ�̷����������ַ
	};
}

#endif
