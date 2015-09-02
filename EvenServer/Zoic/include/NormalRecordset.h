/*! @file
	@ingroup mysql
	@brief ������ͨ��¼��
*/

#ifndef _ZOIC_NORMALRECORDSET_H
#define _ZOIC_NORMALRECORDSET_H
#include "Recordset.h"

namespace Zoic
{
	class Connection;
	class NormalQuery;

	/*! @brief ��ͨ��¼��
		@ingroup mysql	
	*/
	class NormalRecordset:
		public Recordset
	{
		friend class Connection;
		friend class NormalQuery;	
	public:
		//!��ü�¼
		virtual bool fetch();

	private:
		//!���캯��
		NormalRecordset(
			Connection & conn		//!< Connection����
			);

		virtual ~NormalRecordset();

		//!ĳ�ֶ������Ƿ�����
		static bool isInteger(
			enum_field_types type	//!�ֶ�����
			);

		//!���ý����
		void setResult(
			MYSQL_RES *result	//!< �����
			);
		MYSQL_RES*			m_result;	//!< �����
		MYSQL_FIELD *		m_fields;	//!< �ֶ�����
	};
}

#endif
