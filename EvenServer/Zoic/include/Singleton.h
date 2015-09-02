/*! @file
	@ingroup utils
	@brief ���嵥��ģ��
*/

#ifndef __ZOIC_SINGLETON_H__
#define __ZOIC_SINGLETON_H__
#include <vector>
#include <new>
#include "config_flat.h"
#include "NoCopy.h"
namespace Zoic
{
	/*! @brief ����ģ��
		@ingroup utils
		@param T ����
	*/
	template <class T>
	class  Singleton:
		public NoCopy
	{
		friend class Rleaser;
	public:
		//!���ʵ��
		static T* getInstance(void);
	protected:
		Singleton();
	private:
		//! ɾ��ʵ��
		static void destroy();

		static T* m_instance;	//!< ����ָ��
	};

	template <class T> T* Singleton<T>::m_instance = 0;
	
	class SingletonRleaser
	{
		template <class T> friend class Singleton;
	public:
		~SingletonRleaser();
	private:
		typedef void (*DESTROY_FUNC)();
		std::vector<DESTROY_FUNC> m_funcs;
	};
	
	inline SingletonRleaser::~SingletonRleaser()
	{
		for(std::vector<DESTROY_FUNC>::const_iterator it=m_funcs.begin();it!=m_funcs.end();++it)
		{
			(**it)();
		}
		m_funcs.clear();
	}
	
	inline SingletonRleaser & getSingletonRleaser()
	{
		static SingletonRleaser releaser;
		return releaser;
	}
	
	template <class T> 
	T* Singleton<T>::getInstance(void)
	{
		if(m_instance)
		{
			return m_instance;
		}
		else
		{
			m_instance = reinterpret_cast<T*>( malloc( sizeof(T) ) );
			m_instance = new T;
			getSingletonRleaser().m_funcs.push_back(&destroy);
			return m_instance;
		}
	}
	
	template <class T> 
	Singleton<T>::Singleton()
	{
	}
	
	template <class T> 
	void Singleton<T>::destroy()
	{
		if(m_instance)
		{
			m_instance->~T();
			free( m_instance );
			m_instance = 0;
		}
	}
}

#endif
