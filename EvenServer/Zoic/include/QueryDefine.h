/*! @file
	@ingroup mysql
	@brief 定义数据库查询宏
*/

#ifndef __ZOIC_QUERYDEFINE_H
#define __ZOIC_QUERYDEFINE_H

#include <list>
#include <string>
#include "ClassStorage.h"

namespace Zoic
{
	class Query;
	
	struct QueryItem
	{
		Query **	query;	//查询对象对象指针
		std::string sql;	//查询SQL语句
		std::string args;	//参数列表
	};
	
	typedef std::list<QueryItem> QueryItems;
	typedef ClassStorage<QueryItems> QueryStorage;
	
	inline QueryStorage & getQueryStorage()
	{
		static QueryStorage storage;
		return storage;
	}

	/*! @brief 数据库查询定义模板
		@ingroup mysql
	*/
	template<class T> 
	class QueryDefine
	{
	public:
		QueryDefine(
			Query ** query,		//存放生成的查询对象指针
			const char *sql,	//SQL语句
			const char *args	//查询参数列表
			);
	};
	
	template<class T>
	QueryDefine<T>::QueryDefine(Query ** query,const char *sql,const char *args)
	{
		QueryItems & items=getQueryStorage().getClassInfo(typeid(T));
		QueryItem item;
		item.query=query;
		item.sql=sql;
		item.args=args;
		items.push_back(item);
	}
}

/*! @brief 数据库查询定义
	@ingroup mysql
	@param CLASS 数据库连接类名
	@param NAME 查询名称
	@param SQL SQL语句
	@param ARGS 参数列表
*/
#define QUERY_DEFINE(CLASS,NAME,SQL,ARGS)	\
	Zoic::Query * NAME=NULL; \
	static Zoic::QueryDefine<CLASS> _##CLASS##_##NAME(&NAME,SQL,ARGS);
/*! @brief 数据库存储过程定义
	@ingroup mysql
	@param CLASS 数据库连接类名
	@param NAME 存储过程名称
*/
#define QUERY_PROC(CLASS,NAME)	\
	Zoic::Query * NAME=NULL; \
	static Zoic::QueryDefine<CLASS> _##CLASS##_##NAME(&NAME,"CALL "#NAME,"");

#endif
