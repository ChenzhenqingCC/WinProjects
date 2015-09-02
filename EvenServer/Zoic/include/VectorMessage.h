/*! @file
	@ingroup utils
	@brief ���巢��������Ϣģ��

	��ν������Ϣ,��ָ��������Ϣ���а�����������һ�����顣������ʾ��
	<pre>
	//�ʼ������б� Manager->Client
	const WORD MC_MAILBODY_NTF=933;
	struct P_MC_MailBody_Ntf
	{
		std::vector<MailBrief>	infos;	//�ʼ��б�
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
	const int DEFAULT_VECTOR_BLOCK_SIZE = 20;	//!< Ĭ��������С

	/*! @brief  ��Ϣ�����
		������ ��Ϣ������std::vector���͵����ݴ�����⣬��sendVectorMessage��������ʹ�á�
	*/
	class VectorPacker
	{
	public:
		//! ת����������
		typedef void (*PACK_FUNC)(Zoic::Message &msg,void *packet);

		//!���캯��
		VectorPacker(
			void *packet,			//!< ָ�� ��Ϣ�� ��ַ
			void *vector,			//!< ָ�� ��Ϣ���е�std::vector<T> ��ַ
			PACK_FUNC func			//!< ת������
			);
		void pack(Message &msgSend) const;
		void * getVectorPointer() const;
	private:
		void *	m_packet;	//!< ָ�� ��Ϣ�� ��ַ
		void *	m_vector;	//!< ָ�� ��Ϣ���е�std::vector<T> ��ַ
		PACK_FUNC m_func;	//!< ת������
	};

	/*! @brief  ��Ϣ�����
		������ ��Ϣ������std::vector���͵����ݴ�����⣬��sendVectorMessage��������ʹ�á�
	*/
	template
	<
		class PACKET
	>
	class DefaultVectorPacker
	{
	public:
		//!���캯��
		DefaultVectorPacker(
			PACKET & packet,	//!< ָ�� ��Ϣ�� ��ַ
			void * vector,		//!< ָ�� ��Ϣ���е�std::vector<T> ��ַ
			WORD msgID			//!< Ҫ���͵���ϢID
			);
		void pack(Message &msgSend) const;
		void * getVectorPointer() const;
	private:
		PACKET &	m_packet;	//!<��Ϣ��
		void *		m_vector;	//!< ָ�� ��Ϣ���е�std::vector<T> ��ַ
		WORD		m_msgID;	//!< Ҫ���͵���ϢID
	};

	/*! @brief ��Ϣ����ģ����

		ͨ��Ӧ�ò�����"()"������Ϣ��
		@ingroup utils
	
		@param SENDER		���Ͷ���������
		@param RESULT		���ͺ�������ֵ
		@param ARGUMENT		���ͺ��������������
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
		typedef RESULT (SENDER::*SEND_FUNC)(const Zoic::Message &msg,ARGUMENT arg);	//!< ���ͺ�������
	public:
		//!���캯��
		MessageSender(
			SENDER * sendObject,		//!< ������Ϣ����
			SEND_FUNC sendFunction,		//!< ������Ϣ����
			ARGUMENT arg				//!< ������Ϣ����
			);
		
		//!ִ�з�����Ϣ����
		void operator() (
			const Zoic::Message &msg	//!< Ҫ���͵���Ϣ
			) const;
	private:
		SENDER * m_sendObject;			//!< ������Ϣ����
		SEND_FUNC m_sendFunction;		//!< ������Ϣ����
		ARGUMENT m_arg;					//!< ������Ϣ����
	};

	/*! @brief ��Ϣ����ģ����,�޶��������

		ͨ��Ӧ�ò�����"()"������Ϣ��
		@ingroup utils
	
		@param SENDER		���Ͷ���������
		@param RESULT		���ͺ�������ֵ
	*/
	template
	<
		class SENDER,
		class RESULT
	>
	class MessageSender<SENDER,RESULT,void>
	{
	private:
		typedef RESULT (SENDER::*SEND_FUNC)(const Zoic::Message &msg);	//!< ���ͺ�������
	public:
		//!���캯��
		MessageSender(
			SENDER * sendObject,		//!< ������Ϣ����
			SEND_FUNC sendFunction		//!< ������Ϣ����
			);
		
		//!ִ�з�����Ϣ����
		void operator() (
			const Zoic::Message &msg	//!< Ҫ���͵���Ϣ
			) const;
	private:
		SENDER * m_sendObject;			//!< ������Ϣ����
		SEND_FUNC m_sendFunction;		//!< ������Ϣ����
	};

	/*! @brief ��������ģ����
		@ingroup utils
	
		����Zoic::for_each�㷨��
		
		@param ELEMENT		������Ԫ������
		@param SENDER		���Ͷ���������
		@param CONTAINER	����ԭʼ���ݵ�ת����
		@param PACKET		���͵����ݰ������Ľṹ
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
		//!���캯��
		VectorSender(
			const SENDER &sender,								//!< ������Ϣ��������
			const PACKER & packer,								//!< ������Ϣ�������
			void (*pusher)(std::vector<ELEMENT> &vec,const typename CONTAINER::value_type & val),	//!< ת������
			const int blockSize									//!< ���С
			);

		/*! @brief ��������
		
			���������Ὣ���Ĳ��ɿ����ݷ��ͳ�ȥ
		*/
		~VectorSender();
		
		//! �ռ�����,���ڴﵽ���Сʱ��������
		void operator() (
			const typename CONTAINER::value_type & elem		//!< ������Ԫ������
			);
	private:
		const SENDER & m_sender;					//!< ������Ϣ��������
		void (*m_pusher)(std::vector<ELEMENT> &vec,const typename CONTAINER::value_type & val);//!< ת������
		int m_blockSize;							//!< ���С
		const PACKER & m_packer;					//!< ������Ϣ�������
		std::vector<ELEMENT>& m_elements;			//!< ��Ҫ���͵�Ԫ������

		//!����Ԫ��
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
		//using Proto::operator <<;//��Ҫ���������ռ�����⣬Э�������ռ䲻ֻProtoһ��
		msgSend.ID(m_msgID) << m_packet;
	}

	template <class PACKET>
	void * DefaultVectorPacker<PACKET>::getVectorPointer() const
	{
		return m_vector;
	}

	/*! @brief Ĭ��������Ϣת������
		@ingroup utils
	*/
	template <class ELEMENT,class VALUE>
	void defaultVectorMessagePusher(
		std::vector<ELEMENT> &vec,	//!< ��Ž������
		const VALUE & val			//!< ԭʼ��������
		)
	{
		vec.push_back(val);
	}
	/*! @brief Ĭ�ϵ�ָ��������Ϣת������
	@ingroup utils
	*/
	template <class ELEMENT,class VALUE>
	void defaultVectorMessagePusher(
		std::vector<ELEMENT> &vec,	//!< ��Ž������
		VALUE* const& pVal			//!< ԭʼ��������ָ��
		)
	{
		vec.push_back(*pVal);
	}

	/*! @brief ����������Ϣ
		@ingroup utils
	*/
	template <class ELEMENT,class SENDER,class CONTAINER>
	void sendVectorMessage(
		const SENDER & sender,						//!< ������Ϣ��������
		WORD msgID,									//!< Ҫ���͵���ϢID
		const CONTAINER & container,				//!< ԭʼ��������
		void (*pusher)(std::vector<ELEMENT> &vec,const typename CONTAINER::value_type & val)=defaultVectorMessagePusher,//!< ת������
		const int blockSize=DEFAULT_VECTOR_BLOCK_SIZE	//!< ���С
		)
	{
		std::vector<ELEMENT> elements;
		DefaultVectorPacker<std::vector<ELEMENT> > packer(elements,&elements,msgID);
		VectorSender<ELEMENT,SENDER,CONTAINER,DefaultVectorPacker<std::vector<ELEMENT> >  > vSender(sender,packer,pusher,blockSize);
		Zoic::for_each(container.begin(),container.end(),vSender);
	}

	/*! @brief ����������Ϣ,���÷��Ϳ��С
		@ingroup utils
	*/
	template <class ELEMENT,class SENDER,class CONTAINER>
	void sendVectorMessage(
		const SENDER & sender,						//!< ������Ϣ��������
		WORD msgID,									//!< Ҫ���͵���ϢID
		const CONTAINER & container,				//!< ԭʼ��������
		const int blockSize							//!< ���С
		)
	{
		std::vector<ELEMENT> elements;
		DefaultVectorPacker<std::vector<ELEMENT> > packer(elements,&elements,msgID);
		VectorSender<ELEMENT,SENDER,CONTAINER,DefaultVectorPacker<std::vector<ELEMENT> >  > vSender(sender,packer,defaultVectorMessagePusher,blockSize);
		Zoic::for_each(container.begin(),container.end(),vSender);
	}

	/*! @brief ����������Ϣ
		@ingroup utils
	*/
	template <class ELEMENT,class SENDER,class CONTAINER,class PACKET>
	void sendVectorMessage(
		const SENDER & sender,						//!< ������Ϣ��������
		WORD msgID,									//!< Ҫ���͵���ϢID
		const PACKET & packet,						//!< Ҫ���͵����ݰ�����
		const std::vector<ELEMENT> & elements,		//!< Ҫ���͵����ݰ��е��������ڵı���
		const CONTAINER & container,				//!< ԭʼ��������
		void (*pusher)(std::vector<ELEMENT> &vec,const typename CONTAINER::value_type & val)=defaultVectorMessagePusher,//!< ת������
		const int blockSize=DEFAULT_VECTOR_BLOCK_SIZE	//!< ���С
		)
	{
		PACKET pkt(packet);
		DefaultVectorPacker<PACKET> packer(pkt,reinterpret_cast<char *>(&pkt)+(reinterpret_cast<const char *>(&elements)-reinterpret_cast<const char *>(&packet)),msgID);
		VectorSender<ELEMENT,SENDER,CONTAINER,DefaultVectorPacker<PACKET> > vSender(sender,packer,pusher,blockSize);
		Zoic::for_each(container.begin(),container.end(),vSender);
	}

	/*! @brief ����������Ϣ,���÷��Ϳ��С
		@ingroup utils
	*/
	template <class ELEMENT,class SENDER,class CONTAINER,class PACKET>
	void sendVectorMessage(
		const SENDER & sender,						//!< ������Ϣ��������
		WORD msgID,									//!< Ҫ���͵���ϢID
		const PACKET & packet,						//!< Ҫ���͵����ݰ�����
		const std::vector<ELEMENT> & elements,		//!< Ҫ���͵����ݰ��е��������ڵı���
		const CONTAINER & container,				//!< ԭʼ��������
		const int blockSize							//!< ���С
		)
	{
		PACKET pkt(packet);
		DefaultVectorPacker<PACKET> packer(pkt,reinterpret_cast<char *>(&pkt)+(reinterpret_cast<const char *>(&elements)-reinterpret_cast<const char *>(&packet)),msgID);
		VectorSender<ELEMENT,SENDER,CONTAINER,DefaultVectorPacker<PACKET> > vSender(sender,packer,defaultVectorMessagePusher,blockSize);
		Zoic::for_each(container.begin(),container.end(),vSender);
	}

	/*! @brief ����������Ϣ
		@ingroup utils
	*/
	template <class ELEMENT,class SENDER,class CONTAINER>
	void sendVectorMessage(
		const SENDER & sender,						//!< ��Ϣ���ͺ�������
		const VectorPacker & packer,				//!< ��Ϣ�����������
		const CONTAINER & container,				//!< ԭʼ��������
		void (*pusher)(std::vector<ELEMENT> &vec,const typename CONTAINER::value_type & val)=defaultVectorMessagePusher,//!< ת������
		const int blockSize=DEFAULT_VECTOR_BLOCK_SIZE	//!< ���С
		)
	{
		VectorSender<ELEMENT,SENDER,CONTAINER,VectorPacker > vSender(sender,packer,pusher,blockSize);
		Zoic::for_each(container.begin(),container.end(),vSender);
	}

	/*! @brief ����������Ϣ,���÷��Ϳ��С
		@ingroup utils
	*/
	template <class ELEMENT,class SENDER,class CONTAINER>
	void sendVectorMessage(
		const SENDER & sender,						//!< ��Ϣ���ͺ�������
		const VectorPacker & packer,				//!< ��Ϣ�����������
		const CONTAINER & container,				//!< ԭʼ��������
		const int blockSize							//!< ���С
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
			{//Ԫ�����ݴ�࣬����һ��һ���ķ���
				delete e;
				
				std::vector<ELEMENT> elements;
				m_elements.swap(elements);
				VectorSender<ELEMENT,SENDER,std::vector<ELEMENT>,PACKER > vSender(m_sender,m_packer,defaultVectorMessagePusher,1);
				Zoic::for_each(elements.begin(),elements.end(),vSender);
			}
			else
			{//���������һ��Ԫ�ػ�����,ֱ���׳��쳣
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
