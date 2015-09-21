////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	SampleTxtLogger   version:  1.0   ? date: 1/28/2010
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 dasjack- All Rights Reserved
//	Written By Jack Liu
//	created:	[1/28/2010]
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef SAMPLE_TXT_LOGGER_H
#define SAMPLE_TXT_LOGGER_H
#include <fstream>
using namespace std;
/****************************************************************************************************************************/
/* 
 *
 *
*/
/****************************************************************************************************************************/
class SampleTxtLogger
{
public:
	/**
	*
	*/
	SampleTxtLogger():m_state(false)
	{
		m_path = _T("log.txt");
	}
	/**
	*
	*/
	~SampleTxtLogger()
	{
		UnInitialize();
	}
	/**
	*init logger
	*/
	virtual int Initialize(LPCTSTR params)
	{
		m_path = params;
		m_file.open(m_path.c_str(),ios::app);
		if (m_file.is_open())
		{
			m_state = true;
		}
		return -1;
	}
	/**
	*uninit logger
	*/
	virtual int UnInitialize()
	{
		if (m_file.is_open())
		{
			m_file.flush();
			m_file.close();
		}
		return 0;
	}
	/**
	*log the debug messages,the function type is almost seam with sprintf
	*@level :trace,info,debug,error,as ace
	*@msg : format string
	*/
	virtual int Log(const int level,LPCTSTR format,...)
	{
		//if (level >=m_level)
		{
			va_list argptr;
			va_start(argptr, format);

			TCHAR buffer[2048]={0};
			_vstprintf_s(buffer,2047,format,argptr);
			m_file<<buffer<<_T("\n");
			
			va_end(argptr);
			
			m_file.flush();
		}
		return 0;
	}
protected:
	basic_fstream<TCHAR>			m_file;
	basic_string<TCHAR>			m_path;
public:
	bool											m_state;
};
#endif