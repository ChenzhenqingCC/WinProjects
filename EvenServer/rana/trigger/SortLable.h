#ifndef __TRIGGER_SORT_LABLE_H_
#define __TRIGGER_SORT_LABLE_H_

#include "TriggerHeader.h"

namespace Trigger
{
	class SortLable
	{
	public:
		virtual bool sortLableFront(const SortLable& o) const;
	};

	inline bool SortLable::sortLableFront(const SortLable& o) const
	{
		return false;
	};

	//////////////////////////////Compare Method////////////////////////////
	class CmpSortLable
	{
	public:
		bool operator()(const SortLable* a,const SortLable* b) const;
	};

	inline bool CmpSortLable::operator()(const SortLable* a,const SortLable* b) const
	{
		return a->sortLableFront(*b);
	}
}


#endif //__RANA_EVENT_H_
