/*! @file
	@ingroup utils
	@brief 定义禁止拷贝基类
*/

#ifndef _ZOIC_NOCOPY_H
#define _ZOIC_NOCOPY_H

namespace Zoic
{
	/*! @brief 禁止拷贝基类
		@ingroup utils


		对于想禁止拷贝操作的类，可通过继承该类实现该目的。
	*/
	class NoCopy
	{
	protected:
		NoCopy();
		~NoCopy();
	private:
		//!禁止拷贝操作
		NoCopy(const NoCopy &);
		//!禁止拷贝操作
		NoCopy & operator=(const NoCopy &);
	};

	inline NoCopy::NoCopy()
	{
	}

	inline NoCopy::~NoCopy()
	{
	}
}

#endif
