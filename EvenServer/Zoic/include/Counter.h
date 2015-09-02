/*! @file
	@ingroup utils
	@brief 定义计数器模板
*/

#ifndef __ZOIC_COUNTER_H
#define __ZOIC_COUNTER_H
#include <map>

namespace Zoic
{
	/*!
		@brief 计数器模板
		@ingroup utils
		@param KEY 键值类型
		@param CMP 比较类
		@param CNT 计数类型

		根据Key值实现计数,计数类型默认为 unsigned int ,
		* const_iterator 返回 std::pair.
		其中 first 为 KEY 类型
		second 为 计数值
	*/
	template <class KEY,class CMP=std::less<KEY>,class CNT=unsigned int>
	class Counter
	{
	public:
		//!迭代器
		typedef typename std::map<KEY,CNT,CMP>::iterator iterator;

		//!只读迭代器
		typedef typename std::map<KEY,CNT,CMP>::const_iterator const_iterator;
		
		//!增加统计数据
		void addUp(
			const KEY & key,	//!< 键值
			const CNT & count=1	//!< 计数
			);

		//@{
		//!获得迭代器开始位置
		const_iterator begin() const;
		iterator begin();
		//@{

		//@{
		//!获得迭代器结束位置
		const_iterator end() const;
		iterator end();
		//@}

		//@{
		/*! @brief 查找数据
			@param key 键值
		*/
		const_iterator find(const KEY & key) const;
		iterator find(const KEY & key);
		//@}
		
		//!根据迭代器删除计数项
		void erase(
			iterator &it	//!< 迭代器
			);

		//!清除数据
		void clear();
	private:
		typedef std::map<KEY,CNT,CMP> MAP;	//!< 计数map类型定义
		MAP		m_data;						//!< 计数数据
	};

	template <class KEY,class CMP,class CNT>
	void Counter<KEY,CMP,CNT>::addUp(const KEY & key,const CNT & count)
	{
		std::pair<typename MAP::iterator,bool> pr = m_data.insert(std::make_pair(key,count));
		if(!pr.second)
		{
			pr.first->second += count;
		}
	}

	template <class KEY,class CMP,class CNT>
	void Counter<KEY,CMP,CNT>::clear()
	{
		m_data.clear();
	}

	template <class KEY,class CMP,class CNT>
	typename Counter<KEY,CMP,CNT>::const_iterator Counter<KEY,CMP,CNT>::begin() const
	{
		return m_data.begin();
	}

	template <class KEY,class CMP,class CNT>
	typename Counter<KEY,CMP,CNT>::iterator Counter<KEY,CMP,CNT>::begin()
	{
		return m_data.begin();
	}

	template <class KEY,class CMP,class CNT>
	typename Counter<KEY,CMP,CNT>::const_iterator Counter<KEY,CMP,CNT>::end() const
	{
		return m_data.end();
	}

	template <class KEY,class CMP,class CNT>
	typename Counter<KEY,CMP,CNT>::iterator Counter<KEY,CMP,CNT>::end()
	{
		return m_data.end();
	}

	template <class KEY,class CMP,class CNT>
	typename Counter<KEY,CMP,CNT>::const_iterator Counter<KEY,CMP,CNT>::find(const KEY & key) const
	{
		return m_data.find(key);
	}

	template <class KEY,class CMP,class CNT>
	typename Counter<KEY,CMP,CNT>::iterator Counter<KEY,CMP,CNT>::find(const KEY & key)
	{
		return m_data.find(key);
	}

	template <class KEY,class CMP,class CNT>
	void Counter<KEY,CMP,CNT>::erase(iterator &it)
	{
		m_data.erase(it);
	}
}

#endif
