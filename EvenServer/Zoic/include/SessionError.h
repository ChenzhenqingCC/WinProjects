/*! @file
	@ingroup network
	@brief �����������Ӵ���
*/

#ifndef __ZOIC_SESSIONERROR_H
#define __ZOIC_SESSIONERROR_H

namespace Zoic
{
	const int ERROR_DEFAULT			=-1;	//!< Ĭ�ϴ���
	const int ERROR_REMOTECLOSED	=-2;	//!< Զ�˹ر�
	const int ERROR_PINGTIMEOUT		=-3;	//!< PING��ʱ
	const int ERROR_SENDBUFFER		=-4;	//!< ���ͻ��治��
	const int ERROR_PROTOCOLVERSION	=-5;	//!< Э��汾����
	const int ERROR_UNPACK			=-6;	//!< ���Э��
	const int ERROR_PARSEMESSAGE	=-7;	//!< ����Э��
	const int ERROR_SENDMESSAGE		=-8;	//!< ����Э��
	const int ERROR_RECVFAILED		=-9;	//!< ����ʧ��
	const int ERROR_SENDFAILED		=-10;	//!< ����ʧ��1
	const int ERROR_SENDFAILED2		=-11;	//!< ����ʧ��2
	const int ERROR_SENDLOCK		=-12;	//!< ����ʧ��2
	const int ERROR_RECVLOCK		=-13;	//!< ����ʧ��2
	const int ERROR_RECVBUFFER		=-14;	//!< ���ܻ��治��
}

#endif
