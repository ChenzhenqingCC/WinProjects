/*! @file
	@ingroup buildin
	@brief ����Э��汾��
*/

#ifndef __ZOIC_PROTOCOLVERSION_H
#define __ZOIC_PROTOCOLVERSION_H

#include "Types.h"
#include "Module.h"

namespace Zoic
{
	class Session;
	class Message;

	/*! @brief Э��汾��
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
		//!���Э��汾
		DWORD	getVersion(void);

		//!����Э��汾
		void	setVersion(
			DWORD version	//!< Э��汾
			);

		/*! @brief Э��汾������
			@exception Zoic::ProtocolVersionException *����Э��汾����
			@return �������Ϣ�л�õİ汾�뱾�ذ汾��ͬ,����true�������׳��쳣
		*/
		static bool onProtocolVersionNtf(
			Session *pSession,	//!< ���Ӷ���ָ��
			Message &msg		//!< ������Ϣ
			);
#ifndef ZOIC_NO_SERVICE
	private:
#else
	public:
#endif
	ProtocolVersion();
	private:
		DWORD	m_version;	//!< Э��汾
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
