#include "include/Recordset.h"
#include "include/Exception.h"
#include "include/Connection.h"

using namespace Zoic;

Recordset::Recordset(Connection & conn):
	m_conn(conn),
	m_fieldCount(0),
	m_fieldMaxCount(0),
	m_Values(NULL)
{
}

Recordset::~Recordset()
{
	if(m_Values)
	{
		delete [] m_Values;
	}
}

//!列格式解析
struct ColumnFormatParser
{
	BYTE					mysql_type;	//MySQL列类型
	ParameterValue::TYPE	sPVT;		//有符号参数值类型(PVT = ParameterValue Type)
	ParameterValue::TYPE	uPVT;		//无符号参数值类型
};

//参数类型解析表
static ColumnFormatParser s_ColumnFormatParserTable[]=
{
		//MySQL列类型			有符号参数值类型			无符号参数值类型
	{	MYSQL_TYPE_BIT,			ParameterValue::T_INTEGER,	ParameterValue::T_INTEGERU	},
	{	MYSQL_TYPE_TINY,		ParameterValue::T_INTEGER,	ParameterValue::T_INTEGERU	},
	{	MYSQL_TYPE_SHORT,		ParameterValue::T_INTEGER,	ParameterValue::T_INTEGERU	},
	{	MYSQL_TYPE_LONG,		ParameterValue::T_INTEGER,	ParameterValue::T_INTEGERU	},
	{	MYSQL_TYPE_INT24,		ParameterValue::T_INTEGER,	ParameterValue::T_INTEGERU	},
	{	MYSQL_TYPE_LONGLONG,	ParameterValue::T_INTEGER,	ParameterValue::T_INTEGERU	},
	{	MYSQL_TYPE_DOUBLE,		ParameterValue::T_DOUBLE,	ParameterValue::T_DOUBLE	},
	{	MYSQL_TYPE_FLOAT,		ParameterValue::T_DOUBLE,	ParameterValue::T_DOUBLE	},
	{	MYSQL_TYPE_DECIMAL,		ParameterValue::T_DOUBLE,	ParameterValue::T_DOUBLE	},
	{	MYSQL_TYPE_NEWDECIMAL,	ParameterValue::T_DOUBLE,	ParameterValue::T_DOUBLE	},
	{	MYSQL_TYPE_DATE,		ParameterValue::T_DATE,		ParameterValue::T_DATE		},
	{	MYSQL_TYPE_TIME,		ParameterValue::T_TIME,		ParameterValue::T_TIME		},
	{	MYSQL_TYPE_DATETIME,	ParameterValue::T_DATETIME,	ParameterValue::T_DATETIME	},
	{	MYSQL_TYPE_TIMESTAMP,	ParameterValue::T_DATETIME,	ParameterValue::T_DATETIME	},
	{	MYSQL_TYPE_STRING,		ParameterValue::T_STRING,	ParameterValue::T_STRING	},
	{	MYSQL_TYPE_VAR_STRING,	ParameterValue::T_STRING,	ParameterValue::T_STRING	},
	{	MYSQL_TYPE_BLOB,		ParameterValue::T_STRING,	ParameterValue::T_STRING	},
};

void Recordset::setColumnFormat(WORD column,const MYSQL_FIELD &field)
{
	for(int i=0;i< sizeof(s_ColumnFormatParserTable)/sizeof(s_ColumnFormatParserTable[0]);++i)
	{
		if(s_ColumnFormatParserTable[i].mysql_type == field.type)
		{
			m_Values[column].type = (field.flags & UNSIGNED_FLAG)?s_ColumnFormatParserTable[i].uPVT:s_ColumnFormatParserTable[i].sPVT;
			if(m_Values[column].type==ParameterValue::T_STRING)
			{
				m_Values[column].data._string.dynamicAlloced=false;
			}
			return;
		}
	}
	throw new Exception("Recordset::setColumnFormat: Can't find field.type:%d",field.type);
}

void Recordset::close()
{
	m_conn.closeLastQuery();
}
