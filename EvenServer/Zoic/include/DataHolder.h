#ifndef __DATAHOLDER_H
#define __DATAHOLDER_H

#include <set>
#include <utility>
#include <Zoic/include/NoCopy.h>

template < class ITEM > struct DataHolderTraits
{
	typedef DWORD ID;
};

template < class ITEM > class DataHolder : public Zoic::NoCopy
{
public:
	class CmpID
	{
	public:
		const bool operator()(const ITEM * a,const ITEM *b) const
		{
			return a->id < b->id;
		}		
	};
	class CmpVR
	{
	public:
		const bool operator()(const ITEM * a,const ITEM *b) const
		{
			return a->version < b->version;
		}		
	};
	typedef std::set<ITEM *,CmpID> ID_SET;
	typedef std::set<ITEM *,CmpVR> VR_SET;
	typedef std::pair<typename VR_SET::const_iterator,typename VR_SET::const_iterator> VR_PAIR;
	
	void setLoadTotal(int total);
	void decLoadTotal();
	void updateItem(const ITEM & item);
	const VR_PAIR & getData(DWORD *version);
	const ITEM * getItem(const typename DataHolderTraits<ITEM>::ID & id);
	virtual void onLoaded(){};
protected:
	virtual void onItemUpdated(const ITEM & item);
	DataHolder( );
	virtual ~DataHolder();

	enum	LOAD_STATUS
	{
		EMPTY,		//空，初始状态
		LOADING,	//正在载入数据
		LOADED		//已载入数据
	};
	LOAD_STATUS	m_loadstatus;	//载入状态
	int			m_loadtotal;	//要载入的数据量
	int			m_loadcount;	//已经载入的数据
	
	DWORD 		m_version;		//最新的版本
	
	ID_SET 		m_dataID;
	VR_SET		m_dataVR;
	VR_PAIR		m_pairVR;
};

template < class ITEM > DataHolder<ITEM>::DataHolder():
	m_loadstatus(EMPTY),
	m_version(0)
{
	m_pairVR.first=m_pairVR.second=m_dataVR.end();
}

template < class ITEM > DataHolder<ITEM>::~DataHolder()
{
	for(typename ID_SET::iterator it=m_dataID.begin();it!=m_dataID.end();++it)
	{
		delete *it;
	}
}

template < class ITEM > void DataHolder<ITEM>::setLoadTotal(int total)
{
	m_loadstatus=LOADING;
	m_loadtotal=total;
	m_loadcount=0;
	if(m_loadcount==m_loadtotal)
	{
		m_loadstatus=LOADED;
		onLoaded();
	}
}

template < class ITEM > void DataHolder<ITEM>::decLoadTotal()
{
	if(m_loadstatus==LOADING)
	{
		m_loadtotal--;
		if(m_loadcount==m_loadtotal)
		{
			m_loadstatus=LOADED;
			onLoaded();
		}
	}
}

template < class ITEM > void DataHolder<ITEM>::updateItem(const ITEM & itm)
{
	if(m_loadstatus==EMPTY)
	{
		return;
	}
	char _cmp_buffer[sizeof(ITEM)];
	ITEM * item = reinterpret_cast<ITEM *>(_cmp_buffer);

	item->id = itm.id;
	typename ID_SET::iterator itID = m_dataID.find(item);

	if(itID!=m_dataID.end())
	{
		item = *itID;
		m_dataVR.erase(item);
		*item = itm;
	}
	else
	{
		item=new ITEM(itm);
		m_dataID.insert(item);
	}
	m_dataVR.insert(item);
	m_pairVR.first=m_pairVR.second=m_dataVR.end();
	if(m_loadstatus==LOADING)
	{
		m_loadcount++;
		if(m_loadcount==m_loadtotal)
		{
			m_version = item->version;
			m_loadstatus=LOADED;
			onLoaded();
		}
	}
	else
	{
		m_version = item->version;
		onItemUpdated(*item);
	}
}

template < class ITEM > void DataHolder<ITEM>::onItemUpdated(const ITEM & item)
{
}

template < class ITEM > const typename DataHolder<ITEM>::VR_PAIR & DataHolder<ITEM>::getData(DWORD *version)
{
	if(m_loadstatus==LOADED)
	{
		ITEM item;
		item.version=*version;
		m_pairVR.first=m_dataVR.upper_bound(&item);
	}
	*version=m_version;
	return m_pairVR;
}

template < class ITEM > const ITEM * DataHolder<ITEM>::getItem(const typename DataHolderTraits<ITEM>::ID & id)
{
	ITEM item;
	item.id=id;
	typename ID_SET::const_iterator it=m_dataID.find(&item);
	if(it!=m_dataID.end())
	{
		return *it;
	}
	else
	{
		return NULL;
	}
}

#endif
