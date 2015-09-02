/*! @file
	@ingroup utils
	@brief ��������Ϣ�洢ģ��
*/

#ifndef __ZOIC_CLASSSTORAGE_H
#define __ZOIC_CLASSSTORAGE_H

#include <typeinfo>
#include <list>
#include <algorithm>
#include "NoCopy.h"

namespace Zoic
{
	/*! @brief ����Ϣ�洢ģ��
		@ingroup utils
		@param INFO �洢�����ݽṹ


		Ϊ��ͬ����������Ϣ���洢һ�����ض��ṹ
	*/
	template <class INFO>
	class  ClassStorage:
		public NoCopy
	{
	public:
		//!����������Ϣ����ض�����Ϣ
    	INFO & getClassInfo(
			const std::type_info &info	//!< ������Ϣ
			);
    private:
		//!��Ϣ��
		struct InfoItem
		{
			const std::type_info *	typeInfo;	//!< ������Ϣָ��
			INFO					classInfo;	//!< �洢������Ϣ
		};

		//!����������Ϣ������
		class FindItemFunc
    	{
    	public:
			//!���캯��
    		FindItemFunc(
				const std::type_info &info	//!< Ҫ���ҵ�������Ϣ
				);

			/*! @brief ����ƥ�����
				@retval true ���item.typeInfo ��Ҫ���ҵ�������Ϣ
				@retval fasle ���item.typeInfo ����Ҫ���ҵ�������Ϣ
			*/
    		bool operator()(const InfoItem &item);
    	private:
    		const std::type_info *info;		//!< Ҫ���ҵ�������Ϣ
    	};
		std::list<InfoItem>	m_infoList;		//!< ����Ϣ�б�
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
