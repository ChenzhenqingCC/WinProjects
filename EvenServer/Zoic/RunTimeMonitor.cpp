#include "include/RunTimeMonitor.h"
#include "include/Log.h"
#include "include/Clock.h"
#include "include/Service.h"
#include <list>

#ifndef _ZOIC_CLIENT

using namespace Zoic;

#ifdef ZOIC_NO_SERVICE
RunTimeMonitor Zoic::theRunTimeMonitor;
#endif

static ModuleInfo* g_curModuleInfo = NULL;
static MessageInfo* g_curMessageInfo = NULL;
static std::list<FileInfo*> g_curFileInfoList;

RunTimeMonitor::RunTimeMonitor()
{
	m_oneModuleCheckTime = 5;
	m_allModuleCheckTime = 10;
	m_isStarted = false;
	m_checkStartTime = getRunMilliseconds();
	m_logAllFile = "runtime_all_";
	m_logOneFile = "runtime_one_";
	m_logErrFile = "runtime_err_";
}

TIME_MSEL RunTimeMonitor::getRunMilliseconds()
{
#ifdef HAVE_MINGW32
	return timeGetTime();
#else
	timeval current;
	gettimeofday(&current, NULL);
	return current.tv_sec*1000+current.tv_usec/1000;
#endif
	
}

void RunTimeMonitor::setLogName(const char * logName)
{
	if (!g_Service->getArgc())
	{
		return;
	}
	m_logAllFile.clear();
	m_logOneFile.clear();
	m_logErrFile.clear();

	m_logAllFile = logName;
	m_logAllFile.append("_runtime_all_");

	m_logOneFile = logName;
	m_logOneFile.append("_runtime_one_");

	m_logErrFile = logName;
	m_logErrFile.append("_runtime_err_");
}

void RunTimeMonitor::check()
{
	
	if (!m_isStarted)
	{
		return;
	}
	
	TIME_MSEL checkAllModuleTime = 0;
	for (size_t i = 0; i < m_moduleInfos.size(); ++i)
	{
		ModuleInfo * pInfo = m_moduleInfos[i];
		checkAllModuleTime+=pInfo->runTime;
		if (pInfo->runTime >= m_oneModuleCheckTime)
		{
			logOneModuleInfo(pInfo,m_logOneFile.c_str());
		}
	}
	if (checkAllModuleTime >= m_allModuleCheckTime)
	{
		logAllModuleInfo(checkAllModuleTime);
	}
	for (size_t i = 0; i < m_moduleInfos.size(); ++i)
	{
		ModuleInfo * pInfo = m_moduleInfos[i];
		for (size_t j = 0; j < pInfo->fileInfos.size(); ++j)
		{
			delete pInfo->fileInfos[j];
		}
		for (size_t j = 0; j < pInfo->msgInfos.size(); ++j)
		{
			delete pInfo->msgInfos[j];
		}
		delete pInfo;
	}
	m_moduleInfos.clear();
	ASSERT(g_curFileInfoList.empty());
	g_curFileInfoList.clear();
}


void RunTimeMonitor::logOneModuleInfo(ModuleInfo * pInfo,const char * logFile)
{
	g_Log->flognoput_f(Zoic::INFO_LOG_LEVEL,logFile,".log","----------------------------------%s::run() cost:%d ms----------------------------------",pInfo->name,pInfo->runTime);
	for (size_t i = 0; i < pInfo->msgInfos.size(); ++i)
	{
		g_Log->flognoput_f(Zoic::INFO_LOG_LEVEL,logFile,".log","%d : MsgID(%d) cost:%d",i,pInfo->msgInfos[i]->msgID,pInfo->msgInfos[i]->runTime);
	}
	g_Log->flognoput_f(Zoic::INFO_LOG_LEVEL,logFile,".log","    ######################");
	int num = 0;
	for (size_t i = 0; i < pInfo->fileInfos.size(); ++i)
	{
		if (pInfo->fileInfos[i]->blank == 0)
		{
			++num;
		}
		g_Log->flognoput_f(Zoic::INFO_LOG_LEVEL,logFile,".log","%d-%d : cost:%d ms ( filename:%s | line:%d )",num,pInfo->fileInfos[i]->blank,static_cast<int>(pInfo->fileInfos[i]->runTime),pInfo->fileInfos[i]->file,pInfo->fileInfos[i]->line);
	}
}

void RunTimeMonitor::logAllModuleInfo(TIME_MSEL allCost)
{
	g_Log->flognoput_f(Zoic::INFO_LOG_LEVEL,m_logAllFile.c_str(),".log","**************************************************************************************************************");
	g_Log->flognoput_f(Zoic::INFO_LOG_LEVEL,m_logAllFile.c_str(),".log","*   all cost:%d ms   *",allCost);
	g_Log->flognoput_f(Zoic::INFO_LOG_LEVEL,m_logAllFile.c_str(),".log","**************************************************************************************************************");
	for (size_t i = 0; i < m_moduleInfos.size(); ++i)
	{
		ModuleInfo * pInfo = m_moduleInfos[i];
		logOneModuleInfo(pInfo,m_logAllFile.c_str());
	}
}

ModuleInfo * RunTimeMonitor::insertModuleInfo(ModuleInfo * pInfo)
{
	return *m_moduleInfos.insert(m_moduleInfos.end(),pInfo);
}

void RunTimeMonitor::resetCheckStartTime()
{
	if (!g_RunTimeMonitor->isStarted())
	{
		return;
	}
	m_checkStartTime = getRunMilliseconds();
}

void RunTimeMonitor::setCheckStartTime(TIME_MSEL checkTime)
{
	m_checkStartTime = checkTime;
}

ModuleRunTimeMonitorHelper::ModuleRunTimeMonitorHelper(const char * name)
{
	if (!g_RunTimeMonitor->isStarted())
	{
		return;
	}

	m_checkTime = g_RunTimeMonitor->getCheckStartTime();
	if (g_curModuleInfo)
	{
		g_Log->flognoput_f(Zoic::INFO_LOG_LEVEL,g_RunTimeMonitor->getLogErrFileName(),".log","ModuleRunTimeMonitorHelper create: g_curModuleInfo is not NULL! ");
	}
	ModuleInfo * newInfo = new ModuleInfo();
	newInfo->name = name;
	newInfo->runTime = 0;
	g_curModuleInfo = g_RunTimeMonitor->insertModuleInfo(newInfo);
}

ModuleRunTimeMonitorHelper::~ModuleRunTimeMonitorHelper()
{
	if (!g_RunTimeMonitor->isStarted())
	{
		return;
	}
	if (!g_curModuleInfo)
	{
		g_Log->flognoput_f(Zoic::INFO_LOG_LEVEL,g_RunTimeMonitor->getLogErrFileName(),".log","ModuleRunTimeMonitorHelper destroy: g_curModuleInfo is NULL! ");
		return;
	}
	g_RunTimeMonitor->resetCheckStartTime();
	g_curModuleInfo->runTime =g_RunTimeMonitor->getCheckStartTime() - m_checkTime;
	g_curModuleInfo=NULL;
}

MessageRunTimeMonitorHelper::MessageRunTimeMonitorHelper(WORD msgID)
{
	if (!g_RunTimeMonitor->isStarted())
	{
		return;
	}
	m_checkTime = RunTimeMonitor::getRunMilliseconds();
	if (g_curMessageInfo)
	{
		g_Log->flognoput_f(Zoic::INFO_LOG_LEVEL,g_RunTimeMonitor->getLogErrFileName(),".log","MessageRunTimeMonitorHelper create: g_curMessageInfo is not NULL! ");
	}
	if (!g_curModuleInfo)
	{
		g_Log->flognoput_f(Zoic::INFO_LOG_LEVEL,g_RunTimeMonitor->getLogErrFileName(),".log","MessageRunTimeMonitorHelper create: g_curModuleInfo is NULL! ");
		return;
	}
	MessageInfo * newInfo = new MessageInfo();
	newInfo->msgID = msgID;
	newInfo->runTime = 0;
	g_curModuleInfo->msgInfos.push_back(newInfo);
	g_curMessageInfo = *(g_curModuleInfo->msgInfos.rbegin());
}

MessageRunTimeMonitorHelper::~MessageRunTimeMonitorHelper()
{
	if (!g_RunTimeMonitor->isStarted())
	{
		return;
	}
	if (!g_curMessageInfo)
	{
		g_Log->flognoput_f(Zoic::INFO_LOG_LEVEL,g_RunTimeMonitor->getLogErrFileName(),".log","MessageRunTimeMonitorHelper destroy: g_curMessageInfo is NULL! ");
		return;
	}
	g_curMessageInfo->runTime =RunTimeMonitor::getRunMilliseconds() - m_checkTime;
	g_curMessageInfo=NULL;
}

FileRunTimeMonitorHelper::FileRunTimeMonitorHelper(const char * fileName, int line)
{
	if (!g_RunTimeMonitor->isStarted())
	{
		return;
	}
	m_checkTime = RunTimeMonitor::getRunMilliseconds();
	if (!g_curModuleInfo)
	{
		g_Log->flognoput_f(Zoic::INFO_LOG_LEVEL,g_RunTimeMonitor->getLogErrFileName(),".log","FileRunTimeMonitorHelper create: g_curModuleInfo is NULL! ");
		return;
	}
	FileInfo * newInfo = new FileInfo();
	newInfo->file = fileName;
	newInfo->line = line;
	newInfo->runTime = 0;
	newInfo->blank = g_curFileInfoList.size();
	g_curModuleInfo->fileInfos.push_back(newInfo);
	g_curFileInfoList.push_back(newInfo);
}

FileRunTimeMonitorHelper::~FileRunTimeMonitorHelper()
{
	if (!g_RunTimeMonitor->isStarted())
	{
		return;
	}
	if (g_curFileInfoList.empty())
	{
		g_Log->flognoput_f(Zoic::INFO_LOG_LEVEL,g_RunTimeMonitor->getLogErrFileName(),".log","FileRunTimeMonitorHelper destroy: g_curFileInfoList is EMPTY! ");
		return;
	}
	FileInfo* fileInfo = g_curFileInfoList.back();
	fileInfo->runTime = RunTimeMonitor::getRunMilliseconds() - m_checkTime;
	g_curFileInfoList.pop_back();
}

#endif
