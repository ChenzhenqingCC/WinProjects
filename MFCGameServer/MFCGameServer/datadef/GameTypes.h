/*! @file
	@ingroup flat_define
	@brief 游戏数据类型定义	
*/

#ifndef __GAME_TYPES_H
#define __GAME_TYPES_H
#include "Zoic/include/Types.h"
#include <vector>
#include <Zoic/include/Message.h>
#include <Zoic/include/String.h>
#include <Zoic/include/StrCharArray.h>



namespace GameEnum
{
	enum PlayerWorkEnum
	{
		PLAYERWORK_WARR = 1,		//战士
		LOGINERROR_MAGI = 2,		//法师
		LOGINERROR_BOW = 3,			//射手
	};
	static const BYTE MAXROLENUM = 3;	//最大角色数量
}

namespace GameType
{
	static const BYTE ACCNAMELEN = 64;	//最大账号名长度
	static const BYTE PASSLEN = 64;	//最大密码长度
	static const BYTE NICKNAMELEN = 64;	//最大角色名长度
	typedef		Zoic::String<ACCNAMELEN>		UserAcc;	//用户账号名
	typedef		Zoic::String<PASSLEN>		PassWord;	//密码
	typedef		Zoic::String<NICKNAMELEN>		PlayerName;	//用户角色名
	typedef		DWORD					UserID;//用户账号编号
	typedef		DWORD					PlayerID;//用户角色编号
	typedef		DWORD					PlayerLevel;//用户角色等级
	typedef		BYTE					PlayerWork;//用户角色职业
}

#endif
