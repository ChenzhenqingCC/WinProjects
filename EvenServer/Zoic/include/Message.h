/*! @file
	@ingroup network
	@brief ����������Ϣ��
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

	/*! @brief ������Ϣ״̬��
		@ingroup network


		���ڱ���������Ϣ���״̬
	*/
	class MessageState:
		public NoCopy
	{//��Ϣ״̬
		friend class Message;
	private:
		BYTE * m_pRead;		//!< ��ȡλ��
		BYTE * m_pWrite;	//!< д��λ��
		Message * m_object;	//!< ������Ϣ����ָ��
	};

	/*! @brief ������Ϣ��
		@ingroup network
	*/
	class Message
	{
	public:
		/*! @brief !α��Ϣ��
			
			���ڽ�һ���ڴ�ǿתΪMessage����
		*/
		struct Fake
		{
			WORD *	pSize;				//!< ָ�򳤶�ָ��
			WORD *	pID;				//!< ָ����ϢIDָ��
			BYTE *	pRead;				//!< ��ȡλ��
			BYTE *	pWrite;				//!< д��λ��
			BYTE *	buffer[1];			//!< ���ݻ���
		};

		Message();
		Message(const Message &rhd);

		//!�������
		void clear();
		/*! @brief ������ϢID
			@return ���������
		*/
		Message & ID(
			WORD wID	//!< ��ϢID
			);

		/*! @brief ���ʣ�����ݴ�С
		

			��ȡ������ʱʹ��
		*/
		int getRemainingRData();

		//!���ID
		WORD ID() const;

		//!�����Ϣ���߼����ݳ���
		WORD getSize() const;

		//!�����Ϣ�߼�����ָ��
		const BYTE * getData() const;
		
		//!����״̬
		void	saveState(
			MessageState &state	//!< ��Ϣ״̬
			);
		//!����״̬
		void	loadState(
			const MessageState &state	//!< ��Ϣ״̬
			);

		/*! @brief ���ַ���
			@exception Zoic::OutOfBoundException *������Ҳ�������С��size���ַ���
		*/
		void	readString(
			char *buffer,	//!< �ַ�������ָ��
			int size		//!< �����С
			);

		/*! @brief ��һ�����ȵ�����
			@exception Zoic::OutOfBoundException *�����Ȳ���
		*/
		void	readData(
			void * pData,	//!< ���ݻ���ָ��
			int n			//!< ��ȡ�����ݳ���
			);

		/*! @brief ����һ�����ȵ�����
			@exception Zoic::OutOfBoundException *�����Ȳ���
		*/
		void skipData(
			int n			//!< �����ĳ���
			);

		/*! @brief д��һ����������
			@exception Zoic::OutOfBoundException *��д�����ݳ������泤��
		*/
		void	writeData(
			const void * pData,
			int n );

		//!���д��λ��
		void * getWritePosition() const;

		//!��ö�ȡλ��
		void * getReadPosition() const;

		Message& operator = (const Message& o);
		
		//@{
		/*! @brief �������,������д����Ϣ��
			@exception Zoic::OutOfBoundException *��д�����ݳ������泤��
			@return �����������
			@param val �������			
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
		/*! @brief �������,����Ϣ�ж�ȡ����
			@exception Zoic::OutOfBoundException *����ȡ���ݳ������泤��
			@return �����������
			@param val �������			
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
		WORD * m_pSize;					//!< ָ�򳤶�ָ��
		WORD * m_pID;					//!< ָ����ϢIDָ��
		BYTE * m_pRead;					//!< ��ȡλ��
		BYTE * m_pWrite;				//!< д��λ��
		BYTE m_pBuf[NETWORK_BUF_SIZE];	//!< ���ݻ���
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

//��������
#include "MessageImpl.h"//���ļ��ͱ��ļ��ȼ�
#endif
