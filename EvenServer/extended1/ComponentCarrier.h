#ifndef __COMPONENTCARRIER_H
#define __COMPONENTCARRIER_H

typedef unsigned char ImplementCategory;

template<class CONTEXT>
class BaseComponentImplement
{
};

//���(Component)ʵ���Ļ���ģ����,�����������ͨ����ģ������Ϊ���࣬������CONTEXT��ComponentCarrierģ������ֻ࣬�и�CONTEXT�����ܹ����ش����
//��������ʱ��Ҫ����INDEX��ֵ(����С��CONTEXT��COMPONENT_COUNT��������ͬһ��CONTEXT�µĲ�ͬģ������INDEX���벻ͬ)
//�ڴ�ģ������Чǰ��Ҫ���ô����Լ���CONTEXT�����ĵ�regist�����Լ�����ע�᣻��Ҫ�ر��Ѿ�ע��õĴ�ģ��ʱ�ٴ���regist������NULL����ע�ἴ��
template<
	class CONTEXT,	//���ڴ����Լ����������࣬��Ȼ�Ǽ̳�ComponentCarrier����
	class CT		//�����ʵ����ص��Լ�����������
>
class ComponentImplement
	:public BaseComponentImplement<CONTEXT>
{
public:
	static const ImplementCategory INDEX;
};

//���ڴ���ComponentImplement����������Ļ���ģ���࣬����������������ֵ�͵�COMPONENT_COUNT����
//�Լ������ͨ��regist���Լ�����ע���Կ��Ź��ܣ��ظ���ͬ����ģ��������Լ�����ע�����Ǻ��߸���ǰ��(ǰ��ɥʧ����)
//���ݱ�ע����������Լ���ӵ�еĹ��ܵĶ��٣�����ڱ�ͬһ��ϵͳ�����Ĳ�ͬ�����¾������޺��ʵ�����Ե��ý��д���
template<
	class CONTEXT	//�������������ط�Χ�Ͷ��⹦�ܵ��Լ�����������
>
class ComponentCarrier
{
public:
	ComponentCarrier();
	~ComponentCarrier();
	template<class CT> CT * getComponent() const;
protected:
	template<class CT> void regist(CT * data);
protected:
	BaseComponentImplement<CONTEXT>* getComponent(ImplementCategory index) const;
private:
	BaseComponentImplement<CONTEXT>**const m_components;
};

template<class CONTEXT>
inline ComponentCarrier<CONTEXT>::ComponentCarrier()
	:m_components(new BaseComponentImplement<CONTEXT>*[CONTEXT::COMPONENT_COUNT])
{
	memset(m_components,0,sizeof( ComponentCarrier<CONTEXT>* )*CONTEXT::COMPONENT_COUNT);
}

template<class CONTEXT>
inline ComponentCarrier<CONTEXT>::~ComponentCarrier()
{
	delete [] m_components;
}

template<class CONTEXT>
inline BaseComponentImplement<CONTEXT>* ComponentCarrier<CONTEXT>::getComponent(ImplementCategory index) const
{
	return m_components[index];
}

template<class CONTEXT>
template<class CT>
inline CT * ComponentCarrier<CONTEXT>::getComponent() const
{
	return static_cast<CT*>( getComponent( ComponentImplement<CONTEXT,CT>::INDEX ) );
}

template<class CONTEXT>
template<class CT>
inline void ComponentCarrier<CONTEXT>::regist(CT* data)
{
	m_components[ComponentImplement<CONTEXT,CT>::INDEX] = data;
}

#endif
