#include <vector>
#include "include/Connection.h"
#include "include/QueryDefine.h"
#include "include/NormalQuery.h"
#include "include/StmtQuery.h"
#include "include/Exception.h"
#include "include/NormalRecordset.h"
#include "include/Log.h"

using namespace Zoic;

Connection::Connection():
	m_isConnected(false),
	m_reconnect(true),
	m_dead(true),
	m_lastQuery(NULL),
	m_nextPingTime(0)
{
}

Connection::~Connection()
{
	close();
}

void Connection::setConnectParams(const char* db,const char* host,const char* user,const char* pass,WORD port)
{
	m_db=db;
	m_host=host;
	m_user=user;
	m_pass=pass;
	m_port=port;
}

bool Connection::connect()
{
	if(m_isConnected)
	{
		return false;
	}
	mysql_init(&m_mysql);
	mysql_options(&m_mysql, MYSQL_READ_DEFAULT_FILE,"my");
	unsigned int connect_timeout=60;
	mysql_options(&m_mysql, MYSQL_OPT_CONNECT_TIMEOUT,static_cast<const char*>(static_cast<void *>(&connect_timeout)));
	mysql_options(&m_mysql, MYSQL_OPT_RECONNECT,static_cast<const char*>(&m_reconnect));
	if (mysql_real_connect(
		&m_mysql,
		m_host.c_str(),
		m_user.c_str(),
		m_pass.c_str(),
		m_db.c_str(),
		m_port,
		NULL,
		CLIENT_MULTI_STATEMENTS|CLIENT_MULTI_RESULTS|CLIENT_FOUND_ROWS
		))
	{
		if (!mysql_set_character_set(&m_mysql, "utf8"))
		{
			m_dead = false;
			m_isConnected = true;
		}
	}
	if(!m_isConnected)
	{
		mysql_close(&m_mysql);
	}
	else
	{
		createQueries();
	}
	return m_isConnected;
}

void Connection::close()
{
	if(m_isConnected)
	{
		closeLastQuery();
		for(std::list<Query *>::iterator it=m_querys.begin();it!=m_querys.end();++it)
		{
			delete *it;
		}
		m_querys.clear();
		mysql_close(&m_mysql);
		mysql_server_end();
		m_isConnected=false;
		m_dead = true;
	}
}

static bool canPrepareStatement(const char *command)
{
	return stricmp(command,"SELECT")==0
		|| stricmp(command,"INSERT")==0
		|| stricmp(command,"DELETE")==0
		|| stricmp(command,"UPDATE")==0	
		|| stricmp(command,"REPLACE")==0		
		;
}
static void invalidSQL(const char *sql)
{
	throw new Exception("Invalid QUERY_DEFINE:\n\t%s",sql);
}

static bool oneStatement(char *sqls) 
{
	bool oneStatement=false;
	char *pos=strchr(sqls,';');
	if(pos)
	{
		if(*(pos+1)==0)
		{
			*pos='\0';
			oneStatement=true;
		}
	}
	else
	{
		oneStatement=true;
	}
	return oneStatement;
}

void Connection::createQueries()
{
	QueryItems & items=getQueryStorage().getClassInfo(typeid(*this));
	
	for(QueryItems::iterator it=items.begin();it!=items.end();++it)
	{
		char buf[1024];
		strlcpy(buf,it->sql.c_str(),sizeof(buf));
		const char * separator=" \t\r\n()[]=,;";
		char * command=strtok(buf,separator);
		std::string body;
		std::string args;
		if(!command)
		{
			invalidSQL(it->sql.c_str());
		}
		else if(stricmp(command,"CALL")==0)
		{
			char * procedure=strtok(NULL,separator);
			if(!procedure)
			{
				invalidSQL(it->sql.c_str());
			}			
			std::string content;
			if(!getProcedureContent(procedure,content))
			{
				invalidSQL(it->sql.c_str());
			}			
			parseProcedure(procedure,content,body,args);
			it->sql=body.c_str();
			it->args=args.c_str();
			strlcpy(buf,it->sql.c_str(),sizeof(buf));
			char * command=strtok(buf,separator);
			if(!command)
			{
				invalidSQL(it->sql.c_str());
			}
		}
		bool normalQuery=true;
		if(canPrepareStatement(command)&&strchr(it->sql.c_str(),';')==NULL)
		{
			try
			{
				*(it->query)=new StmtQuery(*this,it->sql.c_str(),it->args.c_str());
				normalQuery=false;
			}catch(Zoic::Exception * e)
			{
				delete e;
			}
		}
		if(normalQuery)
		{
			*(it->query)=new NormalQuery(*this,it->sql.c_str(),it->args.c_str());
		}
		m_querys.push_back(*(it->query));
	}
}

void Connection::parseProcedure(const char *name,const std::string &content,std::string &body,std::string &args)
{
	char *buf=new char[content.length()+1];
	strcpy(buf,content.c_str());
	char *head=strtok(buf,"\n");
	char *sqls=head+strlen(head)+1;
	const char * separator=" \t\r\n()[]=,`\";";
	char *word=strtok(head,separator);
	while(strcmp(word,name))
	{
		word=strtok(NULL,separator);
	}
	//去掉参数列表中的(
	char *args_begin=word+strlen(word)+1;
	while(*args_begin++!='(');
	//去掉参数列表中的)
	char *args_end=args_begin+strlen(args_begin);
	while(*--args_end!=')');
	*args_end='\0';
	args=args_begin;
	
	if(oneStatement(sqls))
	{
		body=sqls;
	}
	else
	{
		std::vector<Query::ParameterType> types;
		Query::parseArgTypes(args_begin,types);
		int types_size=static_cast<int>(types.size());
		body = "CALL ";
		body += name;
		body += "(";
		for(int i=0;i<types_size;++i)
		{
			body+=types[i].name;
			if(i!=types_size-1)
			{
				body += ",";
			}
		}
		body += ")";
	}
	delete [] buf;
}



bool Connection::getProcedureContent(const char *name,std::string &content)
{
	char sql[1024];
	snprintf(sql,sizeof(sql)-1,"SHOW CREATE PROCEDURE %s",name);
	sql[sizeof(sql)-1]='\0';
	NormalQuery showProc(*this,sql,"");
	RecordsetPtr rs = showProc.store();
	if(!rs.fetchRow())
	{
		return false;
	}
	char tmp;
	*rs >> tmp >> tmp >> content;
	return true;
}

void Connection::setLastQuery(Query * query)
{
	ASSERT(m_lastQuery==NULL);
	m_lastQuery = query;
}

void Connection::closeLastQuery()
{
	if(m_lastQuery)
	{
		m_lastQuery->close();
		m_lastQuery=NULL;
	}
}

void Connection::pingDB(TIME_MSEL curTime)
{
	if (curTime < m_nextPingTime)
	{
		return;
	}
	m_nextPingTime = curTime+30*1000;
	int ret = mysql_ping(&m_mysql);
	if (ret)
	{
		flog_f(Zoic::INFO_LOG_LEVEL,"mysql",".log","mysql ping fail error code:%d",ret );
	}
}

