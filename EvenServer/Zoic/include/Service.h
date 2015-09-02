/*! @file
	@ingroup service
	@brief 定义服务类
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
	//!模块启动函数类型
	typedef int (*FUNC_MODULE_INITIAL)(__ModuleContext *context);

	//!模块启动函数类型
	typedef int (*FUNC_MODULE_START)(__ModuleContext *context);
	
	//!模块运行函数类型
	typedef int (*FUNC_MODULE_RUN)(__ModuleContext *context);
	
	//!模块停止函数类型
	typedef int (*FUNC_MODULE_STOP)(__ModuleContext *context);
	
	/*! @brief 服务类
		@ingroup service
	*/
	class Service:
		public Singleton<Service>
	{
		friend class Singleton<Service>;
		friend class ::ModuleInit;
	public:
		//!保存启动参数
		void setArgs(
			int argc,		//!< 参数数量
			char ** argv	//!< 参数列表
			);

		//!获得启动参数数量
		int getArgc();

		//!获得启动参数列表
		char ** getArgv();
		
		//!开始初始化工作
		int start();

		//!运行程序
		int run();

		//!运行一次run,如果每次重复run的过程需要通过外部控制时使用
		int runOnce();

		//!退出程序
		void stop();

		//!注册程序模块
		void registModule(
			__ModuleContext *context,					//!< 模块对象指针
			FUNC_MODULE_INITIAL initial,	//!< 开始函数
			FUNC_MODULE_START start,		//!< 开始函数
			FUNC_MODULE_RUN run,			//!< 运行函数
			FUNC_MODULE_STOP stop			//!< 停止函数
			);

		unsigned long getRunSerial() const;
		void setSleepSkipRate(unsigned long rate);
		unsigned long getSleepSkipRate() const;
	private:
		int initial();
		/*! @brief 设置初始化模块


			初始化模块在start()最开始运行,在stop()中最后运行。
		*/
		void setInitModule(
			__ModuleContext *context		//!< 模块对象指针
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
		std::vector<ModuleStart>	m_startFuncs;	//!< 启动模块集合
		std::vector<ModuleRun>		m_runFuncs;		//!< 运行模块集合
		std::vector<ModuleStop>		m_stopFuncs;	//!< 停止模块集合

		unsigned long m_runSerial;
		unsigned long m_sleepSkipRate;

		int			m_argc;		//!< 启动参数数量
		char ** 	m_argv;		//!< 启动参数列表
	};
	#define g_Service Zoic::Service::getInstance()
}

#endif
#endif
