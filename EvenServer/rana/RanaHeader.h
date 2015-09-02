#pragma warning (disable : 4786)

#ifndef __RANA_HEADER_H__
#define __RANA_HEADER_H__

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <time.h>
#include <stdio.h>
#include <string.h>
#include "trigger/TriggerHeader.h"

extern void ranaLog(const char *str, ...);
extern void ranaError(const char *str, ...);
#ifdef _DEBUG
#define debug_ranaLog         ranaLog
#define debug_ranaError       ranaError
#else
inline void debug_ranaLog(const char *str, ...){}
#define debug_ranaError       ranaLog
#endif

#define RANA_ASSERT TRIGGER_ASSERT

#define RANA_OK 0

// 常用类型
typedef Trigger::BYTE		RanaBYTE;
typedef Trigger::WORD		RanaWORD;
typedef	Trigger::DWORD		RanaDWORD;
typedef long long			RanaTIME;
typedef int					RanaCOUNT;

struct RanaVariant	//Rana通用变量
{
	class RanaBadVariant
	{
	private:
		RanaBadVariant();
	};
	enum VariantType	//变量类型定义
	{
		VT_VOID,		//nil
		VT_BOOL,		//布尔型
		VT_INTEGER,		//整型
		VT_STRING,		//字符串型
		VT_USERDATA,	//用户定义类型
	} type;
	union BODY
	{
		bool			bVal;	//布尔型值 (type==VT_BOOL)
		int				iVal;	//整型值 (type==VT_INTEGER)
		const char *	sVal;	//字符串值 (type==VT_STRING)
		void *			vVal;	//用户定义类型(type==VT_USERDATA)
	} body;	
	RanaVariant();
	void clear();
	VariantType getType() const;
	operator int () const;
	operator bool() const;
	operator const char * () const;
	operator void * () const;
	RanaVariant & operator=(int val);
	RanaVariant & operator=(bool val);
	RanaVariant & operator=(const char * val);
	RanaVariant & operator=(void * val);
};

inline RanaVariant::RanaVariant()
	:type(VT_VOID)
{
}

inline void RanaVariant::clear()
{
	type = VT_VOID;
}

inline RanaVariant::VariantType RanaVariant::getType() const
{
	return type;
}

inline RanaVariant::operator bool () const
{
	if(type==VT_BOOL)
	{
		return body.bVal;
	}
	throw static_cast<RanaBadVariant *>(0);
}

inline RanaVariant::operator int () const
{
	if(type==VT_INTEGER)
	{
		return body.iVal;
	}
	throw static_cast<RanaBadVariant *>(0);
}

inline RanaVariant::operator const char * () const
{
	if(type==VT_STRING)
	{
		return body.sVal;
	}
	throw static_cast<RanaBadVariant *>(0);
}

inline RanaVariant::operator void * () const
{
	if(type==VT_USERDATA)
	{
		return body.vVal;
	}
	throw static_cast<RanaBadVariant *>(0);
}

inline RanaVariant & RanaVariant::operator=(bool val)
{
	type=VT_BOOL;
	body.bVal=val;
	return *this;
}

inline RanaVariant & RanaVariant::operator=(int val)
{
	type=VT_INTEGER;
	body.iVal=val;
	return *this;
}

inline RanaVariant & RanaVariant::operator=(const char * val)
{
	type=VT_STRING;
	body.sVal=val;
	return *this;
}

inline RanaVariant & RanaVariant::operator=(void * val)
{
	type=VT_USERDATA;
	body.vVal=val;
	return *this;
}

// Rana系统类型
const RanaBYTE _RanaNameCodeLen = 14;
union _RanaName
{
	RanaBYTE name[4*_RanaNameCodeLen+1];
	RanaDWORD code[_RanaNameCodeLen];

	_RanaName();
	_RanaName(const char *_str);
	~_RanaName();

	_RanaName &operator =(const char *_str);
	_RanaName &operator =(const _RanaName &_name);
	bool operator ==(const _RanaName &_name) const;
	bool operator !=(const _RanaName &_name) const;
	bool operator < (const _RanaName &_name) const;
	operator const char *() const;
	char *data(char *_buf) const;
};

struct RanaSpaceHandle
{
	unsigned short handle;
	unsigned long serial;
};


typedef _RanaName		RanaSpaceName;
typedef _RanaName		RanaTriggerName;
typedef _RanaName		RanaEventName;

typedef Trigger::EventCategoryID	RanaEventType;
typedef RanaDWORD					RanaTimerHandle;

typedef lua_State*		RanaState;

// 非法值
extern const RanaBYTE RANA_INVALID_BYTE;
extern const RanaWORD RANA_INVALID_WORD;
extern const RanaDWORD RANA_INVALID_DWORD;

extern const RanaEventType RANA_INVALID_EVENTTYPE;		// 非法事件类型
extern const RanaTimerHandle RANA_INVALID_TIMERHANDLE;	// 非法计时器句柄

// 常量

// 事件类型
enum
{
	RANA_TIME_EVENT = 0,	// 计时器事件类型
	RANA_USER_EVENT = 1000	// 用户自定义事件类型（类型编号从此数值开始）
};

// Rana脚本回调C时，C回调函数中使用的API
const char *ranaGetString(RanaState _R, unsigned int _index);	// 获得Rana脚本中的字符串传参
double ranaGetNumber(RanaState _R, unsigned int _index);		// 获得Rana脚本中的数值传参
bool ranaGetBoolean(RanaState _R, unsigned int _index);			// 获得Rana脚本中的布尔传参
void ranaReturnString(RanaState _R, char *_string);				// 向Rana脚本返回字符串
void ranaReturnNumber(RanaState _R, double _number);			// 向Rana脚本返回数值
void ranaReturnBoolean(RanaState _R, bool _boolean);			// 向Rana脚本返回布尔

// 向Rana脚本中注册过的C API
int ranaRegisterTimeEvent(RanaState _s);	// 注册计时器事件触发器
// Rana脚本调用方式
// RanaRegisterTimeEvent(triggerName, timerHandle)
// triggerName -- 关联触发器名
// timerHandle -- 计时器句柄

int ranaCreateTimer(RanaState _s);			// 创建一个新的计时器
// Rana脚本调用方式
// timerHandle = RanaCreateTimer()
// timerHandle -- 计时器句柄

int ranaSetTimerPeriodTime(RanaState _s);	// 设置计时器超时时间
// Rana脚本调用方式
// RanaSetTimerPeriodTime(timerHandle, periodTime)
// timerHandle -- 计时器句柄
// periodTime -- 设置定时器超时时间（单位：百分之一秒）

int ranaSetTimerRepeat(RanaState _s);		// 设置计时器是否可重复计时
// Rana脚本调用方式
// RanaSetTimerRepeat(timerHandle, isRepeat)
// timerHandle -- 计时器句柄
// isRepeat -- 是否可重复计时

int ranaSetTimerOnOff(RanaState _s);		// 设置计时器开关
// Rana脚本调用方式
// RanaSetTimerOnOff(timerHandle, isOn)
// timerHandle -- 计时器句柄
// isOn -- 计时器开关

int ranaResetTimer(RanaState _s);			// 重置计时器
// Rana脚本调用方式
// RanaResetTimer(timerHandle)
// timerHandle -- 计时器句柄

int ranaSetSpaceEventOnOff(RanaState _s);	// 打开/关闭一个Rana空间中的所有事件
// Rana脚本调用方式
// RanaSetSpaceEventOnOff(isOn)
// isOn -- 事件开关

int ranaSetTypeEventOnOff(RanaState _s);	// 打开/关闭一个Rana空间中某种类型的所有事件
// Rana脚本调用方式
// RanaSetTypeEventOnOff(eventType, isOn)
// eventType -- 事件类型
// isOn -- 事件开关

int ranaSetTriggerEventOnOff(RanaState _s);	// 打开/关闭一个Rana空间中关联到某个触发器上的所有事件
// Rana脚本调用方式
// RanaSetTriggerEventOnOff(triggerName, isOn)
// triggerName -- 关联的触发器名
// isOn -- 事件开关

int ranaSetEventOnOff(RanaState _s);// 打开/关闭一个Rana空间中关联到某个触发器上的所有事件
// Rana脚本调用方式
// RanaSetEventOnOff(eventName, isOn)
// eventName -- 关联的触发器名
// isOn -- 事件开关

int ranaSetTriggerOnOff(RanaState _s);	// 打开/关闭一个Rana空间中的触发器
// Rana脚本调用方式
// ranaSetTriggerOnOff(triggerName, isOn)
// triggerName -- 关联的触发器名
// isOn -- 事件开关

#if defined(_MSC_VER)
#define snprintf _snprintf
#if _MSC_VER < 1500
	#define vsnprintf _vsnprintf
#endif
#endif

#endif //__RANA_HEADER_H__

