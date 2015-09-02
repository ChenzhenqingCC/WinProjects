/*! @file
@ingroup buildin
@brief 定义运行时间监控器类
*/

#ifndef __ZOIC_RUNTIMECONTEXT_H
#define __ZOIC_RUNTIMECONTEXT_H

namespace Zoic
{
	template<class CONTEXT>
	class RunTimeContext
	{
	public:
		virtual void runTimeCheckBegin(CONTEXT * pContext) = 0;
		virtual void runTimeCheckEnd(CONTEXT * pContext) = 0;
	protected:
		RunTimeContext(){};
	};
}

#endif
