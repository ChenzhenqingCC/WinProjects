#include "include/RanaServicesStrategy.h"
#include "include/AccessMask.h"
#include "include/RanaClient.h"
#include <string>

namespace
{
	class DummyTrigger
		:public RanaTrigger
	{
		friend class ::RanaServicesStrategy;
	};
}

RanaServicesStrategy::RanaServicesStrategy()
{
}

RanaServicesStrategy::~RanaServicesStrategy()
{
}

int RanaServicesStrategy::certifcate(const char *user,const char *pass,DWORD *access)
{
	*access = RSAM_SPACE_ALL;
	return 0;
}

RanaEvent* RanaServicesStrategy::CreateRanaEvent(  RanaSpace* pSpace,WORD eventType,const char* pName,const std::vector<const char*> & args,const char* pTriggerName )
{
	return NULL;
}

RanaTrigger* RanaServicesStrategy::CreateRanaTrigger( RanaSpace* pSpace,const char* pName)
{
	RanaTriggerBank* pRanaTriggerBank = pSpace->getTriggerBank();
	if( pRanaTriggerBank->getTrigger(pName) )
	{
		return NULL;
	}
	if( 0 != DummyTrigger::cleanRanaScriptCondition(pSpace,pName) )
	{
		return NULL;
	}
	if( 0 != DummyTrigger::cleanRanaScriptAction(pSpace,pName) )
	{
		DummyTrigger::deleteRanaScriptCondition(pSpace,pName);
		return NULL;
	}
	RanaTrigger* pRanaTrigger = new RanaTrigger;
	if( pRanaTriggerBank->addTrigger((const RanaTriggerName)(pName),pRanaTrigger) != 0)
	{
		delete pRanaTrigger;
		DummyTrigger::deleteRanaScriptAction(pSpace,pName);
		DummyTrigger::deleteRanaScriptCondition(pSpace,pName);
		return NULL;
	}
	pRanaTrigger->setOnOff(true);
	return pRanaTrigger;
}

bool RanaServicesStrategy::DeleteEvent( RanaSpace* pSpace,RanaEvent* pEvent)
{
	RanaTriggerBank* pRanaTriggerBank = pSpace->getTriggerBank();
	if( pRanaTriggerBank->getEvent(pEvent->getHandle()) != pEvent )
	{
		return false;
	}
	if( pRanaTriggerBank->deleteEvent(pEvent->getHandle()) != 0 )
	{
		return false;
	}
	return true;
}

bool RanaServicesStrategy::DeleteTrigger( RanaSpace* pSpace,RanaTrigger* pTrigger)
{
	RanaTriggerBank* pRanaTriggerBank = pSpace->getTriggerBank();
	if( pRanaTriggerBank->getTrigger(pTrigger->getHandle()) != pTrigger )
	{
		return false;
	}
	const RanaTriggerName triggerName = pTrigger->getName();
	if( pSpace->getTriggerBank()->deleteTrigger( pTrigger->getHandle() ) != 0 )
	{
		return false;
	}
	DummyTrigger::deleteRanaScriptCondition(pSpace,triggerName);
	DummyTrigger::deleteRanaScriptAction(pSpace,triggerName);
	return true;
}

bool RanaServicesStrategy::ModifyCondition( RanaSpace* pSpace,RanaTrigger* pTrigger,const char* conditionDetail)
{
	RanaTriggerBank* pRanaTriggerBank = pSpace->getTriggerBank();
	if( pRanaTriggerBank->getTrigger(pTrigger->getHandle()) != pTrigger )
	{
		return false;
	}
	const RanaTriggerName &triggerName = pTrigger->getName();
	if( 0 != DummyTrigger::setRanaScriptCondition(pSpace,triggerName,conditionDetail) )
	{
		return false;
	}
	return true;
}

bool RanaServicesStrategy::ModifyAction( RanaSpace* pSpace,RanaTrigger* pTrigger,const char* actionDetail)
{
	RanaTriggerBank* pRanaTriggerBank = pSpace->getTriggerBank();
	if( pRanaTriggerBank->getTrigger(pTrigger->getHandle()) != pTrigger )
	{
		return false;
	}
	const RanaTriggerName &triggerName = pTrigger->getName();
	if( 0 != DummyTrigger::setRanaScriptAction(pSpace,triggerName,actionDetail) )
	{
		return false;
	}
	return true;
}

const char* RanaServicesStrategy::GetBaseScriptPath()
{
	return "";
}

const char* RanaServicesStrategy::GetEventCodeFromType( RanaEventType eventType )
{
	return "";
}

bool RanaServicesStrategy::GetEventTypeFromCode(const char* eventTypeCode, RanaEventType& eventTypeOut)
{
	return false;
}

bool RanaServicesStrategy::doOpenClient(RanaClient& ranaClient,SOCKET sock,const sockaddr_in &addr)
{
	return ranaClient.openClientDefault(sock,addr);
}

void RanaServicesStrategy::doCloseClient(RanaClient& ranaClient)
{
	ranaClient.closeClientDefault();
}
