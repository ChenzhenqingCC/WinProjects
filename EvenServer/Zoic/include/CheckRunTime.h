/*! @file
	@ingroup utils
	@brief 定义检测运行期时间类
*/

#ifndef __CHECK_RUNTIME_H
#define __CHECK_RUNTIME_H

#include "Types.h"


#define CHECK_RUNTIME_TIMEOUT	0
#ifdef _DEBUG
#undef CHECK_RUNTIME_TIMEOUT
#define CHECK_RUNTIME_TIMEOUT	50
#endif

// 09/11/2008-15:15 added by YYBear for force enabled in release
#ifndef _DEBUG
#ifdef CHECK_RUNTIME_RELEASE
#undef CHECK_RUNTIME_TIMEOUT
#define CHECK_RUNTIME_TIMEOUT	50
#endif
#endif
// 09/11/2008-15:17 end adding by YYBear

namespace Zoic
{
	/*!
		@brief 检测运行期时间类
		@ingroup utils
	*/
	class CheckRunTime
	{
	public:
		//!构造函数
		CheckRunTime(
			const char *name,	//!< 检查名称
			const char *file,	//!< 检查所在文件名
			int line,			//!< 检查行号
			int timeout = CHECK_RUNTIME_TIMEOUT	//!< 超时时间(毫秒)
			);

		~CheckRunTime();

		//!设置上下文1
		void setContext1(
			const char *file,	//!< 文件名
			int line			//!< 行号
			);

		//!设置上下文2
		void setContext2(
			const char *file,	//!< 文件名
			int line			//!< 行号
			);

		//!设置超时时间
		void setTimeout(
			int timeout			//!< 超时时间(毫秒)
			);

		/*! @brief 检测超时时间


			如果当前时间与开始检测时间超过timeout,会在日志文件中写入记录
		*/
		void checkTimeOut(int timeout);
	private:
		//获得运行时间
		TIME_MSEL getRunTime();

		TIME_MSEL	m_runtime;			//!< 开始检测时间
		const char *	m_name;				//!< 名称
		const char * 	m_file;				//!< 文件名
		int				m_line;				//!< 行号
		const char *	m_context_file1;	//!< 上下文1文件名
		int				m_context_line1;	//!< 上下文1行号
		const char *	m_context_file2;	//!< 上下文2文件名
		int				m_context_line2;	//!< 上下文2行号
		int				m_timeout;			//!< 超时时间(毫秒)
	};

	inline void CheckRunTime::setContext1(const char *file,int line)
	{
		m_context_file1=file;
		m_context_line1=line;
	}

	inline void CheckRunTime::setContext2(const char *file,int line)
	{
		m_context_file2=file;
		m_context_line2=line;
	}

	inline void CheckRunTime::setTimeout(int timeout)
	{
		m_timeout = timeout;
	}
}


#if CHECK_RUNTIME_TIMEOUT > 0
#define CRT(x) Zoic::CheckRunTime _##x(#x,__FILE__,__LINE__);
#define CRT_TIMEOUT(x,t) Zoic::CheckRunTime _##x(#x,__FILE__,__LINE__,t);
#define CRT2(x,n) Zoic::CheckRunTime _##x(n,__FILE__,__LINE__);
#define CRT_INIT \
{ \
	CRT(init) \
	for(int i=0;i<10000;i++) \
	{ \
		CRT(_) \
	} \
	_init.checkTimeOut(0); \
}
#else
#define CRT(x)
#define CRT_TIMEOUT(x,t)
#define CRT2(x,n)
#define CRT_INIT
#endif

#endif
