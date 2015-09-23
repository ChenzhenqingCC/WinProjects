////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	publicfun   version:  1.0   ? date: 08/20/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 dasjack- All Rights Reserved
//	Written By Jack Liu
//	created:	21:9:2009   9:37
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "publicfun.h"
#define SECURITY_WIN32
#include "Security.h"
#include <ShlObj.h>
#include <lm.h>
#include <io.h>
#include <Tlhelp32.h>
#pragma comment(lib, "Secur32.lib")
HRESULT (STDAPICALLTYPE *lpDllEntryPoint)(void);
//////////////////////////////////////////////////////////////////////////
#define FAIL_ARGS   0
#define FAIL_OLE    -1
#define FAIL_LOAD   -2
#define FAIL_ENTRY  -3
#define FAIL_REG    -4
const char _szDllRegSvr[] = ("DllRegisterServer");
const char _szDllUnregSvr[] = ("DllUnregisterServer");
//=============================================================================
// 函数名称:	获取当前工作目录
// 作者说明:	mushuai
// 修改时间:	2010-03-08
//=============================================================================
CString GetLocalPath(HMODULE hm,const bool isname)
{
	CString m_path;
	TCHAR buffer[MAX_PATH]={0};
	if (::GetModuleFileName(hm,buffer,MAX_PATH))
	{
		m_path = buffer;
		int dist = m_path.ReverseFind(_T('\\'));
		if (dist > -1)
		{
			if (isname)
			{
				m_path.Delete(0,dist);
			}
			else
			{
				m_path.Delete(dist,m_path.GetLength()-dist);
			}
		}
	}
	return m_path;
}
//=============================================================================
// 函数名称:	设置当前工作目录
// 作者说明:	mushuai
// 修改时间:	2010-03-08
//=============================================================================
BOOL SetLocalCurrentDir()
{
	CString path=GetLocalPath();
	path+='\\';
	DWORD dwret=SetCurrentDirectory(path.GetBuffer(0));
	if (dwret==0)
		return FALSE;
	return TRUE;
}
//=============================================================================
//  函数说明:	判断文件是否存在
//  作者说明:	mushuai
//  修改说明:	2010-05-20
//=============================================================================
BOOL IsFileExists(LPCTSTR pFilePath)
{
	BOOL bRes=TRUE;
	if (GetFileAttributes(pFilePath)==-1)
	{
		bRes=FALSE;
	}
	return bRes;
}
//=============================================================================
// 函数名称:	读取配置文件
// 作者说明:	mushuai
// 修改时间:	2010-03-08
//=============================================================================
CString GetConfig(LPCTSTR path,LPCTSTR secname,LPCTSTR keyname,LPCTSTR default_value)
{
	TCHAR tmp_buffer[MAX_PATH]={0};
	GetPrivateProfileString(secname, keyname, default_value, tmp_buffer,MAX_PATH,path); 
	return tmp_buffer;
}
//=============================================================================
// 函数名称:	写入配置文件
// 作者说明:	mushuai
// 修改时间:	2010-03-08
//=============================================================================
BOOL  SetConfig(LPCTSTR path,LPCTSTR secname,LPCTSTR keyname,LPCTSTR value)
{
	return WritePrivateProfileString(secname, keyname, value,path); 
}
//=============================================================================
//  函数说明:	提升进程级别
//  函数作者:	mushuai
//  修改说明:	2010-03-08
//=============================================================================
BOOL SetPrivileges(VOID)
{
	HANDLE hProc;
	LUID luid;
	TOKEN_PRIVILEGES tp;
	HANDLE hToken;
	TOKEN_PRIVILEGES oldtp;
	DWORD dwSize;

	hProc = GetCurrentProcess();

	if (!OpenProcessToken(hProc, TOKEN_QUERY |
		TOKEN_ADJUST_PRIVILEGES, &hToken))
		return FALSE;

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
	{
		CloseHandle (hToken);
		return FALSE;
	}

	ZeroMemory (&tp, sizeof (tp));

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES),
		&oldtp, &dwSize))
	{
		CloseHandle(hToken);
		return FALSE;
	}

	return TRUE;
}
//=============================================================================
// 函数名称:	获得文件版本信息
// 作者说明:	mushuai
// 修改时间:	2010-03-08
//=============================================================================
CString GetFileVersion(LPCTSTR  FileName)  
{     
	int   iVerInfoSize;  
	char   *pBuf=0;  
	CString   asVer;  
	VS_FIXEDFILEINFO   *pVsInfo=0;  
	unsigned   int   iFileInfoSize   =   sizeof(VS_FIXEDFILEINFO);  
	iVerInfoSize   =   GetFileVersionInfoSize(FileName,NULL);   
	if(iVerInfoSize!= 0)  
	{     
		pBuf   =   new  char[iVerInfoSize];  
		if(GetFileVersionInfo(FileName,0,iVerInfoSize, pBuf))     
		{     
			if(VerQueryValue(pBuf,   _T("\\"),(void**)&pVsInfo,&iFileInfoSize))     
			{     
				asVer.Format(_T("%d.%d.%d.%d"),HIWORD(pVsInfo->dwFileVersionMS),
					LOWORD(pVsInfo->dwFileVersionMS),HIWORD(pVsInfo->dwFileVersionLS),
					LOWORD(pVsInfo->dwFileVersionLS));  
			}     
		}     
		delete   pBuf;     
	}     
	return   asVer;     
}
//=============================================================================
// 函数名称:	获得文件描述信息
// 作者说明:	mushuai
// 修改时间:	2010-03-08
//=============================================================================
CString GetFileDescription(LPCTSTR  FileName) 
{     
	return _T("");
}
//=============================================================================
//  函数说明:	根据进程ID结束进程
//  函数作者:	KernelSword
//  修改说明:	2007-08-05
//=============================================================================
void KillProcessById(ULONG ulProcessId)
{
	if (ulProcessId == GetCurrentProcessId()) {
		return;
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,ulProcessId+1);
	if (NULL != hProcess) {
		TerminateProcess(hProcess,0);
		CloseHandle(hProcess);
	}
	hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,ulProcessId);
	if (NULL != hProcess) {
		TerminateProcess(hProcess,0);
		CloseHandle(hProcess);
	}
}
//=============================================================================
//  函数说明:	根据进程名,结束进程
//  函数作者:	mushuai
//  修改说明:	2011-08-05
//=============================================================================
void KillProcessByName(LPCTSTR pname)
{
	PROCESSENTRY32 pe;
	DWORD id=0;
	HANDLE hkz = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	pe.dwSize=sizeof(PROCESSENTRY32);
	if (Process32First(hkz,&pe)) {
		do
		{
			if (_tcscmp(pe.szExeFile,pname)==0) 
			{
				id = pe.th32ProcessID+1;
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,id);
				if (NULL != hProcess) {
					TerminateProcess(hProcess,0);
					CloseHandle(hProcess);
				}
				hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pe.th32ProcessID);
				if (NULL != hProcess) {
					TerminateProcess(hProcess,0);
					CloseHandle(hProcess);
				}
			}
		}
		while(Process32Next(hkz,&pe));		
	}
	CloseHandle(hkz);
}
//=============================================================================
// 函数名称:	获得系统目录
// 作者说明:	mushuai
// 修改时间:	2010-03-08
//=============================================================================
/*fid:
CSIDL_BITBUCKET   回收站    
CSIDL_CONTROLS   控制面板    
CSIDL_DESKTOP   Windows   桌面Desktop    
CSIDL_DESKTOPDIRECTORY   Desktop的目录    
CSIDL_DRIVES   我的电脑    
CSIDL_FONTS   字体目录    
CSIDL_NETHOOD   网上邻居    
CSIDL_NETWORK   网上邻居虚拟目录    
CSIDL_PERSONAL   我的文档    
CSIDL_PRINTERS   打印机    
CSIDL_PROGRAMS   程序组    
CSIDL_RECENT   最近打开的文档    
CSIDL_SENDTO   “发送到”菜单项    
CSIDL_STARTMENU   任务条启动菜单项    
CSIDL_STARTUP   启动目录    
CSIDL_TEMPLATES   文档模板
*/
CString GetSysFolderPath(int fid)
{
	CString path;
	LPITEMIDLIST pidl=0;
	HRESULT hr = SHGetFolderLocation(NULL,fid,NULL,NULL,&pidl);
	if (pidl)
	{
		TCHAR fpath[MAX_PATH]={0};
		if(SHGetPathFromIDList(pidl,fpath))
		{
			TCHAR ffpath[MAX_PATH]={0};
			GetShortPathName(fpath,ffpath,MAX_PATH);
			path = ffpath;
		}
	}
	return path;
}
//=============================================================================
// 函数名称:	设置动态链接库版本
// 作者说明:	mushuai
// 修改时间:	2010-03-08
//=============================================================================
DWORD GetDllVersion(LPCTSTR dllpath)
{
	DWORD dversion=0;
	HINSTANCE hdll = LoadLibrary(dllpath);
	DLLGETVERSIONPROC pDllGetVersion =0;
	if (hdll)
	{
		pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hdll,"DllGetVersion");
	}
	if (pDllGetVersion)
	{
		DLLVERSIONINFO dvi={0};
		dvi.cbSize=sizeof(dvi);
		HRESULT hr = (*pDllGetVersion)(&dvi);
		if (SUCCEEDED(hr))
		{
			dversion = MAKELONG(dvi.dwMajorVersion,dvi.dwMinorVersion);
		}
	}
	FreeLibrary(hdll);
	return dversion;
}
//=============================================================================
// 函数名称:	获得系统版本
// 作者说明:	mushuai
// 修改时间:	2010-03-08
//=============================================================================
/*Shell32.dll 
Version Distribution Platform 
4.0 Windows 95 and Microsoft Windows NT 4.0 
4.71 Microsoft Internet Explorer 4.0. See note 1. 
4.72 Microsoft Internet Explorer 4.01 and Windows 98. See note 1. 
5.0 Windows 2000 and Windows Millennium Edition (Windows Me). See note 2. 
6.0 Windows XP 
6.0.1 Windows Vista 
6.1 Windows 7 

Shlwapi.dll 
Version Distribution Platform 
4.0 Windows 95 and Microsoft Windows NT 4.0 
4.71 Microsoft Internet Explorer 4.0. See note 1. 
4.72 Microsoft Internet Explorer 4.01 and Windows 98. See note 1. 
4.7 Internet Explorer 3.x 
5.0 Microsoft Internet Explorer 5 and Windows 98 SE. See note 2. 
5.5 Microsoft Internet Explorer 5.5 and Windows Millennium Edition (Windows Me) 
6.0 Windows XP and Windows Vista 
*/
DWORD GetSysVersionInfo(BOOL isie)
{
	if (isie)
	{
		CString shellPath = GetSysFolderPath(CSIDL_SYSTEM);
		shellPath +=_T("\\Shlwapi.dll");
		return GetDllVersion(shellPath);
	}
	else
	{
		return GetVersion();
	}
}
//=============================================================================
// 函数名称:	获得域用户
// 作者说明:	mushuai
// 修改时间:	2010-03-08
//=============================================================================
CString GetDomainUser()
{
	TCHAR buffer[MAX_PATH]={0};
	unsigned long len=MAX_PATH;
	GetUserNameEx(EXTENDED_NAME_FORMAT::NameSamCompatible,buffer,&len);
	return buffer;
}
//=============================================================================
// 函数名称:	获得短路径
// 作者说明:	mushuai
// 修改时间:	2010-03-08
//=============================================================================
CString GetShortPath(LPCTSTR path)
{
	CString m_path = path;
	long     length = 0;
	TCHAR*   buffer = NULL;
	length = GetShortPathName(m_path, NULL, 0);
	if (length == 0)
	{
		return m_path;
	}
	buffer = new TCHAR[length];
	length = GetShortPathName(m_path, buffer, length);
	if (length == 0)
	{
		delete [] buffer;
		return m_path;
	}
	m_path = buffer;
	delete [] buffer;
	return m_path;
}
//=============================================================================
// 函数名称:	注册DLL
// 作者说明:	mushuai
// 修改时间:	2010-03-08
//=============================================================================
int RegSvr(LPCTSTR pszDllName,BOOL install)
{
	int iReturn=1;
	LPCSTR pszDllEntryPoint = install ? _szDllRegSvr : _szDllUnregSvr;
	ATLASSERT(pszDllName);
	// Initialize OLE.
	if (FAILED(OleInitialize(NULL))) 
	{
		iReturn =  FAIL_OLE;
		OleUninitialize();
		return iReturn;
	}
	//
	SetErrorMode(SEM_FAILCRITICALERRORS);       // Make sure LoadLib fails.
	// Load the library.
	CString dllName = GetShortPath(pszDllName);
	HINSTANCE hLib = LoadLibraryEx(dllName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (hLib < (HINSTANCE)HINSTANCE_ERROR) 
	{
		iReturn = FAIL_LOAD;
		OleUninitialize();
		return iReturn;
	}
	// Find the entry point.
	(FARPROC&)lpDllEntryPoint = GetProcAddress(hLib, pszDllEntryPoint);
	if (lpDllEntryPoint == NULL) 
	{
		iReturn = FAIL_ENTRY;
		FreeLibrary(hLib);
		OleUninitialize();
		return iReturn;
	}
	// Call the entry point.
	HRESULT hr = (*lpDllEntryPoint)();
	if (FAILED(hr)) 
	{
		iReturn = FAIL_REG;
		FreeLibrary(hLib);
		OleUninitialize();
		return iReturn;
	}
	return iReturn;
}
//=============================================================================
// 函数名称:	判断控件是否已安装
// 作者说明:	mushuai
// 修改时间:	2010-03-08
//=============================================================================
int IsOcxInstalled(const CString& id)
{
	int res=0;
	CLSID clsid;
	//check id is CLSID
	HRESULT hr = CLSIDFromString(CComBSTR(id),&clsid);
	if (SUCCEEDED(hr))
	{
		LPOLESTR progid=0;
		hr = ProgIDFromCLSID(clsid,&progid);
		if (SUCCEEDED(hr))
		{
			res=1;
		}
		else
		{
			if (hr == REGDB_E_CLASSNOTREG)
			{
				res=0;
			}
			else
			{
				res=-1;
			}
		}
	}
	else
	{
		hr =  CLSIDFromProgID(CComBSTR(id),&clsid);
		if (SUCCEEDED(hr))
		{
			res=1;
		}
		else if(hr ==REGDB_E_CLASSNOTREG || hr==CO_E_CLASSSTRING)
		{
			res = 0;
		}
		else 
		{
			res = -1;
		}
	}
	return res;
}
//=============================================================================
// 函数名称:	移动覆盖一个指定的文件
// 作者说明:	mushuai
// 修改时间:	2013-03-14
//=============================================================================
int ConvertFile(LPCTSTR srcpath,LPCTSTR targpath)
{
	SHFILEOPSTRUCT fileop={0};
	memset(&fileop,sizeof(fileop),0);
	//////////////////////////////////////////////////////////////////////////
	CString from = srcpath;
	from+=(TCHAR)0;
	CString to = targpath;
	to +=(TCHAR)0;
	//////////////////////////////////////////////////////////////////////////
	fileop.fFlags = FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR|FOF_NOERRORUI|FOF_SILENT;
	fileop.hNameMappings = NULL;
	fileop.hwnd = NULL;
	fileop.lpszProgressTitle=NULL;
	fileop.pFrom = (LPCTSTR)from;
	fileop.pTo = (LPCTSTR)to;
	fileop.wFunc = FO_MOVE;
	//////////////////////////////////////////////////////////////////////////
	bool res = (SHFileOperation(&fileop) ==0);
	return res;
}
//=============================================================================
// 函数名称:	移动覆盖一个指定的目录
// 作者说明:	mushuai
// 修改时间:	2013-03-14
//=============================================================================
int ConvertPath(LPCTSTR srcpath,LPCTSTR targpath)
{
	int iresult = 1;
	CFindFile finder;
	if(finder.FindFile(srcpath))
	{
		CString fileName,filePath;
		do
		{
			fileName=finder.GetFileName();
			filePath = finder.GetFilePath();
			//. ..
			if (finder.IsDots())
			{
				continue;
			}
			//dir
			else if (finder.IsDirectory())
			{
				CString tTargPath = targpath;
				tTargPath +=_T("\\")+fileName;
				ConvertPath(filePath+_T("\\*"),tTargPath);
				RemoveDirectory(filePath);
			}
			else//file
			{
				CString newFilePath = targpath;
				newFilePath +=_T("\\")+fileName;					
				if (!PathFileExists(targpath))
				{
					if(ERROR_SUCCESS != SHCreateDirectoryEx(0,targpath,0))
					{
						return 0;
					}
				}
				BOOL res=MoveFileEx(filePath,newFilePath,MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING);
				if (!res)
				{
					SetFileAttributes(newFilePath,FILE_ATTRIBUTE_NORMAL);
					if (!DeleteFile(newFilePath))
					{
						MoveFileEx(filePath,newFilePath,MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING);
					}
				}
			}
		}while (finder.FindNextFile());
	}
	finder.Close();
	return iresult;
}