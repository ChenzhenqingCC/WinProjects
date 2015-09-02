/*! @file
	@ingroup utils
	@brief �����ֹ��������
*/

#ifndef _ZOIC_NOCOPY_H
#define _ZOIC_NOCOPY_H

namespace Zoic
{
	/*! @brief ��ֹ��������
		@ingroup utils


		�������ֹ�����������࣬��ͨ���̳и���ʵ�ָ�Ŀ�ġ�
	*/
	class NoCopy
	{
	protected:
		NoCopy();
		~NoCopy();
	private:
		//!��ֹ��������
		NoCopy(const NoCopy &);
		//!��ֹ��������
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
