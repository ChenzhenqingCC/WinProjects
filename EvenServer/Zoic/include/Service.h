/*! @file
	@ingroup service
	@brief ���������
*/

#ifndef _ZOIC_SERVICE_H
#define _ZOIC_SERVICE_H
#ifndef ZOIC_NO_SERVICE

#include "Singleton.h"
#include <vector>
#include <string>

class ModuleInit;

namespace Zoic
{
	class __ModuleContext;
	//!ģ��������������
	typedef int (*FUNC_MODULE_INITIAL)(__ModuleContext *context);

	//!ģ��������������
	typedef int (*FUNC_MODULE_START)(__ModuleContext *context);
	
	//!ģ�����к�������
	typedef int (*FUNC_MODULE_RUN)(__ModuleContext *context);
	
	//!ģ��ֹͣ��������
	typedef int (*FUNC_MODULE_STOP)(__ModuleContext *context);
	
	/*! @brief ������
		@ingroup service
	*/
	class Service:
		public Singleton<Service>
	{
		friend class Singleton<Service>;
		friend class ::ModuleInit;
	public:
		//!������������
		void setArgs(
			int argc,		//!< ��������
			char ** argv	//!< �����б�
			);

		//!���������������
		int getArgc();

		//!������������б�
		char ** getArgv();
		
		//!��ʼ��ʼ������
		int start();

		//!���г���
		int run();

		//!����һ��run,���ÿ���ظ�run�Ĺ�����Ҫͨ���ⲿ����ʱʹ��
		int runOnce();

		//!�˳�����
		void stop();

		//!ע�����ģ��
		void registModule(
			__ModuleContext *context,					//!< ģ�����ָ��
			FUNC_MODULE_INITIAL initial,	//!< ��ʼ����
			FUNC_MODULE_START start,		//!< ��ʼ����
			FUNC_MODULE_RUN run,			//!< ���к���
			FUNC_MODULE_STOP stop			//!< ֹͣ����
			);

		unsigned long getRunSerial() const;
		void setSleepSkipRate(unsigned long rate);
		unsigned long getSleepSkipRate() const;
	private:
		int initial();
		/*! @brief ���ó�ʼ��ģ��


			��ʼ��ģ����start()�ʼ����,��stop()��������С�
		*/
		void setInitModule(
			__ModuleContext *context		//!< ģ�����ָ��
			);
		Service();
		~Service();
		struct ModuleInitial
		{
			FUNC_MODULE_INITIAL	func;
			__ModuleContext * 		context;
		};
		struct ModuleStart
		{
			FUNC_MODULE_START	func;
			__ModuleContext * 		context;
		};
		struct ModuleRun
		{
			FUNC_MODULE_RUN		func;
			__ModuleContext * 		context;
		};
		struct ModuleStop
		{
			FUNC_MODULE_STOP	func;
			__ModuleContext * 		context;
		};

		std::vector<__ModuleContext*>		m_allModules;
		std::vector<ModuleInitial>		m_initialFuncs;
		std::vector<ModuleStart>	m_startFuncs;	//!< ����ģ�鼯��
		std::vector<ModuleRun>		m_runFuncs;		//!< ����ģ�鼯��
		std::vector<ModuleStop>		m_stopFuncs;	//!< ֹͣģ�鼯��

		unsigned long m_runSerial;
		unsigned long m_sleepSkipRate;

		int			m_argc;		//!< ������������
		char ** 	m_argv;		//!< ���������б�
	};
	#define g_Service Zoic::Service::getInstance()
}

#endif
#endif
