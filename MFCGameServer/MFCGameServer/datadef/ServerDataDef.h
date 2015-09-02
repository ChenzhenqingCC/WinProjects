#ifndef __DATASRVDEF_H
#define __DATASRVDEF_H

#include "GameTypes.h"
#include "Zoic/include/Recordset.h"
#include <Zoic/include/Message.h>
using namespace GameType;
using namespace Zoic;

namespace GameDataDef
{//游戏数据结构定义

	//玩家账号
	struct UserAccount
	{
		UserID			uid;		//用户编号
		UserAcc			username;	//用户名
		PassWord		password;	//密码
		UserAccount()
		{
			uid = 0;
		}
	};

	//游戏角色数据库存储
	struct PlayerDBBase
	{
		PlayerID			pid;			//角色编号
		UserID				uid;			//所属用户编号
		PlayerLevel			level;			//角色等级
		PlayerWork			worktype;		//角色职业
		PlayerName			nickname;		//角色昵称
		PlayerDBBase()
		{
			pid = 0;
			uid = 0;
			level = 1;
			worktype = 0;
		}
	};

	//游戏角色主数据
	struct PlayerBase :public PlayerDBBase
	{
		DWORD maxhp;		//最大生命值
		DWORD maxmp;		//最大魔法值 
		DWORD maxtp;		//最大技力
		DWORD hp;			//生命值 health point
		DWORD mp;			//魔法值 mana point
		DWORD tp;			//技力 technology point
		DWORD power;		//力量
		DWORD wisdom;		//智力
		DWORD quick;		//敏捷
		DWORD constitution;	//体质
		DWORD fighting;		//战力
		DWORD phyhurt;		//物理攻击
		DWORD maghurt;		//魔法攻击
		DWORD defense;		//防御
		DWORD magdef;		//魔防
		DWORD speed;		//速度
		BYTE fireanti;		//炎抗
		BYTE iceanti;		//冰抗
		BYTE thunderanti;	//类抗
		BYTE darkanti;		//冥抗
		PlayerBase() :PlayerDBBase()
		{
			maxhp = 0;
			maxmp = 0;
			maxtp = 0;
			hp = 0;
			mp = 0;
			tp = 0;
			power = 0;
			wisdom = 0;
			quick = 0;
			constitution = 0;
			fighting = 0;
			phyhurt = 0;
			maghurt = 0;
			defense = 0;
			magdef = 0;
			speed = 0;
			fireanti = 0;
			iceanti = 0;
			thunderanti = 0;
			darkanti = 0;
		}
	};


	//数据库解包函数
	inline Recordset & operator >> (Recordset & rd, UserAccount &ua)
	{
		rd >> ua.uid;
		rd >> ua.username;
		rd >> ua.password;
		return rd;
	}

	inline Recordset & operator >> (Recordset & rd, PlayerDBBase &ua)
	{
		rd >> ua.pid;
		rd >> ua.uid;
		rd >> ua.level;
		rd >> ua.worktype;
		rd >> ua.nickname;
		return rd;
	}


	//消息封包函数
	inline Zoic::Message & operator << (Zoic::Message &m, const UserAccount & p)
	{
		m << p.uid;
		m << p.username;
		m << p.password;
		return m;
	}

	inline Zoic::Message & operator >> (Zoic::Message &m, UserAccount & p)
	{
		m >> p.uid;
		m >> p.username;
		m >> p.password;
		return m;
	}

	inline Zoic::Message & operator << (Zoic::Message &m, const PlayerDBBase & p)
	{
		m << p.pid;
		m << p.uid;
		m << p.level;
		m << p.worktype;
		m << p.nickname;
		return m;
	}

	inline Zoic::Message & operator >> (Zoic::Message &m, PlayerDBBase & p)
	{
		m << p.pid;
		m >> p.uid;
		m >> p.level;
		m >> p.worktype;
		m >> p.nickname;
		return m;
	}

	inline Zoic::Message & operator << (Zoic::Message &m, const PlayerBase & p)
	{
		m << p.pid;
		m << p.uid;
		m << p.level;
		m << p.worktype;
		m << p.nickname;
		m << p.maxhp;
		m << p.maxmp;
		m << p.maxtp;
		m << p.hp;
		m << p.mp;
		m << p.tp;
		m << p.power;
		m << p.wisdom;
		m << p.quick;
		m << p.constitution;
		m << p.fighting;
		m << p.phyhurt;
		m << p.maghurt;
		m << p.defense;
		m << p.magdef;
		m << p.speed;
		m << p.fireanti;
		m << p.iceanti;
		m << p.thunderanti;
		m << p.darkanti;
		return m;
	}

	inline Zoic::Message & operator >> (Zoic::Message &m, PlayerBase & p)
	{
		m >> p.pid;
		m >> p.uid;
		m >> p.level;
		m >> p.worktype;
		m >> p.nickname;
		m >> p.maxhp;
		m >> p.maxmp;
		m >> p.maxtp;
		m >> p.hp;
		m >> p.mp;
		m >> p.tp;
		m >> p.power;
		m >> p.wisdom;
		m >> p.quick;
		m >> p.constitution;
		m >> p.fighting;
		m >> p.phyhurt;
		m >> p.maghurt;
		m >> p.defense;
		m >> p.magdef;
		m >> p.speed;
		m >> p.fireanti;
		m >> p.iceanti;
		m >> p.thunderanti;
		m >> p.darkanti;
		return m;
	}
}


#endif
