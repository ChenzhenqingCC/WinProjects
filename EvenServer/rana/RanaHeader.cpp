#pragma warning(disable:4996)

#include "RanaHeader.h"
#include "RanaSpaceBank.h"
#include "RanaTimerBank.h"
#include "RanaTriggerBank.h"
#include "RanaTimeEvent.h"
#include "RanaPush.h"

static const int RANALOGBUF_MAXLEN=1024*1024;

_RanaName::_RanaName()
{
	memset( name,0,sizeof(RanaBYTE)*(4*_RanaNameCodeLen+1) );
}

_RanaName::_RanaName(const char *_str)
{
	memset( name,0,sizeof(RanaBYTE)*(4*_RanaNameCodeLen+1) );
	strncpy((char *)name, _str, 4*_RanaNameCodeLen);
}

_RanaName::~_RanaName()
{
}

_RanaName &_RanaName::operator =(const char *_str)
{
	memset( name,0,sizeof(RanaBYTE)*(4*_RanaNameCodeLen) );
	strncpy((char *)name, _str, 4*_RanaNameCodeLen);
	return *this;
}

_RanaName &_RanaName::operator =(const _RanaName &_name)
{
	for(RanaBYTE i =0;i<_RanaNameCodeLen;++i)
	{
		code[i] = _name.code[i];
	}
	return *this;
}

bool _RanaName::operator ==(const _RanaName &_name) const
{
	for(RanaBYTE i =0;i<_RanaNameCodeLen;++i)
	{
		if(code[i] != _name.code[i])
		{
			return false;
		}
	}
	return true;
}

bool _RanaName::operator !=(const _RanaName &_name) const
{
	return !(*this==_name);
}

bool _RanaName::operator < (const _RanaName &_name) const
{
	for(RanaBYTE i =0;i<_RanaNameCodeLen-1;++i)
	{
		if(code[i] != _name.code[i])
		{
			return code[i] < _name.code[i];
		}
	}
	return code[_RanaNameCodeLen-1] < _name.code[_RanaNameCodeLen-1];
}

_RanaName::operator const char *() const
{
	return (char *)name;
}

char *_RanaName::data(char *_buf) const
{
	if (_buf)
	{
		strncpy(_buf, (char *)name, 4*_RanaNameCodeLen);
	}
	return _buf;
}

// 非法值定义
const RanaBYTE RANA_INVALID_BYTE					= 0xFF;
const RanaWORD RANA_INVALID_WORD					= 0xFFFF;
const RanaDWORD RANA_INVALID_DWORD					= 0xFFFFFFFF;

const RanaEventType RANA_INVALID_EVENTTYPE		= ((RanaEventType)(~0));
const RanaTimerHandle RANA_INVALID_TIMERHANDLE		= ((RanaTimerHandle)(~0));

// 常量定义


// Rana脚本回调C的API
const char *ranaGetString(RanaState _R, unsigned int _index)
// 获得Rana脚本中的字符串传参
{
	return (char *)lua_tostring(_R, _index);
}

double ranaGetNumber(RanaState _R, unsigned int _index)
// 获得Rana脚本中的数值传参
{
	return lua_tonumber(_R, _index);
}

bool ranaGetBoolean(RanaState _R, unsigned int _index)
// 获得Rana脚本中的布尔传参
{
	return lua_toboolean(_R, _index)==0?false:true;
}

void ranaReturnString(RanaState _R, char *_string)
// 向Rana脚本返回字符串
{
	lua_pushstring(_R, _string);
}

void ranaReturnNumber(RanaState _R, double _number)
// 向Rana脚本返回数值
{
	lua_pushnumber(_R, _number);
}

void ranaReturnBoolean(RanaState _R, bool _boolean)
// 向Rana脚本返回布尔
{
	lua_pushboolean(_R, _boolean);
}

void ranaLog(const char *str, ...)
{
	va_list args;
	char buf[1024];
	char* pBuf = buf;
	char* newBuf = NULL;

	va_start(args, str);
	int len=vsnprintf(buf, 1000,str, args);
	if(len<0||len >= 1000)
	{
		newBuf = new char[RANALOGBUF_MAXLEN+1];
		newBuf[RANALOGBUF_MAXLEN] = 0;
		vsnprintf(newBuf, RANALOGBUF_MAXLEN,str, args);
		pBuf = newBuf;
	}
	va_end(args);

	RanaSpaceBank::getInstance()->printLog(pBuf);

	if(newBuf)
	{
		delete [] newBuf;
	}
}

void ranaError(const char *str, ...)
{
	va_list args;
	char buf[1024];
	char* pBuf = buf;
	char* newBuf = NULL;

	va_start(args, str);
	int len=vsnprintf(buf, 1000,str, args);
	if(len<0||len >= 1000)
	{
		newBuf = new char[RANALOGBUF_MAXLEN+1];
		newBuf[RANALOGBUF_MAXLEN] = 0;
		vsnprintf(newBuf, RANALOGBUF_MAXLEN,str, args);
		pBuf = newBuf;
	}
	va_end(args);

	RanaSpaceBank::getInstance()->printError(pBuf);

	if(newBuf)
	{
		delete [] newBuf;
	}
}

int ranaRegisterTimeEvent(RanaState L)
// 注册计时器事件触发器
{
	RanaSpace * pRanaSpace = GetRanaSpace(L);
	
	const char* triggerName = (const char *)lua_tostring(L, 1);
	const char* eventName = (const char *)lua_tostring(L, 2);
	if (!triggerName||!eventName)
	{
		debug_ranaLog("ranaRegisterTimeEvent : triggerNameStr NULL");
		return 0;
	}
	
	RanaTrigger* pRanaTrigger = pRanaSpace->getTriggerBank()->getTrigger(triggerName);
	if( !pRanaTrigger )
	{
		pRanaTrigger = new RanaTrigger;
		if( 0 != pRanaSpace->getTriggerBank()->addTrigger(triggerName,pRanaTrigger) )
		{
			return 0;
		}
		pRanaTrigger->setOnOff(true);
	}

	RanaTimeEvent *pRanaEvent = new RanaTimeEvent;
	pRanaEvent->setTimerHandle((RanaTimerHandle)lua_tonumber(L, 3));
	if( RANA_OK != pRanaSpace->getTriggerBank()->addEvent(eventName,pRanaEvent) )
	{
		delete pRanaEvent;
		return false;
	}
	if(  RANA_OK != pRanaSpace->getTriggerBank()->bindEvent(pRanaEvent->getHandle(),pRanaTrigger->getHandle()) )
	{
		RANA_ASSERT(0);
	}
	pRanaEvent->setOnOff(true);	
	
	return 0;
}

int ranaCreateTimer(RanaState L)
// 创建一个新的计时器
{
	RanaSpace * pRanaSpace = GetRanaSpace(L);

	RanaTimerHandle timerHandle = RanaTimerBank::getInstance()->createTimer(pRanaSpace->getHandle());
	if (timerHandle == RANA_INVALID_TIMERHANDLE)
	{
		debug_ranaLog("ranaCreateTimer : invalid timerHandle");
		return 0;
	}
	lua_pushnumber(L, timerHandle);
	return 1;
}

int ranaSetTimerPeriodTime(RanaState L)
// 设置计时器超时时间
{
	RanaSpace * pRanaSpace = GetRanaSpace(L);

	RanaTimerHandle timerHandle = (RanaTimerHandle)lua_tonumber(L, 1);
	if (timerHandle == RANA_INVALID_TIMERHANDLE)
	{
		debug_ranaLog("ranaSetTimerPeriodTime : invalid timerHandle");
		return 0;
	}
	RanaTIME periodTime = (RanaTIME)lua_tonumber(L, 2);
	if (periodTime == 0)
	{
		debug_ranaLog("ranaSetTimerPeriodTime : invalid periodTime");
		return 0;
	}

	RanaTimerBank::getInstance()->setTimerPeriodTime(pRanaSpace->getHandle(), timerHandle, periodTime);
	return 0;
}

int ranaSetTimerRepeat(RanaState L)
// 设置计时器是否可重复计时
{
	RanaSpace * pRanaSpace = GetRanaSpace(L);

	RanaTimerHandle timerHandle = (RanaTimerHandle)lua_tonumber(L, 1);
	if (timerHandle == RANA_INVALID_TIMERHANDLE)
	{
		debug_ranaLog("ranaSetTimerRepeat : invalid timerHandle");
		return 0;
	}
	bool isRepeat = lua_toboolean(L, 2)==0?false:true;

	RanaTimerBank::getInstance()->setTimerRepeat(pRanaSpace->getHandle(), timerHandle, isRepeat);
	return 0;
}

int ranaSetTimerOnOff(RanaState L)
// 设置计时器开关
{
	RanaSpace * pRanaSpace = GetRanaSpace(L);

	RanaTimerHandle timerHandle = (RanaTimerHandle)lua_tonumber(L, 1);
	if (timerHandle == RANA_INVALID_TIMERHANDLE)
	{
		debug_ranaLog("ranaSetTimerOnOff : invalid timerHandle");
		return 0;
	}
	bool isOn = lua_toboolean(L, 2)==0?false:true;

	RanaTimerBank::getInstance()->setTimerOnOff(pRanaSpace->getHandle(), timerHandle, isOn);
	return 0;
}

int ranaResetTimer(RanaState L)
// 重置计时器
{
	RanaSpace * pRanaSpace = GetRanaSpace(L);

	RanaTimerHandle timerHandle = (RanaTimerHandle)lua_tonumber(L, 1);
	if (timerHandle == RANA_INVALID_TIMERHANDLE)
	{
		debug_ranaLog("ranaResetTimer : invalid timerHandle");
		return 0;
	}

	RanaTimerBank::getInstance()->resetTimer(pRanaSpace->getHandle(), timerHandle);
	return 0;
}

int ranaSetSpaceEventOnOff(RanaState L)
// 打开/关闭一个Rana空间中的所有事件
{
	RanaSpace * pRanaSpace = GetRanaSpace(L);

	bool isOn = lua_toboolean(L, 1)==0?false:true;

	std::vector<RanaEvent*> allEvents;
	pRanaSpace->getTriggerBank()->listEvents(allEvents);
	for(size_t i=0;i<allEvents.size();++i)
	{
		allEvents[i]->setOnOff(isOn);
	}
	return 0;
}

int ranaSetTypeEventOnOff(RanaState L)
// 打开/关闭一个Rana空间中某种类型的所有事件
{
	RanaSpace * pRanaSpace = GetRanaSpace(L);

	RanaEventType eventType = (RanaEventType)lua_tonumber(L, 1);
	bool isOn = lua_toboolean(L, 2)==0?false:true;

	std::vector<RanaEvent*> events;
	pRanaSpace->getTriggerBank()->listEventByType(eventType,events);
	for(size_t i=0;i<events.size();++i)
	{
		events[i]->setOnOff(isOn);
	}
	return 0;
}

int ranaSetTriggerEventOnOff(RanaState L)
// 打开/关闭一个Rana空间中关联到某个触发器上的所有事件
{
	RanaSpace * pRanaSpace = GetRanaSpace(L);

	char *triggerName = (char *)lua_tostring(L, 1);
	if (!triggerName)
	{
		debug_ranaLog("ranaSetTriggerEventOnOff : triggerName NULL");
		return 0;
	}
	RanaTriggerName luaTriggerName(triggerName);

	bool isOn = lua_toboolean(L, 2)==0?false:true;

	RanaTrigger* pTrigger = pRanaSpace->getTriggerBank()->getTrigger(luaTriggerName);

	if(!pTrigger)
	{
		return 0;
	}

	std::vector<RanaEvent*> events;
	pRanaSpace->getTriggerBank()->listEventByTrigger(pTrigger->getHandle(),events);
	for(size_t i=0;i<events.size();++i)
	{
		events[i]->setOnOff(isOn);
	}
	return 0;
}

int ranaSetEventOnOff(RanaState L)
{
	RanaSpace * pRanaSpace = GetRanaSpace(L);

	char *eventName = (char *)lua_tostring(L, 1);
	if (!eventName)
	{
		debug_ranaLog("ranaSetEventOnOff : eventName NULL");
		return 0;
	}
	RanaEventName luaEventName(eventName);

	bool isOn = lua_toboolean(L, 2)==0?false:true;

	RanaEvent* pEvent = pRanaSpace->getTriggerBank()->getEvent(luaEventName);
	if(pEvent)
	{
		pEvent->setOnOff(isOn);
	}
	return 0;
}

int ranaSetTriggerOnOff(RanaState L)
// 打开/关闭一个Rana空间中某个触发器
{
	RanaSpace * pRanaSpace = GetRanaSpace(L);
	
	char *triggerName = (char *)lua_tostring(L, 1);
	if (!triggerName)
	{
		debug_ranaLog("ranaSetTriggerOnOff : triggerName NULL");
		return 0;
	}
	RanaTriggerName luaTriggerName(triggerName);

	bool isOn = lua_toboolean(L, 2)==0?false:true;

	RanaTrigger* pTrigger = pRanaSpace->getTriggerBank()->getTrigger(luaTriggerName);

	if(!pTrigger)
	{
		return 0;
	}
	pTrigger->setOnOff(isOn);

	return 0;
}

int l_getContextBool(RanaState L)
{
	RanaSpace * pRanaSpace = GetRanaSpace(L);
	const char* pKey = lua_tostring(L,1);
	lua_pushboolean(L, pRanaSpace->getBoolean(pKey) );
	return 1;
}

int l_getContextString(RanaState L)
{
	RanaSpace * pRanaSpace = GetRanaSpace(L);
	const char* pKey = lua_tostring(L,1);
	lua_pushstring(L, pRanaSpace->getString(pKey) );
	return 1;
}

int l_getContextNumber(RanaState L)
{
	RanaSpace * pRanaSpace = GetRanaSpace(L);
	const char* pKey = lua_tostring(L,1);
	lua_pushnumber(L, pRanaSpace->getNumber(pKey) );
	return 1;
}

int l_getContext(RanaState L)
{
	RanaSpace * pRanaSpace = GetRanaSpace(L);
	const char* pKey = lua_tostring(L,1);
	const RanaVariant *val=pRanaSpace->getContext(pKey);
	if(val)
	{
		rana_push(L,*val);
		return 1;
	}
	else
	{
		return 0;
	}
}

