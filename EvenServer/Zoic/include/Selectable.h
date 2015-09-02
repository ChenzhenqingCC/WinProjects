/*! @file
	@ingroup network
	@brief 定义选择接口
*/

#ifndef __ZOIC_SELECTABLE_H
#define __ZOIC_SELECTABLE_H

#include "Types.h"

namespace Zoic
{
	class Selector;
	/*! @brief 选择接口

		仅用于TCP协议,该类不管理相关联socket的打开与关闭操作。
	*/
	class Selectable
	{
		friend class Selector;
	public:
		//! 获得套接字
		SOCKET getSocket() const;
	protected:
		/*! @brief 构造函数

			在构造函数中加入到选择列表中
		*/
		Selectable();

		//! 析构函数
		~Selectable();

		//! 加入选择器
		bool joinSelector(
			SOCKET socket		//!< 套接字
			);

		//! 退出选择器
		bool quitSelector();

		//! 设置可以接收数据标志
		void setCanRecvData(
			bool can			//!< 是否可以接收
			);

		//! 是否可以接收数据
		bool canRecvData() const;

		//! 设置可以发送数据标志
		void setCanSendData(
			bool can			//!< 是否可以发送
			);

		//! 是否可以发送数据
		bool canSendData() const;

	private:
		SOCKET	m_socket;		//!< 套按字
		bool	m_canRecvData;	//!< 可以接收数据
		bool	m_canSendData;	//!< 可以发送数据
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
