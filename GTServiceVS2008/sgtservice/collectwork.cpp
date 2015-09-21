////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	workitem   version:  1.0   ? date: 1/15/2013
//  -------------------------------------------------------------
// 
//
//
//  -------------------------------------------------------------
//	Copyright (C) 2013 dasjack- All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <atltime.h>
#include "utility.h"
//#include <utility/charset_helper.h>
#include <utility/stringfun.h>
#include "collectwork.h"
#include "sgtservice.h"

CCollectWork::CCollectWork():CWorkItem(_T("collect"),1)
{
	//m_psinterval=m_timed= 1200000;
	m_psinterval=m_timed= 6000;
	m_games[_T("zhengtu.dav")]=_T("zhengtu.dat");
	m_games[_T("zhengtu.dat")]=_T("zhengtu.dat");
	m_games[_T("zhengtu_mini.dav")]=_T("zhengtu.dat");
	m_games[_T("zhengtu_mini.dat")]=_T("zhengtu.dat");

	m_games[_T("xzhengtu.dav")]=_T("xzhengtu.dat");
	m_games[_T("xzhengtu.dat")]=_T("xzhengtu.dat");
	m_games[_T("xzhengtu_mini.dav")]=_T("xzhengtu.dat");
	m_games[_T("xzhengtu_mini.dat")]=_T("xzhengtu.dat");

	m_games[_T("xxsj.exe")]=_T("xxsj.exe");
	m_games[_T("zhengtu2.dat")]=_T("zhengtu2.dat");
	m_games[_T("xtgame.dat")]=_T("xtgame.dat");
	m_games[_T("kok3.exe")]=_T("kok3.exe");
	m_games[_T("hugemanclient.exe")]=_T("hugemanclient.exe");
	m_games[_T("aogame.exe")]=_T("aogame.exe");
	m_games[_T("x2.exe")]=_T("x2.exe");
	m_games[_T("jianghu.dat")]=_T("jianghu.dat");
	m_games[_T("jianghu.dav")]=_T("jianghu.dat");
	//make guid,first check cfg got or not
	m_gamealases = m_games;
	m_deviceID = GetDeviceID();
	
}

CCollectWork::~CCollectWork()
{
}
CString CCollectWork::GetDeviceID()
{
	//先读注册表，注册表没有在读本地
	CString strid(_T(""));
	strid =ReadReg(_T("id"));
	if(!strid.IsEmpty())
	{
		return strid;
	}

	//读本地记录
	TCHAR szPath[MAX_PATH] = { 0 };
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, FALSE);
	CString dir,idFilePath;
	dir.Append(szPath);
	dir.Append(_T("\\gtcenter"));
	idFilePath.Append(dir);
	idFilePath.Append(_T("\\clientId.db"));

	wifstream file(idFilePath);	
	wstring ids;
	if(file)
	{
		file>>ids;
		strid = ids.c_str();
	}
	file.close();
	if (!strid.IsEmpty())
	{
		return strid;
	}

	//@2 gen an uuid for this machine
	//先写注册表，写入失败再选择写入本地
	CString sid;
	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid))   
	{   
		TCHAR* StringUuid=NULL;
		if (RPC_S_OK==UuidToString(&guid,(RPC_WSTR *)&StringUuid))
		{
			sid = StringUuid;
			if (!WriteReg(_T("id"),sid))
			{
				//写本地记录
				if (!PathFileExists(dir))
				{
					SHCreateDirectoryEx(0,dir,0);
				}
				wofstream ofile(idFilePath);
				if(ofile)
				{
					ofile<<sid.GetString();
				}
				else
				{
					CString msg(_T(""));
					msg.Format(_T("%s,%s"), _T("File_Open"), _T("CreateFailed"));
					SendErrorUrl(_T("guid"),msg);
				}
				ofile.close();
			}
		}
		RpcStringFree((RPC_WSTR *)&StringUuid);
	} 
	else
	{
		CString msg(_T(""));
		msg.Format(_T("%s,%d"), _T("CoCreateGuid"), GetLastError());
		SendErrorUrl(_T("guid"),msg);
	}

	return sid;
}

CString CCollectWork::NormalName(const CString& name)
{
	return m_gamealases[name];
}
/**
*get player name from zt2 config.ini
*/
CString CCollectWork::GetGtUserName()
{
	return _T("nobody");
}
/**
// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1
*/
CString CCollectWork::GetProcName(const DWORD processID,LPTSTR ppath)
{
	TCHAR szProcessName[MAX_PATH] = TEXT("");
	// Get a handle to the process.
	HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID );
	// Get the process name.
	if (NULL != hProcess )
	{
		HMODULE hMod;
		DWORD cbNeeded;
		if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), 
			&cbNeeded) )
		{
			GetModuleBaseName( hProcess, hMod, szProcessName, 
				sizeof(szProcessName)/sizeof(TCHAR) );
			if(ppath)
			{
				GetModuleFileNameEx(hProcess,hMod,ppath,MAX_PATH);
			}

		}
		else
		{
			CString msg(_T(""));
			msg.Format(_T("%s,%d"), _T("EnumProcessModules"), GetLastError());
			SendErrorUrl(_T("proclist"),msg);
		}
	}
	else
	{
		CString msg(_T(""));
		msg.Format(_T("%s,%d"), _T("OpenProcess"), GetLastError());
		SendErrorUrl(_T("proclist"),msg);
	}
	//m_papp->LogEvent(_T("CCollectWork:: pname:%s"),szProcessName);
	// Release the handle to the process.
	CloseHandle( hProcess );
	return szProcessName;
}
#include <TlHelp32.h>
/**
*
*/
map<CString,CString> CCollectWork::GenCurProcessList()
{
	map<CString,CString> processes;
	HANDLE hSnap = NULL; 
	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (NULL == hSnap)
	{
		return processes;
	}
	PROCESSENTRY32 procinfo = {0};
	procinfo.dwSize = sizeof(PROCESSENTRY32);
	int index =0;
	while(Process32Next(hSnap,&procinfo) != FALSE)
	{
		processes[CString(procinfo.szExeFile).MakeLower().Trim()]=_T("");
	}
	CloseHandle(hSnap);

	return processes;
}
/**
*
*/
BOOL CCollectWork::AnyliseProccessList()
{
	BOOL canBack = TRUE;
	CTime curtime = CTime::GetCurrentTime();
	map<CString,CString> curproclist =GenCurProcessList();
	map<CString,CString>::iterator iter = m_games.begin();

	while(iter != m_games.end())
	{
		CString pname = iter->first;
		//m_papp->LogEvent(_T("search proc:%s"),pname);
		if(curproclist.find(iter->first) != curproclist.end())
		{
			m_papp->LogEvent(_T(" game start:%s"),pname);

			vector<pair<DWORD,DWORD> >& hist = m_history[pname];
			m_games[pname]=curproclist[pname];
			//no times,add one first
			if(hist.size()==0)
			{
				hist.push_back(make_pair(curtime.GetTime(),0));
			}
			else
			{
				//get the last one
				pair<DWORD,DWORD>& times = hist[hist.size()-1];
				if(times.second !=0)//this is the finished one,so add new one
				{
					hist.push_back(make_pair(curtime.GetTime(),0));
				}

			}
			canBack = FALSE;
		}
		else
		{
			if(m_history.find(pname) !=m_history.end())
			{
				m_papp->LogEvent(_T(" game quit:%s"),pname);
				vector<pair<DWORD,DWORD> >& hist = m_history[pname];
				if(hist.size())
				{
					//get the last one
					pair<DWORD,DWORD>& times = hist[hist.size()-1];
					if(times.second ==0)
					{
						times.second = curtime.GetTime();
					}
				}
			}

		}
		iter++;
	}
	return canBack;
}
/**
*
*/
void CCollectWork::Run(const DWORD interval,const DWORD timed)
{
	BOOL canback = AnyliseProccessList();
	m_timed -= interval;
	if(m_timed <=0)//sent heart beat
	{
		m_timed = m_psinterval;
		CString res = HttpSendHb();
		ParseResponse(res);
	}

	m_papp->LogEvent(_T("ready to send canback=%d,history.size=%d,hearbeat time=%d"),canback,m_history.size(),m_timed);
	if(!canback || (m_history.size()==0))
	{
		return;
	}

	//
	CString parsed=_T("data=[");
	map<CString,vector< pair<DWORD,DWORD> > >::iterator iter = m_history.begin();

	while(iter != m_history.end())
	{
		//
		CString game=_T("{");
		game +=_T("\"name\":\"")+ NormalName(iter->first)+_T("\",");
		//game += _T("\"path\":\"")+m_games[iter->first]+_T("\",");
		game +=_T("\"hist\":[");
		vector<pair<DWORD,DWORD> >& hist = iter->second;
		for(int i=0;i<hist.size();i++)
		{
			game +=_T("[");
			pair<DWORD,DWORD>& times = hist[i];
			//game +=times.first+_T(",")+times.second;
			CString tmp;
			tmp.Format(_T("%d,%d"),times.first,times.second);
			game +=tmp+_T("],");
		}
		if (game[game.GetLength()-1]==TCHAR(','))
		{
			game.Delete(game.GetLength()-1);
		}
		game +=_T("]");
		game +=_T("},");

		parsed += game;
		iter++;
	}
	if (parsed[parsed.GetLength()-1]==TCHAR(','))
	{
		parsed.Delete(parsed.GetLength()-1);
	}
	parsed +=_T("]&");

	CString res = HttpSendLog(parsed);
	if(res.GetLength()==0)//error
	{
		return;//not clear history
	}
	//
	ParseResponse(res);
	res = HttpSendHb();
	ParseResponse(res);

	m_history.clear();
}
/**
*
*/
void CCollectWork::ParseResponse(const CString& res)
{
	CString cfgpath = GetPath()+_T("\\cfg.ini");
	//parse cmd line
	map<CString,CString> cmds = split_to_map(res,_T("&"),_T("="));
	

#ifdef DEBUG
	//cmds[_T("notify")] = _T("1");
	//cmds[_T("width")] = _T("500");
	//cmds[_T("height")] = _T("500");
	//cmds[_T("popurl")] = _T("http://zt2.ztgame.com/activity/tips/tips.html");
	//cmds[_T("popurl")] = _T("http://act.dz.ztgame.com/tips/tips.html");
	
#endif
	//config pop panel position
	if(cmds.find(_T("wndpos")) != cmds.end())
	{
		vector<CString> pos = split_to_array(cmds[_T("wndpos")],_T(","));
		if(pos.size()==2)
		{
			int x = 0;
			bool res1 = is_numeric(pos[0],&x);

			int y = 0;
			bool res2 = is_numeric(pos[1],&y);
			if(res1&&res2)
			{
				SetConfig(cfgpath,_T("poppanel"),_T("x"),pos[0]);
				SetConfig(cfgpath,_T("poppanel"),_T("y"),pos[1]);
			}
		}
	}
		//should change to process name to watch name
	if(cmds.find(_T("games")) != cmds.end())
	{
		CString glist = cmds[_T("games")];

		vector<CString> games = split_to_array(glist,_T(","));

		m_games.clear();
		for (int i=0;i<games.size();i++)
		{
			m_games[games[i].MakeLower().Trim()]=_T("");
		}
	}
	if(cmds.find(_T("notify")) != cmds.end())
	{
		CString cmdLine=cmds[_T("width")] + " ";
		cmdLine += cmds[_T("height")] + " ";
		cmdLine += cmds[_T("popurl")];
		m_papp->NotifyCenter(cmdLine);
	}
	//make this case be latest
	if(cmds.find(_T("stop")) != cmds.end())
	{
		m_papp->Stop();
	}
	//强制更新程序
	//if(cmds.find(_T("forceupdate")) != cmds.end())
	//{
	//	m_papp->ForceUpdate();
	//}
	/////////////////////////////////////////////////////////////////////////////
	///////////////////////以下暂时未用到////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	//download bk image
	if(cmds.find(_T("backgrd")) != cmds.end())
	{
		CString url = cmds[_T("backgrd")];
		if(m_ajax.DownLoad(url,GetPath()+_T("\\res\\")))
		{
			CString name = url;
			int index = name.ReverseFind(_T('/'));
			if(index != -1)
			{
				name.Delete(0,index+1);
				SetConfig(cfgpath,_T("resource"),_T("1"),_T("\\res\\")+name);
			}
		}
	}

	if(cmds.find(_T("psinterval")) != cmds.end())
	{
		is_numeric(cmds[_T("psinterval")],&m_psinterval);
		SetConfig(cfgpath,_T("app"),_T("psinterval"),cmds[_T("psinterval")]);
	}

	if(cmds.find(_T("checkinterval")) != cmds.end())
	{
		DWORD checkinterval=0;
		if(is_numeric(cmds[_T("checkinterval")],&checkinterval))
		{
			m_papp->UpdateInterval(checkinterval);
			SetConfig(cfgpath,_T("app"),_T("checkinterval"),cmds[_T("checkinterval")]);
		}
	}
	if(cmds.find(_T("idwithguid")) != cmds.end())
	{

	}
	if(cmds.find(_T("bkimage")) != cmds.end())
	{
		DelConfig(cfgpath,_T("poppanel"),_T("bkimage"));
	}
	if(cmds.find(_T("clsbtimage")) != cmds.end())
	{
		DelConfig(cfgpath,_T("poppanel"),_T("clsbtimage"));
	}
	if(cmds.find(_T("clsbtimage2")) != cmds.end())
	{
		DelConfig(cfgpath,_T("poppanel"),_T("clsbtimage2"));
	}
	if(cmds.find(_T("minbtimage")) != cmds.end())
	{
		DelConfig(cfgpath,_T("poppanel"),_T("minbtimage"));
	}
	if(cmds.find(_T("minbtimage2")) != cmds.end())
	{
		DelConfig(cfgpath,_T("poppanel"),_T("minbtimage2"));
	}
	if(cmds.find(_T("headheight")) != cmds.end())
	{
		SetConfig(cfgpath,_T("poppanel"),_T("headheight"),cmds[_T("headheight")]);
	}
	if(cmds.find(_T("footheight")) != cmds.end())
	{
		SetConfig(cfgpath,_T("poppanel"),_T("footheight"),cmds[_T("footheight")]);
	}
	if(cmds.find(_T("innerborder")) != cmds.end())
	{
		SetConfig(cfgpath,_T("poppanel"),_T("innerborder"),cmds[_T("innerborder")]);
	}
	if(cmds.find(_T("outborder")) != cmds.end())
	{
		SetConfig(cfgpath,_T("poppanel"),_T("outborder"),cmds[_T("outborder")]);
	}
	if (cmds.find(_T("protect")) != cmds.end())
	{
		WriteReg(_T("protect"), cmds[_T("protect")]);
	}
}
/**
*
*/
CString CCollectWork::HttpSendLog(CString& data)
{
	CString url= g_baseUrl +CString(_T("/log"));
	url +=_T("?");
	url +=_T("id=")+m_deviceID+_T("&");
	//url +=_T("&oid=")+m_oldDeviceID+_T("&");
	url +=_T("ver=")+m_papp->GetVersion()+_T("&");
	CTime time = CTime::GetCurrentTime();
	CString strt;
	strt.Format(_T("t=%d&"),time.GetTime());
	url +=strt;
	//encode data to utf-8 
	url +=data;
	m_papp->LogEvent(url);

	CString res = m_ajax.Request(url);
	m_papp->LogEvent(_T("rback:%s"),res);
	return res;
}
CString CCollectWork::HttpSendHb()
{
	CString url=  g_baseUrl + CString(_T("/hb"));
	url +=_T("?");
	url +=_T("id=")+m_deviceID;
	//url +=_T("&oid=")+m_oldDeviceID;
	url +=_T("&ver=")+m_papp->GetVersion();
	CTime time = CTime::GetCurrentTime();
	CString strt;
	strt.Format(_T("&t=%d"),time.GetTime());
	url +=strt;
	m_papp->LogEvent(url);
	CString res = m_ajax.Request(url);             
	m_papp->LogEvent(_T("next heartbeat timed=%d,res=%s"),m_timed,res);
	return res;
}
void CCollectWork::KillProcess(CString processName)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	PROCESSENTRY32 procinfo = {0};
	procinfo.dwSize = sizeof(PROCESSENTRY32);
	while(Process32Next(hSnap,&procinfo) != FALSE)
	{

		if(processName.Compare(procinfo.szExeFile) == 0)
		{

			HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, procinfo.th32ProcessID);
			if( hProcess)
			{
				TerminateProcess(hProcess,0);
			}
		}
	}
	CloseHandle(hSnap);
}