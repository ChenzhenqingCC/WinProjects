#ifndef __ZOIC_MESSAGETRANSFER_H
#define __ZOIC_MESSAGETRANSFER_H

#include "Message.h"

namespace Zoic
{
	void transferMessageJoinMessage(Zoic::Message& output,WORD msgID,const Zoic::Message& headMsg,const Zoic::Message& tailMsg);

	template<class T>
	void transferMessageAddHeader(Zoic::Message& target,const T& header)
	{
		Message extraMsg;
		extraMsg.ID(0) << header;
		transferMessageJoinMessage(target,target.ID(),extraMsg,target);
	}

	template<class T>
	void transferMessageAddHeader(Zoic::Message& output,const T& header,const Zoic::Message& inputMsg)
	{
		if( &inputMsg == &output )
		{
			transferMessageAddHeader(output,header);
			return;
		}
		output.ID( inputMsg.ID() ) << header;
		transferMessageJoinMessage(output,inputMsg.ID(),output,inputMsg);
	}

}

#endif
