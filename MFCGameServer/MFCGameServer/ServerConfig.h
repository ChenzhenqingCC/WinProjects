#pragma once
#include <Zoic/include/Module.h>

class ServerConfig :public Zoic::Module<ServerConfig>
{
	friend class Zoic::Module<ServerConfig>;
public:
	ServerConfig();
	~ServerConfig();
	void init();
};

#define g_ServerConfig ServerConfig::getInstance()

