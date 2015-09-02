#ifndef ZOIC_NO_SERVICE

#include "include/Types.h"
#ifdef HAVE_MINGW32
#include <windows.h>
#endif

#include "include/Service.h"
#include "include/SysModuleRegister.h"
#ifndef _ZOIC_CLIENT
#include "include/RunTimeMonitor.h"
#else
#include "include/Module.h"
#endif

#include "include/Log.h"

using namespace Zoic;

Service::Service()
	:m_argc(0)
	,m_runSerial(0)
	,m_sleepSkipRate(0)
{
	registerSysModules();
}

Service::~Service()
{
#ifndef HAVE_MINGW32
//重新打开标准输出与标准错误,使valgrind不产生内存报告
	int out = dup(fileno(stdout));
	fclose(stdout);
	stdout = fdopen(out,"w");
	int err = dup(fileno(stderr));
	fclose(stderr);
	stderr = fdopen(err,"w");
#endif
}

int Service::initial()
{
	int moduleSize=static_cast<int>(m_allModules.size());
	if (moduleSize)
	{
		for(int i=0;i<moduleSize;++i)
		{
			const std::type_info& typeInfo =  typeid(*(m_allModules[i]));
			m_allModules[i]->m_moduleName = typeInfo.name();
		}
	}
	int size=static_cast<int>(m_initialFuncs.size());
	if(size)
	{
		for(int i=0;i<size;++i)
		{
			int code=(*m_initialFuncs[i].func)(m_initialFuncs[i].context);
			if(code)
			{
				return code;
			}
		}
	}
	return 0;
}

int Service::start()
{
	int initCode = initial();
	if(initCode!=0)
	{
		return initCode;
	}
	int size=static_cast<int>(m_startFuncs.size());
	//要运行的函数
	std::vector<ModuleStart> left=m_startFuncs;
	//要等待的函数
	std::vector<ModuleStart> pending;
#ifndef _MSC_VER
	if (getArgc())
	{
		g_RunTimeMonitor->setLogName(getArgv()[0]);
	}
#endif
	for(;;)
	{
		int size=static_cast<int>(left.size());
		for(int i=0;i<size;++i)
		{
			int code=(*left[i].func)(left[i].context);
			if(code==-1)
			{
				pending.push_back(left[i]);
			}
			else if(code)
			{
				return code;
			}
		}
		if(pending.size()==0)
		{//全部成功处理
			return 0;
		}
		else if(left.size()==pending.size())
		{//依赖性错误
			break;
		}
		else
		{//继续下一轮处理
			left.swap(pending);
			pending.clear();
		}
	}
	return -1;
}

int Service::runOnce()
{
	int size=static_cast<int>(m_runFuncs.size());
	if(size)
	{
#ifndef _ZOIC_CLIENT
		g_RunTimeMonitor->resetCheckStartTime();
#endif
		for(int i=0;i<size;++i)
		{
#ifndef _ZOIC_CLIENT
			MODULE_RUNTIME(m_runFuncs[i].context->getModuleName());
#endif
			int code=(*m_runFuncs[i].func)(m_runFuncs[i].context);
			if(code)
			{
#ifndef _ZOIC_CLIENT
				logerror_f("[%s]: module run fail, name[%s], code[%d]", __PRETTY_FUNCTION__,
					m_runFuncs[i].context->getModuleName(), code);
#endif
				return code;
			}
		}
	}
	++m_runSerial;
	return 0;
}

int Service::run()
{
	for(;;)
	{
		int code=runOnce();
		if(code)
		{
			return code;
		}
#ifndef _ZOIC_CLIENT
		g_RunTimeMonitor->check();
#endif
		if( m_sleepSkipRate <= 1 || (m_runSerial % m_sleepSkipRate) == 0 )
		{
			WAIT_A_WHILE
		}
	}
	return 0;
}

void Service::stop()
{
	int size=static_cast<int>(m_stopFuncs.size());
	//要运行的函数
	std::vector<ModuleStop> left=m_stopFuncs;
	//要等待的函数
	std::vector<ModuleStop> pending;
	for(;;)
	{
		int size=static_cast<int>(left.size());
		for(int i=0;i<size;++i)
		{
			int code=(*left[i].func)(left[i].context);
			if(code==-1)
			{
				pending.push_back(left[i]);
			}
			else if(code)
			{
				return;
			}
		}
		if(pending.size()==0)
		{//全部成功处理
			return;
		}
		else if(left.size()==pending.size())
		{//依赖性错误
			break;
		}
		else
		{//继续下一轮处理
			left.swap(pending);
			pending.clear();
		}
	}
}

void Service::setArgs(int argc,char ** argv)
{
	m_argc=argc;
	m_argv=argv;
}

char ** Service::getArgv(void)
{
	return m_argv;
}

int Service::getArgc(void)
{
	return m_argc;
}

unsigned long Service::getRunSerial() const
{
	return m_runSerial;
}

void Service::setSleepSkipRate(unsigned long rate)
{
	m_sleepSkipRate = rate;
}

unsigned long Service::getSleepSkipRate() const
{
	return m_sleepSkipRate;
}

void Service::registModule(__ModuleContext *context,FUNC_MODULE_INITIAL initial,FUNC_MODULE_START start,FUNC_MODULE_RUN run,FUNC_MODULE_STOP stop)
{
	m_allModules.push_back(context);
	if(initial)
	{
		ModuleInitial item;
		item.func=initial;
		item.context=context;
		m_initialFuncs.push_back(item);
	}
	if(start)
	{
		ModuleStart item;
		item.func=start;
		item.context=context;
		m_startFuncs.push_back(item);
	}
	if(run)
	{
		ModuleRun item;
		item.func=run;
		item.context=context;
		m_runFuncs.push_back(item);
	}
	if(stop)
	{
		ModuleStop item;
		item.func=stop;
		item.context=context;
		m_stopFuncs.insert(m_stopFuncs.begin(),item);
	}
}

void Service::setInitModule(__ModuleContext *context)
{
	int i,size;
	//调整初始化操作
	size=static_cast<int>(m_initialFuncs.size());
	for(i=0;i<size;++i)
	{
		if(m_initialFuncs[i].context==context)
		{
			ModuleInitial item=m_initialFuncs[i];
			m_initialFuncs.erase(m_initialFuncs.begin()+i);
			m_initialFuncs.insert(m_initialFuncs.begin(),item);
			break;
		}
	}
	//调整启动操作
	size=static_cast<int>(m_startFuncs.size());
	for(i=0;i<size;++i)
	{
		if(m_startFuncs[i].context==context)
		{
			ModuleStart item=m_startFuncs[i];
			m_startFuncs.erase(m_startFuncs.begin()+i);
			m_startFuncs.insert(m_startFuncs.begin(),item);
			break;
		}
	}
	//调整结束操作
	size=static_cast<int>(m_stopFuncs.size());
	for(i=0;i<size;++i)
	{
		if(m_stopFuncs[i].context==context)
		{
			ModuleStop item=m_stopFuncs[i];
			m_stopFuncs.erase(m_stopFuncs.begin()+i);
			m_stopFuncs.push_back(item);
			break;
		}
	}
}

#endif
