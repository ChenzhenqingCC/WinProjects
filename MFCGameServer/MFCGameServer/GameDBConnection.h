#ifndef __GAMESRV_GAMEDBCONNECTION_H
#define __GAMESRV_GAMEDBCONNECTION_H
#include <Zoic/include/Types.h>
#include <Zoic/include/Connection.h>
#include <Zoic/include/Module.h>
#include "Zoic/include/QueryDefine.h"

using namespace Zoic;

class GameDBConnection:
	public Zoic::Connection
	, public Zoic::Module<GameDBConnection>
{
public:
	void init();
	void disconnect();
};

#define g_DBConn GameDBConnection::getInstance()
#endif 
