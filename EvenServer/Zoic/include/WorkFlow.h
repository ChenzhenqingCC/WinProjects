/*! @file
	@ingroup buildin
	@brief ��������
*/

#ifndef __ZOIC_WORKFLOW_H
#define __ZOIC_WORKFLOW_H

#include "Types.h"
#include "Module.h"
#include <map>

namespace Zoic
{
	class WorkUnit
	{
	public:
		virtual void action() = 0;
		virtual void release(){delete this;};
	protected:
		virtual ~WorkUnit(){};
	};
	/*! @brief ������
		@ingroup buildin
	*/
	class WorkFlow
		:public NoCopy
	{
	public:
		WorkFlow();
		void delay(WorkUnit* unit,DWORD delayMs);
		void insert(WorkUnit* unit,TIME_MSEL workTime);
		void clear();
		int routine();
	private:
		TIME_MSEL m_nextActionTime;
		std::multimap<TIME_MSEL,WorkUnit*> m_workFlows;
	};
}
#endif
