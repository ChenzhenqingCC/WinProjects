/*! @file
	@ingroup mysql
	@brief 定义预处理查询类
*/

#ifndef _ZOIC_STMTQUERY_H
#define _ZOIC_STMTQUERY_H
#include <list>
#include "Query.h"
#include "StmtRecordset.h"

namespace Zoic
{
	/*! @brief 预处理查询类
		@ingroup mysql	
	*/
	class StmtQuery:
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
		//!构造函数
		StmtQuery(
			Connection & conn,	//!< 数据库连接对象
			const char *sql,	//!< SQL语句
			const char *args	//!< 参数列表
			);

		virtual ~StmtQuery();

		//!生成预处理SQL语句
		void makePrepareSQL(
			std::string &str	//!< 存放生成的SQL预处理语句
			);

		//!设置全部绑定参数
		void setupBindParamters();

		//!设置绑定参数
		static void setupBindParamter(
			const ParameterType & paramType,	//!< 参数类型
			ParameterValue & paramValue,		//!< 参数值
			MYSQL_BIND & bind);					//!< 绑定结构

		//!绑定输入参数
		void bindInputParamters();

		//!执行查询命令
		bool exec();

		//!清除数据
		void clear();

		//!重建Stml
		void rebuildStmt(
			int err		//!< 上一次错误号
			);

		//!重新执行
		bool reexec(
			int err		//!< 上一次错误号
			);

		/*! @brief 关闭查询
			
			由Connect::closeLastQuery调用
		*/
		void close();

		StmtRecordset			m_recordset;	//!< 记录集对象
		MYSQL_STMT *			m_stmt;			//!< 预处理句柄
		MYSQL_BIND *			m_input;		//!< 输入参数
		WORD					m_inputCount;	//!< 输入参数个数
		WORD *					m_inputIndices;	//!< 输入参数对应于m_paramTypes的索引,有可能同一个参数在SQL中出现多次
		std::string 			m_prepareSQL;	//!< 预处理SQL语句
	};
}

#endif
