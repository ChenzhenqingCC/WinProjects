/*! @file
	@ingroup buildin
	@brief ����������־��
*/

#ifndef __ZOIC_NETLOG_H
#define __ZOIC_NETLOG_H

#include "Types.h"
#include "Module.h"

namespace Zoic
{
	typedef int LogLevelValue;
	extern const LogLevelValue DEBUG_LOG_LEVEL;
}

namespace Zoic
{
	class Logger;
	/*! @brief ��־��
		@ingroup buildin


		�������շ����ݶ���¼������
	*/
	class NetLog:
		public Module<NetLog>
	{
		friend class Module<NetLog>;
	public:
		/*! @brief ����������־
			
			
			����������־����Ŀ¼��,��Ҫ��־ǰ׺�ͺ�׺(���Ϊ����д��Ҫ��־)<br>
			ֻ���������˸ú������Ժ���д������־ʱ����Ч������д
		*/
		void config(
			const char *basedir,		//!< ����Ŀ¼
			const char *prefix=NULL,	//!< ��Ҫ��־ǰ׺
			const char *suffix=NULL		//!< ��Ҫ��־��׺
			);
		
		/*! @brief д������־
			
			
			��ʹ��ǰӦ�ȵ���config,������ǿպ�������
		*/
		void netlog(
			const char *ip,			//IP��ַ
			DWORD port,				//�˿ں�
			const char *local_ip,	//����IP��ַ
			DWORD local_port,		//���ض˿ں�
			const void *buffer,		//���ݻ���
			int len,				//���ݳ���
			bool recv,				//���ձ�־(true:����;false:����)
			LogLevelValue logLevel = DEBUG_LOG_LEVEL
			);

		/*! @brief д������־


		ͨ��ÿ����Config��������ʱд������־
		*/
		void netlogWithConfig(
			LogLevelValue logLevel,
			const char *basedir,			//!< ����Ŀ¼(��β������б��)
			const char *ip,					//!< IP��ַ
			DWORD port,						//!< �˿ں�
			const char *binary_pathname,	//!< �����ļ���(�����basedir)
			const void *buffer,				//!< ���ݻ���
			int len,						//!< ���ݳ���
			bool recv,						//!< ���ձ�־(true:����;false:����)
			const char *brief_pathname		//!< ��Ҫ��־·��
			);
		
	private:
		NetLog();
		//!������־
		void local_flog(
			LogLevelValue logLevel,	//!< ��־����
			const char *pathname,	//!< ·��
			const char *msg,	//!< ���Ƹ�ʽ
			...);

		//!����ʱ��
		void updateTime();
		std::string m_basedir;		//!< ��־����Ŀ¼
		std::string m_prefix;		//!< ǰ׺
		std::string m_suffix;		//!< ��׺
		TIME_SEC	m_lasttime;		//!< ��һ�ε���log��ʱ��
		char 		m_timebuf[20];	//!< ʱ���ַ���ֵ
		char		m_datebuf[20];	//!< �����ַ���ֵ
	};
}

#define netlog Zoic::NetLog::getInstance()->netlog
#define g_Netlog Zoic::NetLog::getInstance()

#endif
