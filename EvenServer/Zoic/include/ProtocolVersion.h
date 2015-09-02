/*! @file
	@ingroup buildin
	@brief 定义协议版本类
*/

#ifndef __ZOIC_PROTOCOLVERSION_H
#define __ZOIC_PROTOCOLVERSION_H

#include "Types.h"
#include "Module.h"

namespace Zoic
{
	class Session;
	class Message;

	/*! @brief 协议版本类
		@ingroup buildin
	*/
	class ProtocolVersion
#ifndef ZOIC_NO_SERVICE
		:public Module<ProtocolVersion>
#else
		:public NoCopy
#endif
	{
#ifndef ZOIC_NO_SERVICE
		friend class Module<ProtocolVersion>;
#endif
	public:
		//!获得协议版本
		DWORD	getVersion(void);

		//!设置协议版本
		void	setVersion(
			DWORD version	//!< 协议版本
			);

		/*! @brief 协议版本处理函数
			@exception Zoic::ProtocolVersionException *　　协议版本不对
			@return 如果从消息中获得的版本与本地版本相同,返回true，否则抛出异常
		*/
		static bool onProtocolVersionNtf(
			Session *pSession,	//!< 连接对象指针
			Message &msg		//!< 网络消息
			);
#ifndef ZOIC_NO_SERVICE
	private:
#else
	public:
#endif
	ProtocolVersion();
	private:
		DWORD	m_version;	//!< 协议版本
	};
	
	inline ProtocolVersion::ProtocolVersion():
		m_version(0)
	{
	}
	
	inline DWORD ProtocolVersion::getVersion(void)
	{
		return m_version;
	}
	
	inline void ProtocolVersion::setVersion(DWORD version)
	{
		m_version=version;
	}

#ifndef ZOIC_NO_SERVICE
	#define g_ProtocolVersion Zoic::ProtocolVersion::getInstance()
#else
	extern ProtocolVersion theProtocolVersion;
	#define g_ProtocolVersion (&Zoic::theProtocolVersion)
#endif
}

#endif
