#ifndef __DATALOADER_H
#define __DATALOADER_H
#include <set>

#include <Zoic/include/Recordset.h>
#include <Zoic/include/Query.h>
#include <Zoic/include/Clock.h>

template < class ITEM > struct DataLoaderTraits
{
	typedef DWORD ID;
};

template < class ITEM > class DataLoader
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
	void routine(DWORD tm);
	const VR_SET & getData(DWORD *version);
	const ITEM * getItem(const typename DataLoaderTraits<ITEM>::ID & id);
protected:
	DataLoader(Zoic::Query * & loadproc,DWORD interval = 600 );
	virtual bool fetch(Zoic::RecordsetPtr & record,ITEM & item)=0;
	virtual void onItemUpdated(const ITEM & item)=0;
	virtual ~DataLoader();
	bool load();
	bool init();

	DWORD 			m_updatetime;	//更新数据时刻
	DWORD 			m_version;		//最新的版本
	Zoic::Query * &	m_loadproc;		//载入数据的存储过程
	DWORD			m_interval;		//重新更新数据的时间间隔(秒)
	
	ID_SET 			m_dataID;
	VR_SET			m_dataVR;
};

template < class ITEM > DataLoader<ITEM>::DataLoader(Zoic::Query * & loadproc,DWORD interval):
	m_version(0),
	m_interval(interval),
	m_loadproc(loadproc)
{
}

template < class ITEM > DataLoader<ITEM>::~DataLoader()
{
	for(typename ID_SET::iterator it=m_dataID.begin();it!=m_dataID.end();++it)
	{
		delete *it;
	}
}

template < class ITEM > bool DataLoader<ITEM>::load()
{
	bool success=false;
	Zoic::RecordsetPtr rs=m_loadproc->store();
	if(rs)
	{
		while(rs->fetch())
		{
			ITEM * item=new ITEM();
			if(fetch(rs,*item))
			{
				typename ID_SET::iterator itID = m_dataID.find(item);
				if(itID!=m_dataID.end())
				{
					ITEM * old = *itID;
					m_dataVR.erase(old);
					m_dataID.erase(itID);
					delete old;
				}
				m_dataID.insert(item);
				m_dataVR.insert(item);
				m_version = item->version;
				onItemUpdated(*item);
			}
			else
			{
				delete item;
			}
		}
		success = true;
	}
	return success;
}

template < class ITEM > void DataLoader<ITEM>::routine(DWORD tm)
{
	if(tm > m_updatetime)
	{
		if(load())
		{
			m_updatetime+=m_interval/2+rand()%m_interval;
		}
		else
		{
			m_updatetime+=m_interval/10+1;
		}
	}
}

template < class ITEM > const typename DataLoader<ITEM>::VR_SET & DataLoader<ITEM>::getData(DWORD *version)
{
	*version = m_version;
	return m_dataVR;
}

template < class ITEM > bool DataLoader<ITEM>::init()
{
	if(!load())
	{
		return false;
	}
	m_updatetime=g_Clock->getSeconds()+m_interval/2+rand()%m_interval;
	return true;
}

template < class ITEM > const ITEM * DataLoader<ITEM>::getItem(const typename DataLoaderTraits<ITEM>::ID & id)
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
