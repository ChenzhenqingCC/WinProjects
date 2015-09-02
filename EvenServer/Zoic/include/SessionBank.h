/*! @file
	@ingroup network
	@brief �����������ӹ�����
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
		@brief �������ӹ�����
		@ingroup network
	*/
	class SessionBank:
		public CommandFactory,
		public Selectable
	{
	public:
		SessionBank( MESSAGE_FUNC defaultFunc = CommandFactory::onDummyMessage );
		virtual ~SessionBank();

		//!�㲥��Ϣ
		void broadcast(
			const Zoic::Message &msg	//!< Ҫ�㲥����Ϣ
			);

		//!���ü�����ַ
		void setListenAddress(
			const char *ip,	//!< ����IP��ַ
			WORD port		//!< �����˿ں�
			);

		//!��ü���IP
		const char* getListenIP() const;

		//!��ü���IP
		WORD getListenPort() const;

		//!���ÿ����ӵ��������
		bool setMaxSessions(
			HandleType maxSessions	//!< �����ӵ��������
			);

		//!����,��ȡ������Ϣ,��������
		int  start();

		//!����,�������ӵĽ�����ɾ��������������շ�����
		int	 run();

		//!ֹͣ
		int	 stop();

		/*! @brief ���ݾ�������кŲ���SessionS
			@return ����ɹ�,�����ҵ���SessionSָ�룬���򷵻�NULL
		*/
		SessionS * findSession(
			const HandleType & handle,	//!< ����
			const SerialType &serial	//!< ���к�
			);

		//!��������
		void allowRun();

		const LIST_SESSIONS & getActiveSessions() const;
	protected:
		/*! @brief����Ҫ�����SessionS����
			@return �´�����SessionS����ָ��
		*/
		virtual SessionS * createSession()=0;
		HandleType getMaxSessionCount() const;
		/*! @brief ��û�����б�
			@return ������б�
		*/
		SessionS * const * getSessionHandleTable() const;
		bool addSessionToActiveList(SessionS * pSession);
		bool addSessionToUnusedList(SessionS * pSession);
		bool				m_allowRun;			//!< �Ƿ���������run����
	private:
		void clearSessions();

		virtual void onSessionOpened(SessionS *pSession);
		virtual void onSessionClosed(SessionS *pSession);
		//!�ر�����
		void closeSession(
			SessionS *pSession	//!< ���Ӷ���
			);

		//! ��������
		void recvData();
		
		LIST_SESSIONS		m_unusedSessions;	//!< δ�õ�Session
		LIST_SESSIONS		m_activeSessions;	//!< ���Session		
		SessionS **			m_sessions;			//!< �Session����
		HandleType			m_maxSessions;		//!< ���Session����

		int					m_listenSocket;		//!< �����׽���
		std::string			m_listenIP;			//!< ����IP��ַ
		WORD 				m_listenPort;		//!< �����˿ں�
		SerialType			m_nextSerial;		//!< ��һ�ε����к�
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
