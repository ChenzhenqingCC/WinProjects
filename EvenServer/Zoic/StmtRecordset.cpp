#include "include/StmtRecordset.h"
#include "include/Exception.h"

using namespace Zoic;

StmtRecordset::StmtRecordset(Connection & conn):
	Recordset(conn),
	m_stmt(NULL),
	m_output(NULL)
{
}

StmtRecordset::~StmtRecordset()
{
	close();
	for(int i=0;i<getFieldCount();++i)
	{
		switch(m_Values[i].type)
		{
			case ParameterValue::T_DATE:
			case ParameterValue::T_TIME:
			case ParameterValue::T_DATETIME:
				delete static_cast<MYSQL_TIME *>(m_output[i].buffer);
				break;
		}
	}
	if(m_output)
	{
		delete [] m_output;
	}
}

bool StmtRecordset::fetch()
{

	if(m_stmt && mysql_stmt_fetch(m_stmt)==0)
	{
		resetFieldIndex();
		for(int i=0;i<getFieldCount();++i)
		{
			MYSQL_TIME *mysqlTime;
			switch(m_Values[i].type)
			{
				case ParameterValue::T_DATE:
					mysqlTime=static_cast<MYSQL_TIME *>(m_output[i].buffer);
					new(&m_Values[i].data._datetime) DateTime(Date(mysqlTime->year,mysqlTime->month,mysqlTime->day));
					break;
				case ParameterValue::T_TIME:
					mysqlTime=static_cast<MYSQL_TIME *>(m_output[i].buffer);
					new(&m_Values[i].data._datetime) DateTime(Time(mysqlTime->hour,mysqlTime->minute,mysqlTime->second));
					break;	
				case ParameterValue::T_DATETIME:
					mysqlTime=static_cast<MYSQL_TIME *>(m_output[i].buffer);
					new(&m_Values[i].data._datetime) DateTime(mysqlTime->year,mysqlTime->month,mysqlTime->day,mysqlTime->hour,mysqlTime->minute,mysqlTime->second);
					break;
			}
		}
		resetFieldIndex();
		return true;
	}
	else
	{
		return false;
	}
}

void StmtRecordset::setResult(MYSQL_STMT *stmt)
{
	m_stmt=stmt;
	if(m_stmt)
	{
		setRows(mysql_stmt_num_rows(m_stmt));
	}
	else
	{
		setRows(0);
	}
}

bool StmtRecordset::isInteger(enum_field_types type)
{
	switch(type)
	{
		case MYSQL_TYPE_TINY:
		case MYSQL_TYPE_SHORT:
		case MYSQL_TYPE_LONG:
		case MYSQL_TYPE_INT24:
		case MYSQL_TYPE_LONGLONG:
		case MYSQL_TYPE_BIT:
			return true;
		default:
			return false;
	}
}

void StmtRecordset::bindParameters(MYSQL_STMT *stmt)
{
	MYSQL_RES * prepare_meta_result = mysql_stmt_result_metadata(stmt);
	if (prepare_meta_result)
	{
		WORD column_count= mysql_num_fields(prepare_meta_result);
		if(column_count)
		{
			setFieldCount(column_count);
			m_fields = mysql_fetch_fields(prepare_meta_result);
			m_output = new MYSQL_BIND[column_count];
			memset(m_output,0,sizeof(m_output[0])*column_count);
			for(int i=0;i<column_count;i++)
			{
				setColumnFormat(i,m_fields[i]);
				setupBindParameter(m_fields[i],m_Values[i],m_output[i]);
			}
		}
		mysql_free_result(prepare_meta_result);
		if(mysql_stmt_bind_result(stmt,m_output))
		{
			throw new Exception("mysql_stmt_bind_result\n%s",mysql_stmt_error(m_stmt));
		}
	}
}

void StmtRecordset::setupBindParameter(const MYSQL_FIELD & field,ParameterValue &value,MYSQL_BIND &bind)
{
	switch(value.type)
	{
		case ParameterValue::T_INTEGER:
		case ParameterValue::T_INTEGERU:
			bind.buffer_type=MYSQL_TYPE_LONGLONG;
			bind.buffer=&value.data._integer;
			bind.buffer_length=8;
			break;
		case ParameterValue::T_STRING:
			bind.buffer_type=MYSQL_TYPE_BLOB;
			bind.buffer_length=field.length;
			value.data._string.buffer=new char[bind.buffer_length];
			value.data._string.dynamicAlloced=true;					
			bind.buffer=const_cast<char *>(value.data._string.buffer);
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
			bind.buffer=&value.data._double;
			bind.buffer_length=sizeof(double);
			break;
	}
	bind.length=&value.length;
}
