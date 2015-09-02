#include <sstream>
#include <map>
#include "include/NormalQuery.h"
#include "include/Log.h"

using namespace Zoic;

NormalQuery::NormalQuery(Connection & conn,const char *sql,const char *args):
	Query(conn),
	m_recordset(conn)
{
	parseArgTypes(args);
	parseElements(sql);
}

NormalQuery::~NormalQuery()
{
}
	
bool NormalQuery::execute()
{	
	if(exec())
	{
		MYSQL_RES *res;
		do
		{
  			res=mysql_use_result(getMySQL());
  			if(res)
  			{
  				mysql_free_result(res);
  			}
  		}
		while (!mysql_next_result(getMySQL()));
		return true;
	}
	else
	{
		return false;
	}	
}

Recordset * NormalQuery::use()
{
	MYSQL_RES* res=NULL;
	if(!exec())
	{
		m_recordset.setResult(NULL);
		return NULL;
	}
	do
	{
		res=mysql_use_result(getMySQL());
		if(res)
		{
			break;
		}
	}
	while (!mysql_next_result(getMySQL()));

	m_recordset.setResult(res);
	return &m_recordset;
}

Recordset * NormalQuery::store()
{
	MYSQL_RES* res=NULL;
	if(!exec())
	{
		m_recordset.setResult(NULL);
		return NULL;
	}
	do
	{
		res=mysql_store_result(getMySQL());
		if(res)
		{
			break;
		}
	}
	while (!mysql_next_result(getMySQL()));
	m_recordset.setResult(res);
	return &m_recordset;
}

struct block
{
	char *	str;
	int		len;
};

void NormalQuery::makeSQL(std::string &str)
{
	std::map<BYTE,block> mapStringParams;
	for(std::list<QueryElement>::iterator it=m_queryElements.begin();it!=m_queryElements.end();++it)
	{
		if(it->type==QueryElement::T_TEXT)
		{
			str.append(it->element.text.begin,it->element.text.len);
		}
		else
		{
			ParameterValue & value=m_args[it->element.param.index];
			if(value.type==ParameterValue::T_STRING)
			{//在字符串的前后加上',并转码
				block blk;
				std::map<BYTE,block>::iterator it_block=mapStringParams.find(it->element.param.index);
				char *tmp_buf=NULL;	//临时内存
				if(it_block!=mapStringParams.end())
				{
					blk=it_block->second;
				}
				else
				{
					blk.str=tmp_buf=new char[2*value.length+3];
					blk.str[0]='\'';
					int len=mysql_real_escape_string(
						getMySQL(),
						blk.str+1,
						value.data._string.buffer,
						value.length);
					blk.str[len+1]='\'';
					blk.str[len+2]='\0';
					blk.len=len+2;
				}
				str.append(blk.str,blk.len);
				if(tmp_buf)
				{
					delete [] tmp_buf;
				}
			}
			else
			{
				bool timeType=( value.type==ParameterValue::T_DATE
					|| value.type==ParameterValue::T_TIME
					|| value.type==ParameterValue::T_DATETIME);
				if(timeType)
				{
					str.append("\'");
				}
				std::string param_value;
				value.getValue(param_value);
				str.append(param_value);
				if(timeType)
				{
					str.append("\'");
				}
			}
		}
	}
	for(std::map<BYTE,block>::iterator it_block=mapStringParams.begin();it_block!=mapStringParams.end();++it_block)
	{
		delete [] it_block->second.str;
	}
	m_argIndex=0;
}

unsigned long long NormalQuery::getAffactedRows()
{
	return mysql_affected_rows(getMySQL());
}

unsigned long long NormalQuery::getInsertID()
{
	return mysql_insert_id(getMySQL());
}

const char * NormalQuery::error()
{
	return mysql_error(getMySQL());
}

bool NormalQuery::exec()
{
	if(m_conn.isDead())
	{
		return false;
	}

	std::string str;
	makeSQL(str);

	m_conn.closeLastQuery();
	if(mysql_real_query(getMySQL(),str.c_str(),static_cast<unsigned long>(str.length())))
	{
		flog_f(Zoic::ERROR_LOG_LEVEL,"mysql",".log","norl error[%d]:%s\n\t%s",mysql_errno(getMySQL()),mysql_error(getMySQL()),str.c_str());
		if(!getMySQL()->net.vio)
		{
			m_conn.dead();
		}
		return false;
	}
	else
	{
		m_conn.setLastQuery(this);
		return true;
	}
}

void NormalQuery::close()
{
	MYSQL_RES* res=m_recordset.m_result;
	if(res==NULL)
	{
		return;
	}
	m_recordset.setResult(NULL);
	do
	{
		res=mysql_use_result(getMySQL());
		if(res)
		{
			mysql_free_result(res);
		}
	}
	while (!mysql_next_result(getMySQL()));
}
