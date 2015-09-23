////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	all cast case   version:  1.0   ? date: 10/12/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 dasjack- All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef ALL_CAST_H
#define ALL_CAST_H
//#undef BOOST_EXCEPTION_DISABLE
#include <boost/lexical_cast.hpp>
//#ifndef DEBUG
//
//inline void boost::throw_exception( std::exception const & e )
//{
//	throw e;
//}
//#endif
//////////////////////////////////////////////////////////////////////////
template<class T> inline bool is_numeric(LPCTSTR pstr,T* pres)
{
	bool result = true;
	try
	{
		T res = boost::lexical_cast<T>(pstr);
		if (pres)
		{
			*pres = res;
		}
	}
	catch(boost::bad_lexical_cast &)
	{
		result = false;
	}
	catch(...)
	{
		result = false;
	}
	return result;
}

/**
*file information struct
*/
struct CFileInfo{
	CFileInfo():fsize(0)
	{
	}
public:
	CString fname;
	CString fpath;
	CString ftitle;
	CString furl;
	CString froot;
	FILETIME latime;
	FILETIME lwtime;
	FILETIME ctime;
	DWORD fsize;
};
/**
*get file information under the specified directory
*@path
*@finfos
*@recurved:
*/
inline LRESULT GetFileInfos(const CString& path,vector<CFileInfo>& finfos,const bool recurved=false,LPCTSTR filter=_T("\\*.*"))
{
	CString strReg = path+filter;
	CFindFile finder;
	if (finder.FindFile(strReg))
	{
		do 
		{
			CFileInfo finfo;
			//check file info
			finfo.fname = finder.GetFileName();
			finfo.fpath = finder.GetFilePath();
			finfo.ftitle = finder.GetFileTitle();
			finfo.furl = finder.GetFileURL();
			finfo.froot = finder.GetRoot();

			finder.GetLastAccessTime(&finfo.latime);
			finder.GetLastWriteTime(&finfo.lwtime);
			finder.GetCreationTime(&finfo.ctime);

			finfo.fsize = finder.GetFileSize();

			if(finder.IsDirectory())
			{
				if (finder.IsDots())
				{
					continue;
				}
				if (recurved)
				{
					CString subpath = finder.GetFilePath();
					GetFileInfos(subpath,finfos,recurved);
				}
			}
			finfos.push_back(finfo);
		} while (finder.FindNextFile());
	}
	finder.Close();
	return TRUE;
}
#endif