/*! @file
@ingroup buildin
@brief 定义运行时间监控器类
*/

#ifndef __ZOIC_RUNTIMEMONITOR_H
#define __ZOIC_RUNTIMEMONITOR_H
#include "Types.h"
#include "Module.h"
#include <vector>
#include <string>
#ifndef _ZOIC_CLIENT

namespace Zoic
{
	/*! @brief 管理类运行时间监控
	@ingroup buildin
	*/
	struct MessageInfo
	{
		WORD msgID;
		TIME_MSEL runTime;
	};
	struct FileInfo
	{
		const char * file;
		int line;
		TIME_MSEL runTime;
		size_t blank;
	};
	struct ModuleInfo
	{
		const char * name;
		TIME_MSEL runTime;
		std::vector<MessageInfo*> msgInfos;
		std::vector<FileInfo*> fileInfos;
	};
	
	class RunTimeMonitor
#ifndef ZOIC_NO_SERVICE
		:public Module<RunTimeMonitor>
#else
		:public NoCopy
#endif
	{
#ifndef ZOIC_NO_SERVICE
		friend class Module<RunTimeMonitor>;
#endif
	public:
		static TIME_MSEL getRunMilliseconds();
		void check();
		void setLogName(const char * logName);
		ModuleInfo * insertModuleInfo(ModuleInfo * pInfo);
		void setStartStatus(bool isStarted);
		bool isStarted();
		void setOneModuleCheckTime(TIME_MSEL checkTime);
		const TIME_MSEL & getOneModuleCheckTime() const;
		void setAllModuleCheckTime(TIME_MSEL checkTime);
		const TIME_MSEL & getAllModuleCheckTime() const;
		void resetCheckStartTime();
		void setCheckStartTime(TIME_MSEL checkTime);
		const TIME_MSEL & getCheckStartTime() const;
		const char * getLogAllFileName();
		const char * getLogOneFileName();
		const char * getLogErrFileName();
#ifndef ZOIC_NO_SERVICE
	private:
#else
	public:
#endif
		RunTimeMonitor();
	private:
		void logOneModuleInfo(ModuleInfo * pInfo,const char * logFile);
		void logAllModuleInfo(TIME_MSEL allCost);
		std::vector<ModuleInfo*> m_moduleInfos;
		TIME_MSEL m_allModuleCheckTime;
		TIME_MSEL m_oneModuleCheckTime;
		TIME_MSEL m_checkStartTime;
		bool m_isStarted;
		std::string m_logAllFile;
		std::string m_logOneFile;
		std::string m_logErrFile;
	};
	
	inline void RunTimeMonitor::setStartStatus(bool isStarted)
	{
		m_isStarted = isStarted;
	}

	inline bool RunTimeMonitor::isStarted()
	{
		return m_isStarted;
	}

	inline void RunTimeMonitor::setOneModuleCheckTime(TIME_MSEL checkTime)
	{
		m_oneModuleCheckTime = checkTime;
	}

	inline const TIME_MSEL & RunTimeMonitor::getOneModuleCheckTime() const
	{
		return m_oneModuleCheckTime;
	}

	inline void RunTimeMonitor::setAllModuleCheckTime(TIME_MSEL checkTime)
	{
		m_allModuleCheckTime = checkTime;
	}

	inline const TIME_MSEL & RunTimeMonitor::getAllModuleCheckTime() const
	{
		return m_allModuleCheckTime;
	}

	inline const TIME_MSEL & RunTimeMonitor::getCheckStartTime() const
	{
		return m_checkStartTime;
	}

	inline const char * RunTimeMonitor::getLogAllFileName()
	{
		return m_logAllFile.c_str();
	}

	inline const char * RunTimeMonitor::getLogOneFileName()
	{
		return m_logOneFile.c_str();
	}

	inline const char * RunTimeMonitor::getLogErrFileName()
	{
		return m_logErrFile.c_str();
	}

	class ModuleRunTimeMonitorHelper
	{
	public:
		ModuleRunTimeMonitorHelper(const char * name);
		~ModuleRunTimeMonitorHelper();
	private:
		TIME_MSEL m_checkTime;
	};

	class MessageRunTimeMonitorHelper
	{
	public:
		MessageRunTimeMonitorHelper(WORD msgID);
		~MessageRunTimeMonitorHelper();
	private:
		TIME_MSEL m_checkTime;
	};

	class FileRunTimeMonitorHelper
	{
	public:
		FileRunTimeMonitorHelper(const char * fileName, int line);
		~FileRunTimeMonitorHelper();
	private:
		TIME_MSEL m_checkTime;
	};
}

#ifndef ZOIC_NO_SERVICE
#define g_RunTimeMonitor Zoic::RunTimeMonitor::getInstance()
#else
extern RunTimeMonitor theRunTimeMonitor;
#define g_RunTimeMonitor (&Zoic::theRunTimeMonitor)
#endif


#define MODULE_RUNTIME(NAME) Zoic::ModuleRunTimeMonitorHelper __temp_Module(NAME); 
#define MESSAGE_RUNTIME(ID) Zoic::MessageRunTimeMonitorHelper __temp_Message(ID); 
#define FILE_RUNTIME(NAME,LINE) Zoic::FileRunTimeMonitorHelper __temp_File(NAME,LINE); 

#else
#define MODULE_RUNTIME(CONTEXT)
#define MESSAGE_RUNTIME(ID)
#define FILE_RUNTIME(NAME,LINE)
#endif

#endif
