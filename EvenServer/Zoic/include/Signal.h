/*! @file
	@ingroup buildin
	@brief 定义信号类
*/

#ifndef __ZOIC_SIGNAL_H
#define __ZOIC_SIGNAL_H
#ifndef ZOIC_NO_SERVICE

#include "Module.h"


namespace Zoic
{
	/*! @brief 信号类
		@ingroup buildin
	*/
	class Signal:
		public Module<Signal>
	{
		friend class Module<Signal>;
	private :
		Signal();

		//!运行例程，检测是否有信号中断
		int run();

		//!信号处理过程
		static void sighandler(int sig);

		int	m_sig;	//!< 上一次中断的信号
	};
}
#endif
#endif
