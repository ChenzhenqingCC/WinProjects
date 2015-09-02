/*! @file
	@ingroup utils
	@brief ����������
*/

#ifndef __ZOIC_CONFIG_H
#define __ZOIC_CONFIG_H

#include <Zoic/include/Types.h>
#include <string>
#include <map>

namespace Zoic
{
	/*!
		@brief ������
		@ingroup utils


		��ȡ�����ļ�
		�����ļ��ĸ�ʽΪ:
		1.key=value����ʽ,�����ڵȺ����˴��ڿո�,����ȡ�ᱻ����
		2.#��ͷ���б���Ϊע��
	*/
	class ConfigLoader
	{
		protected:
			ConfigLoader();
			//!���������ļ�
			bool load(
				const char *file	//!< �ļ���
				);
			bool load(
				FILE *fp	//!< �ļ�
				);
			bool isLoaded() const;
		protected:
			typedef std::map<std::string,std::string> VAR_MAP;
			static bool readLine(
				VAR_MAP& output,
				const char *line	//!< ������
				);
			VAR_MAP	m_vars;	//! ���ñ���
			bool m_loaded;
	};

	inline bool ConfigLoader::isLoaded() const
	{
		return m_loaded;
	}
}

#endif
