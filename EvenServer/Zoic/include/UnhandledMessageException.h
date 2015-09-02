/*! @file
	@ingroup buildin
	@brief ����δ������Ϣ�쳣��
*/

#ifndef __ZOIC_UNHANDLEDMESSAGEEXCEPTION_H
#define __ZOIC_UNHANDLEDMESSAGEEXCEPTION_H

#include "Exception.h"

namespace Zoic
{
	/*! @brief Э��汾�쳣��
		@ingroup buildin
	*/
	class UnhandledMessageException:
		public Exception
	{
	public:
		//!���캯��
		UnhandledMessageException(
			WORD id,	//!< ��ϢID
			WORD size	//!< ��Ϣ����
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
