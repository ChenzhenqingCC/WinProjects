////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	httphelper   version:  1.0   ? date: 04/30/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 - All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef HTTPHELPER_H
#define	HTTPHELPER_H
//////////////////////////////////////////////////////////////////////////
#include "loader.h"
/************************************************************************/
/* loader implement  InternetAPI
*/
/************************************************************************/
struct Http_Loader : public Loader
{
	Http_Loader(const string& url,const string& username,const string& pwd);

	void Init();

	~Http_Loader();
	
	virtual bool DownLoad(const string& srcfile,const string &targfile);

	virtual int Read(const string& srcfile,char *buf,const DWORD len,const DWORD start);

	virtual int Write(const string& srcfile,const char*buf,const DWORD len);

protected:
	bool OpenUrl(const string& srcfile);

	void CloseUrl();

	FILE* CreateFile(const string &targfile);

protected:
	string				m_base;
	string				m_username;
	string				m_pwd;
	LPVOID			m_hinet;
	LPVOID			m_http;
};
#endif