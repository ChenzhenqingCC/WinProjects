#ifndef __COMPONENTCARRIER_H
#define __COMPONENTCARRIER_H

typedef unsigned char ImplementCategory;

template<class CONTEXT>
class BaseComponentImplement
{
};

//组件(Component)实例的基类模版类,所有组件必须通过此模版类作为基类，其载体CONTEXT是ComponentCarrier模版的子类，只有该CONTEXT类型能够搭载此组件
//构造该组件时需要定义INDEX的值(必须小于CONTEXT的COMPONENT_COUNT变量，且同一个CONTEXT下的不同模块的组件INDEX必须不同)
//在此模块在生效前需要调用搭载自己的CONTEXT上下文的regist函数自己进行注册；需要关闭已经注册好的此模块时再次用regist函数将NULL代替注册即可
template<
	class CONTEXT,	//用于搭载自己的上下文类，必然是继承ComponentCarrier的类
	class CT		//与具体实现相关的自己的子类类型
>
class ComponentImplement
	:public BaseComponentImplement<CONTEXT>
{
public:
	static const ImplementCategory INDEX;
};

//用于搭载ComponentImplement组件的上下文基类模版类，他的子类必须存在数值型的COMPONENT_COUNT变量
//自己的组件通过regist向自己进行注册以开放功能，重复的同类型模块组件对自己进行注册总是后者覆盖前者(前者丧失功能)
//根据被注册组件决定自己所拥有的功能的多少，便可在被同一个系统环境的不同调用下决定有无合适的组件对调用进行处理
template<
	class CONTEXT	//具体决定自身搭载范围和额外功能的自己的子类类型
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
