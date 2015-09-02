/*! @file
	@ingroup mysql
	@brief 定义数据库连接类
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

	/*! @brief 数据库连接类
		@ingroup mysql
	*/
	class Connection:
		public NoCopy
	{
		friend class Query;
	public:
		//!连接是否断开
		bool isDead();
		//!设置连接断开状态
		void dead();

		/*! @brief 设置最后一次连接

			由Query类的查询数据库接口使用
		*/
		void setLastQuery(
			Query * query
			);

		/*! @brief 关闭最后一次查询

			由 Recordset::close 或 Query类的查询数据库接口使用
		*/
		void closeLastQuery();

		//! 获得MYSQL 对象指针
		MYSQL * getMySQL();
	protected:
		Connection();
		virtual ~Connection();
	
		//!设置连接参数
		void setConnectParams(
			const char* db = "",	//!< 数据库名
			const char* host = "",	//!< 服务器地址
			const char* user = "",	//!< 访问账号
			const char* pass = "",	//!< 密码
			WORD port=0);			//!< 端口号
		
		//!连接到数据库
		bool connect();

		//!关闭与数据库的连接
		void close();

		//!ping一次数据库,保持连接
		void pingDB(TIME_MSEL curTime);

		//!获得错误信息
		const char* error();
		
		MYSQL m_mysql;			//!< MYSQL对象
		bool m_isConnected;		//!< 已连接标志
		bool m_dead;			//!< 连接异常状态
		std::string m_db;		//!< 数据库名
		std::string m_host;		//!< 服务器地址
		std::string m_user;		//!< 访问账号
		std::string m_pass;		//!< 密码
		WORD m_port;			//!< 端口号
		my_bool m_reconnect;	//!< 是否重新连接
	private:
		//!创建查询对象
		void createQueries();

		//!获得存储过程内容
		bool getProcedureContent(
			const char *name,				//!< 存储过程名称
			std::string &content			//!< 存储过程内容
			);

		//!解板存储过程
		static void parseProcedure(
			const char *name,				//!< 存储过程名称
			const std::string &content,		//!< 存储过程内容
			std::string &body,				//!< 返回存储过程执行代码
			std::string &args				//!< 返回存储过程参数列表
			);

		Query *				m_lastQuery;	//!< 最后一次连接
		std::list<Query *>	m_querys;		//!< 查询列表
		TIME_MSEL			m_nextPingTime;	//!< 下一次ping时间
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
