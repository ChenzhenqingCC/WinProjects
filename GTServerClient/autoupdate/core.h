////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	core   version:  1.0   ? date: 09/29/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 ztgame Corporation- All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef UPDATOR_CORE_H
#define UPDATOR_CORE_H
//////////////////////////////////////////////////////////////////////////
#include "httphelper.h"//loader interface
#include "eventmodule.h"//dispatch interface
#include "config_xml.h"//config
#include "convert.h"//convert impl
#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>
#include "utility/utility.h"
/************************************************************************/
/* 
//CVersion
*/
/************************************************************************/
class CVersion{
public:
	CVersion(const DWORD marj=0,const DWORD subj=0,const DWORD ssubj=0,const DWORD sssubj=0);
	  //helper function to compare the version
	  friend bool operator>(const CVersion&l,const CVersion& r);
	  //
	  friend CVersion operator-(const CVersion&l,const CVersion& r);
	  //
	  friend bool operator==(const CVersion&l,const CVersion& r);
	  //[50][50][50][50]
	  friend CVersion& operator++(CVersion&l);
protected:
	DWORD m_marj;
	DWORD m_subj;
	DWORD m_ssubj;
	DWORD m_sssubj;
};
/**
*local
*/
class CVersionFromXML : public CVersion{
public:
	CVersionFromXML(const tstring &path);
};
typedef CVersionFromXML CLocalVersion;
/**
*remote
*/
class CVersionFromFile : public CVersion{
public:
	CVersionFromFile(const tstring &rm_path,boost::shared_ptr<Loader>& m_loader);
};
typedef CVersionFromFile CRemoteVersion;
/************************************************************************/
/* Version information
*/
/************************************************************************/
class CVersionInfo{
public:
	CVersionInfo(const CVersion&,const CVersion&);
	/**
	*set paths
	*/
	void SetUpdateVerPath(const tstring &uvpath)
	{
		m_update_ver_path = uvpath;
	}
	/**
	*return the list of files needed to update
	*/
	map<tstring,tstring>	 GetUpdateList(vector<tstring>&);
protected:
	CVersion				m_local;
	CVersion				m_remote;
	tstring					m_update_ver_path;
	vector<tstring>	m_updatelist;
	
};
/************************************************************************/
/* 
//CUpdator class 
//control the version update 
*/
/************************************************************************/
class CUpdator {
public:
	//event type defines
	enum{
				E_CONFIG_LOAD,
				E_CHECKED,
				E_DOWNLOAD,
				E_LOADING,
				E_UPDATE_OVER,
				E_LOAD_VER,
				E_TOTALFILES,
				E_SEND_WND,
				E_MOVED	
	};
public:
	/**
	*
	*/
	CUpdator(const tstring &config,const tstring &localbase);
	/**
	*strategy method for dispatcher
	*/
	DWORD AddListener(const Event&);
	/**
	*strategy method for dispatcher
	*/
	DWORD RemoveListener(const Event&);
	/**
	*strategy method for dispatcher
	*/
	int Notify(const Event&);
	/**
	*main method,start all the works in flow,catch all the exception happen in
	*sub works,in the flow,need to communicate with host app 
	*/
	int RunUpdate();
	/**
	*
	*/
	static void Execute(LPCTSTR path,LPCTSTR param=0)
	{
		//CString path = ::GetLocalPath();
		//path +=updatee;
		SHELLEXECUTEINFO sei={0};
		sei.cbSize = sizeof(SHELLEXECUTEINFO);
		//sei.hwnd = notifyWnd;
		sei.nShow = SW_SHOWNORMAL;
		sei.lpFile = (LPCTSTR)path;
		sei.lpParameters=param;
		sei.fMask = SEE_MASK_FLAG_NO_UI;
		ShellExecuteEx(&sei);
	}
	/**
	*
	*/
	void NeedRestart(BOOL nr)
	{
		m_needRestart = nr;
	}
	/**
	*
	*/
	BOOL IsNeedRestart()
	{
		return m_needRestart && (m_updatee !=0);
	}
	/**
	*
	*/
	void UpdateRestart();
public:
	Copy_Convertor::MoveFileCallback m_callBackMoveFile;
	/**
	*
	*/
	void NotifyMoveFile(LPCTSTR src,LPCTSTR tar,const BOOL res)
	{
		struct package{
			TCHAR src[MAX_PATH];
			TCHAR tar[MAX_PATH];
			BOOL	result;
		};
		package p={0};
		SafeStrCpy(p.src,src);
		SafeStrCpy(p.tar,tar);
		p.result = res;

		Event file_moved(E_MOVED,0,&p,sizeof(p));
		Notify(file_moved);
	}
protected:
	//
	int Init();
	//
	int Check();
	//get remote version
	CVersion GetRemoteVersion();
	//get local version
	CVersion GetLocalVersion();
	//check versions ,got a versioninfo obj which contains the updatelist
	bool CheckVersion(const CVersion&,const CVersion&);
	//download files
	bool DownLoad();
	//convert local file using the download files
	int Update();
	//
	int PreUpdate();
	//do the end job
	int Finish();
	//
	bool DownLoadFile(const pair<tstring,tstring>& src,const tstring &targpath);
protected:
	boost::shared_ptr<Dispatcher>		m_disp;
	boost::shared_ptr<CVersionInfo>	m_pverinfo;
	boost::shared_ptr<Config>				m_config;
	boost::shared_ptr<Loader>				m_loader;
	tstring													m_configpath;
	tstring													m_localbase;
	tstring													m_updatee_path;
	BOOL														m_needRestart;
	vector<tstring> m_services;
public:
	bool														m_confirmed;
	HANDLE													m_updatee;
	//write log
	int Log(LPCTSTR);
};
#endif