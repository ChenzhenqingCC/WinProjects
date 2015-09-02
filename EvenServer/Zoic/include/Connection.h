/*! @file
	@ingroup mysql
	@brief �������ݿ�������
*/

#ifndef _ZOIC_CONNECTION_H
#define _ZOIC_CONNECTION_H
#include <string>
#include <list>
#include "Types.h"
#include "NoCopy.h"
#include <Zoic/mysql/include/mysql.h>

#ifdef HAVE_MINGW32
#ifdef _MSC_VER
#pragma comment (lib, "ws2_32.lib")
#endif
#endif

namespace Zoic
{
	class Query;

	/*! @brief ���ݿ�������
		@ingroup mysql
	*/
	class Connection:
		public NoCopy
	{
		friend class Query;
	public:
		//!�����Ƿ�Ͽ�
		bool isDead();
		//!�������ӶϿ�״̬
		void dead();

		/*! @brief �������һ������

			��Query��Ĳ�ѯ���ݿ�ӿ�ʹ��
		*/
		void setLastQuery(
			Query * query
			);

		/*! @brief �ر����һ�β�ѯ

			�� Recordset::close �� Query��Ĳ�ѯ���ݿ�ӿ�ʹ��
		*/
		void closeLastQuery();

		//! ���MYSQL ����ָ��
		MYSQL * getMySQL();
	protected:
		Connection();
		virtual ~Connection();
	
		//!�������Ӳ���
		void setConnectParams(
			const char* db = "",	//!< ���ݿ���
			const char* host = "",	//!< ��������ַ
			const char* user = "",	//!< �����˺�
			const char* pass = "",	//!< ����
			WORD port=0);			//!< �˿ں�
		
		//!���ӵ����ݿ�
		bool connect();

		//!�ر������ݿ������
		void close();

		//!pingһ�����ݿ�,��������
		void pingDB(TIME_MSEL curTime);

		//!��ô�����Ϣ
		const char* error();
		
		MYSQL m_mysql;			//!< MYSQL����
		bool m_isConnected;		//!< �����ӱ�־
		bool m_dead;			//!< �����쳣״̬
		std::string m_db;		//!< ���ݿ���
		std::string m_host;		//!< ��������ַ
		std::string m_user;		//!< �����˺�
		std::string m_pass;		//!< ����
		WORD m_port;			//!< �˿ں�
		my_bool m_reconnect;	//!< �Ƿ���������
	private:
		//!������ѯ����
		void createQueries();

		//!��ô洢��������
		bool getProcedureContent(
			const char *name,				//!< �洢��������
			std::string &content			//!< �洢��������
			);

		//!���洢����
		static void parseProcedure(
			const char *name,				//!< �洢��������
			const std::string &content,		//!< �洢��������
			std::string &body,				//!< ���ش洢����ִ�д���
			std::string &args				//!< ���ش洢���̲����б�
			);

		Query *				m_lastQuery;	//!< ���һ������
		std::list<Query *>	m_querys;		//!< ��ѯ�б�
		TIME_MSEL			m_nextPingTime;	//!< ��һ��pingʱ��
	};
	
	inline bool Connection::isDead()
	{
		return m_dead;
	}
	
	inline void Connection::dead()
	{
		m_dead=true;
	}
	
	inline const char* Connection::error()
	{
		return mysql_error(&m_mysql);
	}

	inline MYSQL * Connection::getMySQL()
	{
		return &m_mysql;
	}
}

#endif
