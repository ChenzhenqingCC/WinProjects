////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	convert   version:  1.0   ? date: 04/27/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 - All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef CONVERT_H
#define CONVERT_H
//////////////////////////////////////////////////////////////////////////
#include<ShellAPI.h>
#include <ShlObj.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "utility/utility.h"
/************************************************************************/
/* convert by CopyFile
*/
/************************************************************************/
struct Copy_Convertor 
{
	/**
	*copy src folder files to targpath
	*/
	static bool Convert(LPCTSTR srcpath,LPCTSTR targpath)
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
	/**
	*walk through dir
	*/
	//typedef void (*lpfnCallback)(LPCTSTR srcfile,LPCTSTR targfile,const BOOL result);
	typedef boost::function<void(LPCTSTR srcfile,LPCTSTR targfile,const BOOL result)> MoveFileCallback;
	static BOOL Convert(LPCTSTR srcpath,LPCTSTR targpath,MoveFileCallback lpfnCallback)
	{
		BOOL result = TRUE;
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
					Convert(filePath+_T("\\*"),tTargPath,lpfnCallback);
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
							return FALSE;
						}
					}
					//MOVEFILE_DELAY_UNTIL_REBOOT
					if(fileName == _T("autoupdate.exe"))
					{
						CString newName = targpath+CString(_T("\\autoupdate.tmp")); 
					    rename(newFilePath, newName);
					}
					BOOL res=MoveFileEx(filePath,newFilePath,MOVEFILE_REPLACE_EXISTING);
					if (!res)
					{
						CString name = GetName();
						if (newFilePath.Find(name) != -1)//self
						{
							MoveFileEx(filePath,newFilePath+_T("_"),MOVEFILE_REPLACE_EXISTING);
						}
					}
					//call callback
					if(lpfnCallback)
					{
						lpfnCallback(filePath,newFilePath,res);
					}
					if (!res)
					{
						result = FALSE;
					}
				}
			}while (finder.FindNextFile());
		}
		finder.Close();
		return result;
	}
	
};
#endif