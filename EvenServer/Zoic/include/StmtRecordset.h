/*! @file
	@ingroup mysql
	@brief 定义预处理记录类
*/

#ifndef _ZOIC_STMTRECORDSET_H
#define _ZOIC_STMTRECORDSET_H
#include "Recordset.h"
#include "ParameterValue.h"

namespace Zoic
{
	class StmtQuery;
	/*! @brief 预处理记录类
		@ingroup mysql	
	*/
	class StmtRecordset:
		public Recordset
	{
		friend class StmtQuery;	
	public:
		//!获得记录
		virtual bool fetch();
	private:
		//构造函数
		StmtRecordset(
			Connection & conn		//!< Connection对象
			);
		virtual ~StmtRecordset();

		//!某字段类型是否整型
		static bool isInteger(
			enum_field_types type	//!字段类型
			);

		//!设置结果集
		void setResult(
			MYSQL_STMT *stmt	//!< 结果集
			);

		//!绑定参数
		void bindParameters(
			MYSQL_STMT *stmt	//!< 预处理对象
			);

		//!设置绑定参数
		static void setupBindParameter(
			const MYSQL_FIELD & field,	//!< 字段结构
			ParameterValue &value,		//!< 参数值
			MYSQL_BIND &bind			//!< 绑定结构
			);

		MYSQL_FIELD *		m_fields;	//!< 字段数组
		MYSQL_STMT *		m_stmt;		//!< 预处理对象
		MYSQL_BIND *		m_output;	//!< 输出参数
	};
}

#endif
