/*! @file
	@ingroup mysql
	@brief ����Ԥ�����¼��
*/

#ifndef _ZOIC_STMTRECORDSET_H
#define _ZOIC_STMTRECORDSET_H
#include "Recordset.h"
#include "ParameterValue.h"

namespace Zoic
{
	class StmtQuery;
	/*! @brief Ԥ�����¼��
		@ingroup mysql	
	*/
	class StmtRecordset:
		public Recordset
	{
		friend class StmtQuery;	
	public:
		//!��ü�¼
		virtual bool fetch();
	private:
		//���캯��
		StmtRecordset(
			Connection & conn		//!< Connection����
			);
		virtual ~StmtRecordset();

		//!ĳ�ֶ������Ƿ�����
		static bool isInteger(
			enum_field_types type	//!�ֶ�����
			);

		//!���ý����
		void setResult(
			MYSQL_STMT *stmt	//!< �����
			);

		//!�󶨲���
		void bindParameters(
			MYSQL_STMT *stmt	//!< Ԥ�������
			);

		//!���ð󶨲���
		static void setupBindParameter(
			const MYSQL_FIELD & field,	//!< �ֶνṹ
			ParameterValue &value,		//!< ����ֵ
			MYSQL_BIND &bind			//!< �󶨽ṹ
			);

		MYSQL_FIELD *		m_fields;	//!< �ֶ�����
		MYSQL_STMT *		m_stmt;		//!< Ԥ�������
		MYSQL_BIND *		m_output;	//!< �������
	};
}

#endif
