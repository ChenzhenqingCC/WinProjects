#include "include/NetLog.h"
#include "include/Log.h"
#include "include/Clock.h"
#include "logger/loglevel.h"
#include <sys/stat.h>
#include <stdarg.h>
#ifdef HAVE_MINGW32
#include <direct.h>
#else
#endif

using namespace Zoic;
#undef netlog

static const int TIMEBUF_LENGTH=12;
static const int DEFALUT_NETLOG_BLOCK_SIZE = 4096;

NetLog::NetLog():
	m_basedir("NetLog/_Undefined_Implement"),
	m_prefix("net"),
	m_suffix(".log"),
	m_lasttime(0)
{

}


/*
static inline FILE * open(const char *basedir,const char *prefix,const char *suffix,const char *date)
{
	char fname[640];
	snprintf(fname,sizeof(fname),"%s/%s%s%s",basedir,prefix,date,suffix);
	fname[sizeof(fname)-1]='\0';
	return fopen(fname, "a");
}
*/
void NetLog::local_flog(LogLevelValue logLevel, const char *pathname, const char *msg, ...)
{
	va_list args;
	char buf[1024];

	va_start(args,msg);
	int len = vsnprintf(buf,1024,msg,args);
	va_end(args);

	char* tail = buf + len;
	*tail++ = '\n';
	*tail = '\0';
	++len;
	g_Log->_textnetlogger(logLevel,pathname,buf);
}

static FILE * fopen(char *filename)
{
	FILE *fp=::fopen(filename,"ab+");
	if(!fp)
	{
		for(int i=1;filename[i];i++)
		{
			if(filename[i]=='/')
			{
				filename[i]='\0';
			#ifdef HAVE_MINGW32
				_mkdir(filename);
			#else
				mkdir(filename,0777);
			#endif
				filename[i]='/';
			}
		}
		fp=::fopen(filename,"ab+");
	}
	return fp;
}

// static void net_flog(char *filename,BYTE *buf,DWORD len)
// {
// 	g_Log->_binlogger(filename,buf,len);
// }

void NetLog::config(const char * basedir,const char *prefix,const char *suffix)
{
	m_basedir=basedir;
	m_prefix=prefix;
	m_suffix=suffix;
}

void NetLog::netlogWithConfig(LogLevelValue logLevel,const char *basedir,const char *ip,DWORD port,const char *binary_pathname,const void *buffer,int len,bool recv,const char *brief_pathname)
{
	updateTime();
	static const int block_size=DEFALUT_NETLOG_BLOCK_SIZE;

	char fname[640];
	sprintf(fname,"%s/%s",basedir,binary_pathname);

	g_Log->_netlog(logLevel,fname,g_Clock->getTime(),recv,len,(BYTE*)buffer);

	if(brief_pathname)
	{
		sprintf(fname,"%s/%s",basedir,brief_pathname);
		local_flog(logLevel,fname,"%u:%s %s:%d %d",DWORD( g_Clock->getMilliSeconds() ),recv?"Recv":"Send",ip,port,len );
	}
}

void NetLog::netlog(const char *ip,DWORD port,const char *local_ip,DWORD local_port,const void *buffer,int len,bool recv,LogLevelValue logLevel)
{
	if (g_Log->getLogLevel(Log::LST_NETLOG) > logLevel)
	{
		return;
	}
	if(m_basedir.empty()) 
	{
		return;
	}
	updateTime();

	char bin_fname[640];
	sprintf(bin_fname,"%s/%s_%lu_%s_%lu.bin",m_datebuf,ip,port,local_ip,local_port);

	char log_fname[640];
	snprintf(log_fname,sizeof(log_fname),"%s%s%s",m_prefix.c_str(),m_datebuf,m_suffix.c_str());
	log_fname[sizeof(log_fname)-1]='\0';

	netlogWithConfig(logLevel,m_basedir.c_str(),ip,port,bin_fname,buffer,len,recv,log_fname);
}

void NetLog::updateTime()
{
	TIME_SEC now=g_Clock->getTime();
	if(m_lasttime!=now)
	{
		time_t timeValue = static_cast<time_t>( now );
		struct tm * tm_now=localtime(&timeValue);
		strftime( m_timebuf, sizeof(m_timebuf) ,"[%H:%M:%S]: ", tm_now );
		strftime( m_datebuf, sizeof(m_datebuf) ,"%Y-%m-%d", tm_now);
		m_lasttime=now;
	}
}


