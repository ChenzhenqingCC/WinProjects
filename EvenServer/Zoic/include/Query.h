/*! @file
	@ingroup mysql
	@brief 定义数据库查询类
*/

#ifndef _ZOIC_QUERY_H
#define _ZOIC_QUERY_H
#include <string>
#include <vector>
#include "Types.h"
#include "Recordset.h"
#include "Connection.h"
#include "ParameterValue.h"
#include "NoCopy.h"

namespace Zoic
{
	class Connection;
	class Recordset;
	/*! @brief 数据库查询类
		@ingroup mysql	
	*/
	class Query:
		public NoCopy
	{
		friend class Connection;
	public:
		//!参数类型
		struct ParameterType
		{
			std::string name;	//!< 参数名称
			enum TYPE
			{
				T_CHAR,
				T_VARCHAR,
				T_BINARY,
				T_VARBINARY,
				T_TINYBLOB,
				T_TINYTEXT,
				T_BLOB,
				T_TEXT,
				T_MEDIUMBLOB,
				T_MEDIUMTEXT,
				T_LONGBLOB,
				T_LONGTEXT,
				T_BIT,
				T_BOOL,
				T_TINYINT,
				T_SMALLINT,
				T_MEDIUMINT,
				T_INT,
				T_BIGINT,
				T_FLOAT,
				T_DOUBLE,
				T_DECIMAL,
				T_DATE,
				T_TIME,
				T_DATETIME,
				T_TIMESTAMP
			} type;		//!< 参数类型
			bool sign;	//!< 符号标成(true:有符号;false:无符号)
		};

		//!仅执行SQL语句
		virtual bool execute()=0;

		//!使用Use的方式保存结果集
		virtual Recordset * use()=0;

		//!使用store的方式保存结果集
		virtual Recordset * store()=0;

		//!得到影响行数
		virtual unsigned long long getAffactedRows()=0;

		//!得到新插入记录的ID
		virtual unsigned long long getInsertID()=0;

		//!得到错误消息
		virtual const char * error()=0;
		
		//@{
		/*! @brief 输出操作
			@param arg 输出参数
		*/
		Query & operator << ( const char & arg );
		Query & operator << ( const unsigned char & arg );
		Query & operator << ( const short & arg );
		Query & operator << ( const unsigned short & arg );
		Query & operator << ( const int & arg );
		Query & operator << ( const unsigned int & arg );
		Query & operator << ( const long & arg );
		Query & operator << ( const unsigned long & arg );
		Query & operator << ( const long long & arg );
		Query & operator << ( const unsigned long long & arg );
		Query & operator << ( const char * arg );
		Query & operator << ( const std::string & arg );
		Query & operator << ( const float & arg);
		Query & operator << ( const double & arg);
		Query & operator << ( const Date & arg);
		Query & operator << ( const Time & arg);
		Query & operator << ( const DateTime & arg);
		Query & operator << ( const TinyBinary & arg);
		//@}
	protected:
		//!构造函数
		Query(
			Connection & conn	//!< 数据库连接对象
			);
		virtual ~Query();

		//!获得MYSQL对象
		MYSQL * getMySQL();

		//!解板参数
		void parseArgTypes(
			const char *args	//!< 查询参数
			);

		/*! @brief 查找参数
			@retval >=0 参数位置索引
			@retval <0 没找到
		*/
		int findParameter(
			const char *name	//!< 参数名称
			);

		//!解析参数类型
		static void parseArgTypes(
			const char *args,					//!< 参数列表
			std::vector<ParameterType> &types	//!< 存放返回的参数类型
			);

		//! 解析符号属性
		static bool parseSign(
			const char * str_len,	//!< 长度字符串
			const char *separator,	//!< 要隔符
			bool & sign				//!< 有无符号
			);

		ParameterType * m_paramTypes;	//!< 参数类型数组
		struct QueryElement
		{
			enum
			{
				T_TEXT,		//!< 文本类型，不作任何替换
				T_PARAM		//!< 用参数替换
			} type;
			union
			{
				struct		
				{
					const char * begin;	//!< 开始文本地址
					int len;			//!< 文本长度
				} text;
				struct
				{
					int index;			//!< 参数位置索引
				} param;
			} element;
		};
		//解析元素
		void parseElements(
			const char *sql		//!< SQL语句
			);

		std::list<QueryElement> m_queryElements;	//!< 解析后元素列表
		ParameterValue  * m_args;					//!< 当前参数数组
		WORD m_argIndex;							//!< 参数索引
		WORD m_argCount;							//!< 参数个数
		Connection & m_conn;						//!< 连接对象
	private:
		/*! @brief 关闭查询
			
			由Connect::closeLastQuery调用
		*/
		virtual void close()=0;
	};
	
	inline Query & Query::operator << ( const char & arg )
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
	
	inline Query & Query::operator << ( const unsigned char & arg )
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
	
	inline Query & Query::operator << ( const short & arg )
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
	
	inline Query & Query::operator << ( const unsigned short & arg )
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
	
	inline Query & Query::operator << ( const int & arg )
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
	
	inline Query & Query::operator << ( const unsigned int & arg )
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
	
	inline Query & Query::operator << ( const long & arg )
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
		
	inline Query & Query::operator << ( const unsigned long & arg )
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
		
	inline Query & Query::operator << ( const long long & arg )
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
	
	inline Query & Query::operator << ( const unsigned long long & arg )
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
	
	inline Query & Query::operator << ( const char * arg )
	{
		m_args[m_argIndex++].setValue(arg,static_cast<unsigned int>(strlen(arg)));
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
	
	inline Query & Query::operator << ( const std::string & arg )
	{
		m_args[m_argIndex++].setValue(arg.c_str(),static_cast<unsigned int>(arg.length()));
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
		
	inline Query & Query::operator << ( const float & arg)
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
		
	inline Query & Query::operator << ( const double & arg)
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
		
	inline Query & Query::operator << ( const Date & arg)
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
		
	inline Query & Query::operator << ( const Time & arg)
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
		
	inline Query & Query::operator << ( const DateTime & arg)
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
	
	inline Query & Query::operator << ( const TinyBinary & arg)
	{
		m_args[m_argIndex++].setValue(reinterpret_cast<const char*>(arg.getData()), arg.getSize());
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}

	inline MYSQL * Query::getMySQL()
	{
		return m_conn.getMySQL();
	}
}
	
#endif
