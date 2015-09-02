/*! @file
	@ingroup mysql
	@brief ����Ԥ�����ѯ��
*/

#ifndef _ZOIC_STMTQUERY_H
#define _ZOIC_STMTQUERY_H
#include <list>
#include "Query.h"
#include "StmtRecordset.h"

namespace Zoic
{
	/*! @brief Ԥ�����ѯ��
		@ingroup mysql	
	*/
	class StmtQuery:
		public Query
	{
		friend class Connection;
	public:
		//!��ִ��SQL���
		virtual bool execute();

		//!ʹ��Use�ķ�ʽ��������
		virtual Recordset * use();

		//!ʹ��store�ķ�ʽ��������
		virtual Recordset * store();

		//!�õ�Ӱ������
		virtual unsigned long long getAffactedRows();

		//!�õ��²����¼��ID
		virtual unsigned long long getInsertID();

		//!�õ�������Ϣ
		virtual const char * error();
	private:
		//!���캯��
		StmtQuery(
			Connection & conn,	//!< ���ݿ����Ӷ���
			const char *sql,	//!< SQL���
			const char *args	//!< �����б�
			);

		virtual ~StmtQuery();

		//!����Ԥ����SQL���
		void makePrepareSQL(
			std::string &str	//!< ������ɵ�SQLԤ�������
			);

		//!����ȫ���󶨲���
		void setupBindParamters();

		//!���ð󶨲���
		static void setupBindParamter(
			const ParameterType & paramType,	//!< ��������
			ParameterValue & paramValue,		//!< ����ֵ
			MYSQL_BIND & bind);					//!< �󶨽ṹ

		//!���������
		void bindInputParamters();

		//!ִ�в�ѯ����
		bool exec();

		//!�������
		void clear();

		//!�ؽ�Stml
		void rebuildStmt(
			int err		//!< ��һ�δ����
			);

		//!����ִ��
		bool reexec(
			int err		//!< ��һ�δ����
			);

		/*! @brief �رղ�ѯ
			
			��Connect::closeLastQuery����
		*/
		void close();

		StmtRecordset			m_recordset;	//!< ��¼������
		MYSQL_STMT *			m_stmt;			//!< Ԥ������
		MYSQL_BIND *			m_input;		//!< �������
		WORD					m_inputCount;	//!< �����������
		WORD *					m_inputIndices;	//!< ���������Ӧ��m_paramTypes������,�п���ͬһ��������SQL�г��ֶ��
		std::string 			m_prepareSQL;	//!< Ԥ����SQL���
	};
}

#endif
