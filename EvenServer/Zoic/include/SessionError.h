/*! @file
	@ingroup network
	@brief 定义网络连接错误
*/

#ifndef __ZOIC_SESSIONERROR_H
#define __ZOIC_SESSIONERROR_H

namespace Zoic
{
	const int ERROR_DEFAULT			=-1;	//!< 默认错误
	const int ERROR_REMOTECLOSED	=-2;	//!< 远端关闭
	const int ERROR_PINGTIMEOUT		=-3;	//!< PING超时
	const int ERROR_SENDBUFFER		=-4;	//!< 发送缓存不足
	const int ERROR_PROTOCOLVERSION	=-5;	//!< 协议版本不对
	const int ERROR_UNPACK			=-6;	//!< 解包协议
	const int ERROR_PARSEMESSAGE	=-7;	//!< 解析协议
	const int ERROR_SENDMESSAGE		=-8;	//!< 发送协议
	const int ERROR_RECVFAILED		=-9;	//!< 接收失败
	const int ERROR_SENDFAILED		=-10;	//!< 发送失败1
	const int ERROR_SENDFAILED2		=-11;	//!< 发送失败2
	const int ERROR_SENDLOCK		=-12;	//!< 发送失败2
	const int ERROR_RECVLOCK		=-13;	//!< 发送失败2
	const int ERROR_RECVBUFFER		=-14;	//!< 接受缓存不足
}

#endif
