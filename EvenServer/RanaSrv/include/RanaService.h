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
	
	//����Rana�ͻ���
	RanaClient * getClient(Zoic::Message &msg);
	RanaClient* getClient(HandleType handle,SerialType serial);
	const HandleType & getClientHandle();
	const SerialType & getClientSerial();
	//�õ�����ָ��
	RanaServicesStrategy * getStrategy(void);
	//���ò���ָ��
	void setStrategy(RanaServicesStrategy *strategy);
	//�õ��ű�Ŀ¼
	const char * getScriptPath(void);
	//���ýű�Ŀ¼
	void setScriptPath(const char *path);

	//������ʱϵͳ
	void updateCurrentTime();
#ifndef RANASRV_NO_MODULE
private:
#endif
	RanaService();
	~RanaService();
private:
	static void printRanaLog(const char* pChar);
	bool m_clientEnable;				//Rana��������SessionBank�Ƿ���
	RanaServicesStrategy * m_strategy;	//����ָ��
	HandleType	m_clientHandle;		//�ͻ�������
	SerialType	m_clientSerial;		//�ͻ������к�	
	std::string	m_scriptPath;		//�ű�Ŀ¼

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
