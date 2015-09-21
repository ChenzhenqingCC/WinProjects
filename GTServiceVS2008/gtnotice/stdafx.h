////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	stdafx   version:  1.0   ? date: 08/20/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 dasjack- All Rights Reserved
//	Written By Jack Liu
//	created:	21:9:2009   10:11
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
// Change these values to use different versions
#define WINVER								0x0500
//#define _WIN32_WINNT					0x0501

#define NTDDI_VERSION					NTDDI_LONGHORN
#define _WIN32_IE							_WIN32_IE_IE70
#define _RICHEDIT_VER					0x0200
//
//#define _ATL_APARTMENT_THREADED
//for cache page content
//#define PAGESAVE
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
#include <atlhost.h>
#include <atlctl.h>
#include <atlframe.h>
#include <atlcrack.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atlddx.h>
#include <exdisp.h>
#include <exdispid.h>
///////////////////////////////////////////////////////////////////////////////////
#pragma warning (disable : 4200)
#pragma  warning (disable:4018)
#pragma  warning (disable:4996)
#pragma  warning (disable:4800)
#pragma  warning (disable:4244)
#pragma  warning (disable:4535)
#pragma  warning (disable:6011)
///////////////////////////////////////////////////////////////////////////////////
extern CAppModule	_Module;
extern HWND				_MainHwnd;
///////////////////////////////////////////////////////////////////////////////////
#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
