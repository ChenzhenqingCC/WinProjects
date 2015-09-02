#include "include/Exception.h"
#include "include/CommandFactory.h"
#include "include/Message.h"
#include "include/SessionCommand.h"
#include "include/SysPing.h"
#include "include/ProtocolVersion.h"
#include "include/UnhandledMessageException.h"
#ifndef _ZOIC_CLIENT
#include "include/RunTimeMonitor.h"
#endif


using namespace Zoic;

CommandFactory::CommandFactory(MESSAGE_FUNC defaultFunc)
	:m_runTimeChecker(NULL)
{
	for(int i=0;i<sizeof(m_commands)/sizeof(m_commands[0]);i++)
	{
		m_commands[i]=defaultFunc;
	}
}

bool CommandFactory::onDummyMessage(Session *pSession,Message &msg)
{
	throw new UnhandledMessageException(msg.ID(),msg.getSize());
	return false;
}

bool CommandFactory::parseMessage(Session *pSession,Message &msg)
{
	ZOIC_VMP_BEGIN
#ifndef _ZOIC_CLIENT
	MESSAGE_RUNTIME(msg.ID());
#endif
	if(!m_runTimeChecker)
	{
		return (*m_commands[msg.ID()])(pSession,msg);
	}
	m_runTimeChecker->runTimeCheckBegin(pSession);
	const bool res = (*m_commands[msg.ID()])(pSession,msg);
	m_runTimeChecker->runTimeCheckEnd(pSession);
	ZOIC_VMP_END
	return res;
}

void CommandFactory::registerCommand(WORD cmd,MESSAGE_FUNC func)
{
	m_commands[cmd]=func;
}

void CommandFactory::registerCommands(const std::type_info &typeinfo)
{
	registerCommand(SYSPING_REQ,onSysPingReq);
	registerCommand(SYSPING_ACK,onSysPingAck);
	registerCommand(PROTOCOL_VERSION_NTF,ProtocolVersion::onProtocolVersionNtf);

	CommandClass & commandclass=getCommandStorage().getClassInfo(typeinfo);
	
	for(CommandClass::iterator it=commandclass.begin();it!=commandclass.end();++it)
	{
		registerCommand(it->command,it->function);
	}
}

void CommandFactory::registerRunTimeCheck(CommandFactory::RUNTIME_CHECKER* pChecker)
{
	m_runTimeChecker = pChecker;
}

