#ifndef __DATASRVDEF_H
#define __DATASRVDEF_H

#include "GameTypes.h"
#include "Zoic/include/Recordset.h"
#include <Zoic/include/Message.h>
using namespace GameType;
using namespace Zoic;

namespace GameDataDef
{//��Ϸ���ݽṹ����

	//����˺�
	struct UserAccount
	{
		UserID			uid;		//�û����
		UserAcc			username;	//�û���
		PassWord		password;	//����
		UserAccount()
		{
			uid = 0;
		}
	};

	//��Ϸ��ɫ���ݿ�洢
	struct PlayerDBBase
	{
		PlayerID			pid;			//��ɫ���
		UserID				uid;			//�����û����
		PlayerLevel			level;			//��ɫ�ȼ�
		PlayerWork			worktype;		//��ɫְҵ
		PlayerName			nickname;		//��ɫ�ǳ�
		PlayerDBBase()
		{
			pid = 0;
			uid = 0;
			level = 1;
			worktype = 0;
		}
	};

	//��Ϸ��ɫ������
	struct PlayerBase :public PlayerDBBase
	{
		DWORD maxhp;		//�������ֵ
		DWORD maxmp;		//���ħ��ֵ 
		DWORD maxtp;		//�����
		DWORD hp;			//����ֵ health point
		DWORD mp;			//ħ��ֵ mana point
		DWORD tp;			//���� technology point
		DWORD power;		//����
		DWORD wisdom;		//����
		DWORD quick;		//����
		DWORD constitution;	//����
		DWORD fighting;		//ս��
		DWORD phyhurt;		//������
		DWORD maghurt;		//ħ������
		DWORD defense;		//����
		DWORD magdef;		//ħ��
		DWORD speed;		//�ٶ�
		BYTE fireanti;		//�׿�
		BYTE iceanti;		//����
		BYTE thunderanti;	//�࿹
		BYTE darkanti;		//ڤ��
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


	//���ݿ�������
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


	//��Ϣ�������
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
