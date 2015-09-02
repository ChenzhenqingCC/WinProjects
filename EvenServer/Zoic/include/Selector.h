/*! @file
	@ingroup network
	@brief ����ѡ������
*/

#ifndef __ZOIC_SELECTOR_H
#define __ZOIC_SELECTOR_H

#include <set>
#include "Types.h"
#include "Module.h"
#include <time.h>
#include "FdSet.h"
#ifdef HAVE_MINGW32
#include <winsock2.h>
#ifdef _MSC_VER
#pragma comment (lib, "winmm.lib")
#endif
#endif

namespace Zoic
{
	class Selectable;

	/*! @brief ѡ������
		@ingroup buildin
	*/
	class Selector
#ifndef ZOIC_NO_SERVICE
		:public Module<Selector>
#else
		:public NoCopy
#endif
	{
#ifndef ZOIC_NO_SERVICE
		friend class Module<Selector>;
#endif
	public :
		//! �����׽���
		bool insert(
			Selectable * socket		//!< �׽���
			);

		//! ɾ���׽���
		bool remove(
			Selectable * socket		//!< �׽���
			);

		//! ��������
		~Selector();

#ifndef ZOIC_NO_SERVICE
	private:
#else
	public:
#endif
		//! ���캯��
		Selector();

		//!�������̣�����ʱ��
		int run();
private:
		class CmpSocket
		{
		public:
			bool operator()(Selectable *a ,Selectable * b) const;
		};
		typedef std::set<Selectable *,CmpSocket> SOCKETS;


		/*! @brief���������Ŀ�ѡ�����

			���÷�Ӧȷ��������Чֵ,�ڸú����ڼٶ����ǻ��ҵ���ض���
		*/
		Selectable * getSelectable(
			SOCKET socket
			);

		SOCKETS			m_sockets;			//!< �׽��ּ���
		FdSet			m_baseFdSet;		//!< ����������
		FdSet			m_recvFdSet;		//!< ��������������
		FdSet			m_sendFdSet;		//!< ��������������
		Selectable **	m_socketArray;		//!< �׽������飬���׽���Ϊ�±�
		unsigned int	m_socketArraySize;	//!< �׽��������С		
		SOCKET *		m_activeSockets;	//!< ��׽�������
		unsigned int	m_activeSocketsSize;//!< ��׽��������С
	};

#ifndef ZOIC_NO_SERVICE
	#define g_Selector Zoic::Selector::getInstance()
#else
	extern Selector theSelector;
	#define g_Selector (&Zoic::theSelector)
#endif

	inline Selectable * Selector::getSelectable(SOCKET socket)
	{
		return m_socketArray[socket];
	}

}
#endif
