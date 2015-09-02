/*! @file
	@ingroup utils
	@brief 定义越界异常类
*/

#ifndef __ZOIC_OUTOFBOUNDEXCEPTION_H
#define __ZOIC_OUTOFBOUNDEXCEPTION_H

#include "Exception.h"

namespace Zoic
{
	/*! @brief 越界异常类
		@ingroup utils
	*/
	class  OutOfBoundException:
		public Exception
	{
	public:
		//@{
		//!构造函数
		OutOfBoundException();
		OutOfBoundException(
			const char * format,	//!< 格式控制字符串
			...
			);
		//@}

		virtual	~OutOfBoundException() throw();
	};
	
	inline OutOfBoundException::OutOfBoundException()
	{
	}
	
	inline OutOfBoundException::OutOfBoundException( const char * format,...)
	{
		MAKE_EXCEPTION_STRING(format)
	}
	
	inline OutOfBoundException::~OutOfBoundException() throw()
	{
	}

}

#endif
