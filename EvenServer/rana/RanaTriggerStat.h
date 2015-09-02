#ifndef __TRIGGERSTATMANAGER_H__
#define __TRIGGERSTATMANAGER_H__

#include <queue>

class RanaTriggerStat
{
public:
	RanaTriggerStat();
	~RanaTriggerStat();

	void countM();				//发生了消息，无论是否通过预检(preValidate函数)均对M_Count计数
	void countMec();			//发生了消息，并且通过了预检然后进行了上下文判定但没有通过，对MEC_Count计数
	void countMeca();			//发生了消息，通过上下文判定后运行了Action函数，对MECA_Count计数
	unsigned long getMCount();			//得到M_Count计数
	unsigned long getMecCount();			//得到MEC_Count计数
	unsigned long getMecaCount();			//得到MECA_Count计数
	bool ConditionStart();					//开始对Condition计时，应在执行Condition前调用
	bool ActionStart();						//开始对Action计时，应在执行Condition前调用
	bool ConditionOver();					//结束对Condition计时，应在执行Condition完成时调用，所花费的时间可通过getLastConditionTime()得到
	bool ActionOver();						//结束对Action计时，应在执行Action完成时调用，所花费的时间可通过getLastActionTime()得到
	unsigned long getLastConditionTime();	//得到上一次Condition花费的时间
	unsigned long getLastActionTime();		//得到上一次Action花费的时间
private:
	unsigned long m_M_Count;//Message Count
	unsigned long m_MEC_Count;//Message-Event-Context Count
	unsigned long m_MECA_Count;//Message-Event-Context-Action Count
	unsigned long m_LastConditionTime;
	unsigned long m_LastActionTime;
	//std::queue<unsigned long> m_ConditionTime;
	//std::queue<unsigned long> m_ActionTime;

	unsigned long m_LastConditionStartTime;
	unsigned long m_LastActionStartTime;
	bool m_ConditionStarting;
	bool m_ActionStarting;
	void clearEventCount();		//清空触发统计次数
};

inline void RanaTriggerStat::countM()
{
	m_M_Count++;
}

inline void RanaTriggerStat::countMec()
{
	m_MEC_Count++;
}

inline void RanaTriggerStat::countMeca()
{
	m_MECA_Count++;
}

inline unsigned long RanaTriggerStat::getMCount()
{
	return m_M_Count;
}

inline unsigned long RanaTriggerStat::getMecCount()
{
	return m_MEC_Count;
}

inline unsigned long RanaTriggerStat::getMecaCount()
{
	return m_MECA_Count;
}

inline unsigned long RanaTriggerStat::getLastConditionTime()
{
	return m_LastConditionTime;
}

inline unsigned long RanaTriggerStat::getLastActionTime()
{
	return m_LastActionTime;
}

#endif

