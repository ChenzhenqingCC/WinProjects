#include "include/SessionS.h"
#include "include/CommandFactory.h"

using namespace Zoic;

SessionS::SessionS(SessionUnpackMethod* unpack_method)
	:Session(unpack_method)
	,m_ready(true)
	,m_commandFactory(NULL)
{
}

bool SessionS::parseMessage(Message &msg)
{
	return m_commandFactory->parseMessage(this,msg);
}
