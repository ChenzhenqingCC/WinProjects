/*! @file
	@ingroup buildin
	@brief 定义网络日志类
*/

#ifndef __ZOIC_NETLOG_H
#define __ZOIC_NETLOG_H

#include "Types.h"
#include "Module.h"

namespace Zoic
{
	typedef int LogLevelValue;
	extern const LogLevelValue DEBUG_LOG_LEVEL;
}

namespace Zoic
{
	class Logger;
	/*! @brief 日志类
		@ingroup buildin


		将网络收发数据都记录下来。
	*/
	class NetLog:
		public Module<NetLog>
	{
		friend class Module<NetLog>;
	public:
		/*! @brief 配置网络日志
			
			
			设置网络日志基本目录名,简要日志前缀和后缀(如果为空则不写简要日志)<br>
			只有在设置了该函数后，以后在写网络日志时才有效，否则不写
		*/
		void config(
			const char *basedir,		//!< 基本目录
			const char *prefix=NULL,	//!< 简要日志前缀
			const char *suffix=NULL		//!< 简要日志后缀
			);
		
		/*! @brief 写网络日志
			
			
			在使用前应先调用config,否则仅是空函数调用
		*/
		void netlog(
			const char *ip,			//IP地址
			DWORD port,				//端口号
			const char *local_ip,	//本地IP地址
			DWORD local_port,		//本地端口号
			const void *buffer,		//数据缓存
			int len,				//数据长度
			bool recv,				//接收标志(true:接收;false:发送)
			LogLevelValue logLevel = DEBUG_LOG_LEVEL
			);

		/*! @brief 写网络日志


		通过每次另传Config参数来临时写网络日志
		*/
		void netlogWithConfig(
			LogLevelValue logLevel,
			const char *basedir,			//!< 基本目录(结尾不能有斜杠)
			const char *ip,					//!< IP地址
			DWORD port,						//!< 端口号
			const char *binary_pathname,	//!< 数据文件名(相对于basedir)
			const void *buffer,				//!< 数据缓存
			int len,						//!< 数据长度
			bool recv,						//!< 接收标志(true:接收;false:发送)
			const char *brief_pathname		//!< 简要日志路径
			);
		
	private:
		NetLog();
		//!本地日志
		void local_flog(
			LogLevelValue logLevel,	//!< 日志级别
			const char *pathname,	//!< 路径
			const char *msg,	//!< 控制格式
			...);

		//!更新时间
		void updateTime();
		std::string m_basedir;		//!< 日志所在目录
		std::string m_prefix;		//!< 前缀
		std::string m_suffix;		//!< 后缀
		TIME_SEC	m_lasttime;		//!< 上一次调用log的时间
		char 		m_timebuf[20];	//!< 时间字符串值
		char		m_datebuf[20];	//!< 日期字符串值
	};
}

#define netlog Zoic::NetLog::getInstance()->netlog
#define g_Netlog Zoic::NetLog::getInstance()

#endif
