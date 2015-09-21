/************************************************************************
* 
*
*
/************************************************************************/
#ifndef DAS_UTILITY_H
#define DAS_UTILITY_H
#include <atlbase.h>
#include <atlstr.h>
#include <atlcrypt.h>

#include <boost/lexical_cast.hpp>
#ifndef DEBUG

inline void boost::throw_exception( std::exception const & e )
{
	throw e;
}
#endif
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
*get process path
*@hm:process handle,if null,means current process
*/
CString GetPath(HMODULE hm=0);
/**
*get short path
*/
CString GetShortPath(LPCTSTR path);
/**
*
*/
CString GetConfig(LPCTSTR path,LPCTSTR secname,LPCTSTR keyname,LPCTSTR default_value=0);
/**
*
*/
int GetConfigInt(LPCTSTR path,LPCTSTR secname,LPCTSTR keyname,int default_value=0);
/**
*
*/
BOOL SetConfig(LPCTSTR path,LPCTSTR secname,LPCTSTR keyname,LPCTSTR value);
/**
*if keyname if not NULL,will delete key,else delete the section
*
*/
BOOL DelConfig(LPCTSTR path,LPCTSTR secname,LPCTSTR keyname=0);
/**
*
*/
//inline BOOL is_numeric(LPCTSTR pstr)
//{
//	BOOL result = TRUE;
//	long pres = _ttol(pstr);
//	result = !pres;
//	return result;
//}
/**
*
*/
DWORD MyMessageBox(const CString& msg,const CString& cap);
/**
*
*/
PROCESS_INFORMATION LaunchApp(LPCTSTR lpszClientPath,LPTSTR lpszCmdLine=0);
/**
*
*/
BOOL CreateSA(SECURITY_ATTRIBUTES& sa,BOOL inherit=FALSE);
/**
*
*/
CString PackMd5Hash(const BYTE *md5order,const DWORD len);
struct RawSMBIOSData
{
	BYTE    Used20CallingMethod;
	BYTE    SMBIOSMajorVersion;
	BYTE    SMBIOSMinorVersion;
	BYTE    DmiRevision;
	DWORD    Length;
	BYTE    SMBIOSTableData[];
};
/**
*
*/
CString GetMac();
/**
* this topic is so common
* not use HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Cryptography\MachineGuid 
* in china,
* be care about smb,maybe some pc will update this table when running
* be care about 
*/
CString GenDeviceID();
#endif