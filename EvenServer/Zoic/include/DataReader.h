
/*! @file
	@brief 定义数据读取模板
*/
#ifndef __DATAREADER_H
#define __DATAREADER_H

#include <vector>
#include <set>
#include <utility>
#include <Zoic/include/NoCopy.h>
#include <Zoic/include/Message.h>
#include <Zoic/include/Log.h>
#include <Zoic/include/OutOfBoundException.h>
#include "DataImage.h"

#ifdef _MSC_VER
#include <memory>
#else
#include <tr1/memory>
#endif

/*! @brief 数据读取特性模板
	@param ITEM 元素类型
*/
template < class ITEM > struct DataReaderTraits
{
	typedef DWORD ID;	//!< 元素ID类型,默认为DWORD
};

/*! @brief 数据读取模板
	@param ITEM 元素类型
*/
template < class ITEM > class DataReader : public Zoic::NoCopy
{
public:	
	class CmpID;
	typedef std::tr1::shared_ptr<const ITEM> ITEM_PTR;	
	typedef std::set<ITEM_PTR, CmpID> SET;

	typedef std::tr1::shared_ptr<ITEM> NONCONST_ITEM_PTR;
	typedef std::set<NONCONST_ITEM_PTR, CmpID> NONCONST_SET;

	class CmpID
	{
	public:
		const bool operator()(const ITEM_PTR& a, const ITEM_PTR& b) const
		{
			return a->id < b->id;
		}
	};
	class NoDelete
	{
	public:
		void operator()(ITEM* ptr)
		{
		}
	};

	~DataReader();

	//!获得数据
	const SET& getData() const; 

	//!根据ID获得条目
	ITEM_PTR getItem(
		const typename DataReaderTraits<ITEM>::ID &id,	//!< 条目ID
		bool useCache = true //!< 使用cache
		) const;

protected:
	//!获得数据
	const NONCONST_SET& getDataForModify() const; 

	//!构造函数
	DataReader(
		const char *file	//!< 保存数据的文件名
		);
	DataReader();	

	/*! @brief 通过条目基类扩展数据
		
		主要用在派生类的构造函数中
	*/
	template < class BASE_ITEM>
	bool extend(
		const char *file	//< 保存数据的文件名
		);
	SET	m_data;		//!< 条目数据
	bool		m_loaded;
	bool load(const char *file);

	void clearData();	

	// 08/14/2009-13:04 by YYBear [BEGIN]
	// 最近一次cache
	mutable bool m_cached; //!< 是否有cache
	mutable typename DataReaderTraits<ITEM>::ID m_cachedID; //!< 已经cache的ID
	mutable ITEM_PTR m_cachedItem; //!< 已经cache的ITEM
	// 08/14/2009-13:04 by YYBear [END] 	

	template < class BASE_ITEM>
	class BaseReader
		:public DataReader<BASE_ITEM>
	{
	public:
		inline bool loadFile(const char* file){ return load(file); };
	};

	template < class BASE_ITEM>
	bool extend(
		const char *file,	//< 保存数据的文件名
		DataReaderTraits<BASE_ITEM> dummy
		);
};

template < class ITEM > DataReader<ITEM>::DataReader()
	:m_loaded(false), m_cached(false)
{
}

template < class ITEM > DataReader<ITEM>::DataReader(const char *file)
{
	if(!load(file))
	{
		loginfo_f("Error:Can't load file %s",file);
		exit(0);
	}		
}

template < class ITEM > DataReader<ITEM>::~DataReader()
{	
	clearData();
}

template < class ITEM > const typename DataReader<ITEM>::SET& DataReader<ITEM>::getData() const
{
	return m_data;
}

template < class ITEM > const typename DataReader<ITEM>::NONCONST_SET& DataReader<ITEM>::getDataForModify() const
{
	return reinterpret_cast<const NONCONST_SET&>(getData());
}

template < class ITEM > typename DataReader<ITEM>::ITEM_PTR DataReader<ITEM>::getItem(const typename DataReaderTraits<ITEM>::ID &id, bool useCache/* = true*/) const
{
	// 08/14/2009-13:09 by YYBear [BEGIN]
	// 使用cache
	if (m_cached && m_cachedItem && useCache)
	{
		if (!(id < m_cachedID) && !(m_cachedID < id))
		{
			return m_cachedItem;
		}
	}
	// 08/14/2009-13:09 by YYBear [END] 
	
	CMP_PTR(ITEM,pSearch)
	pSearch->id=id;
	ITEM_PTR pItem(pSearch, NoDelete());

	typename SET::const_iterator it=m_data.find(pItem);
	if(it!=m_data.end())
	{
		// 08/14/2009-13:11 by YYBear [BEGIN]
		// 保存最近一次cache
		m_cached = true;
		m_cachedID = id;
		m_cachedItem = *it;
		// 08/14/2009-13:11 by YYBear [END] 

		return *it;
	}
	else
	{
		return DataReader<ITEM>::ITEM_PTR();
	}
}

template < class ITEM > bool DataReader<ITEM>::load(const char *file)
{
	FILE *fp=fopen(file,"rb");
	if(!fp)
	{
		logerror_f("DataReader Load\"%s\" Failed:can't open file",file);
		return false;
	}
	char pData[NETWORK_BUF_SIZE-NETWORK_MSG_HEADER];
	DWORD maxCount = 0;
	DWORD loadedCount = 0;
	DWORD dataCount = 0;
	DWORD dataNum;
	dataNum = static_cast<DWORD>( fread(&maxCount,1,sizeof(DWORD),fp) );
	if(dataNum)
	{
		DWORD nextDataSize = sizeof(pData);
		int remainDataSize = 0;
		Zoic::Message msg;
		msg.ID(0);
		while( dataNum=static_cast<DWORD>( fread(pData,1,nextDataSize,fp) ) )
		{
			msg.writeData(pData,dataNum);

			Zoic::MessageState state;
			msg.saveState(state);			
			ITEM* pRawItem = new ITEM;
			try
			{
				msg>>*pRawItem;
				++loadedCount;
			}catch(Zoic::OutOfBoundException *e)
			{
				msg.loadState(state);
				delete pRawItem;
				delete e;
				remainDataSize = msg.getRemainingRData();
				nextDataSize = sizeof(pData) - remainDataSize;
				continue;
			}
			++dataCount;
			ITEM_PTR pItem(pRawItem);
			if( m_data.find(pItem) == m_data.end() )
			{
				m_data.insert(pItem);
			}
			remainDataSize = msg.getRemainingRData();
			nextDataSize = sizeof(pData) - remainDataSize;
			if(remainDataSize)
			{
				msg.readData(pData,remainDataSize);
				msg.ID(0).writeData(pData,remainDataSize);
			}
			else
			{
				msg.ID(0);
			}
		}
		fclose(fp);
		while( remainDataSize )
		{
			ITEM* pRawItem = new ITEM;
			try
			{
				msg>>*pRawItem;
				++loadedCount;
			}catch(Zoic::OutOfBoundException *e)
			{
				logerror_f("DataReader Load\"%s\" Failed(%d/%d):%s",file,loadedCount,maxCount,e->what());
				delete pRawItem;
				delete e;
				return false;
			}
			++dataCount;
			ITEM_PTR pItem(pRawItem);
			if( m_data.find(pItem) == m_data.end() )
			{
				m_data.insert(pItem);
			}		
			remainDataSize = msg.getRemainingRData();
			if(remainDataSize)
			{
				msg.readData(pData,remainDataSize);
				msg.ID(0).writeData(pData,remainDataSize);
			}
			else
			{
				msg.ID(0);
			}
		}
		if(maxCount != loadedCount)
		{
			logerror_f("DataReader Load\"%s\" Failed(%d/%d):unexpected file end",file,loadedCount,maxCount);
			return false;
		}
	}
	m_loaded = true;
	m_cached = false; // 08/14/2009-13:13 by YYBear 	
	return true;
}

template < class ITEM >
template <class BASE_ITEM >
bool DataReader<ITEM>::extend(const char *file)
{
	BaseReader<BASE_ITEM> reader;
	if( !reader.loadFile(file) )
	{
		return false;
	}
	const typename DataReader<BASE_ITEM>::SET & data = reader.getData();
	for(typename DataReader<BASE_ITEM>::SET::const_iterator it = data.begin();it!=data.end();++ it)
	{
		try
		{
			ITEM_PTR pItemImpl(new ITEM(**it));
			m_data.insert(pItemImpl);
		}
		catch(std::exception * e)
		{
			delete e;
		}
	}	
	return true;
}

template < class ITEM >
template <class BASE_ITEM >
bool DataReader<ITEM>::extend(const char *file, DataReaderTraits<BASE_ITEM> dummy)
{
	return extend<BASE_ITEM>(file);
}

template < class ITEM > void DataReader<ITEM>::clearData()
{
	m_data.clear();
	m_loaded = false;
	m_cached = false;
}

#endif
