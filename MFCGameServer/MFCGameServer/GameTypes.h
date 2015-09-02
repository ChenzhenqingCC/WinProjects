/*! @file
	@ingroup flat_define
	@brief ��Ϸ�������Ͷ���	
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
	typedef		Zoic::String<32>		UserAcc;	//�û��˺���
	typedef		Zoic::String<32>		PassWord;	//����
	typedef		Zoic::String<32>		PlayerName;	//�û���ɫ��
	typedef		DWORD					UserID;//�û��˺ű��
	typedef		DWORD					PlayerID;//�û���ɫ���
	typedef		DWORD					PlayerLevel;//�û���ɫ�ȼ�
	typedef		DWORD					PlayerWork;//�û���ɫ�ȼ�
}

#endif
