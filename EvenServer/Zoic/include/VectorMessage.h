/*! @file
	@ingroup utils
	@brief 定义发送向量消息模板

	所谓向量消息,是指在网络消息包中包括的数据是一个数组。如下所示：
	<pre>
	//邮件数据列表 Manager->Client
	const WORD MC_MAILBODY_NTF=933;
	struct P_MC_MailBody_Ntf
	{
		std::vector<MailBrief>	infos;	//邮件列表
	};
	</pre>
*/

#ifndef __ZOIC_VECTORMESSAGE_H
#define __ZOIC_VECTORMESSAGE_H
#include <algorithm>
#include "Message.h"
#include "NoCopy.h"
#include "for_each.h"
#include "OutOfBoundException.h"

namespace Zoic
{
	const int DEFAULT_VECTOR_BLOCK_SIZE = 20;	//!< 默认数组块大小

	/*! @brief  消息打包类
		处理在 消息包中有std::vector类型的数据打包问题，与sendVectorMessage函数联合使用。
	*/
	class VectorPacker
	{
	public:
		//! 转换函数类型
		typedef void (*PACK_FUNC)(Zoic::Message &msg,void *packet);

		//!构造函数
		VectorPacker(
			void *packet,			//!< 指向 消息包 地址
			void *vector,			//!< 指向 消息包中的std::vector<T> 地址
			PACK_FUNC func			//!< 转换函数
			);
		void pack(Message &msgSend) const;
		void * getVectorPointer() const;
	private:
		void *	m_packet;	//!< 指向 消息包 地址
		void *	m_vector;	//!< 指向 消息包中的std::vector<T> 地址
		PACK_FUNC m_func;	//!< 转换函数
	};

	/*! @brief  消息打包类
		处理在 消息包中有std::vector类型的数据打包问题，与sendVectorMessage函数联合使用。
	*/
	template
	<
		class PACKET
	>
	class DefaultVectorPacker
	{
	public:
		//!构造函数
		DefaultVectorPacker(
			PACKET & packet,	//!< 指向 消息包 地址
			void * vector,		//!< 指向 消息包中的std::vector<T> 地址
			WORD msgID			//!< 要发送的消息ID
			);
		void pack(Message &msgSend) const;
		void * getVectorPointer() const;
	private:
		PACKET &	m_packet;	//!<消息包
		void *		m_vector;	//!< 指向 消息包中的std::vector<T> 地址
		WORD		m_msgID;	//!< 要发送的消息ID
	};

	/*! @brief 消息发送模板类

		通过应用操作符"()"发送消息。
		@ingroup utils
	
		@param SENDER		发送对象所属类
		@param RESULT		发送函数返回值
		@param ARGUMENT		发送函数额外参数类型
	*/
	template
	<
		class SENDER,
		class RESULT = bool,
		class ARGUMENT = void
	>
	class MessageSender
	{
	private:
		typedef RESULT (SENDER::*SEND_FUNC)(const Zoic::Message &msg,ARGUMENT arg);	//!< 发送函数类型
	public:
		//!构造函数
		MessageSender(
			SENDER * sendObject,		//!< 发送消息对象
			SEND_FUNC sendFunction,		//!< 发送消息函数
			ARGUMENT arg				//!< 发送消息参数
			);
		
		//!执行发送消息操作
		void operator() (
			const Zoic::Message &msg	//!< 要发送的消息
			) const;
	private:
		SENDER * m_sendObject;			//!< 发送消息对象
		SEND_FUNC m_sendFunction;		//!< 发送消息函数
		ARGUMENT m_arg;					//!< 发送消息函数
	};

	/*! @brief 消息发送模板类,无额外参数版

		通过应用操作符"()"发送消息。
		@ingroup utils
	
		@param SENDER		发送对象所属类
		@param RESULT		发送函数返回值
	*/
	template
	<
		class SENDER,
		class RESULT
	>
	class MessageSender<SENDER,RESULT,void>
	{
	private:
		typedef RESULT (SENDER::*SEND_FUNC)(const Zoic::Message &msg);	//!< 发送函数类型
	public:
		//!构造函数
		MessageSender(
			SENDER * sendObject,		//!< 发送消息对象
			SEND_FUNC sendFunction		//!< 发送消息函数
			);
		
		//!执行发送消息操作
		void operator() (
			const Zoic::Message &msg	//!< 要发送的消息
			) const;
	private:
		SENDER * m_sendObject;			//!< 发送消息对象
		SEND_FUNC m_sendFunction;		//!< 发送消息函数
	};

	/*! @brief 向量发送模板类
		@ingroup utils
	
		用在Zoic::for_each算法中
		
		@param ELEMENT		向量中元素类型
		@param SENDER		发送对象所属类
		@param CONTAINER	包含原始数据的转换类
		@param PACKET		发送的数据包所属的结构
	*/
	template
	<
		class ELEMENT,
		class SENDER,
		class CONTAINER,
		class PACKER
	>
	class VectorSender:
		public NoCopy
	{
	public:
		//!构造函数
		VectorSender(
			const SENDER &sender,								//!< 发送消息函数对象
			const PACKER & packer,								//!< 发送消息打包对象
			void (*pusher)(std::vector<ELEMENT> &vec,const typename CONTAINER::value_type & val),	//!< 转换函数
			const int blockSize									//!< 块大小
			);

		/*! @brief 析构函数
		
			析构函数会将最后的不成块数据发送出去
		*/
		~VectorSender();
		
		//! 收集数据,并在达到块大小时发送数据
		void operator() (
			const typename CONTAINER::value_type & elem		//!< 容器中元素类型
			);
	private:
		const SENDER & m_sender;					//!< 发送消息函数对象
		void (*m_pusher)(std::vector<ELEMENT> &vec,const typename CONTAINER::value_type & val);//!< 转换函数
		int m_blockSize;							//!< 块大小
		const PACKER & m_packer;					//!< 发送消息打包对象
		std::vector<ELEMENT>& m_elements;			//!< 将要发送的元素数组

		//!发送元素
		void sendElements();
	};
	
	template <class PACKET>
	DefaultVectorPacker<PACKET>::DefaultVectorPacker(
		PACKET & packet,
		void *vector,
		WORD msgID
		):
		m_packet(packet),
		m_vector(vector),
		m_msgID(msgID)
	{

	}

	template <class PACKET>
	void DefaultVectorPacker<PACKET>::pack(Message &msgSend) const
	{
		//using Proto::operator <<;//需要考虑命名空间的问题，协议命名空间不只Proto一个
		msgSend.ID(m_msgID) << m_packet;
	}

	template <class PACKET>
	void * DefaultVectorPacker<PACKET>::getVectorPointer() const
	{
		return m_vector;
	}

	/*! @brief 默认向量消息转换函数
		@ingroup utils
	*/
	template <class ELEMENT,class VALUE>
	void defaultVectorMessagePusher(
		std::vector<ELEMENT> &vec,	//!< 存放结果数组
		const VALUE & val			//!< 原始类型数据
		)
	{
		vec.push_back(val);
	}
	/*! @brief 默认的指针向量消息转换函数
	@ingroup utils
	*/
	template <class ELEMENT,class VALUE>
	void defaultVectorMessagePusher(
		std::vector<ELEMENT> &vec,	//!< 存放结果数组
		VALUE* const& pVal			//!< 原始类型数据指针
		)
	{
		vec.push_back(*pVal);
	}

	/*! @brief 发送向量消息
		@ingroup utils
	*/
	template <class ELEMENT,class SENDER,class CONTAINER>
	void sendVectorMessage(
		const SENDER & sender,						//!< 发送消息函数对象
		WORD msgID,									//!< 要发送的消息ID
		const CONTAINER & container,				//!< 原始数据容器
		void (*pusher)(std::vector<ELEMENT> &vec,const typename CONTAINER::value_type & val)=defaultVectorMessagePusher,//!< 转换函数
		const int blockSize=DEFAULT_VECTOR_BLOCK_SIZE	//!< 块大小
		)
	{
		std::vector<ELEMENT> elements;
		DefaultVectorPacker<std::vector<ELEMENT> > packer(elements,&elements,msgID);
		VectorSender<ELEMENT,SENDER,CONTAINER,DefaultVectorPacker<std::vector<ELEMENT> >  > vSender(sender,packer,pusher,blockSize);
		Zoic::for_each(container.begin(),container.end(),vSender);
	}

	/*! @brief 发送向量消息,设置发送块大小
		@ingroup utils
	*/
	template <class ELEMENT,class SENDER,class CONTAINER>
	void sendVectorMessage(
		const SENDER & sender,						//!< 发送消息函数对象
		WORD msgID,									//!< 要发送的消息ID
		const CONTAINER & container,				//!< 原始数据容器
		const int blockSize							//!< 块大小
		)
	{
		std::vector<ELEMENT> elements;
		DefaultVectorPacker<std::vector<ELEMENT> > packer(elements,&elements,msgID);
		VectorSender<ELEMENT,SENDER,CONTAINER,DefaultVectorPacker<std::vector<ELEMENT> >  > vSender(sender,packer,defaultVectorMessagePusher,blockSize);
		Zoic::for_each(container.begin(),container.end(),vSender);
	}

	/*! @brief 发送向量消息
		@ingroup utils
	*/
	template <class ELEMENT,class SENDER,class CONTAINER,class PACKET>
	void sendVectorMessage(
		const SENDER & sender,						//!< 发送消息函数对象
		WORD msgID,									//!< 要发送的消息ID
		const PACKET & packet,						//!< 要发送的数据包内容
		const std::vector<ELEMENT> & elements,		//!< 要发送的数据包中的向量所在的变量
		const CONTAINER & container,				//!< 原始数据容器
		void (*pusher)(std::vector<ELEMENT> &vec,const typename CONTAINER::value_type & val)=defaultVectorMessagePusher,//!< 转换函数
		const int blockSize=DEFAULT_VECTOR_BLOCK_SIZE	//!< 块大小
		)
	{
		PACKET pkt(packet);
		DefaultVectorPacker<PACKET> packer(pkt,reinterpret_cast<char *>(&pkt)+(reinterpret_cast<const char *>(&elements)-reinterpret_cast<const char *>(&packet)),msgID);
		VectorSender<ELEMENT,SENDER,CONTAINER,DefaultVectorPacker<PACKET> > vSender(sender,packer,pusher,blockSize);
		Zoic::for_each(container.begin(),container.end(),vSender);
	}

	/*! @brief 发送向量消息,设置发送块大小
		@ingroup utils
	*/
	template <class ELEMENT,class SENDER,class CONTAINER,class PACKET>
	void sendVectorMessage(
		const SENDER & sender,						//!< 发送消息函数对象
		WORD msgID,									//!< 要发送的消息ID
		const PACKET & packet,						//!< 要发送的数据包内容
		const std::vector<ELEMENT> & elements,		//!< 要发送的数据包中的向量所在的变量
		const CONTAINER & container,				//!< 原始数据容器
		const int blockSize							//!< 块大小
		)
	{
		PACKET pkt(packet);
		DefaultVectorPacker<PACKET> packer(pkt,reinterpret_cast<char *>(&pkt)+(reinterpret_cast<const char *>(&elements)-reinterpret_cast<const char *>(&packet)),msgID);
		VectorSender<ELEMENT,SENDER,CONTAINER,DefaultVectorPacker<PACKET> > vSender(sender,packer,defaultVectorMessagePusher,blockSize);
		Zoic::for_each(container.begin(),container.end(),vSender);
	}

	/*! @brief 发送向量消息
		@ingroup utils
	*/
	template <class ELEMENT,class SENDER,class CONTAINER>
	void sendVectorMessage(
		const SENDER & sender,						//!< 消息发送函数对象
		const VectorPacker & packer,				//!< 消息打包函数对象
		const CONTAINER & container,				//!< 原始数据容器
		void (*pusher)(std::vector<ELEMENT> &vec,const typename CONTAINER::value_type & val)=defaultVectorMessagePusher,//!< 转换函数
		const int blockSize=DEFAULT_VECTOR_BLOCK_SIZE	//!< 块大小
		)
	{
		VectorSender<ELEMENT,SENDER,CONTAINER,VectorPacker > vSender(sender,packer,pusher,blockSize);
		Zoic::for_each(container.begin(),container.end(),vSender);
	}

	/*! @brief 发送向量消息,设置发送块大小
		@ingroup utils
	*/
	template <class ELEMENT,class SENDER,class CONTAINER>
	void sendVectorMessage(
		const SENDER & sender,						//!< 消息发送函数对象
		const VectorPacker & packer,				//!< 消息打包函数对象
		const CONTAINER & container,				//!< 原始数据容器
		const int blockSize							//!< 块大小
		)
	{
		VectorSender<ELEMENT,SENDER,CONTAINER,VectorPacker > vSender(sender,packer,defaultVectorMessagePusher,blockSize);
		Zoic::for_each(container.begin(),container.end(),vSender);
	}


	template <class ELEMENT,class SENDER,class CONTAINER,class PACKER>
	void VectorSender<ELEMENT,SENDER,CONTAINER,PACKER>::operator()(const typename CONTAINER::value_type & elem)
	{
		(*m_pusher)(m_elements,elem);
		if(m_elements.size()==m_blockSize)
		{
			sendElements();
		}
	}

	template <class ELEMENT,class SENDER,class CONTAINER,class PACKER>
	VectorSender<ELEMENT,SENDER,CONTAINER,PACKER>::VectorSender(
		const SENDER & sender,
		const PACKER & packer,
		void (*pusher)(std::vector<ELEMENT> &vec,const typename CONTAINER::value_type & val),
		const int blockSize
		):
		m_sender(sender),
		m_packer(packer),
		m_elements( *static_cast<std::vector<ELEMENT> *>(packer.getVectorPointer())),
		m_pusher(pusher),
		m_blockSize(blockSize)
	{
	}

	template <class ELEMENT,class SENDER,class CONTAINER,class PACKER>
	VectorSender<ELEMENT,SENDER,CONTAINER,PACKER>::~VectorSender()
	{
		if(!m_elements.empty())
		{
			sendElements();
		}
	}

	template <class ELEMENT,class SENDER,class CONTAINER,class PACKER>
	void VectorSender<ELEMENT,SENDER,CONTAINER,PACKER>::sendElements()
	{
		Message msgSend;
		try
		{
			m_packer.pack(msgSend);
			m_sender(msgSend);
			m_elements.clear();
		}
		catch(OutOfBoundException * e)
		{
			if(m_elements.size() > 1)
			{//元素数据大多，尝试一个一个的发送
				delete e;
				
				std::vector<ELEMENT> elements;
				m_elements.swap(elements);
				VectorSender<ELEMENT,SENDER,std::vector<ELEMENT>,PACKER > vSender(m_sender,m_packer,defaultVectorMessagePusher,1);
				Zoic::for_each(elements.begin(),elements.end(),vSender);
			}
			else
			{//如果仅发送一个元素还出错,直接抛出异常
				m_elements.clear();
				throw e;
			}			
		}
	}

	template <class SENDER,class RESULT,class ARGUMENT>
	MessageSender<SENDER,RESULT,ARGUMENT>::MessageSender(SENDER * sendObject,SEND_FUNC sendFunction,ARGUMENT arg):
		m_sendObject(sendObject),
		m_sendFunction(sendFunction),
		m_arg(arg)
	{
	}

	template <class SENDER,class RESULT,class ARGUMENT>
	void MessageSender<SENDER,RESULT,ARGUMENT>::operator() (const Zoic::Message &msg) const
	{
		(m_sendObject->*m_sendFunction)(msg,m_arg);
	}

	template <class SENDER,class RESULT>
	MessageSender<SENDER,RESULT,void>::MessageSender(SENDER * sendObject,SEND_FUNC sendFunction):
		m_sendObject(sendObject),
		m_sendFunction(sendFunction)
	{
	}

	template <class SENDER,class RESULT>
	void MessageSender<SENDER,RESULT,void>::operator() (const Zoic::Message &msg) const
	{
		(m_sendObject->*m_sendFunction)(msg);
	}

	template<WORD MSGID,class PACKET>
	void messagePackerFunction(Zoic::Message &msg,void *packet)
	{
		msg.ID(MSGID) << *(static_cast<PACKET *>(packet));
	}

	template<WORD MSGID,class PACKET>
	VectorPacker makeVectorPacker(PACKET & packet,void * vector)
	{
		return VectorPacker(static_cast<void *>(&packet),vector,&messagePackerFunction<MSGID,PACKET>);
	}
}

#endif
