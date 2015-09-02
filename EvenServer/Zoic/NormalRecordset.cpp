#include "include/NormalRecordset.h"

using namespace Zoic;

NormalRecordset::NormalRecordset(Connection & conn):
	Recordset(conn),
	m_result(NULL)
{
}

NormalRecordset::~NormalRecordset()
{
	close();
}

bool NormalRecordset::fetch()
{
	MYSQL_ROW row;
	if(m_result && (row=mysql_fetch_row(m_result)) )
	{
		unsigned long * lengths=mysql_fetch_lengths(m_result);
		for(int i=0;i<getFieldCount();++i)
		{
			m_Values[i].setValue(row[i],lengths[i]);
		}
		resetFieldIndex();
		return true;
	}
	else
	{
		return false;
	}
}

void NormalRecordset::setResult(MYSQL_RES *result)
{
	if(m_result)
	{
		mysql_free_result(m_result);
	}
	m_result=result;
	if(m_result)
	{
		setFieldCount(mysql_num_fields(m_result));
		m_fields = mysql_fetch_fields(m_result);
		setRows(mysql_num_rows(m_result));
		for(int i=0;i<getFieldCount();++i)
		{
			setColumnFormat(i,m_fields[i]);
		}
	}
	else
	{
		setRows(0);
	}
}

bool NormalRecordset::isInteger(enum_field_types type)
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
