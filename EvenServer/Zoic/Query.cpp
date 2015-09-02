#include <vector>
#include "include/Types.h"
#include "include/Query.h"
#include "include/SplitString.h"
#include "include/Exception.h"

using namespace Zoic;

Query::Query(Connection & conn):
	m_argCount(0),
	m_argIndex(0),
	m_args(NULL),
	m_paramTypes(NULL),
	m_conn(conn)
{
}

Query::~Query()
{
	if(m_args)
	{
		delete [] m_args;
		m_args=NULL;
	}
	if(m_paramTypes)
	{
		delete [] m_paramTypes;
		m_paramTypes=NULL;
	}
	
}

//参数类型解析结构定义
struct ParameterTypeParser
{
	const char *	name;						//名称
	Query::ParameterType::TYPE	type;	//类型
	bool	need_len;					//需要长度
	bool	need_sign;					//需要符号
	ParameterValue::TYPE	sPVT;		//有符号参数值类型(PVT = ParameterValue Type)
	ParameterValue::TYPE	uPVT;		//无符号参数值类型
};

//参数类型解析表
static ParameterTypeParser s_ParameterTypeParserTable[]=
{
		//名称			类型								需要长度	需要符号	有符号参数值类型			无符号参数值类型

	//字符类型
	{	"CHAR",			Query::ParameterType::T_CHAR,		true,		false,		ParameterValue::T_STRING,	ParameterValue::T_STRING	},
	{	"VARCHAR",		Query::ParameterType::T_VARCHAR,	true,		false,		ParameterValue::T_STRING,	ParameterValue::T_STRING	},
	{	"BINARY",		Query::ParameterType::T_BINARY,		true,		false,		ParameterValue::T_STRING,	ParameterValue::T_STRING	},
	{	"VARBINARY",	Query::ParameterType::T_VARBINARY,	true,		false,		ParameterValue::T_STRING,	ParameterValue::T_STRING	},
	{	"BLOB",			Query::ParameterType::T_BLOB,		false,		false,		ParameterValue::T_STRING,	ParameterValue::T_STRING	},
	{	"TEXT",			Query::ParameterType::T_TEXT,		false,		false,		ParameterValue::T_STRING,	ParameterValue::T_STRING	},
	{	"TINYBLOB",		Query::ParameterType::T_TINYBLOB,	false,		false,		ParameterValue::T_STRING,	ParameterValue::T_STRING	},
	{	"TINYTEXT",		Query::ParameterType::T_TINYTEXT,	false,		false,		ParameterValue::T_STRING,	ParameterValue::T_STRING	},
	{	"MEDIUMBLOB",	Query::ParameterType::T_MEDIUMBLOB,	false,		false,		ParameterValue::T_STRING,	ParameterValue::T_STRING	},
	{	"MEDIUMTEXT",	Query::ParameterType::T_MEDIUMTEXT,	false,		false,		ParameterValue::T_STRING,	ParameterValue::T_STRING	},
	{	"LONGBLOB",		Query::ParameterType::T_LONGBLOB,	false,		false,		ParameterValue::T_STRING,	ParameterValue::T_STRING	},
	{	"LONGTEXT",		Query::ParameterType::T_LONGTEXT,	false,		false,		ParameterValue::T_STRING,	ParameterValue::T_STRING	},

	//整数型
	{	"BIT",			Query::ParameterType::T_BIT,		false,		true,		ParameterValue::T_INTEGER,	ParameterValue::T_INTEGERU	},
	{	"BOOL",			Query::ParameterType::T_BOOL,		false,		true,		ParameterValue::T_INTEGER,	ParameterValue::T_INTEGERU	},
	{	"BOOLEAN",		Query::ParameterType::T_BOOL,		false,		true,		ParameterValue::T_INTEGER,	ParameterValue::T_INTEGERU	},
	{	"TINYINT",		Query::ParameterType::T_TINYINT,	false,		true,		ParameterValue::T_INTEGER,	ParameterValue::T_INTEGERU	},
	{	"SMALLINT",		Query::ParameterType::T_SMALLINT,	false,		true,		ParameterValue::T_INTEGER,	ParameterValue::T_INTEGERU	},
	{	"MEDIUMINT",	Query::ParameterType::T_MEDIUMINT,	false,		true,		ParameterValue::T_INTEGER,	ParameterValue::T_INTEGERU	},
	{	"INT",			Query::ParameterType::T_INT,		false,		true,		ParameterValue::T_INTEGER,	ParameterValue::T_INTEGERU	},
	{	"INTEGER",		Query::ParameterType::T_INT,		false,		true,		ParameterValue::T_INTEGER,	ParameterValue::T_INTEGERU	},
	{	"BIGINT",		Query::ParameterType::T_BIGINT,		false,		true,		ParameterValue::T_INTEGER,	ParameterValue::T_INTEGERU	},

	//其它类型
	{	"FLOAT",		Query::ParameterType::T_FLOAT,		false,		true,		ParameterValue::T_DOUBLE,	ParameterValue::T_DOUBLE	},
	{	"DOUBLE",		Query::ParameterType::T_DOUBLE,		false,		true,		ParameterValue::T_DOUBLE,	ParameterValue::T_DOUBLE	},
	{	"REAL",			Query::ParameterType::T_DOUBLE,		false,		true,		ParameterValue::T_DOUBLE,	ParameterValue::T_DOUBLE	},
	{	"DECIMAL",		Query::ParameterType::T_DECIMAL,	false,		true,		ParameterValue::T_DOUBLE,	ParameterValue::T_DOUBLE	},
	{	"DEC",			Query::ParameterType::T_DECIMAL,	false,		true,		ParameterValue::T_DOUBLE,	ParameterValue::T_DOUBLE	},
	{	"DATE",			Query::ParameterType::T_DATE,		false,		false,		ParameterValue::T_DATE,		ParameterValue::T_DATE		},
	{	"TIME",			Query::ParameterType::T_TIME,		false,		false,		ParameterValue::T_TIME,		ParameterValue::T_TIME		},
	{	"DATETIME",		Query::ParameterType::T_DATETIME,	false,		false,		ParameterValue::T_DATETIME,	ParameterValue::T_DATETIME	},
	{	"TIMESTAMP",	Query::ParameterType::T_TIMESTAMP,	false,		false,		ParameterValue::T_DATETIME,	ParameterValue::T_DATETIME	},

};

//获得参数类型解析器
static ParameterTypeParser * getParameterTypeParser(const char * name)
{
	for(int i = 0; i < sizeof(s_ParameterTypeParserTable)/sizeof(s_ParameterTypeParserTable[0]);++i)
	{
		if(stricmp(s_ParameterTypeParserTable[i].name,name)==0)
		{
			return &s_ParameterTypeParserTable[i];
		}
	}
	return NULL;
}

//根据参数类型与符号定义返回参数值类型
static ParameterValue::TYPE getParameterValueType(Query::ParameterType::TYPE arg_type,bool sign)
{
	for(int i = 0; i < sizeof(s_ParameterTypeParserTable)/sizeof(s_ParameterTypeParserTable[0]);++i)
	{
		if(s_ParameterTypeParserTable[i].type==arg_type)
		{
			return sign?s_ParameterTypeParserTable[i].sPVT:s_ParameterTypeParserTable[i].uPVT;
		}
	}
	throw new Exception("getParameterValueType: Can't find arg_type:%d",arg_type);
}

//判断字符串是否为数字
static bool isdigit(const char * str)
{
	if(str==NULL)
	{
		return false;
	}
	for(;*str;++str)
	{
		if(!isdigit(*str))
		{
			return false;
		}
	}
	return true;
}

void Query::parseArgTypes(const char *args,std::vector<ParameterType> &types)
{
	std::vector<std::string> vec_args;
	splitString(args,vec_args,",");
	int vec_args_size=static_cast<int>(vec_args.size());
	for(int i=0;i<vec_args_size;i++)
	{
		char buf[100];
		strlcpy(buf,vec_args[i].c_str(),sizeof(buf));
		const char *separator="( )";
		char *name=strtok(buf,separator);
		char *str_type=strtok(NULL,separator);
		if(name&&str_type)
		{
			ParameterType paramType;
			paramType.name=name;
			paramType.sign=true;
			
			ParameterTypeParser * parser = getParameterTypeParser(str_type);
			if(parser==NULL)
			{
				throw new Exception("Query::parsetypes Can't find arg type:%s",vec_args[i].c_str());
			}
			paramType.type = parser->type;
			char *str_len=strtok(NULL,separator);
			if(parser->need_len)
			{
				if(!isdigit(str_len))
				{
					throw new Exception("Query::parsetypes Can't find arg len:%s",vec_args[i].c_str());
				}
			}
			else if(parser->need_sign)
			{
				if(!parseSign(str_len,separator,paramType.sign))
				{
					throw new Exception("Query::parsetypes Invalid arg sign:%s",vec_args[i].c_str());
				}
			}
			types.push_back(paramType);
		}
		else
		{
			//允许参数列表为空,此时的vec_args_size为1
			if(vec_args_size!=1)
			{
				throw new Exception("Query::parsetypes Can't find arg name:%s",vec_args[i].c_str());
			}
		}
	}
}

void Query::parseArgTypes(const char *args)
{
	std::vector<ParameterType> argTypes;
	parseArgTypes(args,argTypes);
	
	if(argTypes.size())
	{
		m_argCount = static_cast<WORD>(argTypes.size());
		m_args= new ParameterValue[m_argCount];
		memset(m_args,0,sizeof(m_args[0])*m_argCount);
		m_paramTypes = new ParameterType[m_argCount];
		for(int i=0;i<m_argCount;i++)
		{
			m_paramTypes[i]=argTypes[i];
			m_args[i].type=getParameterValueType(argTypes[i].type,argTypes[i].sign);
			if(m_args[i].type==ParameterValue::T_STRING)
			{
				m_args[i].data._string.dynamicAlloced=false;
			}
		}
	}
}

int Query::findParameter(const char *name)
{
	for(int i=0;i<m_argCount;i++)
	{
		if(strcmp(name,m_paramTypes[i].name.c_str())==0)
		{
			return i;
		}
	}
	return -1;
}

void Query::parseElements(const char *sql)
{
	char buf[3000];
	strlcpy(buf,sql,sizeof(buf));
	const char * separator=" \t\r\n()[]=+-,;";
	char *token=strtok(buf,separator);
	int text_begin=0;
	while(token)
	{
		int param_index;
		if( (param_index=findParameter(token)) >= 0)
		{//找到参数
			int pos=static_cast<int>(token-buf);
			if(pos!=text_begin)
			{//将以前的文本放入list				
				QueryElement text;
				text.type=QueryElement::T_TEXT;
				text.element.text.begin=sql+text_begin;
				text.element.text.len=pos-text_begin;
				m_queryElements.push_back(text);
				text_begin=pos+static_cast<int>(strlen(token));
			}
			//将找到的参数放入list
			QueryElement param;
			param.type=QueryElement::T_PARAM;
			param.element.param.index=param_index;
			m_queryElements.push_back(param);
		}
		token=strtok(NULL,separator);
	}
	int sql_len=static_cast<int>(strlen(sql));
	if(text_begin!=sql_len)
	{
		//将最后的文本放入list
		QueryElement text;
		text.type=QueryElement::T_TEXT;
		text.element.text.begin=sql+text_begin;
		text.element.text.len=sql_len-text_begin;
		m_queryElements.push_back(text);
	}
}

bool Query::parseSign(const char * str_len,const char *separator,bool &sign)
{
	const char *str_sign;
	if(isdigit(str_len))
	{
		str_sign=strtok(NULL,separator);
	}
	else
	{
		str_sign = str_len;
	}
	if(str_sign)
	{
		if(stricmp(str_sign,"UNSIGNED")==0)
		{
			sign = false;
			return true;
		}
		else
		{
			return false;
		}
	}
	sign = true;
	return true;
}
