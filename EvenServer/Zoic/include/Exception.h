/*! @file
	@ingroup utils
	@brief 定义异常基类
*/

#ifndef __ZOIC_EXCEPTION_H
#define __ZOIC_EXCEPTION_H

#include <exception>
#include <string>
#include <stdarg.h>
#include "NoCopy.h"

namespace Zoic
{
	/*! @brief 异常基类
		@ingroup utils
	*/
	class  Exception:
		public std::exception,
		public NoCopy
	{
	public:
		//@{
		//!构造函数
		Exception();
		Exception(
			const char * format,	//!< 格式控制字符串
			...
			);
		//@}

		//!获得异常信息
		const char *what() const throw();
		~Exception() throw();
	protected:
		std::string m_str;		//!< 异常描述
	};
}

/*! @brief 产生异常字符串
	@param format 格式控制字符串
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
