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



namespace GameEnum
{
	enum PlayerWorkEnum
	{
		PLAYERWORK_WARR = 1,		//սʿ
		LOGINERROR_MAGI = 2,		//��ʦ
		LOGINERROR_BOW = 3,			//����
	};
	static const BYTE MAXROLENUM = 3;	//����ɫ����
}

namespace GameType
{
	static const BYTE ACCNAMELEN = 64;	//����˺�������
	static const BYTE PASSLEN = 64;	//������볤��
	static const BYTE NICKNAMELEN = 64;	//����ɫ������
	typedef		Zoic::String<ACCNAMELEN>		UserAcc;	//�û��˺���
	typedef		Zoic::String<PASSLEN>		PassWord;	//����
	typedef		Zoic::String<NICKNAMELEN>		PlayerName;	//�û���ɫ��
	typedef		DWORD					UserID;//�û��˺ű��
	typedef		DWORD					PlayerID;//�û���ɫ���
	typedef		DWORD					PlayerLevel;//�û���ɫ�ȼ�
	typedef		BYTE					PlayerWork;//�û���ɫְҵ
}

#endif
