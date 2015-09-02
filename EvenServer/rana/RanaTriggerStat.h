#ifndef __TRIGGERSTATMANAGER_H__
#define __TRIGGERSTATMANAGER_H__

#include <queue>

class RanaTriggerStat
{
public:
	RanaTriggerStat();
	~RanaTriggerStat();

	void countM();				//��������Ϣ�������Ƿ�ͨ��Ԥ��(preValidate����)����M_Count����
	void countMec();			//��������Ϣ������ͨ����Ԥ��Ȼ��������������ж���û��ͨ������MEC_Count����
	void countMeca();			//��������Ϣ��ͨ���������ж���������Action��������MECA_Count����
	unsigned long getMCount();			//�õ�M_Count����
	unsigned long getMecCount();			//�õ�MEC_Count����
	unsigned long getMecaCount();			//�õ�MECA_Count����
	bool ConditionStart();					//��ʼ��Condition��ʱ��Ӧ��ִ��Conditionǰ����
	bool ActionStart();						//��ʼ��Action��ʱ��Ӧ��ִ��Conditionǰ����
	bool ConditionOver();					//������Condition��ʱ��Ӧ��ִ��Condition���ʱ���ã������ѵ�ʱ���ͨ��getLastConditionTime()�õ�
	bool ActionOver();						//������Action��ʱ��Ӧ��ִ��Action���ʱ���ã������ѵ�ʱ���ͨ��getLastActionTime()�õ�
	unsigned long getLastConditionTime();	//�õ���һ��Condition���ѵ�ʱ��
	unsigned long getLastActionTime();		//�õ���һ��Action���ѵ�ʱ��
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
	void clearEventCount();		//��մ���ͳ�ƴ���
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

