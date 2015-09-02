////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	ftphelper   version:  1.0   ? date: 04/27/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 - All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef FTPHELPER_H
#define FTPHELPER_H
//////////////////////////////////////////////////////////////////////////
#include <atlmisc.h>
#include <WinInet.h>
#include <list>
using namespace std;
#include "path.h"
#include <ShlObj.h>
typedef boost::function<void(ProcessInfo &)> Reportor;
/************************************************************************/
/* loader implement  InternetAPI
*/
/************************************************************************/
struct Ftp_Loader 
{
	Ftp_Loader(const string& url,const string& username,const string& pwd,const string& workdir):
m_url(url),m_username(username),m_pwd(pwd),m_hinet(0),m_hftp(0),m_workdir(workdir)
{
	Init();
}

void Init()
{
	m_hinet = InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
	if (!m_hinet)
	{
		throw exception(_T("internet open error."));
	}

	m_hftp = InternetConnect(m_hinet,m_url.c_str(),INTERNET_DEFAULT_FTP_PORT,\
		m_username.c_str(),m_pwd.c_str(),INTERNET_SERVICE_FTP,0,NULL);
	
	if (!m_hftp )
	{
		throw exception(_T("ftp open error."));
	}
	//InternetSetOptionEx(m_hftp,,);
	if(m_workdir.length())
	{
		if(!FtpSetCurrentDirectory(m_hftp,m_workdir.c_str()))
		{
			throw exception(_T("set work dir error."));
		}
	}
}
~Ftp_Loader()
{
	if (m_hftp)
	{
		InternetCloseHandle(m_hftp);
		m_hftp = 0;
	}
	if (m_hinet)
	{
		InternetCloseHandle(m_hinet);
		m_hinet = 0;
	}
}
//////////////////////////////////////////////////////////////////////////
bool DownLoad(const string& srcfile,const string &targfile)
{
	if (m_hftp)
	{
		return FtpGetFile(m_hftp,srcfile.c_str(),targfile.c_str(),FALSE,
			FILE_ATTRIBUTE_NORMAL,FTP_TRANSFER_TYPE_BINARY,0);
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
void DownLoad(list<string> &files,const string& tagpath,Reportor rpt)
{
	ProcessInfo pi;

	list<string>::iterator iter = files.begin();
	while (iter != files.end())
	{
		string curpath = *iter;
		size_t pos = curpath.find_last_of(_T("\\"));

		if (pos != string::npos)
		{
			//create new dir
			string dir =tagpath;
			dir +=_T("\\");
			dir += curpath.substr(0,pos);
			if (!PathFileExists(dir.c_str()))
			{
				SHCreateDirectoryEx(0,dir.c_str(),0);
			}
		}
		string tagp = tagpath;
		tagp +=_T("\\");
		tagp += curpath;

		string srcpath= curpath;

		if(FtpGetFile(m_hftp,srcpath.c_str(),tagp.c_str(),FALSE,FILE_ATTRIBUTE_NORMAL\
			|FILE_ATTRIBUTE_DIRECTORY,FTP_TRANSFER_TYPE_BINARY,0))
		{
			pi.filename= iter->c_str();
		}
		else
		{
			pi.filename=iter->c_str();
			pi.filename +=_T("œ¬‘ÿ ß∞‹.");
		}

		pi.progress++;

		if(rpt) rpt(pi);

		iter++;
	}
}
protected:
	string				m_url;
	string				m_username;
	string				m_pwd;
	string				m_workdir;
	HINTERNET	m_hinet;
	HINTERNET	m_hftp;
};
#endif