#pragma once
// Change these values to use different versions
#define WINVER								0x0500
#define _WIN32_WINNT					0x0501
#define _WIN32_IE							0x0501
#define _RICHEDIT_VER					0x0200
//
#include <tchar.h>
//#define _ATL_APARTMENT_THREADED
///////////////////////////////////////////////////////////////////////////////////
//use ATL CString instead of WTL CString
#define _WTL_NO_CSTRING
#define _WTL_NO_WTYPES
#include <atlbase.h>
#include <atlstr.h>
#include <atltypes.h>
//#define _WTL_USE_CSTRING
///////////////////////////////////////////////////////////////////////////////////
#include <atlapp.h>
#include <atlcom.h>
#include <atlhost.h>
#include <atlwin.h>
#include <atlmisc.h>
#include <atlcom.h>
#include <atlctl.h>
#include <atlcrack.h>
#include <atlctrls.h>
///////////////////////////////////////////////////////////////////////////////////
#pragma warning (disable : 4200)
#pragma  warning (disable:4018)
#pragma  warning (disable:4996)
#pragma  warning (disable:4800)
#pragma  warning (disable:4244)
#pragma  warning (disable:4535)
#pragma  warning (disable:6011)
///////////////////////////////////////////////////////////////////////////////////
#include "autoupdate.h";
extern CAutoUpdateModule _Module;

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
