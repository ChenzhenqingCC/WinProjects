#include "stdafx.h"
#include "ServerConfig.h"
#include "CCFileUtils.h"
#include <map>
#include "Zoic/xmlparse/xmlparser.h"
#include "RoleInitialAbility.schema.h"

xml_config<RoleInitialAbility> RoleInitialAbilityCfg;


ServerConfig::ServerConfig()
{

}

ServerConfig::~ServerConfig()
{

}

void ServerConfig::init()
{
	RoleInitialAbilityCfg.load(FileUtils::getInstance()->fullPathForFilename("RoleInitialAbility.xml").c_str());
}

