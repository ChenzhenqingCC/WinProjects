/*! @file
	@ingroup mysql
	@brief �������ݿ��ѯ��
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
		Query **	query;	//��ѯ�������ָ��
		std::string sql;	//��ѯSQL���
		std::string args;	//�����б�
	};
	
	typedef std::list<QueryItem> QueryItems;
	typedef ClassStorage<QueryItems> QueryStorage;
	
	inline QueryStorage & getQueryStorage()
	{
		static QueryStorage storage;
		return storage;
	}

	/*! @brief ���ݿ��ѯ����ģ��
		@ingroup mysql
	*/
	template<class T> 
	class QueryDefine
	{
	public:
		QueryDefine(
			Query ** query,		//������ɵĲ�ѯ����ָ��
			const char *sql,	//SQL���
			const char *args	//��ѯ�����б�
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

/*! @brief ���ݿ��ѯ����
	@ingroup mysql
	@param CLASS ���ݿ���������
	@param NAME ��ѯ����
	@param SQL SQL���
	@param ARGS �����б�
*/
#define QUERY_DEFINE(CLASS,NAME,SQL,ARGS)	\
	Zoic::Query * NAME=NULL; \
	static Zoic::QueryDefine<CLASS> _##CLASS##_##NAME(&NAME,SQL,ARGS);
/*! @brief ���ݿ�洢���̶���
	@ingroup mysql
	@param CLASS ���ݿ���������
	@param NAME �洢��������
*/
#define QUERY_PROC(CLASS,NAME)	\
	Zoic::Query * NAME=NULL; \
	static Zoic::QueryDefine<CLASS> _##CLASS##_##NAME(&NAME,"CALL "#NAME,"");

#endif
