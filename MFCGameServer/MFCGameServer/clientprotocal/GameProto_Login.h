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
		ERROR_OK = 0,			//成功
		ERROR_DBERROR = 1,		//数据库错误
		LOGINERROR_NOACC = 2,		//登入 账号不存在
		LOGINERROR_PASS = 3,		//登入 密码错误
		LOGINERROR_FULL = 4,		//登入 服务器已满
		LOGINERROR_NOCONN = 5,		//登入 服务器未连接
		REGERROR_EXIST = 6,			//注册 用户账号重复
		CREATEERROR_WORK = 7,			//建角色 职业错误
		CREATEERROR_NONAME = 8,		//建角色 昵称为空
		CREATEERROR_NAMEEXITED = 9,		//建角色 昵称已经存在
		CREATEERROR_ROLEFULL = 10,			//建角色 角色已满 最大值为MAXROLENUM
		CREATEERROR_NOUID = 11,		//建角色 账号错误
		CHOOSEROLE_FAILED = 12,		//选角色 失败
	};
}


namespace GameProto
{//协议定义
	//错误号声明


	//通用消息响应 GameService->GameClient
	static const WORD ST_USUAL_ACK = 1;
	struct P_ST_Usual_Ack
	{
		WORD MsgType;
		/*
		TS_LOGIN_REQ
		TS_CREATEPLAYER_REQ
		...
		*/
		ErrorType	code;	//返回码
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

	//玩家登录请求 GameClient->GameService
	static const WORD TS_LOGIN_REQ = 2;
	struct P_TS_Login_Req
	{
		UserAcc	username;	//用户名
		PassWord	password;	//密码
	};

	//玩家建角色 GameClient<->GameService
	static const WORD TS_CREATEPLAYER_REQ = 3;
	struct P_TS_CreatePlayer_Req
	{
		PlayerName name;
		PlayerWork type;
	};

	//玩家角色列表 GameClient<-GameService
	static const WORD TS_PLAYERLIST_ACK = 4;
	struct P_TS_PlayerList_Ack
	{
		std::vector<PlayerDBBase> playerlist;
	};

	//新建账号 GameClient->GameService
	static const WORD TS_CREATEACC_REQ = 5;
	struct P_TS_CreateAcc_Req
	{
		UserAcc	username;	//用户名
		PassWord	password;	//密码
	};

	//选择人物 GameClient->GameService
	static const WORD TS_CHOOSEROLE_REQ = 6;
	struct P_TS_ChooseRole_Req
	{
		PlayerID id;
	};

	//主人物数据 GameClient<-GameService
	static const WORD TS_MAINROLEDATA_ACK = 7;
	struct P_TS_MainRoleData_Ack
	{
		PlayerBase player;
	};

}

namespace GameProto
{//封包函数

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
