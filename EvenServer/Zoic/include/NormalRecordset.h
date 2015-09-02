/*! @file
	@ingroup mysql
	@brief 定义普通记录类
*/

#ifndef _ZOIC_NORMALRECORDSET_H
#define _ZOIC_NORMALRECORDSET_H
#include "Recordset.h"

namespace Zoic
{
	class Connection;
	class NormalQuery;

	/*! @brief 普通记录类
		@ingroup mysql	
	*/
	class NormalRecordset:
		public Recordset
	{
		friend class Connection;
		friend class NormalQuery;	
	public:
		//!获得记录
		virtual bool fetch();

	private:
		//!构造函数
		NormalRecordset(
			Connection & conn		//!< Connection对象
			);

		virtual ~NormalRecordset();

		//!某字段类型是否整型
		static bool isInteger(
			enum_field_types type	//!字段类型
			);

		//!设置结果集
		void setResult(
			MYSQL_RES *result	//!< 结果集
			);
		MYSQL_RES*			m_result;	//!< 结果集
		MYSQL_FIELD *		m_fields;	//!< 字段数组
	};
}

#endif
