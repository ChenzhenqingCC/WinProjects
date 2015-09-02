/*! @file
	@ingroup utils
	@brief ��������ַ�������
*/

#ifndef __ZOIC_SPLITSTRING_H
#define __ZOIC_SPLITSTRING_H

#include <vector>
#include <string>

namespace Zoic
{
	/*! @brief ����ַ���
		@ingroup utils
	*/
	void splitString(
		const char *src,					//!< ԭ�ַ���
		std::vector<std::string> &dst,		//!< ���ɵ��ַ�������
		const char *separator="\t \r\n"		//!< �ָ���
		);
}

#endif
