#include "stdafx.h"
#include <Zoic/include/Message.h>
#include "GameService.h"
#include "GameSession.h"
#include <Zoic/include/Log.h>
#include "GameDBConnection.h"
#include "AllQueryDefine.h"
#include "Zoic/xmlparse/xmlparser.h"
#include "RoleInitialAbility.schema.h"
#include "winsock2.h"



GameService::GameService()
	:m_clientEnable(true)
{
}

GameService::~GameService()
{
	
}

Zoic::SessionS * GameService::createSession()
{
	return new GameSession();
}

bool GameService::GetPlayerList(UserID uid, PlayerList & plist)
{
	std::map<UserID, PlayerList>::iterator it = allPlayers.find(uid);
	if (it != allPlayers.end())
	{
		plist = it->second;
		return true;
	}
	else
	{
		return false;
	}
}

bool GameService::IsAccNameUsed(UserAcc & accname)
{
	std::map<UserAcc, UserAccount>::iterator it = allUsers.find(accname);
	return it != allUsers.end();
}

bool GameService::IsNickNameUsed(PlayerName & nickname)
{
	for (std::map<UserID, PlayerList>::iterator it = allPlayers.begin(); it != allPlayers.end(); ++it)
	{
		PlayerList & plist = it->second;
		for (size_t i = 0; i < plist.size(); ++i)
		{
			if (nickname == plist[i].nickname)
			{
				return true;
			}
		}
	}
	return false;
}

bool GameService::CreateDBRole(UserID uid, P_TS_CreatePlayer_Req & cmd, PlayerID & pid)
{
	*PlayerBaseInsert << uid;
	*PlayerBaseInsert << 1;
	*PlayerBaseInsert << cmd.type;
	*PlayerBaseInsert << cmd.name;
	Recordset * dd = PlayerBaseInsert->use();
	if (dd)
	{
		pid = DWORD(PlayerBaseInsert->getInsertID());
		return true;
	}
	else
	{
		loginfo_f("新建角色插入数据库失败!");
		return false;
	}
}

bool GameService::FillPlayerBase(UserID uid, PlayerBase & plb)
{
	PlayerID pid = plb.pid;
	std::map<UserID, PlayerList>::iterator it = allPlayers.find(uid);
	if (it == allPlayers.end())
		return false;
	for (size_t i = 0; i < it->second.size(); ++i)
	{
		if (pid == it->second[i].pid)
		{
			plb.level = it->second[i].level;
			plb.nickname = it->second[i].nickname;
			plb.worktype = it->second[i].worktype;
			return true;
		}
	}
	return false;
}

void GameService::CalculateRoleProps(PlayerBase & plb)
{
	DWORD level = plb.level - 1;
	extern xml_config<RoleInitialAbility> RoleInitialAbilityCfg;
	RoleInitialAbility::InitMapIter iit = RoleInitialAbilityCfg.init.find(plb.worktype);
	RoleInitialAbility::GrowMapIter git = RoleInitialAbilityCfg.grow.find(plb.worktype);
	if (iit == RoleInitialAbilityCfg.init.end() || git == RoleInitialAbilityCfg.grow.end())
		return;
	RoleInitialAbility::Init & pbi = iit->second;
	RoleInitialAbility::Grow & pbg = git->second;
	plb.constitution = pbi.constitution() + pbg.constitution() * level;
	plb.power = pbi.power() + pbg.power() * level;
	plb.quick = pbi.quick() + pbg.quick() * level;
	plb.wisdom = pbi.wisdom() + pbg.wisdom() * level;
	plb.defense = pbi.defense() + pbg.defense() * level + plb.constitution * 2;
	plb.magdef = pbi.magdef() + pbg.magdef() * level + plb.wisdom * 3;
	plb.phyhurt = plb.power * 10 + plb.quick * 10;
	plb.maghurt = plb.wisdom * 10;
	plb.speed = plb.quick * 10;
	plb.maxhp = pbi.maxhp() + pbg.maxhp() * level + plb.constitution * 10;
	plb.maxmp = pbi.maxmp() + pbg.maxmp() * level + plb.wisdom * 10;
	plb.maxtp = pbi.maxtp() + pbg.maxtp() * level + plb.quick * 10;
}

bool GameService::CreateDBAcc(P_TS_CreateAcc_Req & cmd, UserID & uid)
{
	*UserAccInsert << cmd.username;
	*UserAccInsert << cmd.password;
	Recordset * dd = UserAccInsert->use();
	if (dd)
	{
		uid = DWORD(UserAccInsert->getInsertID());
		return true;
	}
	else
	{
		loginfo_f("注册信息插入数据库失败!");
		return false;
	}
}

ServerError GameService::CreateAccount(P_TS_CreateAcc_Req & cmd)
{
	if (IsAccNameUsed(cmd.username))
	{
		return REGERROR_EXIST;
	}

	UserID uid;
	if (CreateDBAcc(cmd, uid))
	{
		UserAccount uacc;
		uacc.uid = uid;
		uacc.username = cmd.username;
		uacc.password = cmd.password;
		allUsers[uacc.username] = uacc;
		return ERROR_OK;
	}
	else
	{
		return ERROR_DBERROR;
	}
}

ServerError GameService::CreateRole(UserID uid, P_TS_CreatePlayer_Req & cmd)
{
	if (uid > 0)
	{
		if (IsNickNameUsed(cmd.name))
		{
			return CREATEERROR_NAMEEXITED;
		}

		std::map<UserID, PlayerList>::iterator it = allPlayers.find(uid);
		PlayerList * plist = NULL;
		if (it != allPlayers.end())
		{
			plist = &(it->second);
			if (plist->size() > GameEnum::MAXROLENUM)
			{
				return CREATEERROR_ROLEFULL;
			}
		}
		
		PlayerID pid = 0;
		if (CreateDBRole(uid, cmd, pid))
		{
			PlayerDBBase pdb;
			pdb.uid = uid;
			pdb.pid = pid;
			pdb.level = 1;
			pdb.nickname = cmd.name;
			pdb.worktype = cmd.type;
			if (plist == NULL)
			{
				PlayerList newlist;
				newlist.push_back(pdb);
				allPlayers[uid] = newlist;
			}
			else
			{
				plist->push_back(pdb);
				
			}
			return ERROR_OK;
		}
		else
		{
			return ERROR_DBERROR;
		}
	}
	else
	{
		return CREATEERROR_NOUID;
	}
}

ServerError GameService::IsUserCanLogin(UserAcc usrname, PassWord pass, UserID & uid)
{
	std::map<UserAcc, UserAccount>::iterator it = allUsers.find(usrname);
	uid = 0;
	if (it == allUsers.end())
	{
		return LOGINERROR_NOACC;
	}
	else
	{
		if (it->second.password == pass)
		{
			uid = it->second.uid;
			return ERROR_OK;
		}
		else
		{
			return LOGINERROR_PASS;
		}
	}
}

void GameService::SelectAllDataFromDB(SelectType stype, Zoic::Query * query)
{
	if (query)
	{
		Recordset * dd = query->use();
		if (dd)
		{
			while (dd->fetch())
			{
				switch (stype)
				{
					case SELTYPE_USERACC:
					{
											UserAccount useracc;
											(*dd) >> useracc;
											allUsers[useracc.username] = useracc;
					}
					break;
				case SELTYPE_PLAYERBASE:
					{
										   PlayerDBBase playerbb;
										   (*dd) >> playerbb;
										   std::map<UserID, PlayerList>::iterator it = allPlayers.find(playerbb.uid);
										   if (it == allPlayers.end())
										   {
											   PlayerList playerlist;
											   playerlist.push_back(playerbb);
											   allPlayers[playerbb.uid] = playerlist;
										   }
										   else
										   {
											   PlayerList & playerlist = it->second;
											   playerlist.push_back(playerbb);
										   }
					}
					break;
				default:
					break;
				}
				
			}
		}
	}
}

void GameService::init()
{
	//用户账号数据提取
	SelectAllDataFromDB(SELTYPE_USERACC, UserAccSelect);
	SelectAllDataFromDB(SELTYPE_PLAYERBASE, PlayerBaseSelect);
}

int GameService::start()
{
	if( strcmp(getListenIP(),"")==0 )
	{
		loginfo_f("Can't Find LestenIP,Rana-Server Will not Listen For Rana-Client!");
		m_clientEnable = false;
	}
	int ret = 0;
	if( m_clientEnable )
	{
		ret=Zoic::SessionBank::start();
	}
	if(ret==0)
	{
		loginfo_f("GameService start");
	}
	return ret;
}

int GameService::run()
{
	if(m_clientEnable)
	{
		return Zoic::SessionBank::run();
	}
	return 0;
}

int GameService::stop()
{
	loginfo_f("GameService stop");
	if(m_clientEnable)
	{
		return Zoic::SessionBank::stop();
	}
	return 0;
}
