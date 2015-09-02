#include "include/MessageTransfer.h"
#include "include/OutOfBoundException.h"

namespace Zoic
{
	void transferMessageJoinMessage(Zoic::Message& output,WORD msgID,const Zoic::Message& headMsg,const Zoic::Message& tailMsg)
	{
		const int HEAD_SIZE = headMsg.getSize() - NETWORK_MSG_HEADER;
		const int TAIL_SYZE = tailMsg.getSize()-NETWORK_MSG_HEADER;
		const int TARGET_FULL_SIZE = HEAD_SIZE + TAIL_SYZE + NETWORK_MSG_HEADER;
		if( TARGET_FULL_SIZE >= NETWORK_BUF_SIZE )
		{
			throw new OutOfBoundException("[ID:%d] Transfer Message::OUTPUT_SZIE(%d) > NETWORK_BUF_SIZE",output.ID(), TARGET_FULL_SIZE );
		}
		output.ID(msgID);
		BYTE * buf=const_cast<BYTE *>(output.getData());
		*(WORD *)buf=TARGET_FULL_SIZE;
		memmove(buf+NETWORK_MSG_HEADER+HEAD_SIZE, tailMsg.getData()+NETWORK_MSG_HEADER, TAIL_SYZE);
		if(&headMsg != &output)
		{
			memmove(buf+NETWORK_MSG_HEADER, headMsg.getData()+NETWORK_MSG_HEADER, HEAD_SIZE);
		}
	}
}
