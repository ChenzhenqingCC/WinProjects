#ifndef __RANASRV_RANASERVICE_H
#define __RANASRV_RANASERVICE_H
#include <string>
#include <Zoic/include/Module.h>
#include <Zoic/include/SessionBank.h>
#include "RanaServicesStrategy.h"

#ifdef ZOIC_NO_SERVICE
#ifndef RANASRV_NO_MODULE
#define RANASRV_NO_MODULE
#endif
#endif

class RanaClient;
class RanaService:
	public Zoic::SessionBank
#ifndef RANASRV_NO_MODULE
	,public Zoic::Module<RanaService>
#endif
{
#ifndef RANASRV_NO_MODULE
	friend class Zoic::Module<RanaService>;
#endif
public:
	Zoic::SessionS *createSession();
	int start();
	int run();
	int stop();
	
	//查找Rana客户端
	RanaClient * getClient(Zoic::Message &msg);
	RanaClient* getClient(HandleType handle,SerialType serial);
	const HandleType & getClientHandle();
	const SerialType & getClientSerial();
	//得到策略指针
	RanaServicesStrategy * getStrategy(void);
	//设置策略指针
	void setStrategy(RanaServicesStrategy *strategy);
	//得到脚本目录
	const char * getScriptPath(void);
	//设置脚本目录
	void setScriptPath(const char *path);

	//触发计时系统
	void updateCurrentTime();
#ifndef RANASRV_NO_MODULE
private:
#endif
	RanaService();
	~RanaService();
private:
	static void printRanaLog(const char* pChar);
	bool m_clientEnable;				//Rana服务器的SessionBank是否工作
	RanaServicesStrategy * m_strategy;	//策略指针
	HandleType	m_clientHandle;		//客户端名柄
	SerialType	m_clientSerial;		//客户端序列号	
	std::string	m_scriptPath;		//脚本目录

};

inline const HandleType & RanaService::getClientHandle()
{
	return m_clientHandle;
}

inline const SerialType & RanaService::getClientSerial()
{
	return m_clientSerial;
}

inline RanaServicesStrategy * RanaService::getStrategy(void)
{
	return m_strategy;
}

inline void RanaService::setStrategy(RanaServicesStrategy *strategy)
{
	if(m_strategy)
	{
		delete m_strategy;
	}
	m_strategy=strategy;
}

inline const char * RanaService::getScriptPath(void)
{
	return m_scriptPath.c_str();
}

inline void RanaService::setScriptPath(const char *path)
{
	m_scriptPath=path;
}

#ifndef RANASRV_NO_MODULE
#define g_RanaService RanaService::getInstance()
#else
extern RanaService theRanaService;
#define g_RanaService (&theRanaService)
#endif
	
#endif
