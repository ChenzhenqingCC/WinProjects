#include "stdafx.h"
#include <Zoic/include/Log.h>
#include "GameDBConnection.h"

void GameDBConnection::init()
{
	setConnectParams("gamedb", "127.0.0.1", "root", "", 3306);
	if (connect())
	{
		loginfo_f("GameDB connected");
	}
	else
	{
		loginfo_f("GameDB connect failed");
	}
}

void GameDBConnection::disconnect()
{
	close();
}