/*! @file
	@ingroup utils
	@brief ���������ģ��
*/

#ifndef __ZOIC_COUNTER_H
#define __ZOIC_COUNTER_H
#include <map>

namespace Zoic
{
	/*!
		@brief ������ģ��
		@ingroup utils
		@param KEY ��ֵ����
		@param CMP �Ƚ���
		@param CNT ��������

		����Keyֵʵ�ּ���,��������Ĭ��Ϊ unsigned int ,
		* const_iterator ���� std::pair.
		���� first Ϊ KEY ����
		second Ϊ ����ֵ
	*/
	template <class KEY,class CMP=std::less<KEY>,class CNT=unsigned int>
	class Counter
	{
	public:
		//!������
		typedef typename std::map<KEY,CNT,CMP>::iterator iterator;

		//!ֻ��������
		typedef typename std::map<KEY,CNT,CMP>::const_iterator const_iterator;
		
		//!����ͳ������
		void addUp(
			const KEY & key,	//!< ��ֵ
			const CNT & count=1	//!< ����
			);

		//@{
		//!��õ�������ʼλ��
		const_iterator begin() const;
		iterator begin();
		//@{

		//@{
		//!��õ���������λ��
		const_iterator end() const;
		iterator end();
		//@}

		//@{
		/*! @brief ��������
			@param key ��ֵ
		*/
		const_iterator find(const KEY & key) const;
		iterator find(const KEY & key);
		//@}
		
		//!���ݵ�����ɾ��������
		void erase(
			iterator &it	//!< ������
			);

		//!�������
		void clear();
	private:
		typedef std::map<KEY,CNT,CMP> MAP;	//!< ����map���Ͷ���
		MAP		m_data;						//!< ��������
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
