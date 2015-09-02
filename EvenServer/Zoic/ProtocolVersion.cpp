#include "include/ProtocolVersion.h"
#include "include/ProtocolVersionException.h"
#include "include/Message.h"
#include "include/Session.h"
#include "include/SessionError.h"

using namespace Zoic;
#ifdef ZOIC_NO_SERVICE
	ProtocolVersion Zoic::theProtocolVersion;
#endif

bool ProtocolVersion::onProtocolVersionNtf(Session *pSession,Message &msg)
{
#ifdef _DEBUG
	DWORD version;
	msg >> version;
	if(version!=g_ProtocolVersion->getVersion())
	{
		pSession->setError(ERROR_PROTOCOLVERSION);
		throw new ProtocolVersionException(g_ProtocolVersion->getVersion(),version);
	}
#endif
	return true;
}
