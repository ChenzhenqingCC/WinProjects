/*! @file
	@ingroup mysql
	@brief ������ͨ��ѯ��
*/

#ifndef _ZOIC_NORMALQUERY_H
#define _ZOIC_NORMALQUERY_H
#include <list>
#include "Query.h"
#include "NormalRecordset.h"

namespace Zoic
{
	/*! @brief ��ͨ��ѯ��
		@ingroup mysql	
	*/
	class NormalQuery:
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
		//! ���캯��
		NormalQuery(
			Connection & conn,	//!< ���ݿ����Ӷ���
			const char *sql,	//!< SQL���
			const char *args	//!< �����б�
			);

		virtual ~NormalQuery();

		//!����SQL���
		void makeSQL(
			std::string &str	//!< ������ɵ�SQL���
			);

		//!ִ�в�ѯ����
		bool exec();

		/*! @brief �رղ�ѯ
			
			��Connect::closeLastQuery����
		*/
		void close();

		MYSQL_RES*			m_result;	//!< �����
		NormalRecordset			m_recordset;	//!< ��¼������
	};
}

#endif
