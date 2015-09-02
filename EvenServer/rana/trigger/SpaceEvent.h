#ifndef __TRIGGER_SPACE_EVENT_H_
#define __TRIGGER_SPACE_EVENT_H_

#include <vector>
#include <string>
#include "TriggerHeader.h"
#include "SortLable.h"
#include "Message.h"

namespace Trigger
{
	class TriggerSpace;
	class SpaceAction;
	//SpaceEvent�ڽ��д���ƥ��ʱ����˳�����һ�¼���
	//1.EventCategoryIDһ��ƥ��;2.SortLable����ƥ��(�������);3.PreValidate����У��
	//����ȫ�����ͨ��֮����󶨵Ķ����Żᱻ����
	//��ƥ���Message������Խ��Ĳ��豻������,Ч��Խ�õ�����֤

	class SpaceEvent
	{
		friend class TriggerSpace;
		friend class SpaceEventMap;
	public:
		typedef void* EventContext;
		EventContext const& getCustomContext() const;
		SpaceAction* getBindedAction() const;
		// Ԥ����÷���
		EventCategoryID getType() const;
		virtual const SortLable* getSortLable() const;
		virtual bool doValidate(Message *msg) const;
	protected:
		SpaceEvent(EventCategoryID eventType);
		virtual ~SpaceEvent();
		//�������
		TriggerSpace* getPossessingSpace();
		void setCustomContext(EventContext const context);
	private:
		EventContext m_context;
		TriggerSpace* m_pSelfSpace;
		SpaceAction* m_bindedAction;
		const EventCategoryID m_eventType;
	};

	inline SpaceEvent::EventContext const& SpaceEvent::getCustomContext() const
	{
		return m_context;
	}

	inline EventCategoryID SpaceEvent::getType() const
	{
		return m_eventType;
	}

	inline SpaceAction* SpaceEvent::getBindedAction() const
	{
		return m_bindedAction;
	}

	inline bool SpaceEvent::doValidate(Message *msg) const
	{
		return true;
	}

	inline const SortLable* SpaceEvent::getSortLable() const
	{
		return NULL;
	}

	inline TriggerSpace* SpaceEvent::getPossessingSpace()
	{
		return m_pSelfSpace;
	}
}

#endif //__TRIGGER_SPACE_EVENT_H_
