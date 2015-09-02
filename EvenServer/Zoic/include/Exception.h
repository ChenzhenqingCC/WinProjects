/*! @file
	@ingroup utils
	@brief �����쳣����
*/

#ifndef __ZOIC_EXCEPTION_H
#define __ZOIC_EXCEPTION_H

#include <exception>
#include <string>
#include <stdarg.h>
#include "NoCopy.h"

namespace Zoic
{
	/*! @brief �쳣����
		@ingroup utils
	*/
	class  Exception:
		public std::exception,
		public NoCopy
	{
	public:
		//@{
		//!���캯��
		Exception();
		Exception(
			const char * format,	//!< ��ʽ�����ַ���
			...
			);
		//@}

		//!����쳣��Ϣ
		const char *what() const throw();
		~Exception() throw();
	protected:
		std::string m_str;		//!< �쳣����
	};
}

/*! @brief �����쳣�ַ���
	@param format ��ʽ�����ַ���
*/
#define MAKE_EXCEPTION_STRING(format) \
	va_list args; \
	char buf[8096]; \
	va_start(args, format); \
	vsnprintf(buf, sizeof(buf),format, args); \
	va_end(args); \
	buf[sizeof(buf)-1]='\0'; \
	m_str=buf; 

#endif
