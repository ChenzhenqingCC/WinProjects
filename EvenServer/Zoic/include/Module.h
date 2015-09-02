/*! @file
	@ingroup service
	@brief 定义模块模板
*/

#ifndef __ZOIC_MODULE_H__
#define __ZOIC_MODULE_H__
#include <vector>
#include <typeinfo>
#include "NoCopy.h"
#ifndef ZOIC_NO_SERVICE

#include "Service.h"

namespace Zoic
{
	/*! @brief 模块模板
		@ingroup service
		@param T 类型


		模块是一种类似单例的东西，不过它有自己的start,run,stop方法。<br>
		在start和stop时可以设定不同模块之间的先后顺序。
	*/

	class __ModuleContext
	{
		friend class Service;
		template <class T> friend class Module;
	public:
		//!获得模块名称
		const char* getModuleName() const;
		//!判断模块是否成功运行过start，控制start顺序
		bool isStarted() const;
		//!增加本模块的引用计数,控制stop顺序
		void addRef();
		//!释放本模块的引用计数,控制stop顺序
		void releaseRef();
	protected:
		__ModuleContext():m_moduleName(NULL),m_started(false),m_refs(0){};
		virtual ~__ModuleContext(){};
	private:
		const char *	m_moduleName;	//!< 模块名
		bool			m_started;		//!< 是否准备好
		unsigned int	m_refs;			//!< 引用计数
	};

	template <class T> class  Module
		:public __ModuleContext
		,public NoCopy
	{
		friend class INIT;
		friend class Releaser;
		class Zero
		{
		public:
			operator int() const;
		private:
			char m_reserved[11];
		};
		class True
		{
		public:
			operator bool() const;
		private:
			char m_reserved[11];
		};
	public:
		//!获得模块指针
		static T* getInstance(void);


		/*! @brief 模块依赖性测查，控制start顺序


			检查本模块所依赖的模块是否成功运行start<br>
			该函数可以被子类用 bool checkStart() 函数覆盖。编译器会自动决定用哪一个checkStart。
		*/
		True checkStart();

	protected:
		Module();
		virtual ~Module() = 0;

		/*! @brief 模块初始化
			初始化和构造函数的不同点在于init函数运行时可以通过g_Service获得通过main函数传入的标志
			从而通过运行参数决定该模块的初始化的行为

			构造函数会决定用是否向Service注册init方法。<br>
			只有子类用 int initial() 函数覆盖该函数后，本类的静态initial函数才会被注册到Serivce中。
		*/
		Zero initial();
		/*! @brief 模块启动



			构造函数会决定用是否向Service注册start方法。<br>
			只有子类用 int start() 函数覆盖该函数后，本类的静态start函数才会被注册到Serivce中。
		*/
		Zero start();

		/*! @brief 模块运行


			构造函数会决定用是否向Service注册run方法。<br>
			只有子类用 int run() 函数覆盖该函数后，本类的静态run函数才会被注册到Serivce中。
		*/
		Zero run();
		/*! @brief 模块停止


			构造函数会决定用是否向Service注册stop方法。<br>
			只有子类用 int stop() 函数覆盖该函数后，本类的静态stop函数才会被注册到Serivce中。
		*/
		Zero stop();
	private:
		static T *		m_instance;		//!< 模块对象指针

		//!检查能否结束
		bool checkStop();
		
		//!模块初始化
		static int initial(
			__ModuleContext *context	//! 模块对象指针
			);

		//!模块启动
		static int start(
			__ModuleContext *context	//! 模块对象指针
			);

		//!模块运行
		static int run(
			__ModuleContext *context	//! 模块对象指针
			);

		//!模块停止
		static int stop(
			__ModuleContext *context	//! 模块对象指针
			);

		//!删除模块
		static void destroy();
	};
	
	template <class T> T* Module<T>::m_instance = new T;
	
	class ModuleRleaser
	{
		template <class T> friend class Module;
	public:
		~ModuleRleaser();
	private:
		typedef void (*DESTROY_FUNC)();
		std::vector<DESTROY_FUNC> m_funcs;
	};
	
	inline ModuleRleaser::~ModuleRleaser()
	{
		for(std::vector<DESTROY_FUNC>::const_iterator it=m_funcs.begin();it!=m_funcs.end();++it)
		{
			(**it)();
		}
		m_funcs.clear();
	}
	
	inline ModuleRleaser & getModuleRleaser()
	{
		static ModuleRleaser releaser;
		return releaser;
	}
   
	inline std::vector<void *> & getZoicModulesVector()
	{
		static std::vector<void *> modulesVector;
		return modulesVector;    	
	}
	
	template <class T>
	Module<T>::Zero::operator int() const
	{
		return 0;
	}
	
	template <class T>
	Module<T>::True::operator bool() const
	{
		return true;
	}

	template <class T>
	T* Module<T>::getInstance(void)
	{
		return m_instance;
	}
	
	template <class T>
	typename Module<T>::True Module<T>::checkStart()
	{
		return True();
	}

	template <class T>
	Module<T>::Module()
	{
		//比较类T有没有自定义init函数
		FUNC_MODULE_INITIAL fnInitial;
		if(sizeof(static_cast<T *>(0)->initial())==sizeof(int) || sizeof(static_cast<T *>(0)->initial())==sizeof(bool))
		{
			fnInitial=initial;
		}
		else
		{			
			fnInitial=0;
		}
		//比较类T有没有自定义start函数
		FUNC_MODULE_START fnStart;
		if(sizeof(static_cast<T *>(0)->start())==sizeof(int) || sizeof(static_cast<T *>(0)->checkStart())==sizeof(bool))
		{
			fnStart=start;
			m_started=false;
		}
		else
		{			
			fnStart=0;
			m_started=true;
		}
		
		//比较类T有没有自定义run函数
		FUNC_MODULE_START fnRun;
		if(sizeof(static_cast<T *>(0)->run())==sizeof(int))
		{
			fnRun=run;
		}
		else
		{
			fnRun=0;
		}
		
		//总是设置stop函数,以检查退出时的依赖性
		FUNC_MODULE_START fnStop=stop;
		
		Service::getInstance()->registModule(
			this,
			fnInitial,
			fnStart,
			fnRun,
			fnStop
			);
		getModuleRleaser().m_funcs.push_back(&destroy);
	}
	
	template <class T>
	Module<T>::~Module()
	{
	}

	template <class T>
	typename Module<T>::Zero Module<T>::initial()
	{
		return Zero();
	}

	template <class T>
	typename Module<T>::Zero Module<T>::start()
	{
		return Zero();
	}
	
	template <class T>
	typename Module<T>::Zero Module<T>::run()
	{
		return Zero();
	}

	template <class T>
	typename Module<T>::Zero Module<T>::stop()
	{
		return Zero();
	}
	
	template <class T>
	bool Module<T>::checkStop()
	{
		return m_refs==0;
	}
	
	template <class T>
	int Module<T>::initial(__ModuleContext *context)
	{
		T * self=static_cast<T * >(context);
		return self->initial();
	}

	template <class T>
	int Module<T>::start(__ModuleContext *context)
	{
		T * self=static_cast<T * >(context);
		if(!self->checkStart())
		{
			return -1;
		}
		int code=self->start();
		if(code==0)
		{
			self->m_started=true;
		}
		return code;
	}
	
	template <class T>
	int Module<T>::run(__ModuleContext *context)
	{
		T * self=static_cast<T * >(context);
		return self->run();
	}
	
	template <class T>
	int Module<T>::stop(__ModuleContext *context)
	{
		T * self=static_cast<T * >(context);
		if(self->isStarted())
		{
			if(!self->checkStop())
			{
				return -1;
			}
			return self->stop();
		}
		return 0;
	}
	
	template <class T> 
	void Module<T>::destroy()
	{
		if(m_instance)
		{
			delete m_instance;
			m_instance = 0;
		}		
	}
	inline const char* __ModuleContext::getModuleName() const
	{
		return m_moduleName;
	}

	inline bool __ModuleContext::isStarted() const
	{
		return m_started;
	}

	inline void __ModuleContext::addRef()
	{
		++m_refs;
	}

	inline void __ModuleContext::releaseRef()
	{
		--m_refs;
	}
}

//注册模块
/*
	对于其它代码用不到的封闭模块，应使用应宏注册，否则就会被Release版优化掉
*/
#define MODULE_IMPL(T)	\
	class REG_##T	\
	{	\
	public:	\
		REG_##T()	\
		{	\
			Zoic::getZoicModulesVector().push_back(T::getInstance()); \
			Zoic::getZoicModulesVector().clear(); \
		}	\
	} _reg_##T;

#endif
#endif
