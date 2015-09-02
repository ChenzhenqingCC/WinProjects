#include <sstream>
#include <map>
#include <vector>
#include <mysqld_error.h>
#include <errmsg.h>
#include "include/StmtQuery.h"
#include "include/Exception.h"
#include "include/Log.h"

using namespace Zoic;

StmtQuery::StmtQuery(Connection & conn,const char *sql,const char *args):
	Query(conn),
	m_recordset(conn),
	m_input(NULL),
	m_inputIndices(NULL),
	m_stmt(NULL),
	m_inputCount(0)
{
	try
	{
		m_stmt=mysql_stmt_init(getMySQL());
		if(m_stmt==NULL)
		{
			logerror_f("mysql_stmt_init error");
			throw new Exception("mysql_stmt_init error");
		}
		parseArgTypes(args);
		parseElements(sql);
	
		makePrepareSQL(m_prepareSQL);
		int code = mysql_stmt_prepare(m_stmt,m_prepareSQL.c_str(),static_cast<unsigned long>(m_prepareSQL.length()));
		if (code)
		{
			logerror_f("mysql_stmt_prepare error[%d, %s]:\n%s", code, mysql_stmt_error(m_stmt), m_prepareSQL.c_str());
			throw new Exception("mysql_stmt_prepare error:\n%s",m_prepareSQL.c_str());
		}
		setupBindParamters();
	}catch(Zoic::Exception * e)
	{
		clear();
		throw e;
	}
}

StmtQuery::~StmtQuery()
{
	clear();
}

void StmtQuery::clear()
{
	if(m_input)
	{
		for(int i=0;i<m_inputCount;++i)
		{
			const ParameterType & paramType=m_paramTypes[m_inputIndices[i]];
			ParameterValue & paramValue=m_args[m_inputIndices[i]];
			switch(paramValue.type)
			{
				case ParameterValue::T_DATE:
				case ParameterValue::T_TIME:
				case ParameterValue::T_DATETIME:
					delete static_cast<MYSQL_TIME *>(m_input[i].buffer);
					break;
			}
		}
		delete [] m_input;
		m_input = NULL;
		m_inputCount = 0;
	}
	if(m_inputIndices)
	{
		delete [] m_inputIndices;
		m_inputIndices = NULL;
	}
	if(m_stmt)
	{
		mysql_stmt_close(m_stmt);
		m_stmt = NULL;
	}	
}

bool StmtQuery::execute()
{
	if(exec())
	{
		mysql_stmt_store_result(m_stmt);
		return true;
	}
	else
	{
		return false;
	}
}

Recordset * StmtQuery::use()
{
	return store();
}

Recordset * StmtQuery::store()
{
	if(!exec())
	{
		return NULL;
	}
	
	if(mysql_stmt_store_result(m_stmt)==0)
	{
		m_recordset.setResult(m_stmt);	
	}
	else
	{
		m_recordset.setResult(NULL);
	}	
	return &m_recordset;
}

void StmtQuery::makePrepareSQL(std::string &str)
{
	m_inputIndices=NULL;
	m_inputCount=0;
	std::vector<WORD> inputIndices;
	for(std::list<QueryElement>::iterator it=m_queryElements.begin();it!=m_queryElements.end();++it)
	{
		if(it->type==QueryElement::T_TEXT)
		{
			str.append(it->element.text.begin,it->element.text.len);
		}
		else
		{
			str.append("?");			
			inputIndices.push_back(it->element.param.index);
			m_inputCount++;
		}
	}
	if(m_inputCount)
	{
		m_inputIndices=new WORD[m_inputCount];
		for(int i=0;i<m_inputCount;i++)
		{
			m_inputIndices[i]=inputIndices[i];
		}
	}
}

unsigned long long StmtQuery::getAffactedRows()
{
	return mysql_stmt_affected_rows(m_stmt);
}

unsigned long long StmtQuery::getInsertID()
{
	return mysql_stmt_insert_id(m_stmt);
}

void StmtQuery::setupBindParamters()
{
	m_input=NULL;
	if(m_inputCount)
	{
		m_input=new MYSQL_BIND[m_inputCount];
		memset(m_input, 0, sizeof(m_input[0])*m_inputCount);
		for(int i=0;i<m_inputCount;++i)
		{
			const ParameterType & paramType=m_paramTypes[m_inputIndices[i]];
			ParameterValue & paramValue=m_args[m_inputIndices[i]];
			setupBindParamter(paramType,paramValue,m_input[i]);
		}
	}
	m_recordset.bindParameters(m_stmt);
}

const char * StmtQuery::error()
{
	return mysql_stmt_error(m_stmt);
}

bool StmtQuery::exec()
{
	if(m_conn.isDead())
	{
		return false;
	}

	if(m_inputCount)
	{
		bindInputParamters();
	}
	m_argIndex=0;
	
	m_conn.closeLastQuery();
	if(mysql_stmt_execute(m_stmt))
	{
		int err=mysql_stmt_errno(m_stmt);
		if(!getMySQL()->net.vio)
		{
			flog_f(Zoic::ERROR_LOG_LEVEL,"mysql",".log","stmt net error[%d]:%s\n\t%s",err,mysql_stmt_error(m_stmt),m_prepareSQL.c_str());
			m_conn.dead();
			return false;
		}
// MySQL Bug#43560 http://bugs.mysql.com/bug.php?id=43560 极小情况出现：stmt net error[1210]:Incorrect arguments to mysql_stmt_execute
// 		if(err == CR_SERVER_LOST || err == ER_UNKNOWN_STMT_HANDLER
// 			|| err == 0 // MySQL Bug#43560 http://bugs.mysql.com/bug.php?id=43560
// 			)
		{//连接断开过，又重新连接成功，重新生成预处理句柄，并执行语句

			flog_f(Zoic::ERROR_LOG_LEVEL,"mysql",".log","err[%d]: %s\nretry:\t%s", err, mysql_stmt_error(m_stmt), m_prepareSQL.c_str());

			try
			{
				rebuildStmt(err);
				if(reexec(err))
				{
					m_conn.setLastQuery(this);
					return true;
				}
				else
				{
					flog_f(Zoic::ERROR_LOG_LEVEL,"mysql",".log","reexec() failed");
					return false;
				}
			}
			catch (Exception* e)
			{
				flog_f(Zoic::ERROR_LOG_LEVEL,"mysql",".log","exp:%s",e->what());
				delete e;
				return false;
			}
		}
// 		else
// 		{
// 			flog_f(Zoic::INFO_LOG_LEVEL,"mysql",".log","stmt error[%d]:%s\n\t%s",err,mysql_stmt_error(m_stmt),m_prepareSQL.c_str());
// 			return false;
// 		}
	}
	else
	{
		m_conn.setLastQuery(this);
		return true;
	}
}

void StmtQuery::setupBindParamter(const ParameterType & paramType,ParameterValue & paramValue,MYSQL_BIND & bind)
{
	switch(paramValue.type)
	{
		case ParameterValue::T_INTEGER:
		case ParameterValue::T_INTEGERU:
			bind.buffer_type=MYSQL_TYPE_LONGLONG;
			bind.buffer=&paramValue.data._integer;
			bind.buffer_length=8;
			break;
		case ParameterValue::T_STRING:
			bind.buffer_type=MYSQL_TYPE_BLOB;
			break;
		case ParameterValue::T_DATE:
			bind.buffer_type=MYSQL_TYPE_DATE;
			bind.buffer = new MYSQL_TIME;
			bind.buffer_length=sizeof(MYSQL_TIME);
			break;
		case ParameterValue::T_TIME:
			bind.buffer_type=MYSQL_TYPE_TIME;
			bind.buffer = new MYSQL_TIME;
			bind.buffer_length=sizeof(MYSQL_TIME);
			break;
		case ParameterValue::T_DATETIME:
			bind.buffer_type=MYSQL_TYPE_DATETIME;
			bind.buffer = new MYSQL_TIME;
			bind.buffer_length=sizeof(MYSQL_TIME);
			break;
		case ParameterValue::T_DOUBLE:
			bind.buffer_type=MYSQL_TYPE_DOUBLE;
			bind.buffer=&paramValue.data._double;
			bind.buffer_length=sizeof(double);
			break;
		default:
			throw new Exception("StmtQuery::bindParamter Invalid ParameterType %d",paramValue.type);
	}
	bind.length=&paramValue.length;
	bind.is_unsigned=!paramType.sign;
}

void StmtQuery::bindInputParamters()
{
	for(int i=0;i<m_inputCount;++i)
	{
		ParameterValue & paramValue=m_args[m_inputIndices[i]];
		switch(paramValue.type)
		{
			case ParameterValue::T_STRING:
				m_input[i].buffer=const_cast<char *>(m_args[m_inputIndices[i]].data._string.buffer);
				m_input[i].buffer_length=m_args[m_inputIndices[i]].length;
				break;
			case ParameterValue::T_DATE:
			{
				MYSQL_TIME *mysqlTime=static_cast<MYSQL_TIME *>(m_input[i].buffer);
				memset(mysqlTime,0,sizeof(*mysqlTime));
				DateTime * dt=reinterpret_cast<DateTime *>(paramValue.data._datetime);
				mysqlTime->year		=dt->year;
				mysqlTime->month	=dt->month;
				mysqlTime->day		=dt->day;
				break;
			}
			case ParameterValue::T_TIME:
			{
				MYSQL_TIME *mysqlTime=static_cast<MYSQL_TIME *>(m_input[i].buffer);
				memset(mysqlTime,0,sizeof(*mysqlTime));
				DateTime * dt=reinterpret_cast<DateTime *>(paramValue.data._datetime);
				mysqlTime->hour		=dt->hour;
				mysqlTime->minute	=dt->minute;
				mysqlTime->second	=dt->second;
				break;
			}
			case ParameterValue::T_DATETIME:
			{
				MYSQL_TIME *mysqlTime=static_cast<MYSQL_TIME *>(m_input[i].buffer);
				memset(mysqlTime,0,sizeof(*mysqlTime));
				DateTime * dt=reinterpret_cast<DateTime *>(paramValue.data._datetime);
				mysqlTime->year		=dt->year;
				mysqlTime->month	=dt->month;
				mysqlTime->day		=dt->day;
				mysqlTime->hour		=dt->hour;
				mysqlTime->minute	=dt->minute;
				mysqlTime->second	=dt->second;
				break;
			}
		}
	}
	if(mysql_stmt_bind_param(m_stmt,m_input))
	{
		throw new Exception("mysql_stmt_bind_param\n%s",mysql_stmt_error(m_stmt));
	}
}

void StmtQuery::rebuildStmt(int err)
{
	mysql_stmt_reset(m_stmt);	//先执行复位操作,修复由重新连接引起的bug.
	mysql_stmt_close(m_stmt);
	m_stmt=mysql_stmt_init(getMySQL());
	if(m_stmt==NULL)
	{
		throw new Exception("recreate: mysql_stmt_init error");
	}
	if(mysql_stmt_prepare(m_stmt,m_prepareSQL.c_str(),static_cast<unsigned long>(m_prepareSQL.length())))
	{
		throw new Exception("recreate: mysql_stmt_prepare error[%d]:%s\n\t%s",err,mysql_stmt_error(m_stmt),m_prepareSQL.c_str());
	}
	if(m_recordset.m_output!=NULL)
	{
		if(mysql_stmt_bind_result(m_stmt,m_recordset.m_output))
		{
			throw new Exception("recreate: mysql_stmt_bind_result\n%s",mysql_stmt_error(m_stmt));
		}
	}
}

bool StmtQuery::reexec(int err)
{
	if(mysql_stmt_bind_param(m_stmt,m_input))
	{
		throw new Exception("reexec: mysql_stmt_bind_param\n%s",mysql_stmt_error(m_stmt));
	}
	if(mysql_stmt_execute(m_stmt))
	{
		err=mysql_stmt_errno(m_stmt);
		flog_f(Zoic::ERROR_LOG_LEVEL,"mysql",".log","reexec: stmt error[%d]:%s\n\t%s",err,mysql_stmt_error(m_stmt),m_prepareSQL.c_str());
		if(!getMySQL()->net.vio)
		{				
			m_conn.dead();
		}
		return false;
	}
	else
	{
		return true;
	}
}

void StmtQuery::close()
{
	m_recordset.setResult(NULL);
}

