#include "include/CheckRunTime.h"
#include "include/Clock.h"
#include "include/Log.h"

using namespace Zoic;

CheckRunTime::CheckRunTime(const char *name,const char *file,int line,int timeout):
		m_name(name),
		m_file(file),
		m_line(line),
		m_context_file1(NULL),
		m_context_line1(0),
		m_context_file2(NULL),
		m_context_line2(0),
		m_timeout(timeout)
{
	m_runtime=getRunTime();
}

CheckRunTime::~CheckRunTime()
{
	checkTimeOut(m_timeout);
}

void CheckRunTime::checkTimeOut(int timeout)
{
	TIME_MSEL cur=getRunTime();
	TIME_MSEL ms=cur-m_runtime;
	if(ms>=timeout)
	{
		flog_f(Zoic::INFO_LOG_LEVEL,"time",".log","%s:%d:(%s) cost:%d",m_file,m_line,m_name,ms);
		if(m_context_file1)
		{
			flog_f(Zoic::INFO_LOG_LEVEL,"time",".log","context1 %s:%d",m_context_file1,m_context_line1);
		}
		if(m_context_file2)
		{
			flog_f(Zoic::INFO_LOG_LEVEL,"time",".log","context2 %s:%d",m_context_file2,m_context_line2);
		}
	}
}

TIME_MSEL CheckRunTime::getRunTime()
{
	g_Clock->setTime();
	return g_Clock->getMilliSeconds();
}
