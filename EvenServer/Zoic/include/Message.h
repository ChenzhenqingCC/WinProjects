/*! @file
	@ingroup network
	@brief 定义网络消息类
*/

#ifndef __ZOIC_MESSAGE_H
#define __ZOIC_MESSAGE_H

#include "Types.h"
#include <string>
#include <vector>
#include "NoCopy.h"

namespace Zoic
{
	class Message;

	/*! @brief 网络消息状态类
		@ingroup network


		用于保存网络消息类的状态
	*/
	class MessageState:
		public NoCopy
	{//消息状态
		friend class Message;
	private:
		BYTE * m_pRead;		//!< 读取位置
		BYTE * m_pWrite;	//!< 写入位置
		Message * m_object;	//!< 网络消息对象指针
	};

	/*! @brief 网络消息类
		@ingroup network
	*/
	class Message
	{
	public:
		/*! @brief !伪消息类
			
			用于将一段内存强转为Message对象
		*/
		struct Fake
		{
			WORD *	pSize;				//!< 指向长度指针
			WORD *	pID;				//!< 指向消息ID指针
			BYTE *	pRead;				//!< 读取位置
			BYTE *	pWrite;				//!< 写入位置
			BYTE *	buffer[1];			//!< 数据缓存
		};

		Message();
		Message(const Message &rhd);

		//!清空数据
		void clear();
		/*! @brief 设置消息ID
			@return 自身的引用
		*/
		Message & ID(
			WORD wID	//!< 消息ID
			);

		/*! @brief 获得剩余数据大小
		

			在取出数据时使用
		*/
		int getRemainingRData();

		//!获得ID
		WORD ID() const;

		//!获得消息里逻缉数据长度
		WORD getSize() const;

		//!获得消息逻缉数据指针
		const BYTE * getData() const;
		
		//!保存状态
		void	saveState(
			MessageState &state	//!< 消息状态
			);
		//!载入状态
		void	loadState(
			const MessageState &state	//!< 消息状态
			);

		/*! @brief 读字符串
			@exception Zoic::OutOfBoundException *　如果找不到长度小于size的字符串
		*/
		void	readString(
			char *buffer,	//!< 字符串缓存指针
			int size		//!< 缓存大小
			);

		/*! @brief 读一定长度的数据
			@exception Zoic::OutOfBoundException *　长度不足
		*/
		void	readData(
			void * pData,	//!< 数据缓存指针
			int n			//!< 读取的数据长度
			);

		/*! @brief 跳过一定长度的数据
			@exception Zoic::OutOfBoundException *　长度不足
		*/
		void skipData(
			int n			//!< 跳过的长度
			);

		/*! @brief 写入一定长度数据
			@exception Zoic::OutOfBoundException *　写入数据超过缓存长度
		*/
		void	writeData(
			const void * pData,
			int n );

		//!获得写入位置
		void * getWritePosition() const;

		//!获得读取位置
		void * getReadPosition() const;

		Message& operator = (const Message& o);
		
		//@{
		/*! @brief 输出操作,将数据写入消息中
			@exception Zoic::OutOfBoundException *　写入数据超过缓存长度
			@return 对自身的引用
			@param val 输出参数			
		*/
		Message & operator << (const char & val);
		Message & operator << (const unsigned char & val);
		Message & operator << (const short & val);
		Message & operator << (const unsigned short & val);
		Message & operator << (const int & val);
		Message & operator << (const unsigned int & val);
		Message & operator << (const long & val);
		Message & operator << (const unsigned long & val);
		Message & operator << (const long long & val);
		Message & operator << (const unsigned long long & val);
		Message & operator << (const float & val);
		Message & operator << (const double & val);
		Message & operator << (const PCSTR & val);
		Message & operator << (const std::string & val);
		Message & operator << (bool & val);
		//@}
		
		//@{
		/*! @brief 输入操作,从消息中读取数据
			@exception Zoic::OutOfBoundException *　读取数据超过缓存长度
			@return 对自身的引用
			@param val 输入参数			
		*/
		Message & operator >> (char & val);
		Message & operator >> (unsigned char & val);
		Message & operator >> (short & val);
		Message & operator >> (unsigned short & val);
		Message & operator >> (int & val);
		Message & operator >> (unsigned int & val);
		Message & operator >> (long & val);
		Message & operator >> (unsigned long & val);
		Message & operator >> (long long & val);
		Message & operator >> (unsigned long long & val);
		Message & operator >> (float & val);
		Message & operator >> (double & val);
		Message & operator >> (std::string &val);
		Message & operator >> (PCSTR & val);
		Message & operator >> (bool & val);
		//@}
	private:
		WORD * m_pSize;					//!< 指向长度指针
		WORD * m_pID;					//!< 指向消息ID指针
		BYTE * m_pRead;					//!< 读取位置
		BYTE * m_pWrite;				//!< 写入位置
		BYTE m_pBuf[NETWORK_BUF_SIZE];	//!< 数据缓存
	};
	
	inline void Message::clear()
	{
		m_pSize = (WORD *)&m_pBuf[0];
		m_pID	= (WORD *)&m_pBuf[2];
		m_pRead = m_pWrite = &m_pBuf[NETWORK_MSG_HEADER];
	}
	
	inline Message & Message::ID (WORD wID)
	{
		* m_pID		= wID;
		* m_pSize	= NETWORK_MSG_HEADER;                  
		m_pRead		= m_pWrite = &m_pBuf[NETWORK_MSG_HEADER];
		return *this;
	}
	
	inline int Message::getRemainingRData()
	{
		if ( m_pRead < m_pBuf + getSize() )
		{
			return static_cast<int>( m_pBuf + getSize() - m_pRead );
		}
		return 0;
	}
	
	inline WORD Message::ID() const
	{
		return *m_pID;
	}
	
	inline WORD Message::getSize() const
	{
		return *m_pSize;
	}

	inline const BYTE * Message::getData() const
	{
		return m_pBuf;
	}
	
	inline void Message::saveState(MessageState &state)
	{
		state.m_object=this;
		state.m_pRead=m_pRead;
		state.m_pWrite=m_pWrite;
	}

	inline void Message::loadState(const MessageState &state)
	{
		if(this==state.m_object)
		{
			m_pWrite=state.m_pWrite;
			m_pRead=state.m_pRead;
		}
	}

	inline void * Message::getWritePosition() const
	{
		return m_pWrite;
	}

	inline void * Message::getReadPosition() const
	{
		return m_pRead;
	}

	inline Message& Message::operator = (const Message& o)
	{
		clear();
		memmove(m_pBuf,o.getData(),o.getSize());
		return *this;
	}

	inline Message & Message::operator << (bool & val)
	{
		int data = val ? 1 : 0;
		writeData(&data,sizeof(data));
		return *this;
	}

	inline Message & Message::operator << (const char & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}
	
	inline Message & Message::operator << (const unsigned char & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}
	
	inline Message & Message::operator << (const short & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}
	
	inline Message & Message::operator << (const unsigned short & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}
	
	inline Message & Message::operator << (const int & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}
	
	inline Message & Message::operator << (const unsigned int & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}
	
	inline Message & Message::operator << (const long & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}
	
	inline Message & Message::operator << (const unsigned long & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}

	inline Message & Message::operator << (const long long & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}
	
	inline Message & Message::operator << (const unsigned long long & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}

	inline Message & Message::operator << (const float & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}
	
	inline Message & Message::operator << (const double & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}
		
	inline Message & Message::operator << (const PCSTR & val)
	{
		writeData(val, static_cast<int>(strlen(val) + 1));
		return * this;
	}
	
	inline Message & Message::operator << (const std::string & val)
	{
		writeData(val.c_str(), static_cast<int>(val.length() + 1));
		return * this;
	};

	inline Message & Message::operator >> (bool & val)
	{
		int data = 0;
		readData(&data,sizeof(data));
		val = data == 1;
		return *this;
	}
		
	inline Message & Message::operator >> (char & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}
	
	inline Message & Message::operator >> (unsigned char & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}
	
	inline Message & Message::operator >> (short & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}
	
	inline Message & Message::operator >> (unsigned short & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}
	
	inline Message & Message::operator >> (int & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}
	
	inline Message & Message::operator >> (unsigned int & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}
	
	inline Message & Message::operator >> (long & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}
	
	inline Message & Message::operator >> (unsigned long & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline Message & Message::operator >> (long long & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}
	
	inline Message & Message::operator >> (unsigned long long & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline Message & Message::operator >> (float & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}
	
	inline Message & Message::operator >> (double & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}
}

//在最后包含
#include "MessageImpl.h"//此文件和本文件等价
#endif
