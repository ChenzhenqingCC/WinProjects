/*! @file
	@ingroup utils
	@brief 定义类信息存储模板
*/

#ifndef __ZOIC_CLASSSTORAGE_H
#define __ZOIC_CLASSSTORAGE_H

#include <typeinfo>
#include <list>
#include <algorithm>
#include "NoCopy.h"

namespace Zoic
{
	/*! @brief 类信息存储模板
		@ingroup utils
		@param INFO 存储的数据结构


		为不同的类类型信息，存储一部分特定结构
	*/
	template <class INFO>
	class  ClassStorage:
		public NoCopy
	{
	public:
		//!根据类型信息获得特定类信息
    	INFO & getClassInfo(
			const std::type_info &info	//!< 类型信息
			);
    private:
		//!信息项
		struct InfoItem
		{
			const std::type_info *	typeInfo;	//!< 类型信息指针
			INFO					classInfo;	//!< 存储的类信息
		};

		//!查找类型信息函数类
		class FindItemFunc
    	{
    	public:
			//!构造函数
    		FindItemFunc(
				const std::type_info &info	//!< 要查找的类型信息
				);

			/*! @brief 查找匹配操作
				@retval true 如果item.typeInfo 是要查找的类型信息
				@retval fasle 如果item.typeInfo 不是要查找的类型信息
			*/
    		bool operator()(const InfoItem &item);
    	private:
    		const std::type_info *info;		//!< 要查找的类型信息
    	};
		std::list<InfoItem>	m_infoList;		//!< 类信息列表
    };
    
    template <class INFO>
	INFO & ClassStorage<INFO>::getClassInfo(const std::type_info &info)
    {
    	FindItemFunc f(info);   	
    	typename std::list<InfoItem>::iterator it=std::find_if(m_infoList.begin(),m_infoList.end(),f);
    	if(it!=m_infoList.end())
    	{
    		return it->classInfo;
    	}
    	else
    	{
    		InfoItem item;
    		item.typeInfo=&info;
    		m_infoList.push_back(item);
    		return m_infoList.back().classInfo;
    	}
    }

    template <class INFO>
    ClassStorage<INFO>::FindItemFunc::FindItemFunc(const std::type_info &info)
    {
    	this->info=&info;
    }
    
    template <class INFO>
    bool ClassStorage<INFO>::FindItemFunc::operator()(const InfoItem &item)
    {
    	return (*item.typeInfo==*info)?true:false;
    }
}

#endif
