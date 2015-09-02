/*! @file
	@ingroup flat_define
	@brief 基本数据类型定义	
*/

#ifndef __ZOIC_TYPES_H
#define __ZOIC_TYPES_H

#include "config_flat.h"
#ifdef _ZOIC_CPS
#include <CPS/VMPDEF.h>
#define ZOIC_VMP_BEGIN VMPBEGIN
#define ZOIC_VMP_END VMPEND
#else
#define ZOIC_VMP_BEGIN
#define ZOIC_VMP_END
#endif
#include <assert.h>

//1)数据类型定义
typedef unsigned char		BYTE;
typedef unsigned short		WORD;
typedef unsigned long		DWORD;
typedef unsigned long long	QWORD;
typedef long long			TIME_MSEL;
typedef long				TIME_SEC;

typedef const char *		PCSTR;
typedef DWORD				SerialType;
typedef WORD				HandleType;

typedef DWORD				IP_TYPE;
typedef WORD				PORT_TYPE;

#ifndef HAVE_MINGW32
typedef int					SOCKET;
#else
typedef unsigned int		SOCKET;
#endif

//2)常量定义
#ifndef HAVE_MINGW32
const SOCKET INVALID_SOCKET = -1;
#endif

const int NETWORK_BUF_SIZE		=	1024 * 16;
const int NETWORK_MSG_HEADER	=   4;
const int MAX_DYNAMIC_PACKETS 	= 	50000;

//系统PING参数定义
const WORD SYSPING_REQ			=	0xFFFA;// 65530,系统PING请求
const WORD SYSPING_ACK			=	0xFFFB;// 65531,系统PING响应
const WORD PROTOCOL_VERSION_NTF	=	0xFFFC;// 65532,协议版本通知
const int SYSPING_KEEPIDLE 	= 30000;	//(毫秒)多长时间未收到消息就发送SYSPING探测
const int SYSPING_KEEPINTVL	= 10000;	//(毫秒),发送SYSPING探测的间隔是多少
const int SYSPING_KEEPCNT	= 3;		//总共要发多少次SYSPING探测

//3)宏定义
#ifndef NULL
#define NULL	0
#endif

// 09/23/2008-9:43 modified by YYBear
#ifdef WIN32
#ifdef _MSC_VER
#if _MSC_VER >= 1400
#ifdef  __cplusplus
extern "C" {
#endif
	_CRTIMP void __cdecl _wassert(__in_z const wchar_t * _Message, __in_z const wchar_t *_File, __in unsigned _Line);
#ifdef  __cplusplus
}
#endif
#define ASSERT_FORCE(x)		{ (void)( (!!(x)) || (_wassert(_CRT_WIDE(#x), _CRT_WIDE(__FILE__), __LINE__), 0) ); }
#else
#ifdef  __cplusplus
extern "C" {
#endif
	_CRTIMP void __cdecl _assert(const char *, const char *, unsigned);
#ifdef  __cplusplus
}
#endif
#define ASSERT_FORCE(x)		{ (void)( (x) || (_assert(#x, __FILE__, __LINE__), 0) ); }
#endif	//#ifdef _MSC_VER >= 1400
#endif	//#ifdef _MSC_VER
#else
#define ASSERT_FORCE(x)		{ ((x) ? (void)0 : __assert(__func__, __FILE__, __LINE__, #x)); }
#endif

#ifdef _DEBUG
#define ASSERT_DEBUG(x)		ASSERT_FORCE(x)
#define ASSERT_RELEASE(x)	
#else
#define ASSERT_DEBUG(x)		
#define ASSERT_RELEASE(x)	ASSERT_FORCE(x)
#endif

#ifdef ASSERT
#undef ASSERT
#endif
#define ASSERT(x)			ASSERT_DEBUG(x)

// #ifdef _DEBUG
// #define ASSERT(x)	\
// 	if(!(x)) \
// 	{\
// 		*(int *)0=0;\
// 	}
// #else
// #define ASSERT(x)
// #endif
// 09/23/2008-9:43 end modifying by YYBear

#ifndef HAVE_MINGW32
#define closesocket(S) ::close((S))
#endif

#ifdef HAVE_MINGW32
#define WAIT_A_WHILE Sleep(1);
#else
#define WAIT_A_WHILE usleep(1);
#endif

#ifndef HAVE_MINGW32
#define stricmp strcasecmp
#endif

#if defined(_MSC_VER)
#if _MSC_VER < 1500
	#define vsnprintf _vsnprintf
#endif
#define snprintf _snprintf
#define strtoll _strtoi64
#define strtoull _strtoui64
#endif
#include "MissingFunction.h"

#ifdef _DEBUG
#define dputs           puts
#define dprintf         printf
#else
inline void dputs(const char *){}
inline void dprintf(const char *, ...){}
#endif

#define CMP_PTR(CLASS,NAME)\
	char __cmp_inner_buffer_##NAME[sizeof(CLASS)]; \
	CLASS * NAME = reinterpret_cast<CLASS *>(__cmp_inner_buffer_##NAME);

#ifdef _MSC_VER 
	#if _MSC_VER < 1500
		#define CLASS_MEMBER_PTR(NAME) \
			(NAME)
	#else
		#define CLASS_MEMBER_PTR(NAME) \
			(&NAME)
	#endif
#else
	#define CLASS_MEMBER_PTR(NAME) \
		(&NAME)
#endif

#ifdef _MSC_VER
	#ifndef  __PRETTY_FUNCTION__
		#define __PRETTY_FUNCTION__ __FUNCTION__
	#endif
#endif


#endif
