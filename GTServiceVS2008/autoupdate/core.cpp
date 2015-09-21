////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	core   version:  1.0   ? date: 09/29/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 ztgame Corporation- All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "core.h"
#include "muleventmodule.h"
#include "charset_helper.h"
#include "utility/sampletxtlogger.h"


#ifdef DEBUG
#define LOG Log
#else
#define LOG //
#endif
/**
*
*/
CVersion::CVersion(const DWORD marj,const DWORD subj,
									const DWORD ssubj,const DWORD sssubj):\
									m_marj(marj),m_subj(subj),\
									m_ssubj(ssubj),m_sssubj(sssubj)
{
}
/**
*helper function to compare the version
*/
bool operator>(const CVersion&l,const CVersion& r)
{
		
	DWORD el = l.m_marj*1000+l.m_subj*100+l.m_ssubj*10+l.m_sssubj;
	DWORD er = r.m_marj*1000+r.m_subj*100+r.m_ssubj*10+r.m_sssubj;
	return el > er;
}
/**
*
*/
CVersion operator-(const CVersion&l,const CVersion& r)
{
	return CVersion(l.m_marj-r.m_marj,l.m_subj-r.m_subj,l.m_ssubj-r.m_ssubj,l.m_sssubj-r.m_sssubj);
}
/**
*
*/
bool operator==(const CVersion&l,const CVersion& r)
{
	return (l.m_marj == r.m_marj)&&(l.m_subj== r.m_subj)&&(l.m_ssubj==r.m_ssubj)&&(l.m_sssubj==r.m_sssubj);
}
/**
*
*/
//[50][50][50][50]
CVersion& operator++(CVersion&l)
{
	return l;
}
/**
*
*/
CVersionFromXML::CVersionFromXML(const tstring &path)
{
	TiXmlDocument ver;
	
	if(ver.LoadFile(path.c_str()))
	{
		TiXmlNode* root = ver.RootElement();
		TiXmlElement* pver_elem = root->ToElement();
		if(pver_elem)
		{
			//get ver id which > cur version
			tstring cur_ver_str = pver_elem->Attribute("cur");
			sscanf(cur_ver_str.c_str(),"%d.%d.%d.%d",&m_marj,&m_subj,&m_ssubj,&m_sssubj);
		}
	}
}
/**
*
*/
CVersionFromFile::CVersionFromFile(const tstring &rm_path,boost::shared_ptr<Loader>& m_loader)
{
	//read the head buffer to check the version id
	char ver_str[256]={0};
	int read_sz = m_loader->Read(rm_path,ver_str,255,0);

	//use atl reg to search version field
	if(read_sz >0)
	{
		tstring str_ver = ver_str;
		tstring tagname = "<versioninfo cur=\"";
		size_t pos = str_ver.find(tagname);
		
		if (pos != tstring::npos)
		{
			DWORD marj,subj,ssubj,sssubj;
			marj=subj=ssubj=sssubj=0;
			size_t end_pos = str_ver.find("\">",pos+1);
			if (end_pos != tstring::npos)
			{
				tstring ver = str_ver.substr(pos+tagname.length(),end_pos-pos-tagname.length());
				sscanf(ver.c_str(),"%d.%d.%d.%d",&m_marj,&m_subj,&m_ssubj,&m_sssubj);
			}
		}
	}
}
/**
*
*/
CVersionInfo::CVersionInfo(const CVersion &l,const CVersion &r)
{
	m_local = l;
	m_remote = r;
}
/**
*
*/
map<tstring,tstring>	 CVersionInfo::GetUpdateList(vector<tstring>& m_services)
{
	map<tstring,tstring> updatelist;
	//check the remote version file in update folder
	TiXmlDocument remote_ver;
	if(remote_ver.LoadFile(m_update_ver_path.c_str()))
	{
		TiXmlNode* root = remote_ver.RootElement();
		TiXmlElement* pver_elem = root->FirstChildElement();
		while(pver_elem)
		{
			//get ver id which > cur version
			tstring cur_ver_str = pver_elem->Attribute("no");
			DWORD marj,subj,ssubj,sssubj;
			marj=subj=ssubj=sssubj=0;
			sscanf(cur_ver_str.c_str(),"%d.%d.%d.%d",&marj,&subj,&ssubj,&sssubj);
			CVersion cur_ver(marj,subj,ssubj,sssubj);

			if(cur_ver > m_local)
			{
				TiXmlElement* pitem = pver_elem->FirstChildElement();
				while (pitem)
				{
					tstring filepath = pitem->Attribute("path");
					tstring filename = pitem->Attribute("name");
					//res.push_back(filepath);
					updatelist[filename] = filepath;
					const char* pservice = pitem->Attribute("service");
					if(pservice)
					{
						m_services.push_back(filename);
					}
					pitem = pitem->NextSiblingElement();
				}
				
			}
			pver_elem = pver_elem->NextSiblingElement();
		}
	}
	return updatelist;
}
/**
*
*/
DWORD CUpdator::AddListener(const Event& e)
{
	if (m_disp)
	{
		return m_disp->Register(e);
	}
	return 0;
}
/**
*
*/
DWORD CUpdator::RemoveListener(const Event& e)
{
	if (m_disp)
	{
		return m_disp->UnRegister(e);
	}
	return 0;
}
/**
*
*/
int CUpdator::Notify(const Event& e)
{
	if (m_disp)
	{
		return m_disp->Notify(e);
	}
	return -1;
}
/**
*
*/
CVersion CUpdator::GetLocalVersion()
{
	tstring targpath = m_config->GetTargetPath();
	//check the end of path for adding the filename in the end
	if (targpath.length()>0 && targpath[targpath.length()-1] !='\\')
	{
		targpath += _T("\\");
	}
	tstring local_ver_path =targpath+ m_config->GetLocalVerPath();
	CVersionFromXML localver(local_ver_path);
	return localver;
}
/**
*
*/
CVersion CUpdator::GetRemoteVersion()
{
	//get the remote version url to read
	tstring rm_path = m_config->GetRemoteVerPath();
	CVersionFromFile remotever(rm_path,m_loader);
	return remotever;
}
/**
*
*/
bool CUpdator::CheckVersion(const CVersion &local, const CVersion &remote)
{
	//check
	bool is_new = remote > local;
	if(is_new)
	{
		//should let user to confirm to do or cancel
		if(!m_confirmed)
		{
			//notify
			Event version_checked(E_CHECKED,(void*)this,0,is_new);
			Notify(version_checked);
			return false;
		}
		//new versioninfo 
		CVersionInfo *pvinfo = 0;
		pvinfo = new CVersionInfo(local,remote);
		m_pverinfo.reset(pvinfo);
		//get remote version url
		tstring rm_path = m_config->GetRemoteVerPath();
		//get local version temp path 
		tstring updatepath = m_config->GetLocalUpdatePath();
		//check the end of path for adding the filename in the end
		if (updatepath.length()>0 && updatepath[updatepath.length()-1] !='\\')
		{
			updatepath += _T("\\");
		}
		//download the new version file
		bool r = m_loader->DownLoad(rm_path,updatepath+rm_path);
		//notify l
		Event loading(E_LOAD_VER,(void*)this,0,r);
		Notify(loading);
		if(!r)
		{
			return false;
		}
		pvinfo->SetUpdateVerPath(updatepath+rm_path);
	}
	else
	{
		//notify
		DWORD noNeed = 2;
		Event version_checked(E_UPDATE_OVER,(void*)this,0,noNeed);
		Notify(version_checked);
	}
	return is_new;
}
/**
*
*/
bool CUpdator::DownLoad()
{
	//get local update path
	tstring targpath = m_config->GetLocalUpdatePath();
	//get update list
	map<tstring,tstring> updatelist = m_pverinfo->GetUpdateList(m_services);
	//notify l
	int usize = updatelist.size();
	Event need_update(E_TOTALFILES,(void*)this,0,usize);
	Notify(need_update);
	//
	bool res =  find_if(updatelist.begin(),updatelist.end(),boost::bind(&CUpdator::DownLoadFile,this,_1,targpath)) == updatelist.end();
	
	struct dinfo{
		char tarPath[256];
		bool result;
	};
	dinfo df={0};
	df.result = res;
	SafeStrCpy(df.tarPath,targpath.c_str());
	Event download_e(E_DOWNLOAD,0,(void*)&df,sizeof(df));
	Notify(download_e);
	return res;
}
/**
*
*/
bool CUpdator::DownLoadFile(const pair<tstring,tstring>& src,const tstring &targ)
{
	bool r = m_loader->DownLoad(src.second,targ+src.first);
	//notify l
	struct dfinfo{
		char src[256];
		char tar[256];
		bool result;
	};
	dfinfo df={0};
	SafeStrCpy(df.src, src.second.c_str());
	SafeStrCpy(df.tar, targ.c_str());
	df.result = r;

	Event loading(E_LOADING,0,(void*)&df,sizeof(df));
	Notify(loading);
	return !r;
}
/**
*
*/
int CUpdator::Update()
{
	//get local update path
	tstring updatepath = m_config->GetLocalUpdatePath();
	//just remove the files and folders in "update",so use like this 
	updatepath +=_T("\\*");
	//get target path to remove
	tstring targpath = m_config->GetTargetPath();
	//
	BOOL r=TRUE;
	if (m_callBackMoveFile)
	{
		r = Copy_Convertor::Convert(updatepath.c_str(),targpath.c_str(),m_callBackMoveFile);
	}
	else
	{
		r = Copy_Convertor::Convert(updatepath.c_str(),targpath.c_str());
	}

	CString msg;
	msg.Format(_T("update finish:%d"),r);
	LOG(msg);

	//notify l
	DWORD res = r;
	Event loading(E_UPDATE_OVER,(void*)this,0,res);
	Notify(loading);
	return r;
}
#include <TlHelp32.h>
/**
*
*/
int CUpdator::PreUpdate()
{
	vector<HANDLE> proccess;
	//find updatee process and kill it
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	PROCESSENTRY32* procinfo = new PROCESSENTRY32;
	procinfo->dwSize = sizeof(PROCESSENTRY32);
	int index =0;
	while(Process32Next(hSnap,procinfo) != FALSE)
	{

		if(m_updatee_path.find(procinfo->szExeFile) != -1)
		{
			
			HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, procinfo->th32ProcessID);
			if( hProcess)
			{
				proccess.push_back(hProcess);
				//TerminateProcess(hProcess,0);	
			}
			//break;
		}
	}
	vector<HANDLE>::iterator iter = proccess.begin();
	for (; iter!=proccess.end(); ++iter)
	{
		TerminateProcess(*iter, 0);
	}
	CloseHandle(hSnap);

	tstring targpath = m_config->GetTargetPath();
    string tempExe = targpath+string("\\autoupdate.tmp");//remove invalide autoupdate.exe.
	if (PathFileExists(tempExe.c_str()))
	{
		DeleteFile(tempExe.c_str());
	}
	//get local update path
	tstring updatepath = m_config->GetLocalUpdatePath();
	//just remove the files and folders in "update",so use like this 
	updatepath +=_T("\\*");
	//get target path to remove
	//
	BOOL r = Copy_Convertor::Convert(updatepath.c_str(),targpath.c_str(),0);
	
	return r;
}
/**
*
*/
int CUpdator::Finish()
{
	
	return m_config->Close();
}
/**
*
*/
int CUpdator::Log(LPCTSTR msg)
{
	static SampleTxtLogger slog;
	if (!slog.m_state)
	{
		CString spath = GetLocalPath()+_T("\\log.txt");
		slog.Initialize(spath);
	}
	slog.Log(0,_T("%s"),msg);
	return 0;
}
/**
*
*/
CUpdator::CUpdator(const tstring &config,const tstring &localbase)
{
	//no need user to confirm
	m_confirmed = true;
	m_configpath = config;
	m_localbase = localbase;
	//make path as short path to avoid the white char in string
	m_config.reset(new Config(m_localbase));
	m_disp.reset(new MultiDispatcher);
	m_updatee=0;
	m_needRestart=FALSE;
	m_callBackMoveFile=boost::bind(&CUpdator::NotifyMoveFile,this,_1,_2,_3);
}
/**
*
*/
int CUpdator::Init()
{
	//open config of location
	bool open_cfg = m_config->Open(m_configpath);
	Event open_config_error(E_CONFIG_LOAD,(void*)this,(void*)open_cfg,open_cfg);
	Notify(open_config_error);
	if(!open_cfg)
	{
		LOG(_T("autoupdate config open error"));
		DWORD res = 4;
		Event loading(E_UPDATE_OVER,(void*)this,0,res);
		Notify(loading);
		return false;
	}
	//
	m_updatee_path = m_config->GetUpdateePath();
	//check and new the loader
	if (!m_loader)
	{
		tstring base_url = m_config->GetBaseUrl();
		tstring user = m_config->GetUser();
		tstring pwd = m_config->GetPwd();
		m_loader.reset(new Http_Loader(base_url,user,pwd));
	}
	return true;
}
/**
*
*/
int CUpdator::Check()
{
	//get local version
	CVersion local = GetLocalVersion();
	//get remote version
	CVersion remote=GetRemoteVersion();
	//remote is empty,so return
	if (remote ==CVersion())
	{
		LOG(_T("remote version error."));
		//notify l
		DWORD res = 4;
		Event loading(E_UPDATE_OVER,(void*)this,0,res);
		Notify(loading);
		return false;
	}
	//check version
	bool need_update = CheckVersion(local,remote);
	return need_update;
}

int CUpdator::RunUpdate()
{
	try
	{
		if(!Init())
		{
			LOG(_T("updator init error"));
			return -1;
		}

		PreUpdate();

		if (!Check())
		{
			LOG(_T("version is the last one,no need update"));
			return -1;
		}

		if(!DownLoad())
		{
			LOG(_T("download files error"));
			return -1;
		}

		if(!Update())
		{
			LOG(_T("update local files error"));
			//NeedRestart(TRUE);
			return -2;
		}

		if(!Finish())
		{
			LOG(_T("updator close error"));
			return -1;
		}
	}
	catch(CString &e)
	{
		LOG((LPCTSTR)e);
	}
	catch (...)
	{
	}
	return 0;
}

void CUpdator::UpdateRestart()
{
	if(m_updatee)
	{
		//waiting updatee quit
		CString msg;
		msg.Format(_T("wait updatee quit:%d"),m_updatee);
		LOG((LPCTSTR)msg);

		if(::WaitForSingleObject(m_updatee,INFINITE) != WAIT_OBJECT_0)
		{
			//msg.Format(_T("wait updatee error"));
			//MessageBox(0,(LPCTSTR)msg,0,MB_OK);
		}
		CloseHandle(m_updatee);
		m_updatee =0;
		//re update
		Sleep(20);
		int  r = PreUpdate();
	}
	LOG(_T("re start updatee"));

	for (int i=0;i<m_services.size();i++)
	{
		CString service = GetLocalPath()+m_services[i].c_str();
		Execute(service,"/service");
	}
	Sleep(200);
	if(m_updatee_path.length())
	{
		Execute(GetLocalPath()+m_updatee_path.c_str(),"/service");
		Sleep(200);
		Execute(GetLocalPath()+m_updatee_path.c_str());
	}
	
}