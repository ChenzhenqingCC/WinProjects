/*! @file
	@ingroup buildin
	@brief �����ź���
*/

#ifndef __ZOIC_SIGNAL_H
#define __ZOIC_SIGNAL_H
#ifndef ZOIC_NO_SERVICE

#include "Module.h"


namespace Zoic
{
	/*! @brief �ź���
		@ingroup buildin
	*/
	class Signal:
		public Module<Signal>
	{
		friend class Module<Signal>;
	private :
		Signal();

		//!�������̣�����Ƿ����ź��ж�
		int run();

		//!�źŴ������
		static void sighandler(int sig);

		int	m_sig;	//!< ��һ���жϵ��ź�
	};
}
#endif
#endif
