#include "include/Types.h"
#include "include/Exception.h"

using namespace Zoic;

Exception::Exception()
{
}

Exception::Exception(const char * format,...)
{
	MAKE_EXCEPTION_STRING(format)
}

Exception::~Exception() throw ()
{
}

const char * Exception::what() const throw()
{
	return m_str.c_str();
}
