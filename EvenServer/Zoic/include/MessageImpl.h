/*! @file
	@ingroup utils
	@brief 定义字符串模板
*/

#ifndef __ZOIC_MESSAGEIMPL_H
#define __ZOIC_MESSAGEIMPL_H

#include "Message.h"
#include "Binary.h"
#include "FixArray.h"

namespace Zoic
{
	//!抛出越界异常类
	extern void throwOutOfBoundException(
		const char * format,	//!< 格式控制字符串
		...);

	//!在std::vector中可以存放在最大元素数量
	const int	VECTOR_MAX_ELEMENTS = 0xFF;
	typedef BYTE MSG_VEC_COUNT_TYPE;

	struct VarLenValue
	{
		size_t value;
	};
	const size_t	VAR_LEN_MASK_INDEX = 7;
	const size_t	VAR_LEN_MASK_VALUE = 1 << VAR_LEN_MASK_INDEX;

	template<class T>
	class VectorVarLen
		:public std::vector<T>
	{
	};
}

namespace Zoic
{
	template<class T>
	void addBeforeMessage(const T& arg,const Zoic::Message& msgOrg,Zoic::Message& msgOutput)
	{
		msgOutput.ID(msgOrg.ID());
		msgOutput << arg;

		WORD sizeOrg=msgOrg.getSize() - NETWORK_MSG_HEADER;
		WORD sizeBegin=msgOutput.getSize();
		if(sizeBegin+sizeOrg>NETWORK_BUF_SIZE)
		{
			Zoic::throwOutOfBoundException("[ID:%d] Message::addBeforeMessage %d + %d out of bound",msgOrg.ID(),sizeBegin,sizeOrg);
		}
		BYTE * buf=const_cast<BYTE *>(msgOutput.getData());
		*(WORD *)buf=sizeBegin+sizeOrg;
		memmove(buf+sizeBegin,msgOrg.getData()+NETWORK_MSG_HEADER,sizeOrg);
	}

	template<class T>
	Zoic::Message& operator << (Zoic::Message& msg,const std::vector<T> & arg)
	{
		if(arg.size() > VECTOR_MAX_ELEMENTS)
		{
			Zoic::throwOutOfBoundException("[ID:%d] Message::<< vector size(%d) larger than %d",msg.ID(),arg.size(),VECTOR_MAX_ELEMENTS);
		}
		MSG_VEC_COUNT_TYPE size=static_cast<MSG_VEC_COUNT_TYPE>(arg.size());
		msg << size;
		for(MSG_VEC_COUNT_TYPE i=0;i<size;++i)
		{
			msg << arg[i];
		}
		return msg;
	}

	template<class T>
	Zoic::Message& operator >> (Zoic::Message& msg,std::vector<T>& arg)
	{
		MSG_VEC_COUNT_TYPE size;
		arg.clear();
		T tObj;
		msg >> size;
		for(MSG_VEC_COUNT_TYPE i=0;i<size;++i)
		{
			msg >> tObj;
			arg.push_back(tObj);
		}
		return msg;
	}

	Zoic::Message& operator << (Zoic::Message& msg,const VarLenValue& arg);
	Zoic::Message& operator >> (Zoic::Message& msg,VarLenValue& arg);

	template<class T>
	Zoic::Message& operator << (Zoic::Message& msg,const VectorVarLen<T> & arg)
	{
		if( arg.size() > 0x0FFFFFFFL)
		{
			Zoic::throwOutOfBoundException("[ID:%d] Message::<< vector size(%d) larger than %d",msg.ID(),arg.size(),0x0FFFFFFFL);
		}
		VarLenValue vecSize = { arg.size() };
		msg << vecSize;
		for(size_t i=0;i<vecSize.value;++i)
		{
			msg << arg[i];
		}
		return msg;
	}

	template<class T>
	Zoic::Message& operator >> (Zoic::Message& msg,VectorVarLen<T>& arg)
	{
		VarLenValue vecSize;
		arg.clear();
		T tObj;
		msg >> vecSize;
		for(MSG_VEC_COUNT_TYPE i=0;i<vecSize.value;++i)
		{
			msg >> tObj;
			arg.push_back(tObj);
		}
		return msg;
	}


	template<class T,unsigned int LEN>
	Zoic::Message& operator << (Zoic::Message& msg,const Zoic::FixArray<T,LEN> & arg)
	{
		for(typename Zoic::FixArray<T,LEN>::size_type i=0;i<LEN;++i)
		{
			msg << arg[i];
		}		
		return msg;
	}

	template<class T,unsigned int LEN>
	Zoic::Message& operator >> (Zoic::Message& msg,Zoic::FixArray<T,LEN> & arg)
	{
		for(typename Zoic::FixArray<T,LEN>::size_type i=0;i<LEN;++i)
		{
			msg >> arg.getData()[i];
		}
		return msg;
	}

	template<class T,unsigned int LEN,class SIZE_TYPE>
	Zoic::Message& operator << (Zoic::Message& msg,const Zoic::VarArray<T,LEN,SIZE_TYPE> & arg)
	{
		typename Zoic::VarArray<T,LEN,SIZE_TYPE>::size_type size=arg.size();
		if(size > LEN)
		{
			Zoic::throwOutOfBoundException("[ID:%d] Message::<< VarArray size(%d) larger than %d",msg.ID(),size,LEN);
		}
		msg << size;
		for(SIZE_TYPE i=0;i<size;++i)
		{
			msg << arg[i];
		}		
		return msg;
	}

	template<class T,unsigned int LEN,class SIZE_TYPE>
	Zoic::Message& operator >> (Zoic::Message& msg,Zoic::VarArray<T,LEN,SIZE_TYPE> & arg)
	{
		typename Zoic::VarArray<T,LEN,SIZE_TYPE>::size_type size;
		arg.clear();
		T tObj;
		msg >> size;
		if(size > LEN)
		{
			Zoic::throwOutOfBoundException("[ID:%d] Message:: >> VarArray size(%d) larger than %d",msg.ID(),size,LEN);
		}
		for(SIZE_TYPE i=0;i<size;++i)
		{
			msg >> tObj;
			arg.push_back(tObj);
		}
		return msg;
	}

	template<WORD LEN>
	Zoic::Message& operator << (Zoic::Message& msg,const Zoic::Binary<LEN> & arg)
	{
		WORD size=arg.size();
		if(size > LEN)
		{
			Zoic::throwOutOfBoundException("[ID:%d] Message::<< Binary size(%d) larger than %d",msg.ID(),size,LEN);
		}
		msg << size;
		msg.writeData(arg.getData(),size);
		return msg;
	}

	template<WORD LEN>
	Zoic::Message& operator >> (Zoic::Message& msg,Zoic::Binary<LEN> & arg)
	{
		WORD size;
		arg.clear();
		msg >> size;
		if(size > LEN)
		{
			Zoic::throwOutOfBoundException("[ID:%d] Message:: >> Binary size(%d) larger than %d",msg.ID(),size,LEN);
		}
		if(size > msg.getRemainingRData())
		{
			Zoic::throwOutOfBoundException("[ID:%d] Message::ReadData > getRemainingRData()",msg.ID());
		}
		const char * data = reinterpret_cast<const char*>(msg.getReadPosition());
		msg.skipData(size);
		arg.setData( data , size);
		return msg;
	}

	namespace MessageImpl
	{
		void setString(Zoic::Message & msg,const char *str,WORD buffSize);
		void getString(Zoic::Message & msg,char *str,WORD buffSize);
	}
}


#endif
