/*! @file
	@ingroup service
	@brief ����ģ��ģ��
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
	/*! @brief ģ��ģ��
		@ingroup service
		@param T ����


		ģ����һ�����Ƶ����Ķ��������������Լ���start,run,stop������<br>
		��start��stopʱ�����趨��ͬģ��֮����Ⱥ�˳��
	*/

	class __ModuleContext
	{
		friend class Service;
		template <class T> friend class Module;
	public:
		//!���ģ������
		const char* getModuleName() const;
		//!�ж�ģ���Ƿ�ɹ����й�start������start˳��
		bool isStarted() const;
		//!���ӱ�ģ������ü���,����stop˳��
		void addRef();
		//!�ͷű�ģ������ü���,����stop˳��
		void releaseRef();
	protected:
		__ModuleContext():m_moduleName(NULL),m_started(false),m_refs(0){};
		virtual ~__ModuleContext(){};
	private:
		const char *	m_moduleName;	//!< ģ����
		bool			m_started;		//!< �Ƿ�׼����
		unsigned int	m_refs;			//!< ���ü���
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
		//!���ģ��ָ��
		static T* getInstance(void);


		/*! @brief ģ�������Բ�飬����start˳��


			��鱾ģ����������ģ���Ƿ�ɹ�����start<br>
			�ú������Ա������� bool checkStart() �������ǡ����������Զ���������һ��checkStart��
		*/
		True checkStart();

	protected:
		Module();
		virtual ~Module() = 0;

		/*! @brief ģ���ʼ��
			��ʼ���͹��캯���Ĳ�ͬ������init��������ʱ����ͨ��g_Service���ͨ��main��������ı�־
			�Ӷ�ͨ�����в���������ģ��ĳ�ʼ������Ϊ

			���캯����������Ƿ���Serviceע��init������<br>
			ֻ�������� int initial() �������Ǹú����󣬱���ľ�̬initial�����Żᱻע�ᵽSerivce�С�
		*/
		Zero initial();
		/*! @brief ģ������



			���캯����������Ƿ���Serviceע��start������<br>
			ֻ�������� int start() �������Ǹú����󣬱���ľ�̬start�����Żᱻע�ᵽSerivce�С�
		*/
		Zero start();

		/*! @brief ģ������


			���캯����������Ƿ���Serviceע��run������<br>
			ֻ�������� int run() �������Ǹú����󣬱���ľ�̬run�����Żᱻע�ᵽSerivce�С�
		*/
		Zero run();
		/*! @brief ģ��ֹͣ


			���캯����������Ƿ���Serviceע��stop������<br>
			ֻ�������� int stop() �������Ǹú����󣬱���ľ�̬stop�����Żᱻע�ᵽSerivce�С�
		*/
		Zero stop();
	private:
		static T *		m_instance;		//!< ģ�����ָ��

		//!����ܷ����
		bool checkStop();
		
		//!ģ���ʼ��
		static int initial(
			__ModuleContext *context	//! ģ�����ָ��
			);

		//!ģ������
		static int start(
			__ModuleContext *context	//! ģ�����ָ��
			);

		//!ģ������
		static int run(
			__ModuleContext *context	//! ģ�����ָ��
			);

		//!ģ��ֹͣ
		static int stop(
			__ModuleContext *context	//! ģ�����ָ��
			);

		//!ɾ��ģ��
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
		//�Ƚ���T��û���Զ���init����
		FUNC_MODULE_INITIAL fnInitial;
		if(sizeof(static_cast<T *>(0)->initial())==sizeof(int) || sizeof(static_cast<T *>(0)->initial())==sizeof(bool))
		{
			fnInitial=initial;
		}
		else
		{			
			fnInitial=0;
		}
		//�Ƚ���T��û���Զ���start����
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
		
		//�Ƚ���T��û���Զ���run����
		FUNC_MODULE_START fnRun;
		if(sizeof(static_cast<T *>(0)->run())==sizeof(int))
		{
			fnRun=run;
		}
		else
		{
			fnRun=0;
		}
		
		//��������stop����,�Լ���˳�ʱ��������
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

//ע��ģ��
/*
	�������������ò����ķ��ģ�飬Ӧʹ��Ӧ��ע�ᣬ����ͻᱻRelease���Ż���
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
