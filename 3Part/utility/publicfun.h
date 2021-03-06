/************************************************************************
* THIS is DasJack DirectUI lib for WTL
*
*
//	Copyright (C) 2009 dasjack- All Rights Reserved
//	Written By Jack Liu
//	created:	18:9:2009   16:17
/************************************************************************/
#ifndef PUBLICFUN_H
#define PUBLICFUN_H
#define SECURITY_WIN32
//////////////////////////////////////////////////////////////////////////
CString GetLocalPath(HMODULE hm=0,const bool isname=false);
//////////////////////////////////////////////////////////////////////////
CString GetConfig(LPCTSTR path,LPCTSTR secname,LPCTSTR keyname,LPCTSTR default_value=0);
//////////////////////////////////////////////////////////////////////////
BOOL IsFileExists(LPCTSTR pFilePath);
//////////////////////////////////////////////////////////////////////////
BOOL  SetConfig(LPCTSTR path,LPCTSTR secname,LPCTSTR keyname,LPCTSTR value);
//////////////////////////////////////////////////////////////////////////
BOOL SetLocalCurrentDir();
//////////////////////////////////////////////////////////////////////////
BOOL SetPrivileges(VOID);
//////////////////////////////////////////////////////////////////////////
CString GetFileVersion(LPCTSTR  FileName) ;
//////////////////////////////////////////////////////////////////////////
CString GetFileDescription(const char*  FileName) ;
//////////////////////////////////////////////////////////////////////////
void KillProcessById(ULONG ulProcessId);
//////////////////////////////////////////////////////////////////////////
void KillProcessByName(LPCTSTR pname);
//////////////////////////////////////////////////////////////////////////
CString GetSysFolderPath(int fid);
//////////////////////////////////////////////////////////////////////////
DWORD GetDllVersion(LPCTSTR dllpath);
//////////////////////////////////////////////////////////////////////////
DWORD GetSysVersionInfo(BOOL isie);
//////////////////////////////////////////////////////////////////////////
CString GetDomainUser();
//////////////////////////////////////////////////////////////////////////
CString GetShortPath(LPCTSTR path);
//////////////////////////////////////////////////////////////////////////
int RegSvr(LPCTSTR pszDllName,BOOL install);
//////////////////////////////////////////////////////////////////////////
int IsOcxInstalled(const CString& id);
//////////////////////////////////////////////////////////////////////////
int ConvertFile(LPCTSTR srcpath,LPCTSTR targpath);
//////////////////////////////////////////////////////////////////////////
int ConvertPath(LPCTSTR srcpath,LPCTSTR targpath);
#endif