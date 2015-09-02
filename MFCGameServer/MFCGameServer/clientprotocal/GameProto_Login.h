#ifndef __PROTO_GAMELOGIN_H
#define __PROTO_GAMELOGIN_H

#include <vector>
#include <Zoic/include/Message.h>
#include <Zoic/include/String.h>
#include <Zoic/include/StrCharArray.h>
#include "ServerDataDef.h"

using namespace GameType;
using namespace GameDataDef;

namespace GameProto
{
	typedef WORD ErrorType;
	enum ServerError
	{
		ERROR_OK = 0,			//�ɹ�
		ERROR_DBERROR = 1,		//���ݿ����
		LOGINERROR_NOACC = 2,		//���� �˺Ų�����
		LOGINERROR_PASS = 3,		//���� �������
		LOGINERROR_FULL = 4,		//���� ����������
		LOGINERROR_NOCONN = 5,		//���� ������δ����
		REGERROR_EXIST = 6,			//ע�� �û��˺��ظ�
		CREATEERROR_WORK = 7,			//����ɫ ְҵ����
		CREATEERROR_NONAME = 8,		//����ɫ �ǳ�Ϊ��
		CREATEERROR_NAMEEXITED = 9,		//����ɫ �ǳ��Ѿ�����
		CREATEERROR_ROLEFULL = 10,			//����ɫ ��ɫ���� ���ֵΪMAXROLENUM
		CREATEERROR_NOUID = 11,		//����ɫ �˺Ŵ���
		CHOOSEROLE_FAILED = 12,		//ѡ��ɫ ʧ��
	};
}


namespace GameProto
{//Э�鶨��
	//���������


	//ͨ����Ϣ��Ӧ GameService->GameClient
	static const WORD ST_USUAL_ACK = 1;
	struct P_ST_Usual_Ack
	{
		WORD MsgType;
		/*
		TS_LOGIN_REQ
		TS_CREATEPLAYER_REQ
		...
		*/
		ErrorType	code;	//������
		/*
		ERROR_OK
		ERROR_FAIL
		*/
		P_ST_Usual_Ack()
		{
			MsgType = 0;
			code = ERROR_OK;
		}

	};

	//��ҵ�¼���� GameClient->GameService
	static const WORD TS_LOGIN_REQ = 2;
	struct P_TS_Login_Req
	{
		UserAcc	username;	//�û���
		PassWord	password;	//����
	};

	//��ҽ���ɫ GameClient<->GameService
	static const WORD TS_CREATEPLAYER_REQ = 3;
	struct P_TS_CreatePlayer_Req
	{
		PlayerName name;
		PlayerWork type;
	};

	//��ҽ�ɫ�б� GameClient<-GameService
	static const WORD TS_PLAYERLIST_ACK = 4;
	struct P_TS_PlayerList_Ack
	{
		std::vector<PlayerDBBase> playerlist;
	};

	//�½��˺� GameClient->GameService
	static const WORD TS_CREATEACC_REQ = 5;
	struct P_TS_CreateAcc_Req
	{
		UserAcc	username;	//�û���
		PassWord	password;	//����
	};

	//ѡ������ GameClient->GameService
	static const WORD TS_CHOOSEROLE_REQ = 6;
	struct P_TS_ChooseRole_Req
	{
		PlayerID id;
	};

	//���������� GameClient<-GameService
	static const WORD TS_MAINROLEDATA_ACK = 7;
	struct P_TS_MainRoleData_Ack
	{
		PlayerBase player;
	};

}

namespace GameProto
{//�������

	inline Zoic::Message & operator << (Zoic::Message &m, const P_ST_Usual_Ack & p)
	{
		m << p.MsgType;
		m << p.code;
		return m;
	}

	inline Zoic::Message & operator >> (Zoic::Message &m, P_ST_Usual_Ack & p)
	{
		m >> p.MsgType;
		m >> p.code;
		return m;
	}

	inline Zoic::Message & operator << (Zoic::Message &m, const P_TS_CreateAcc_Req & p)
	{
		m << p.username;
		m << p.password;
		return m;
	}

	inline Zoic::Message & operator >> (Zoic::Message &m, P_TS_CreateAcc_Req & p)
	{
		m >> p.username;
		m >> p.password;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_Login_Req & p )
	{
		m << p.username;
		m << p.password;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_Login_Req & p )
	{
		m >> p.username;
		m >> p.password;
		return m;
	}

	inline Zoic::Message & operator << (Zoic::Message &m, const P_TS_CreatePlayer_Req & p)
	{
		m << p.name;
		m << p.type;
		return m;
	}

	inline Zoic::Message & operator >> (Zoic::Message &m, P_TS_CreatePlayer_Req & p)
	{
		m >> p.name;
		m >> p.type;
		return m;
	}

	inline Zoic::Message & operator << (Zoic::Message &m, const P_TS_PlayerList_Ack & p)
	{
		m << p.playerlist;
		return m;
	}

	inline Zoic::Message & operator >> (Zoic::Message &m, P_TS_PlayerList_Ack & p)
	{
		m >> p.playerlist;
		return m;
	}

	inline Zoic::Message & operator << (Zoic::Message &m, const P_TS_ChooseRole_Req & p)
	{
		m << p.id;
		return m;
	}

	inline Zoic::Message & operator >> (Zoic::Message &m, P_TS_ChooseRole_Req & p)
	{
		m >> p.id;
		return m;
	}

	inline Zoic::Message & operator << (Zoic::Message &m, const P_TS_MainRoleData_Ack & p)
	{
		m << p.player;
		return m;
	}

	inline Zoic::Message & operator >> (Zoic::Message &m, P_TS_MainRoleData_Ack & p)
	{
		m >> p.player;
		return m;
	}

}

#endif
