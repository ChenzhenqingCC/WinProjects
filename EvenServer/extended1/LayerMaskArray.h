#ifndef __LAYERMASKARRAY_H
#define __LAYERMASKARRAY_H

#include <vector>
#include "MaskArray.h"
#include "BatchAction.h"
#include "NumericExtend.h"

//多层动态数组(时间换空间方案)
//以下标的形式访问每个元素,但是开销预计为普通数组下标访问的4*(LAYER+1)倍
//每层子Layer下标域没用被用到的部分分配极少的内存，比完全分配内存的数组方式大大节省内存
//适用于解决以下问题:1.减少低有效值率的广范围下标数组的内存开销以及提高其遍历所有有效下标的效率;2.提高连续区间的数值键字典的访问速度

#pragma pack(1)

template<class T,unsigned char LAYER = 0,typename MASK_TYPE = unsigned short>
class LayerMaskArray;

template<class T,typename MASK_TYPE>
class LayerMaskArray<T,0,MASK_TYPE>
	: private MaskArray<T,MASK_TYPE>
{
public:
	typedef MaskArray<T,MASK_TYPE> UNIT_ARRAY;
	typedef T UNIT_TYPE;
	static const size_t SIZE_LIMIT = MaskArray<T,MASK_TYPE>::MASK_INDEX_BOUNT;
	typedef MaskBoundChecker<(unsigned int)SIZE_LIMIT> BOUND;

	~LayerMaskArray();
	T* operator[](unsigned int index) const;
	UNIT_ARRAY& getUnitArray();
	void batchActionInRange(unsigned int beginIndex,unsigned int endIndex,BatchAction<T*>& batchAction) const;
	bool addData(unsigned int index,T* data);
	bool removeData(unsigned int index);
};

template<class T,unsigned char LAYER,typename MASK_TYPE>
class LayerMaskArray
	: private MaskArray< LayerMaskArray<T,LAYER-1,MASK_TYPE>,MASK_TYPE>
{
public:
	typedef MaskArray< LayerMaskArray<T,LAYER-1,MASK_TYPE>,MASK_TYPE> UNIT_ARRAY;
	typedef LayerMaskArray<T,LAYER-1,MASK_TYPE> UNIT_TYPE;
	static const size_t SIZE_LIMIT = UNIT_TYPE::SIZE_LIMIT * MaskArray< UNIT_TYPE,MASK_TYPE>::MASK_INDEX_BOUNT;
	typedef MaskBoundChecker<(unsigned int)SIZE_LIMIT> BOUND;

	~LayerMaskArray();
	T* operator[](unsigned int index) const;
	UNIT_ARRAY& getUnitArray();
	void batchActionInRange(unsigned int beginIndex,unsigned int endIndex,BatchAction<T*>& batchAction) const;
	bool addData(unsigned int index,T* data);
	bool removeData(unsigned int index);
};


#pragma pack()

template<class T,typename MASK_TYPE>
inline LayerMaskArray<T,0,MASK_TYPE>::~LayerMaskArray()
{
}

template<class T,unsigned char LAYER,typename MASK_TYPE>
inline LayerMaskArray<T,LAYER,MASK_TYPE>::~LayerMaskArray()
{
	typename UNIT_ARRAY::DataField dataField = getUnitArray().getDataField();
	for(MaskArrayIndex i=0;i<dataField.count; ++i )
	{
		delete dataField.pBuf[i];
	}
}

template<class T,typename MASK_TYPE>
inline T* LayerMaskArray<T,0,MASK_TYPE>::operator [](unsigned int index) const
{
	return UNIT_ARRAY::getData(index);
}

template<class T,unsigned char LAYER,typename MASK_TYPE>
inline T* LayerMaskArray<T,LAYER,MASK_TYPE>::operator [](unsigned int index) const
{
	const unsigned int unit_index = index >> UNIT_TYPE::BOUND::VALID_BIT_COUNT;
	const unsigned int next_layer_index = index & UNIT_TYPE::BOUND::FULL_MASK_VALUE;
	UNIT_TYPE* pUnitItem = UNIT_ARRAY::getData(unit_index);
	if(!pUnitItem)
	{
		return NULL;
	}
	return (*pUnitItem)[next_layer_index];
}

template<class T,typename MASK_TYPE>
inline typename LayerMaskArray<T,0,MASK_TYPE>::UNIT_ARRAY& LayerMaskArray<T,0,MASK_TYPE>::getUnitArray()
{
	return *this;
}

template<class T,unsigned char LAYER,typename MASK_TYPE>
inline typename LayerMaskArray<T,LAYER,MASK_TYPE>::UNIT_ARRAY& LayerMaskArray<T,LAYER,MASK_TYPE>::getUnitArray()
{
	return *this;
}

template<class T,typename MASK_TYPE>
void LayerMaskArray<T,0,MASK_TYPE>::batchActionInRange(unsigned int beginIndex,unsigned int endIndex,BatchAction<T*>& batchAction) const
{
	typename UNIT_ARRAY::DataField dataField = UNIT_ARRAY::getDataInRange( beginIndex, endIndex );
	for( MaskArrayIndex i = 0; i<dataField.count; ++i )
	{
		T* pItem = dataField.pBuf[i];
		batchAction.doAction(pItem);
	}
}

template<class T,unsigned char LAYER,typename MASK_TYPE>
void LayerMaskArray<T,LAYER,MASK_TYPE>::batchActionInRange(unsigned int beginIndex,unsigned int endIndex,BatchAction<T*>& batchAction) const
{
	if(beginIndex >= endIndex)
	{
		return;
	}
	const unsigned int begin_unit_index = beginIndex >> UNIT_TYPE::BOUND::VALID_BIT_COUNT;
	const unsigned int next_layer_begin_index = beginIndex & UNIT_TYPE::BOUND::FULL_MASK_VALUE;
	const unsigned int end_unit_index = endIndex >> UNIT_TYPE::BOUND::VALID_BIT_COUNT;
	const unsigned int next_layer_end_index = endIndex & UNIT_TYPE::BOUND::FULL_MASK_VALUE;
	unsigned int rangeStartIndex = begin_unit_index;
	if(begin_unit_index == end_unit_index)
	{
		UNIT_TYPE* targetUnit = UNIT_ARRAY::getData(begin_unit_index);
		if(targetUnit)
		{
			targetUnit->batchActionInRange( next_layer_begin_index,next_layer_end_index,batchAction );
		}
		return;
	}
	if( next_layer_begin_index )
	{
		++rangeStartIndex;
		UNIT_TYPE* beginUnit = UNIT_ARRAY::getData(begin_unit_index);
		if(beginUnit)
		{
			beginUnit->batchActionInRange( next_layer_begin_index,UNIT_TYPE::SIZE_LIMIT,batchAction );
		}
	}
	typename UNIT_ARRAY::DataField dataField = UNIT_ARRAY::getDataInRange( rangeStartIndex, end_unit_index );
	for( MaskArrayIndex i = 0; i<dataField.count; ++i )
	{
		dataField.pBuf[i]->batchActionInRange( 0,UNIT_TYPE::SIZE_LIMIT,batchAction );
	}
	if( next_layer_end_index )
	{
		UNIT_TYPE* endUnit = UNIT_ARRAY::getData(end_unit_index);
		if(endUnit)
		{
			endUnit->batchActionInRange( 0,next_layer_end_index,batchAction );
		}
	}
}

template<class T,typename MASK_TYPE>
inline bool LayerMaskArray<T,0,MASK_TYPE>::addData(unsigned int index,T* data)
{
	if( UNIT_ARRAY::haveData(index) )
	{
		return false;
	}
	return UNIT_ARRAY::setData(index,data);
}

template<class T,unsigned char LAYER,typename MASK_TYPE>
bool LayerMaskArray<T,LAYER,MASK_TYPE>::addData(unsigned int index,T* data)
{
	if(index >= SIZE_LIMIT)
	{
		return false;
	}
	const unsigned int unit_index = index >> UNIT_TYPE::BOUND::VALID_BIT_COUNT;
	const unsigned int next_layer_index = index & UNIT_TYPE::BOUND::FULL_MASK_VALUE;
	UNIT_TYPE* pUnitItem = UNIT_ARRAY::getData(unit_index);
	if(!pUnitItem)
	{
		pUnitItem = new UNIT_TYPE;
		if( ! UNIT_ARRAY::setData(unit_index,pUnitItem) )
		{
			delete pUnitItem;
			return false;
		}
	}
	return pUnitItem->addData(next_layer_index,data);
}

template<class T,typename MASK_TYPE>
inline bool LayerMaskArray<T,0,MASK_TYPE>::removeData(unsigned int index)
{
	return UNIT_ARRAY::eraseData(index);
}

template<class T,unsigned char LAYER,typename MASK_TYPE>
bool LayerMaskArray<T,LAYER,MASK_TYPE>::removeData(unsigned int index)
{
	const unsigned int unit_index = index >> UNIT_TYPE::BOUND::VALID_BIT_COUNT;
	const unsigned int next_layer_index = index & UNIT_TYPE::BOUND::FULL_MASK_VALUE;
	UNIT_TYPE* pUnitItem = UNIT_ARRAY::getData(unit_index);
	if(!pUnitItem)
	{
		return false;
	}
	bool result = pUnitItem->removeData(next_layer_index);
	if( !pUnitItem->getUnitArray().getMask() )
	{
		if( UNIT_ARRAY::eraseData(unit_index) )
		{
			delete pUnitItem;
		}
	}
	return result;
}

#endif
