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

// ��������
typedef Trigger::BYTE		RanaBYTE;
typedef Trigger::WORD		RanaWORD;
typedef	Trigger::DWORD		RanaDWORD;
typedef long long			RanaTIME;
typedef int					RanaCOUNT;

struct RanaVariant	//Ranaͨ�ñ���
{
	class RanaBadVariant
	{
	private:
		RanaBadVariant();
	};
	enum VariantType	//�������Ͷ���
	{
		VT_VOID,		//nil
		VT_BOOL,		//������
		VT_INTEGER,		//����
		VT_STRING,		//�ַ�����
		VT_USERDATA,	//�û���������
	} type;
	union BODY
	{
		bool			bVal;	//������ֵ (type==VT_BOOL)
		int				iVal;	//����ֵ (type==VT_INTEGER)
		const char *	sVal;	//�ַ���ֵ (type==VT_STRING)
		void *			vVal;	//�û���������(type==VT_USERDATA)
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

// Ranaϵͳ����
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

// �Ƿ�ֵ
extern const RanaBYTE RANA_INVALID_BYTE;
extern const RanaWORD RANA_INVALID_WORD;
extern const RanaDWORD RANA_INVALID_DWORD;

extern const RanaEventType RANA_INVALID_EVENTTYPE;		// �Ƿ��¼�����
extern const RanaTimerHandle RANA_INVALID_TIMERHANDLE;	// �Ƿ���ʱ�����

// ����

// �¼�����
enum
{
	RANA_TIME_EVENT = 0,	// ��ʱ���¼�����
	RANA_USER_EVENT = 1000	// �û��Զ����¼����ͣ����ͱ�ŴӴ���ֵ��ʼ��
};

// Rana�ű��ص�Cʱ��C�ص�������ʹ�õ�API
const char *ranaGetString(RanaState _R, unsigned int _index);	// ���Rana�ű��е��ַ�������
double ranaGetNumber(RanaState _R, unsigned int _index);		// ���Rana�ű��е���ֵ����
bool ranaGetBoolean(RanaState _R, unsigned int _index);			// ���Rana�ű��еĲ�������
void ranaReturnString(RanaState _R, char *_string);				// ��Rana�ű������ַ���
void ranaReturnNumber(RanaState _R, double _number);			// ��Rana�ű�������ֵ
void ranaReturnBoolean(RanaState _R, bool _boolean);			// ��Rana�ű����ز���

// ��Rana�ű���ע�����C API
int ranaRegisterTimeEvent(RanaState _s);	// ע���ʱ���¼�������
// Rana�ű����÷�ʽ
// RanaRegisterTimeEvent(triggerName, timerHandle)
// triggerName -- ������������
// timerHandle -- ��ʱ�����

int ranaCreateTimer(RanaState _s);			// ����һ���µļ�ʱ��
// Rana�ű����÷�ʽ
// timerHandle = RanaCreateTimer()
// timerHandle -- ��ʱ�����

int ranaSetTimerPeriodTime(RanaState _s);	// ���ü�ʱ����ʱʱ��
// Rana�ű����÷�ʽ
// RanaSetTimerPeriodTime(timerHandle, periodTime)
// timerHandle -- ��ʱ�����
// periodTime -- ���ö�ʱ����ʱʱ�䣨��λ���ٷ�֮һ�룩

int ranaSetTimerRepeat(RanaState _s);		// ���ü�ʱ���Ƿ���ظ���ʱ
// Rana�ű����÷�ʽ
// RanaSetTimerRepeat(timerHandle, isRepeat)
// timerHandle -- ��ʱ�����
// isRepeat -- �Ƿ���ظ���ʱ

int ranaSetTimerOnOff(RanaState _s);		// ���ü�ʱ������
// Rana�ű����÷�ʽ
// RanaSetTimerOnOff(timerHandle, isOn)
// timerHandle -- ��ʱ�����
// isOn -- ��ʱ������

int ranaResetTimer(RanaState _s);			// ���ü�ʱ��
// Rana�ű����÷�ʽ
// RanaResetTimer(timerHandle)
// timerHandle -- ��ʱ�����

int ranaSetSpaceEventOnOff(RanaState _s);	// ��/�ر�һ��Rana�ռ��е������¼�
// Rana�ű����÷�ʽ
// RanaSetSpaceEventOnOff(isOn)
// isOn -- �¼�����

int ranaSetTypeEventOnOff(RanaState _s);	// ��/�ر�һ��Rana�ռ���ĳ�����͵������¼�
// Rana�ű����÷�ʽ
// RanaSetTypeEventOnOff(eventType, isOn)
// eventType -- �¼�����
// isOn -- �¼�����

int ranaSetTriggerEventOnOff(RanaState _s);	// ��/�ر�һ��Rana�ռ��й�����ĳ���������ϵ������¼�
// Rana�ű����÷�ʽ
// RanaSetTriggerEventOnOff(triggerName, isOn)
// triggerName -- �����Ĵ�������
// isOn -- �¼�����

int ranaSetEventOnOff(RanaState _s);// ��/�ر�һ��Rana�ռ��й�����ĳ���������ϵ������¼�
// Rana�ű����÷�ʽ
// RanaSetEventOnOff(eventName, isOn)
// eventName -- �����Ĵ�������
// isOn -- �¼�����

int ranaSetTriggerOnOff(RanaState _s);	// ��/�ر�һ��Rana�ռ��еĴ�����
// Rana�ű����÷�ʽ
// ranaSetTriggerOnOff(triggerName, isOn)
// triggerName -- �����Ĵ�������
// isOn -- �¼�����

#if defined(_MSC_VER)
#define snprintf _snprintf
#if _MSC_VER < 1500
	#define vsnprintf _vsnprintf
#endif
#endif

#endif //__RANA_HEADER_H__

