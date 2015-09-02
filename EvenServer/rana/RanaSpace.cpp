#pragma warning(disable:4996)
#pragma warning(disable:4355)

#include "RanaSpace.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include "RanaSpaceBank.h"
#include "RanaTimerBank.h"
#include "RanaPush.h"

/*RanaState RanaSpace::m_testField = NULL;
char* RanaSpace::m_pFileBuff = NULL;
unsigned long RanaSpace::m_FileLength = 0;
std::map<RanaWORD, const char*> RanaSpace::m_lineMap;
*/

#ifdef _DEBUG
class _RanaStackCheck
{
public:
	_RanaStackCheck(RanaState L)
		:m_state(L)
	{
		m_orgStack = lua_gettop(m_state);
	}
	~_RanaStackCheck()
	{
		const int currentStack = lua_gettop(m_state);
		for( int i = currentStack;i>m_orgStack;--i)
		{
			ranaError( "lua stack leak:%d(%d)", i,lua_type(m_state,i) );
		}
		RANA_ASSERT( currentStack == m_orgStack);
	}
private:
	RanaState m_state;
	int m_orgStack;
};
#define DEBUG_CHECK_STACK(L) _RanaStackCheck _ranaCheckingStack(L);
#else
#define DEBUG_CHECK_STACK(L)
#endif

namespace
{
	struct CFunctionItem
	{
		std::string name;
		int	(*func)(RanaState);
	};
	struct InitialRegister
	{
		typedef std::vector<CFunctionItem> FUNC_LIST;
		typedef std::map<std::string, FUNC_LIST > LIB_LIST;
		LIB_LIST libs;
		FUNC_LIST functions;
	};
	typedef std::map<RanaSpace::SpaceType,InitialRegister> INITIAL_MAP;
	static INITIAL_MAP & getInitialMap()
	{
		static INITIAL_MAP initialMap;
		return initialMap;
	}
	static int luaopen_lib_register(lua_State *L)
	{
		const char* libname = lua_tostring(L, 1);
		luaL_Reg* pLibs = NULL;
		const char* pBufferLocation = lua_tostring(L, 2);
		memcpy(&pLibs,pBufferLocation,sizeof(pLibs) );
		luaL_register(L,libname,pLibs);
		return 1;
	}
	void lua_lib_register(RanaState L,const char* libname,const luaL_Reg* functions)
	{
		lua_pushcfunction(L, &luaopen_lib_register);
		lua_pushstring(L, libname);
		lua_pushlstring(L, (const char*)(void*)(&functions),sizeof(functions) );
		lua_call(L, 2, 0);
	}
	void rana_regist_lib(RanaState L,const char* libname,const InitialRegister::FUNC_LIST& funcList)
	{
		DEBUG_CHECK_STACK(L);
		luaL_Reg* pLibs = new luaL_Reg[funcList.size()+1];
		for(size_t i=0;i<funcList.size();++i)
		{
			const CFunctionItem& libItem = funcList[i];
			pLibs[i].name = libItem.name.c_str();
			pLibs[i].func = libItem.func;
		}
		pLibs[funcList.size()].name = NULL;
		pLibs[funcList.size()].func = NULL;

		lua_lib_register(L,libname,pLibs);
		delete [] pLibs;
	}
}

RanaSpace::CFunctionItemInit::CFunctionItemInit( unsigned char type,const char *libname,const char *name,int (*func)(RanaState) )
{
	CFunctionItem funcItem;
	funcItem.name = name;
	funcItem.func = func;
	if(!libname)
	{
		getInitialMap()[type].functions.push_back(funcItem);
	}
	else
	{
		getInitialMap()[type].libs[libname].push_back(funcItem);
	}
};

RanaSpace::RanaSpace()
	:m_errorCode(EC_SUCCESS)
	,m_state(NULL)
	,m_context(NULL)
	,m_hook(NULL)
	,m_triggerBank( *new RanaTriggerBank(*this,*new Trigger::TriggerSpace) )
{
	m_puttingVariantMap = new VARIANT_MAP;
}


RanaSpace::~RanaSpace()
{
/*
	if (m_context)
	{
		releaseContext();
	}
	m_context = NULL;
*/
//	debug_ranaLog("RanaSpace Delete: handle %u", m_Handle);
//	if(m_pFileBuff)
//	{
//		delete m_pFileBuff;
//		m_pFileBuff = NULL;
//		m_FileLength = 0;
//	}
	while( m_VMapStack.size() )
	{
		delete m_VMapStack.top();
		m_VMapStack.pop();
	}
	if(m_puttingVariantMap)
	{
		delete m_puttingVariantMap;
		m_puttingVariantMap = NULL;
	}
	std::list<VARIANT_MAP*>::iterator it = m_unusedVMaps.begin();
	for(;it!=m_unusedVMaps.end();++it )
	{
		delete (*it);
	}

	Trigger::TriggerSpace* pTriggerSystem = &m_triggerBank.m_triggerSystem;
	delete &m_triggerBank;
	delete pTriggerSystem;
	delete m_puttingVariantMap;
}

int RanaSpace::runFile(const char* fname)
{
	DEBUG_CHECK_STACK(m_state);
	if(!fname || fname[0]=='\0')
	{
		return EMPTYFILENAME;
	}
	char do_buf[2048];
	if( snprintf(do_buf,sizeof(do_buf),"dofile(\"%s\");",fname)>0 )
	{
		int result = luaL_dostring(m_state,do_buf);
		if( result != RANA_OK )
		{
			ranaError( "cannot run file(%s): %s", fname, lua_tostring(m_state, -1) );
			lua_pop(m_state, 1);
			return result;
		}
	}
	else
	{
		ranaError("Filename(%s) is too long. It shouldn't lengther than 2000.",fname);
		return ERROR_NOTEXISTFILE;
	}
	return RANA_OK;
}

int RanaSpace::initialEmptySpace(RanaSpaceName rsn,unsigned char type,RanaContext * context)
{
	if( m_state )
	{
		return -1;
	}
	m_SpaceType = type;
	m_SpaceName = rsn;

	m_state = lua_open();   /* opens Lua */

	//将当前RanaSpace的指针注册到相应的ranastate的空间中
	lua_setallocf(
		m_state,
		lua_getallocf(m_state,NULL),
		this);

	luaL_openlibs(m_state);

	registerCFunction();

	//	m_pfCustomCFuncRegCallBk = pf;
	//	registerCustomCFunction();

	///////////////set space internal name
	m_FileName = "";
	m_context = context;
	m_errorCode = EC_SUCCESS;
	return RESULT_OK;
}

int RanaSpace::initialFromFileForEmptySpace(const char* fname)
{
	if(!m_state)
	{
		return ERROR_STATE;
	}
	if(!fname || fname[0] == '\0' )
	{
		return EMPTYFILENAME;
	}
	char do_buf[2048];
	if(snprintf(do_buf,sizeof(do_buf),"dofile(\"%s\");",fname)>0)
	{
		if(luaL_dostring(m_state,do_buf) != 0)
			//if (luaL_loadfile(m_state, fname) || lua_pcall(m_state, 0, 0, 0))
		{
			ranaError("cannot run configuration file: %s",
				lua_tostring(m_state, -1));
			lua_pop(m_state,1);
			return ERROR_NOTEXISTFILE;
		}
	}
	else
	{
		ranaError("Filename(%s) is too long. It shouldn't lengther than 2000.",fname);
		return ERROR_NOTEXISTFILE;
	}

	lua_getglobal(m_state, "main");
	if (lua_pcall(m_state, 0, 0, 0) != 0)
	{
		ranaError("error running function 'main': %s",
			lua_tostring(m_state, -1));
		lua_pop(m_state, 1);
		return ERROR_MAINFUNCTION;
	}

	///////////////set space internal name
	m_FileName = fname;
	return RESULT_OK;
}


int RanaSpace::initial(const char* fname,RanaSpaceName rsn,unsigned char type,RanaContext * context)
{
	int retCode = initialEmptySpace(rsn,type,context);
	if( retCode != RESULT_OK )
	{
		return retCode;
	}
	retCode = initialFromFileForEmptySpace(fname);
	if( retCode != RESULT_OK )
	{
		clear();
		return retCode;
	}
	return RESULT_OK;
}

void RanaSpace::clear()
{
	if(m_state)
	{
		getTriggerBank()->clear();
		// 删除相应RanaTimer
		RanaTimerBank::getInstance()->delTimer( getHandle() );
		// 释放RanaContext空间
		releaseValueTable();

		lua_close(m_state);
		m_state = NULL;
	}
}

void RanaSpace::trackTriggerLog(bool isTrack,unsigned int lagErrorLimit)
{
	getTriggerBank()->m_trackLogTrigger = isTrack;
	getTriggerBank()->m_lagLimit = lagErrorLimit;
}

void RanaSpace::clearTriggerLogSetting()
{
	getTriggerBank()->clearTrackSetting();
}

void RanaSpace::setHook(RanaFeedback* pHook)
{
	m_hook = pHook;
}

RanaFeedback* RanaSpace::getHook() const
{
	return m_hook;
}


void RanaSpace::registerCFunction()
{
	INITIAL_MAP &initialMap = getInitialMap();
	INITIAL_MAP::iterator itMap = initialMap.find(m_SpaceType);
	if(itMap == initialMap.end() )
	{
		return;
	}
	const InitialRegister& registerMethod = itMap->second;
	
	for( InitialRegister::LIB_LIST::const_iterator itLib = registerMethod.libs.begin(); itLib != registerMethod.libs.end(); ++itLib )
	{
		const InitialRegister::FUNC_LIST& funcList = itLib->second;
		rana_regist_lib(m_state,itLib->first.c_str(),funcList);
	}
	for(size_t i=0;i<registerMethod.functions.size();++i)
	{
		const CFunctionItem& fucntionItem = registerMethod.functions[i];
		lua_register(m_state, fucntionItem.name.c_str(), fucntionItem.func );
	}
/*	// 向Rana脚本中注册C API
	lua_register(m_state, "RanaRegisterTimeEvent", ranaRegisterTimeEvent);
	lua_register(m_state, "RanaCreateTimer", ranaCreateTimer);
	lua_register(m_state, "RanaSetTimerPeriodTime", ranaSetTimerPeriodTime);
	lua_register(m_state, "RanaSetTimerRepeat", ranaSetTimerRepeat);
	lua_register(m_state, "RanaSetTimerOnOff", ranaSetTimerOnOff);
	lua_register(m_state, "RanaResetTimer", ranaResetTimer);
	lua_register(m_state, "RanaSetSpaceEventOnOff", ranaSetSpaceEventOnOff);
	lua_register(m_state, "RanaSetTypeEventOnOff", ranaSetTypeEventOnOff);
	lua_register(m_state, "RanaSetTriggerEventOnOff", ranaSetTriggerEventOnOff);
*/
}

/*void RanaSpace::registerCustomCFunction()
{
	if (m_pfCustomCFuncRegCallBk)
	{
		CFunctionRegister obj_register(m_state);
		m_pfCustomCFuncRegCallBk(obj_register);
	}
}
*/

bool RanaSpace::checkLuaState(RanaState _S)
{
	return (m_state == _S);
}

int RanaSpace::runFunction(const char* funcName,int argc,RanaVariant *argv)
{
	DEBUG_CHECK_STACK(m_state);
	lua_getglobal(m_state, funcName);
	for(int i=0;i<argc;i++)
	{
		rana_push(m_state,*argv++);
	}
	if (lua_pcall(m_state, argc, 0, 0) != 0)
	{
		RanaSpaceBank::getInstance()->setSpaceExit(getHandle(), EC_RUNFUCTION);
		ranaError("error running function '%s': %s",
			funcName,lua_tostring(m_state, -1));
		lua_pop(m_state, 1);
		return ERROR_NO_SUCH_FUNCTION;
	}

	return RESULT_OK;
}

int RanaSpace::runFunction(const char* funcName,int argc,RanaVariant *argv, bool* res)
{
	DEBUG_CHECK_STACK(m_state);
	lua_getglobal(m_state, funcName);
	for(int i=0;i<argc;i++)
	{
		rana_push(m_state,*argv++);
	}
	if (lua_pcall(m_state, argc, 1, 0) != 0)
	{
		RanaSpaceBank::getInstance()->setSpaceExit(getHandle(), EC_RUNFUCTION);
		ranaError("error running function '%s': %s",
			funcName,lua_tostring(m_state, -1));
		lua_pop(m_state, 1);
		return ERROR_NO_SUCH_FUNCTION;
	}
	if (!lua_isboolean(m_state, -1))
	{
        ranaError("function %s must return a boolean",funcName);
		return ERROR_WRONG_RETURN_TYPE;
	}
	*res = lua_toboolean(m_state, -1)==0?false:true;
	lua_pop(m_state, 1);  /* pop returned value */
	return RESULT_OK;
}

int RanaSpace::runFunction(const char* funcName,int argc,RanaVariant *argv, double* res)
{
	DEBUG_CHECK_STACK(m_state);
	lua_getglobal(m_state, funcName);
	for(int i=0;i<argc;i++)
	{
		rana_push(m_state,*argv++);
	}
	if (lua_pcall(m_state, argc, 1, 0) != 0)
	{
		RanaSpaceBank::getInstance()->setSpaceExit(getHandle(),EC_RUNFUCTION);
		ranaError("error running function '%s': %s",
			funcName,lua_tostring(m_state, -1));
		lua_pop(m_state, 1);
		return ERROR_NO_SUCH_FUNCTION;
	}
	if (!lua_isnumber(m_state, -1))
	{
		ranaError("function %s must return a number",funcName);
		return ERROR_WRONG_RETURN_TYPE;
	}
	*res = lua_tonumber(m_state, -1);
	lua_pop(m_state, 1);  /* pop returned value */
	return RESULT_OK;
}

int RanaSpace::runFunction(const char* funcName,int argc,RanaVariant *argv, char* res, int len)
{
	DEBUG_CHECK_STACK(m_state);
	lua_getglobal(m_state, funcName);
	for(int i=0;i<argc;i++)
	{
		rana_push(m_state,*argv++);
	}
	if (lua_pcall(m_state, argc, 1, 0) != 0)
	{
		RanaSpaceBank::getInstance()->setSpaceExit(getHandle(),EC_RUNFUCTION);
		ranaError("error running function '%s': %s",
			funcName,lua_tostring(m_state, -1));
		lua_pop(m_state, 1);
		return ERROR_NO_SUCH_FUNCTION;
	}
	if (!lua_isstring(m_state, -1))
	{
		ranaError("function %s must return a string",funcName);
		return ERROR_WRONG_RETURN_TYPE;
	}
	strncpy(res, lua_tostring(m_state, -1),len);
	res[len-1]=0;
	lua_pop(m_state, 1);  /* pop returned value */
	return RESULT_OK;
}

int RanaSpace::runFunction(const char* funcName,int argc,RanaVariant *argv,RanaVariant& resv)
{
	DEBUG_CHECK_STACK(m_state);
	lua_getglobal(m_state, funcName);
	for(int i=0;i<argc;i++)
	{
		rana_push(m_state,*argv++);
	}
	if (lua_pcall(m_state, argc, 1, 0) != 0)
	{
		RanaSpaceBank::getInstance()->setSpaceExit(getHandle(),EC_RUNFUCTION);
		ranaError("error running function '%s': %s",
			funcName,lua_tostring(m_state, -1));
		lua_pop(m_state, 1);
		return ERROR_NO_SUCH_FUNCTION;
	}
	switch( lua_type(m_state,-1) )
	{
	case LUA_TNIL:
		resv.clear();
		break;
	case LUA_TNUMBER:
		resv = static_cast<int>( lua_tonumber(m_state, -1) );
		break;
	case LUA_TSTRING:
		resv = lua_tostring(m_state, -1);
		break;
	case LUA_TBOOLEAN:
		resv = lua_toboolean(m_state, -1);
		break;
	default:
		ranaError("function %s must return simple type",funcName);
		return ERROR_WRONG_RETURN_TYPE;
	}
	lua_pop(m_state, 1);  /* pop returned value */
	return RESULT_OK;
}

int RanaSpace::runFunction(const char* funcName,int argc,RanaVariant *argv,RanaValues& res)
{
	DEBUG_CHECK_STACK(m_state);
	lua_getglobal(m_state, funcName);
	for(int i=0;i<argc;i++)
	{
		rana_push(m_state,*argv++);
	}
	if (lua_pcall(m_state, argc, 1, 0) != 0)
	{
		RanaSpaceBank::getInstance()->setSpaceExit(getHandle(),EC_RUNFUCTION);
		ranaError("error running function '%s': %s",
			funcName,lua_tostring(m_state, -1));
		lua_pop(m_state, 1);
		return ERROR_NO_SUCH_FUNCTION;
	}
	argc = res.getOutputs(m_state);
	if(argc < 0)
	{
		ranaError("function %s return data failed");
		return ERROR_WRONG_RETURN_TYPE;
	}
	for(;argc>0;--argc)
	{
		lua_pop(m_state, 1);
	}
	return RESULT_OK;
}

int RanaSpace::runFunction(const char* funcName,const RanaSpace::RanaValues& args)
{
	DEBUG_CHECK_STACK(m_state);
	lua_getglobal(m_state, funcName);
	int argc = args.pushInputs(m_state);
	if(argc < 0)
	{
		return ERROR_WRONG_PARAM_TYPE;
	}
	if (lua_pcall(m_state, argc, 0, 0) != 0)
	{
		RanaSpaceBank::getInstance()->setSpaceExit(getHandle(),EC_RUNFUCTION);
		ranaError("error running function '%s': %s",
			funcName,lua_tostring(m_state, -1));
		lua_pop(m_state, 1);
		return ERROR_NO_SUCH_FUNCTION;
	}
	return RESULT_OK;
}

int RanaSpace::runFunction(const char* funcName,const RanaSpace::RanaValues& args,bool* res)
{
	DEBUG_CHECK_STACK(m_state);
	lua_getglobal(m_state, funcName);
	int argc = args.pushInputs(m_state);
	if(argc < 0)
	{
		return ERROR_WRONG_PARAM_TYPE;
	}
	if (lua_pcall(m_state, argc, 1, 0) != 0)
	{
		RanaSpaceBank::getInstance()->setSpaceExit(getHandle(),EC_RUNFUCTION);
		ranaError("error running function '%s': %s",
			funcName,lua_tostring(m_state, -1));
		lua_pop(m_state, 1);
		return ERROR_NO_SUCH_FUNCTION;
	}
	if (!lua_isboolean(m_state, -1))
	{
		ranaError("function %s must return a bool",funcName);
		return ERROR_WRONG_RETURN_TYPE;
	}
	*res = lua_toboolean(m_state, -1)==0?false:true;
	lua_pop(m_state, 1);  /* pop returned value */
	return RESULT_OK;
}

int RanaSpace::runFunction(const char* funcName,const RanaSpace::RanaValues& args,double* res)
{
	DEBUG_CHECK_STACK(m_state);
	lua_getglobal(m_state, funcName);
	int argc = args.pushInputs(m_state);
	if(argc < 0)
	{
		return ERROR_WRONG_PARAM_TYPE;
	}
	if (lua_pcall(m_state, argc, 1, 0) != 0)
	{
		RanaSpaceBank::getInstance()->setSpaceExit(getHandle(),EC_RUNFUCTION);
		ranaError("error running function '%s': %s",
			funcName,lua_tostring(m_state, -1));
		lua_pop(m_state, 1);
		return ERROR_NO_SUCH_FUNCTION;
	}
	if (!lua_isnumber(m_state, -1))
	{
		ranaError("function %s must return a number",funcName);
		return ERROR_WRONG_RETURN_TYPE;
	}
	*res = lua_tonumber(m_state, -1);
	lua_pop(m_state, 1);  /* pop returned value */
	return RESULT_OK;
}

int RanaSpace::runFunction(const char* funcName,const RanaSpace::RanaValues& args,char* res, int len)
{
	DEBUG_CHECK_STACK(m_state);
	lua_getglobal(m_state, funcName);
	int argc = args.pushInputs(m_state);
	if(argc < 0)
	{
		return ERROR_WRONG_PARAM_TYPE;
	}
	if (lua_pcall(m_state, argc, 1, 0) != 0)
	{
		RanaSpaceBank::getInstance()->setSpaceExit(getHandle(),EC_RUNFUCTION);
		ranaError("error running function '%s': %s",
			funcName,lua_tostring(m_state, -1));
		lua_pop(m_state, 1);
		return ERROR_NO_SUCH_FUNCTION;
	}
	if (!lua_isstring(m_state, -1))
	{
		ranaError("function %s must return a string",funcName);
		return ERROR_WRONG_RETURN_TYPE;
	}
	strncpy(res, lua_tostring(m_state, -1),len);
	res[len-1]=0;
	lua_pop(m_state, 1);  /* pop returned value */
	return RESULT_OK;
}

int RanaSpace::runFunction(const char* funcName,const RanaValues& args,RanaValues& res)
{
	DEBUG_CHECK_STACK(m_state);
	lua_getglobal(m_state, funcName);
	int argc = args.pushInputs(m_state);
	if(argc < 0)
	{
		return ERROR_WRONG_PARAM_TYPE;
	}
	if (lua_pcall(m_state, argc, 1, 0) != 0)
	{
		RanaSpaceBank::getInstance()->setSpaceExit(getHandle(),EC_RUNFUCTION);
		ranaError("error running function '%s': %s",
			funcName,lua_tostring(m_state, -1));
		lua_pop(m_state, 1);
		return ERROR_NO_SUCH_FUNCTION;
	}
	argc = res.getOutputs(m_state);
	if(argc < 0)
	{
		ranaError("function %s return data failed");
		return ERROR_WRONG_RETURN_TYPE;
	}
	for(;argc>0;--argc)
	{
		lua_pop(m_state, 1);
	}
	return RESULT_OK;
}

int RanaSpace::runFunction(const char* funcName,const RanaValues& args,RanaVariant& resv)
{
	DEBUG_CHECK_STACK(m_state);
	lua_getglobal(m_state, funcName);
	int argc = args.pushInputs(m_state);
	if(argc < 0)
	{
		return ERROR_WRONG_PARAM_TYPE;
	}
	if (lua_pcall(m_state, argc, 1, 0) != 0)
	{
		RanaSpaceBank::getInstance()->setSpaceExit(getHandle(),EC_RUNFUCTION);
		ranaError("error running function '%s': %s",
			funcName,lua_tostring(m_state, -1));
		lua_pop(m_state, 1);
		return ERROR_NO_SUCH_FUNCTION;
	}
	switch( lua_type(m_state,-1) )
	{
	case LUA_TNIL:
		resv.clear();
		break;
	case LUA_TNUMBER:
		resv = static_cast<int>( lua_tonumber(m_state, -1) );
		break;
	case LUA_TSTRING:
		resv = lua_tostring(m_state, -1);
		break;
	case LUA_TBOOLEAN:
		resv = lua_toboolean(m_state, -1);
		break;
	default:
		ranaError("function %s must return simple type",funcName);
		return ERROR_WRONG_RETURN_TYPE;
	}
	lua_pop(m_state, 1);  /* pop returned value */
	return RESULT_OK;
}

/*
void RanaSpace::setContext(RanaContext *context)
{
	if (m_context)
	{
		releaseContext();
	}
	m_context = context;
}

const RanaContext* RanaSpace::getContext()
{
	return m_context;
}
*/
void RanaSpace::releaseValueTable()
{
//	if (m_context)
//	{
//		delete m_context;
//	}
//	m_context = NULL;
	//2006-09-20 clear contextdata
/*	m_BoolContext.clear();
	m_StringContext.clear();
	m_DoubleContext.clear();*/
}

void RanaSpace::setBoolean(const char* key ,bool value)
{
	(*m_puttingVariantMap)[key] = value;
}

void RanaSpace::setString(const char* key ,const char* value)
{
	(*m_puttingVariantMap)[key] = value;
}

void RanaSpace::setNumber(const char* key ,int value)
{
	(*m_puttingVariantMap)[key] = value;
}

void RanaSpace::setUserData(const char* key ,void * value)
{
	(*m_puttingVariantMap)[key] = value;
}

bool RanaSpace::getBoolean(const char* key)
{
	VARIANT_MAP& vMap = *( m_VMapStack.top() );
	VARIANT_MAP::iterator it = vMap.find(key);
	if(it != vMap.end())
	{
		if( it->second.getType() == RanaVariant::VT_BOOL )
		{
			return it->second;
		}
	}
	return false;
}

const char* RanaSpace::getString(const char* key)
{
	VARIANT_MAP& vMap = *( m_VMapStack.top() );
	VARIANT_MAP::iterator it = vMap.find(key);
	if(it != vMap.end())
	{
		if( it->second.getType() == RanaVariant::VT_STRING )
		{
			return it->second;
		}
	}
	return NULL;
}

int RanaSpace::getNumber (const char* key)
{
	VARIANT_MAP& vMap = *( m_VMapStack.top() );
	VARIANT_MAP::iterator it = vMap.find(key);
	if(it != vMap.end())
	{
		if( it->second.getType() == RanaVariant::VT_INTEGER )
		{
			return int(it->second);
		}
	}
	return 0;
}

void * RanaSpace::getUserData(const char* key)
{
	VARIANT_MAP& vMap = *( m_VMapStack.top() );
	VARIANT_MAP::iterator it = vMap.find(key);
	if(it != vMap.end())
	{
		if( it->second.getType() == RanaVariant::VT_USERDATA)
		{
			return it->second.body.vVal;
		}
	}
	return NULL;
}

const RanaVariant * RanaSpace::getContext(const char* key)
{
	VARIANT_MAP& vMap = *( m_VMapStack.top() );
	VARIANT_MAP::iterator it = vMap.find(key);
	if(it != vMap.end())
	{
		return &it->second;
	}
	return NULL;
}

const char* RanaSpace::getFileName()
{
	return m_FileName.c_str();
}
/*
int RanaSpace::showFunction(const char* funcName, char* retBuf, int len)
{
	return 0;
}
*/

int RanaSpace::runBuf(const char* script)
{
	DEBUG_CHECK_STACK(m_state);
	int retVal = luaL_dobuffer(m_state,script,strlen(script),m_SpaceName);
	if ( retVal != RANA_OK )
	{
		ranaError("running script error: %s\n:\nScript content: %s",
			lua_tostring(m_state, -1),script);
		lua_pop(m_state, 1);
	}
	return retVal;
}

int RanaSpace::runBuf(const char* script, unsigned long nLength)
{
	DEBUG_CHECK_STACK(m_state);
	int retVal = luaL_dobuffer(m_state,script,nLength,m_SpaceName);
	if ( retVal != RANA_OK )
	{
		std::string bufString(script,nLength);
		ranaError("running script error: %s\n:\nScript content: %s",
			lua_tostring(m_state, -1),bufString.c_str() );
		lua_pop(m_state, 1);
	}
	return retVal;
}

void RanaSpace::onMessage(RanaMessage *_msg)
{
	m_VMapStack.push(m_puttingVariantMap);
	if(m_unusedVMaps.size())
	{
		m_puttingVariantMap = m_unusedVMaps.front();
		m_unusedVMaps.pop_front();
	}
	else
	{
		m_puttingVariantMap = new VARIANT_MAP;
	}
	getTriggerBank()->onMessage(_msg);
	m_puttingVariantMap->clear();
	m_unusedVMaps.push_back(m_puttingVariantMap);
	m_puttingVariantMap = m_VMapStack.top();
	m_VMapStack.pop();
	m_puttingVariantMap->clear();
}
