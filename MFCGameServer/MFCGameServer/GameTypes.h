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


namespace GameType
{
	typedef		Zoic::String<32>		UserAcc;	//用户账号名
	typedef		Zoic::String<32>		PassWord;	//密码
	typedef		Zoic::String<32>		PlayerName;	//用户角色名
	typedef		DWORD					UserID;//用户账号编号
	typedef		DWORD					PlayerID;//用户角色编号
	typedef		DWORD					PlayerLevel;//用户角色等级
	typedef		DWORD					PlayerWork;//用户角色等级
}

#endif
