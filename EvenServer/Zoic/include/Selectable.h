/*! @file
	@ingroup network
	@brief ����ѡ��ӿ�
*/

#ifndef __ZOIC_SELECTABLE_H
#define __ZOIC_SELECTABLE_H

#include "Types.h"

namespace Zoic
{
	class Selector;
	/*! @brief ѡ��ӿ�

		������TCPЭ��,���಻���������socket�Ĵ���رղ�����
	*/
	class Selectable
	{
		friend class Selector;
	public:
		//! ����׽���
		SOCKET getSocket() const;
	protected:
		/*! @brief ���캯��

			�ڹ��캯���м��뵽ѡ���б���
		*/
		Selectable();

		//! ��������
		~Selectable();

		//! ����ѡ����
		bool joinSelector(
			SOCKET socket		//!< �׽���
			);

		//! �˳�ѡ����
		bool quitSelector();

		//! ���ÿ��Խ������ݱ�־
		void setCanRecvData(
			bool can			//!< �Ƿ���Խ���
			);

		//! �Ƿ���Խ�������
		bool canRecvData() const;

		//! ���ÿ��Է������ݱ�־
		void setCanSendData(
			bool can			//!< �Ƿ���Է���
			);

		//! �Ƿ���Է�������
		bool canSendData() const;

	private:
		SOCKET	m_socket;		//!< �װ���
		bool	m_canRecvData;	//!< ���Խ�������
		bool	m_canSendData;	//!< ���Է�������
	};

	inline SOCKET Selectable::getSocket() const
	{
		return m_socket;
	}

	inline void Selectable::setCanRecvData(bool can)
	{
		m_canRecvData = can;
	}

	inline bool Selectable::canRecvData() const
	{
		return m_canRecvData;
	}

	inline void Selectable::setCanSendData(bool can)
	{
		m_canSendData = can;
	}

	inline bool Selectable::canSendData() const
	{
		return m_canSendData;
	}

}

#endif
