/*! @file
	@ingroup utils
	@brief ����Խ���쳣��
*/

#ifndef __ZOIC_OUTOFBOUNDEXCEPTION_H
#define __ZOIC_OUTOFBOUNDEXCEPTION_H

#include "Exception.h"

namespace Zoic
{
	/*! @brief Խ���쳣��
		@ingroup utils
	*/
	class  OutOfBoundException:
		public Exception
	{
	public:
		//@{
		//!���캯��
		OutOfBoundException();
		OutOfBoundException(
			const char * format,	//!< ��ʽ�����ַ���
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
