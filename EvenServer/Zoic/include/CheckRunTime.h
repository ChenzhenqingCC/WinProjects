/*! @file
	@ingroup utils
	@brief ������������ʱ����
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
		@brief ���������ʱ����
		@ingroup utils
	*/
	class CheckRunTime
	{
	public:
		//!���캯��
		CheckRunTime(
			const char *name,	//!< �������
			const char *file,	//!< ��������ļ���
			int line,			//!< ����к�
			int timeout = CHECK_RUNTIME_TIMEOUT	//!< ��ʱʱ��(����)
			);

		~CheckRunTime();

		//!����������1
		void setContext1(
			const char *file,	//!< �ļ���
			int line			//!< �к�
			);

		//!����������2
		void setContext2(
			const char *file,	//!< �ļ���
			int line			//!< �к�
			);

		//!���ó�ʱʱ��
		void setTimeout(
			int timeout			//!< ��ʱʱ��(����)
			);

		/*! @brief ��ⳬʱʱ��


			�����ǰʱ���뿪ʼ���ʱ�䳬��timeout,������־�ļ���д���¼
		*/
		void checkTimeOut(int timeout);
	private:
		//�������ʱ��
		TIME_MSEL getRunTime();

		TIME_MSEL	m_runtime;			//!< ��ʼ���ʱ��
		const char *	m_name;				//!< ����
		const char * 	m_file;				//!< �ļ���
		int				m_line;				//!< �к�
		const char *	m_context_file1;	//!< ������1�ļ���
		int				m_context_line1;	//!< ������1�к�
		const char *	m_context_file2;	//!< ������2�ļ���
		int				m_context_line2;	//!< ������2�к�
		int				m_timeout;			//!< ��ʱʱ��(����)
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
