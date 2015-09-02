/*! @file
	@ingroup utils
	@brief 定义配置类
*/

#ifndef __ZOIC_CONFIG_H
#define __ZOIC_CONFIG_H

#include <Zoic/include/Types.h>
#include <string>
#include <map>

namespace Zoic
{
	/*!
		@brief 配置类
		@ingroup utils


		读取配置文件
		配置文件的格式为:
		1.key=value的形式,允许在等号两端存在空格,但读取会被忽略
		2.#开头的行被认为注释
	*/
	class ConfigLoader
	{
		protected:
			ConfigLoader();
			//!载入配置文件
			bool load(
				const char *file	//!< 文件名
				);
			bool load(
				FILE *fp	//!< 文件
				);
			bool isLoaded() const;
		protected:
			typedef std::map<std::string,std::string> VAR_MAP;
			static bool readLine(
				VAR_MAP& output,
				const char *line	//!< 行内容
				);
			VAR_MAP	m_vars;	//! 配置变量
			bool m_loaded;
	};

	inline bool ConfigLoader::isLoaded() const
	{
		return m_loaded;
	}
}

#endif
