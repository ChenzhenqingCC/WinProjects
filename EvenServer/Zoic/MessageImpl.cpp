#include "include/Message.h"
#include "include/OutOfBoundException.h"
#include "include/MessageImpl.h"

namespace Zoic
{
	Zoic::Message& operator << (Zoic::Message& msg,const VarLenValue& arg)
	{
		size_t argValue = arg.value;
		while( argValue >= VAR_LEN_MASK_VALUE )
		{
			BYTE pushNum = static_cast<BYTE>(argValue);
			pushNum |= VAR_LEN_MASK_VALUE;
			msg << pushNum;
			argValue = argValue >> VAR_LEN_MASK_INDEX;
		}
		msg << static_cast<BYTE>(argValue);
		return msg;
	}

	Zoic::Message& operator >> (Zoic::Message& msg,VarLenValue& arg)
	{
		BYTE popNum;
		msg >> popNum;
		arg.value = popNum & (~VAR_LEN_MASK_VALUE);
		int moveIndex=VAR_LEN_MASK_INDEX;
		for(; popNum&VAR_LEN_MASK_VALUE; moveIndex+=VAR_LEN_MASK_INDEX);
		{
			msg >> popNum;
			arg.value |= static_cast<size_t>( popNum & (~VAR_LEN_MASK_VALUE) ) << moveIndex;
		}
		if( moveIndex > sizeof(size_t)*8 )
		{
			Zoic::throwOutOfBoundException("[ID:%d] Message::VarLenValue Value To Large(Use %d Bytes)",msg.ID(), moveIndex/8 );
		}
		return msg;
	}

	namespace MessageImpl
	{

		void setString(Message & msg,const char *str,WORD buffSize)
		{
			size_t strLength = strlen(str);
			if( strLength >= static_cast<size_t>( buffSize ) )
			{
				Zoic::throwOutOfBoundException("[ID:%d] Message::<< String size(%d) larger than %d",msg.ID(),strLength,buffSize-1);
				//msg.writeData(str, buffSize - 1 );
				//msg.writeData("\0", 1);
				//return;
			}
			msg.writeData(str, static_cast<int>( strLength + 1) );
		}

		void getString(Message & msg,char *str,WORD buffSize)
		{
			msg.readString(str,buffSize);
		}

	}
}
