#ifndef __MASKDATACARRIER_H
#define __MASKDATACARRIER_H

#include "MaskCounter.h"
#include <string.h>

typedef unsigned char MaskImplementCategory;

template<
	class CONTEXT,	//具体决定自身搭载范围和额外功能的自己的子类类型
	typename MASK_TYPE
>
class MaskDataCarrier;

template<class CONTEXT,typename MASK_TYPE = unsigned char>
class BaseMaskDataImplement
{
protected:
	virtual void addRef( CONTEXT& carrier );
	virtual void releaseRef( CONTEXT& carrier );
protected:
	virtual ~BaseMaskDataImplement();
};

//组件(Data)实例的基类模版类,所有组件必须通过此模版类作为基类，其载体CONTEXT是DataCarrier模版的子类，只有该CONTEXT类型能够搭载此组件
//构造该组件时需要定义INDEX的值(必须小于CONTEXT的COMPONENT_COUNT变量，且同一个CONTEXT下的不同模块的组件INDEX必须不同)
//在此模块在生效前需要调用搭载自己的CONTEXT上下文的regist函数自己进行注册；需要关闭已经注册好的此模块时再次用regist函数将NULL代替注册即可
template<
	class CONTEXT,	//用于搭载自己的上下文类，必然是继承DataCarrier的类
	class DT,		//与具体实现相关的自己的子类类型
	MaskImplementCategory IDX,
	typename MASK_TYPE = unsigned char
>
class MaskDataImplement
	:public BaseMaskDataImplement<CONTEXT,MASK_TYPE>
{
public:
	typedef MaskDataCarrier<CONTEXT,MASK_TYPE> CARRIER;
	static const MaskImplementCategory INDEX = IDX;
	static const MASK_TYPE MASK_BIT= 1<<INDEX;
	static const MASK_TYPE MASK_LOWER= MASK_BIT - 1;
};

//用于搭载DataImplement组件的上下文基类模版类，他的子类必须存在数值型的COMPONENT_COUNT变量
//自己的组件通过regist向自己进行注册以开放功能，重复的同类型模块组件对自己进行注册总是后者覆盖前者(前者丧失功能)
//根据被注册组件决定自己所拥有的功能的多少，便可在被同一个系统环境的不同调用下决定有无合适的组件对调用进行处理
#pragma pack(1)

template<
	class CONTEXT,	//具体决定自身搭载范围和额外功能的自己的子类类型
	typename MASK_TYPE = unsigned char
>
class MaskDataCarrier
{
public:
	typedef BaseMaskDataImplement<CONTEXT,MASK_TYPE> ImplementType;
	MaskDataCarrier();
	~MaskDataCarrier();
	static const MaskImplementCategory MASK_INDEX_BOUNT = sizeof(MASK_TYPE) * BYTE_BIT_COUNT;
	MASK_TYPE getDataMask() const;
	template<class DT> bool haveData() const;
	template<class DT> DT * getData() const;
	template<class DT> bool insertData(DT * data);
	template<class DT> bool removeData();
protected:
	void forceClear();
private:
	class ImplementAdapter : public ImplementType
	{
		friend class MaskDataCarrier;
	private:
		static ImplementAdapter& adapt(ImplementType& impl);
	};
	ImplementType** m_data;
	MASK_TYPE	m_mask;
};

#pragma pack()

template<class CONTEXT,typename MASK_TYPE>
inline void BaseMaskDataImplement<CONTEXT,MASK_TYPE>::addRef( CONTEXT& carrier )
{
}

template<class CONTEXT,typename MASK_TYPE>
inline void BaseMaskDataImplement<CONTEXT,MASK_TYPE>::releaseRef( CONTEXT& carrier )
{
	delete this;
}

template<class CONTEXT,typename MASK_TYPE>
inline BaseMaskDataImplement<CONTEXT,MASK_TYPE>::~BaseMaskDataImplement()
{
}

template<class CONTEXT,typename MASK_TYPE>
inline typename MaskDataCarrier<CONTEXT,MASK_TYPE>::ImplementAdapter& MaskDataCarrier<CONTEXT,MASK_TYPE>::ImplementAdapter::adapt(typename MaskDataCarrier<CONTEXT,MASK_TYPE>::ImplementType& impl)
{
	return static_cast<ImplementAdapter&>(impl);
}

template<class CONTEXT,typename MASK_TYPE>
inline MaskDataCarrier<CONTEXT,MASK_TYPE>::MaskDataCarrier()
	:m_data(0)
	,m_mask(0)
{
}

template<class CONTEXT,typename MASK_TYPE>
inline MaskDataCarrier<CONTEXT,MASK_TYPE>::~MaskDataCarrier()
{
	forceClear();
	ASSERT(!m_data);
}

template<class CONTEXT,typename MASK_TYPE>
inline void MaskDataCarrier<CONTEXT,MASK_TYPE>::forceClear()
{
	MASK_TYPE const oldMask = m_mask;
	ImplementType** const oldData = m_data;
	m_mask = 0;
	m_data = NULL;
	if(!oldData)
	{
		return;
	}
	MaskImplementCategory dataIndex = 0;
	for(MaskImplementCategory i = 0;i<MASK_INDEX_BOUNT ; ++i)
	{
		if( oldMask & (0x01<<i) )
		{
			ImplementAdapter& adapter = ImplementAdapter::adapt(*oldData[dataIndex]);
			adapter.releaseRef( *static_cast<CONTEXT*>(this) );
			++dataIndex;
		}
	}
	delete [] oldData;
}

template<class CONTEXT,typename MASK_TYPE>
inline MASK_TYPE MaskDataCarrier<CONTEXT,MASK_TYPE>::getDataMask() const
{
	return m_mask;
}

template<class CONTEXT,typename MASK_TYPE>
template<class DT>
inline bool MaskDataCarrier<CONTEXT,MASK_TYPE>::haveData() const
{
	return ( m_mask & DT::MASK_BIT ) ? true : false;
}

template<class CONTEXT,typename MASK_TYPE>
template<class DT>
inline DT * MaskDataCarrier<CONTEXT,MASK_TYPE>::getData() const
{
	if( !( m_mask & DT::MASK_BIT ) )
	{
		return NULL;
	}
	return static_cast<DT *>( m_data[ MaskCounter::getInstance()->getBitCount<MASK_TYPE>( m_mask & DT::MASK_LOWER ) ] );
}

template<class CONTEXT,typename MASK_TYPE>
template<class DT>
inline bool MaskDataCarrier<CONTEXT,MASK_TYPE>::insertData(DT* data)
{
	if( m_mask & DT::MASK_BIT )
	{
		return false;
	}	
	const unsigned char frontCount = MaskCounter::getInstance()->getBitCount<MASK_TYPE>( m_mask & DT::MASK_LOWER );
	const unsigned char backCount = MaskCounter::getInstance()->getBitCount<MASK_TYPE>( m_mask ) - frontCount;
	ImplementAdapter& adapter = ImplementAdapter::adapt(*data);
	BaseMaskDataImplement<CONTEXT,MASK_TYPE> **	tempData = new BaseMaskDataImplement<CONTEXT,MASK_TYPE>*[frontCount+backCount+1];
	if(frontCount)
	{
		memcpy(tempData,m_data,frontCount*sizeof(BaseMaskDataImplement<CONTEXT,MASK_TYPE>*) );
	}
	if(backCount)
	{
		memcpy(&tempData[frontCount+1],&m_data[frontCount],backCount*sizeof(BaseMaskDataImplement<CONTEXT,MASK_TYPE>*) );
	}
	tempData[frontCount] = data;
	if(m_data)
	{
		delete [] m_data;
	}
	m_data = tempData;
	m_mask |= DT::MASK_BIT;
	adapter.addRef( *static_cast<CONTEXT*>(this) );
	return true;
}

template<class CONTEXT,typename MASK_TYPE>
template<class DT>
inline bool MaskDataCarrier<CONTEXT,MASK_TYPE>::removeData()
{
	if( !( m_mask & DT::MASK_BIT ) )
	{
		return false;
	}
	const unsigned char frontCount = MaskCounter::getInstance()->getBitCount<MASK_TYPE>( m_mask & DT::MASK_LOWER );
	const unsigned char backCount = MaskCounter::getInstance()->getBitCount<MASK_TYPE>( m_mask ) - frontCount - 1;
	ImplementAdapter& adapter = ImplementAdapter::adapt(*m_data[frontCount]);
	BaseMaskDataImplement<CONTEXT,MASK_TYPE> **	tempData = NULL;
	if( frontCount || backCount)
	{
		tempData = new BaseMaskDataImplement<CONTEXT,MASK_TYPE>*[frontCount+backCount];
	}
	if(frontCount)
	{
		memcpy(tempData,m_data,frontCount*sizeof(BaseMaskDataImplement<CONTEXT,MASK_TYPE>*) );
	}
	if(backCount)
	{
		memcpy(&tempData[frontCount],&m_data[frontCount+1],backCount*sizeof(BaseMaskDataImplement<CONTEXT,MASK_TYPE>*));
	}
	delete [] m_data;
	m_data = tempData;
	m_mask &= ~(DT::MASK_BIT);
	adapter.releaseRef( *static_cast<CONTEXT*>(this) );
	return true;
}

#endif
