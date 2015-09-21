////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	updator   version:  1.0   ? date: 04/24/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 - All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef UPDATOR_H
#define	UPDATOR_H
//////////////////////////////////////////////////////////////////////////
#include	 "processinfo.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "ftphelper.h"
#include "httphelper.h"
#include "vercompare.h"
#include "convert.h"
#include "config_xml.h"
//////////////////////////////////////////////////////////////////////////
class Updator_Config : public Config
{
public:
	Updator_Config():Config((LPCTSTR)(Path::GetAppPath()+_T("\\autoupdate.xml")))
	{
		Load();
	}
	//////////////////////////////////////////////////////////////////////////
	string GetUrl()
	{
		return xmlnode_get_tag_data(m_root,_T("url"));
	}
	//////////////////////////////////////////////////////////////////////////
	string GetPath()
	{
		return xmlnode_get_tag_data(m_root,_T("path"));
	}
	//////////////////////////////////////////////////////////////////////////
	string GetUserName()
	{
		return xmlnode_get_tag_data(m_root,_T("user"));
	}
	//////////////////////////////////////////////////////////////////////////
	string GetUserPwd()
	{
		return xmlnode_get_tag_data(m_root,_T("pwd"));
	}
	//////////////////////////////////////////////////////////////////////////
	string GetLocalPath()
	{
		return xmlnode_get_tag_data(m_root,_T("localpath"));
	}
	//////////////////////////////////////////////////////////////////////////
	string GetVersionName()
	{
		return xmlnode_get_tag_data(m_root,_T("version"));
	}
};
/************************************************************************/
/* update class
1 run
*/
/************************************************************************/

template<class Checker,class Loader,class Convertor,class Configor>
class Updator
{
public:
	enum{HTTP,FTP};
	
	enum{ERR_MOVE = -10,
				ERR_GETVER,
				ERR_NOUPDATE,
				ERR_DOWNLOAD};
	//////////////////////////////////////////////////////////////////////////
	Updator()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	~Updator(){}
	//////////////////////////////////////////////////////////////////////////
	int Run(UINT nettype = HTTP)
	{
		try
		{
			WTL::CString app_path = Path::GetAppPath();
			//CString update_path = app_path+_T("\\update");
			CString update_path = app_path+_T("\\")+m_config.GetLocalPath().c_str();
			//create new dir
			if (!PathFileExists(update_path))
			{
				SHCreateDirectoryEx(0,update_path,0);
			}
			//convert the last time file
			
			Convertor convert;
			//no pre convert
			//if(!convert.Convert(update_path+_T("\\*.*"),app_path))
			//{
			//	Report(_T("文件更新失败,请重试."));
			//}
			
			//get server config
			Loader loader(m_config.GetUrl(),m_config.GetUserName(),m_config.GetUserPwd(),m_config.GetPath());
			
			CString verpath = update_path+_T("\\")+m_config.GetVersionName().c_str();
			//here ftp and http is different,
			
			if(!loader.DownLoad(m_config.GetVersionName(),(LPCTSTR)verpath))
			{
				Report(_T("获取更新信息失败."));
				return ERR_GETVER;
			}
			
			//compare with local config
			CString ser_config = update_path+_T("\\")+m_config.GetVersionName().c_str();
			CString loc_config = app_path+_T("\\")+m_config.GetVersionName().c_str();
			
			Checker checker((LPCTSTR)ser_config,(LPCTSTR)loc_config);
			if (!checker.Checking())
			{
				return ERR_NOUPDATE;
			}

			list<string> upfiles = checker.GetUpdateFiles();
			if (upfiles.size())
			{
				//
				Report(_T("Total:"),upfiles.size(),ProcessInfo::TOTAL_STEPS);

				//download if local is overdue at the same time update the process info
				loader.DownLoad(upfiles,(LPCTSTR)update_path,m_rpt);
				//
				if (!convert.Convert(update_path+_T("\\*.*"),app_path))
				{
					return ERR_MOVE;
				}
			}
			else
			{
				//
				Report(_T("Total:0"));
			}
		}
		catch(exception &e)
		{
			Report(e.what());
			return -11;
		}
		catch (...)
		{
			return -11;
		}	
		return 0;
	}
	//////////////////////////////////////////////////////////////////////////
	void SetProcessListener(Reportor rpt)
	{
		m_rpt = rpt;
	}
	//////////////////////////////////////////////////////////////////////////
	void Report(LPCTSTR msg,UINT prs=0,UINT descr = 1)
	{
		if (m_rpt)
		{
			ProcessInfo pi;
			pi.filename= msg;
			pi.progress = prs;
			pi.descr = descr;
			m_rpt(pi);
		}
	}
protected:
	Configor			m_config;
	Reportor			m_rpt;
};
//////////////////////////////////////////////////////////////////////////////////////////////

typedef Updator<XML_Checker,Ftp_Loader,Copy_Convertor,Updator_Config>	InternetUpdator_Ftp;

typedef Updator<XML_Checker,Http_Loader,Copy_Convertor,Updator_Config>	InternetUpdator_Http;
#endif