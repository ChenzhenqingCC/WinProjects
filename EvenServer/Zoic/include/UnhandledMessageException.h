/*! @file
	@ingroup buildin
	@brief 定义未处理消息异常类
*/

#ifndef __ZOIC_UNHANDLEDMESSAGEEXCEPTION_H
#define __ZOIC_UNHANDLEDMESSAGEEXCEPTION_H

#include "Exception.h"

namespace Zoic
{
	/*! @brief 协议版本异常类
		@ingroup buildin
	*/
	class UnhandledMessageException:
		public Exception
	{
	public:
		//!构造函数
		UnhandledMessageException(
			WORD id,	//!< 消息ID
			WORD size	//!< 消息长度
			);
		virtual	~UnhandledMessageException() throw();
	};
	
	inline UnhandledMessageException::UnhandledMessageException(WORD id,WORD size):
			Exception("unhandled msg ID:%d size:%d",id,size)
	{
	}
	
	inline UnhandledMessageException::~UnhandledMessageException() throw()
	{
	}
}

#endif
