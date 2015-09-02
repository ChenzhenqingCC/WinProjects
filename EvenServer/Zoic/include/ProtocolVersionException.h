/*! @file
	@ingroup buildin
	@brief 定义协议版本异常类
*/

#ifndef __ZOIC_PROTOCOLVERSIONEXCEPTION_H
#define __ZOIC_PROTOCOLVERSIONEXCEPTION_H

#include "Exception.h"

namespace Zoic
{
	/*! @brief 协议版本异常类
		@ingroup buildin
	*/
	class  ProtocolVersionException:
		public Exception
	{
	public:
		//!构造函数
		ProtocolVersionException(
			DWORD localVersion,	//!< 本地版本
			DWORD farVersion	//!< 远程版本
			);
		virtual	~ProtocolVersionException() throw();
		DWORD getLocalVersion() const;
		DWORD getFarVersion() const;
	private:
		DWORD m_localVersion;
		DWORD m_farVersion;
	};
	
	inline ProtocolVersionException::ProtocolVersionException(DWORD localVersion,DWORD farVersion):
			Exception("Invalid protocol version.(local:%d,far:%d)",
			localVersion,
			farVersion),
			m_localVersion(localVersion),
			m_farVersion(farVersion)
	{
	}
	
	inline ProtocolVersionException::~ProtocolVersionException() throw()
	{
	}

	inline DWORD ProtocolVersionException::getLocalVersion() const
	{
		return m_localVersion;
	}

	inline DWORD ProtocolVersionException::getFarVersion() const
	{
		return m_farVersion;
	}
}

#endif
