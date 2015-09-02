/*! @file
	@ingroup mysql
	@brief 定义普通查询类
*/

#ifndef _ZOIC_NORMALQUERY_H
#define _ZOIC_NORMALQUERY_H
#include <list>
#include "Query.h"
#include "NormalRecordset.h"

namespace Zoic
{
	/*! @brief 普通查询类
		@ingroup mysql	
	*/
	class NormalQuery:
		public Query
	{
		friend class Connection;
	public:
		//!仅执行SQL语句
		virtual bool execute();

		//!使用Use的方式保存结果集
		virtual Recordset * use();

		//!使用store的方式保存结果集
		virtual Recordset * store();

		//!得到影响行数
		virtual unsigned long long getAffactedRows();

		//!得到新插入记录的ID
		virtual unsigned long long getInsertID();

		//!得到错误消息
		virtual const char * error();
	private:
		//! 构造函数
		NormalQuery(
			Connection & conn,	//!< 数据库连接对象
			const char *sql,	//!< SQL语句
			const char *args	//!< 参数列表
			);

		virtual ~NormalQuery();

		//!生成SQL语句
		void makeSQL(
			std::string &str	//!< 存放生成的SQL语句
			);

		//!执行查询命令
		bool exec();

		/*! @brief 关闭查询
			
			由Connect::closeLastQuery调用
		*/
		void close();

		MYSQL_RES*			m_result;	//!< 结果集
		NormalRecordset			m_recordset;	//!< 记录集对象
	};
}

#endif
