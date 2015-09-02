#ifndef __RANASRV_RANASERVICESSTRATEGY_H
#define __RANASRV_RANASERVICESSTRATEGY_H
#include <vector>
#include <Zoic/include/Types.h>
#include <Zoic/include/Session.h>
#include "rana/RanaSpaceBank.h"

class RanaSpace;
class RanaClient;
class RanaServicesStrategy
{
	friend class RanaClient;
public:
	RanaServicesStrategy();
	
	//验证管理员
	/*
		返回值
		<0：正在处理中
		=0：成功
		>0：失败
	*/
	virtual int certifcate(const char *user,const char *pass,DWORD *access);

	virtual RanaEvent* CreateRanaEvent( RanaSpace* pSpace,WORD eventType,const char* pName,const std::vector<const char*> & args,const char* pBindTriggerName);
	virtual RanaTrigger* CreateRanaTrigger( RanaSpace* pSpace,const char* pName);

	virtual bool ModifyCondition( RanaSpace* pSpace,RanaTrigger* pTrigger,const char* conditionDetail);
	virtual bool ModifyAction( RanaSpace* pSpace,RanaTrigger* pTrigger,const char* actionDetail);

	virtual bool DeleteEvent( RanaSpace* pSpace,RanaEvent* pEvent);
	virtual bool DeleteTrigger( RanaSpace* pSpace,RanaTrigger* pTrigger);


	virtual const char* GetBaseScriptPath();

	virtual const char* GetEventCodeFromType( RanaEventType eventType );

	virtual bool GetEventTypeFromCode(const char* eventTypeCode, RanaEventType& eventTypeOut);

	virtual ~RanaServicesStrategy();
protected:
	virtual bool doOpenClient(RanaClient& ranaClient,SOCKET sock,const sockaddr_in &addr);
	virtual void doCloseClient(RanaClient& ranaClient);
};

#endif
