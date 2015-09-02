#ifndef __RANA_SPACE_H__
#define __RANA_SPACE_H__

#include <map>
#include <stack>
#include <list>
#include <string>
#include "RanaTriggerBank.h"
#include "RanaFeedback.h"

typedef RanaTriggerBank RanaEventBank;

class RanaContext;
class RanaSpace
{
	friend class RanaSpaceBank;
public:
	typedef RanaCOUNT (*RANA_VALUE_FUNCTION)(RanaState L,void* context);//返回值:实际进行了操作的值的个数
	class RanaValues
	{
	public:
		inline RanaCOUNT pushInputs(RanaState L) const
		{
			return push_datas(L);
		}
		inline RanaCOUNT getOutputs(RanaState L)
		{
			return get_datas(L);
		}
		virtual ~RanaValues(){};
	protected:
		virtual RanaCOUNT push_datas(RanaState L) const
		{
			RANA_ASSERT(0);
			return 0;
		}
		virtual RanaCOUNT get_datas(RanaState L)
		{
			RANA_ASSERT(0);
			return 0;
		}
	};
	enum EXCEPTION_CODE
	{
		EC_SUCCESS,
		EC_ADMINISTRATOR,
		EC_RUNFUCTION,
		EC_RUNBUFFER,
	};
	enum EXCEPTION_METHOD
	{
		EM_DELSPACE,
		EM_CUSTOM,
	};
	typedef RanaBYTE SpaceType;
	class CFunctionItemInit
	{
	public:
		CFunctionItemInit(unsigned char type,const char *libname,const char *name,int	(*func)(RanaState));
	};

	class CFunctionRegister
	{
	public:
		CFunctionRegister(RanaState _S)
		{
			m_state = _S;
		}

		void registerRanaCallBack(char *funcname, int(*callback)(RanaState _S))
		{
			lua_register(m_state, funcname, callback);
		}

	private:
		RanaState m_state;
	};

	enum ErrorCode
	{
		RESULT_OK=0,
		EMPTYFILENAME,
		ERROR_NO_SUCH_FUNCTION,
		ERROR_WRONG_RETURN_TYPE,
		ERROR_WRONG_PARAM_TYPE,
		ERROR_NOTEXISTFILE,
		ERROR_STATE,
		ERROR_MAINFUNCTION,
	};
	class CmpStr
	{
	public:
		bool operator()(const char *a,const char *b) const
		{
			return strcmp(a,b) < 0;
		}
	};
	typedef std::map<const char*,RanaVariant,CmpStr> VARIANT_MAP;

	RanaSpace();
	~RanaSpace();
	int initialEmptySpace(RanaSpaceName rsn,unsigned char type = 0,RanaContext * context = NULL);
	int initial(const char* fname,RanaSpaceName rsn,unsigned char type = 0,RanaContext * context = NULL);
	void clear();
	bool checkLuaState(RanaState s); //用来检查lua_State是否和成员的lua_State相同
	bool isInitialed() const;
	int runFunction(const char* funcName,int argc,RanaVariant *argv);
	int runFunction(const char* funcName,int argc,RanaVariant *argv,bool* res);
	int runFunction(const char* funcName,int argc,RanaVariant *argv,double* res);
	int runFunction(const char* funcName,int argc,RanaVariant *argv,char* res, int len=255);
	int runFunction(const char* funcName,int argc,RanaVariant *argv,RanaVariant& resv);
	int runFunction(const char* funcName,int argc,RanaVariant *argv,RanaValues& res);
	int runFunction(const char* funcName,const RanaValues& args);
	int runFunction(const char* funcName,const RanaValues& args,bool* res);
	int runFunction(const char* funcName,const RanaValues& args,double* res);
	int runFunction(const char* funcName,const RanaValues& args,char* res, int len=255);
	int runFunction(const char* funcName,const RanaValues& args,RanaValues& res);
	int runFunction(const char* funcName,const RanaValues& args,RanaVariant& resv);
	int runFunction(const char* funcName);
	int runFunction(const char* funcName,bool* res);
	int runFunction(const char* funcName,double* res);
	int runFunction(const char* funcName,char* res, int len=255);
	int runCode(const char* code);
	int runFile(const char* fname);
	//void setContext(RanaContext *context);
	//const RanaContext* getContext();
	void releaseValueTable(); // 释放Lua上下文环境空间
	//int getID();
	//void setID(int aID);

	//2006-09-20 改进上下文
	bool getBoolean(const char* key);
	const char* getString(const char* key);
	int getNumber(const char* key);
	void * getUserData(const char* key);
	void setBoolean(const char *key,bool value);
	void setNumber(const char *key,int value);
	void setString(const char *key,const char * value);
	void setUserData(const char *key,void * value);
	const RanaVariant * getContext(const char *key);

	//句柄操作
	const RanaSpaceHandle& getHandle();
	unsigned long getSerial();
	RanaSpaceName getSpaceName();
	const char* getFileName();
	SpaceType getSpaceType();
	RanaTriggerBank* getTriggerBank();
	RanaEventBank* getEventBank();

//	int showFunction(const char* funcName, char* retBuf, int len = 0);		//Deleted: Function Content Test
	int runBuf(const char* script);
	int runBuf(const char* script, unsigned long nLength);

//	const char* getLoadedFileBuff();					//Deleted: Function Content Test
//	unsigned long getLoadedFileLength();
//	const char* getLinePostion(RanaWORD nLine);
//	int getFunctionContentInFileBuff(const char* pFuncName, const char* &pRetPos, unsigned long &nRetLen);

//	int testBuf(const char* script, unsigned long nLength);
//	void clearTestField();

//	int getFunctionInfo(const char* pFuncName, int indexType, const char *what, lua_Debug *ar);

	RanaContext* getContext();
	// 注册一个事件/触发器
	int registerEvent(RanaEvent *_event);
	// 触发事件
	void onMessage(RanaMessage *_msg);
	//
	void setContext(RanaContext* pContext);

	void trackTriggerLog(bool isTrack,unsigned int lagErrorLimit = 0);

	void setHook(RanaFeedback* pHook);
	RanaFeedback* getHook() const;
#ifdef _DEBUG
	bool checkState(RanaState L){return m_state == L;};
#endif
private:
	void clearTriggerLogSetting();
	int initialFromFileForEmptySpace(const char* fname);
	void registerCFunction();
//	void registerCustomCFunction();
	//int m_id;
	RanaState m_state;
//	void (*m_pfCustomCFuncRegCallBk)(CFunctionRegister &ref_register);
	//RanaContext *m_context; // Lua上下文环境
/*	std::map<std::string,const char*> m_StringContext;
	std::map<std::string,bool> m_BoolContext;
	std::map<std::string,double> m_DoubleContext;
*/
	std::list<VARIANT_MAP*> m_unusedVMaps;
	VARIANT_MAP* m_puttingVariantMap;
	std::stack<VARIANT_MAP*> m_VMapStack;
	RanaSpaceName m_SpaceName;
	SpaceType m_SpaceType;
	RanaSpaceHandle m_spaceHandle;
	std::string m_FileName;
	RanaContext * m_context;

	RanaTriggerBank& m_triggerBank;

//	static RanaState m_testField;		//Deleted: Function Content Test
//	static char* m_pFileBuff;
//	static unsigned long m_FileLength;
//	static std::map<RanaWORD, const char*> m_lineMap;

	RanaFeedback* m_hook;
	EXCEPTION_CODE m_errorCode;
	std::list<RanaSpace*>::iterator m_itErrorList;
};

inline RanaTriggerBank* RanaSpace::getTriggerBank()
{
	return &m_triggerBank;
}

inline RanaEventBank* RanaSpace::getEventBank()
{
	return getTriggerBank();
}

inline const RanaSpaceHandle& RanaSpace::getHandle()
{
	return m_spaceHandle;
}

inline RanaSpaceName RanaSpace::getSpaceName()
{
	return m_SpaceName;
}

inline unsigned char RanaSpace::getSpaceType()
{
	return m_SpaceType;
}

inline RanaContext* RanaSpace::getContext()
{
	return m_context;
}

inline void RanaSpace::setContext(RanaContext* pContext)
{
	m_context = pContext;
}

//inline int RanaSpace::registerEvent(RanaEvent *_event)
//{
//	return getEventBank()->registerEvent(_event);
//}

inline bool RanaSpace::isInitialed() const
{
	return m_state ? true : false;
}

inline int RanaSpace::runFunction(const char* funcName)
{
	return runFunction(funcName,0,static_cast<RanaVariant *>(NULL));
}

inline int RanaSpace::runFunction(const char* funcName,bool* res)
{
	return runFunction(funcName,0,NULL,res);
}

inline int RanaSpace::runFunction(const char* funcName,double* res)
{
	return runFunction(funcName,0,NULL,res);
}

inline int RanaSpace::runFunction(const char* funcName,char* res, int len)
{
	return runFunction(funcName,0,NULL,res,len);
}

inline RanaSpace * GetRanaSpace(RanaState state)
{
	void * space;
	lua_getallocf(state,&space);
#ifdef _DEBUG
	RanaSpace * pRanaSpace = static_cast<RanaSpace *>(space);
	RANA_ASSERT( pRanaSpace->checkState(state) );
#endif
	return static_cast<RanaSpace *>(space);
}

#define REG_RANA_LIB_FUNC_NAME(TYPE,LIB,NAME,FUNC) \
	extern int l_##FUNC(RanaState); \
	static RanaSpace::CFunctionItemInit _init_type##TYPE##_##NAME(TYPE,#LIB,#NAME,l_##FUNC);

#define REG_RANA_FUNC_NAME(TYPE,NAME,FUNC) \
	extern int l_##FUNC(RanaState); \
	static RanaSpace::CFunctionItemInit _init_type##TYPE##_##NAME(TYPE,NULL,#NAME,l_##FUNC);

#define REG_RANA_FUNC(TYPE,NAME) \
	extern int l_##NAME(RanaState); \
	static RanaSpace::CFunctionItemInit _init_type##TYPE##_##NAME(TYPE,NULL,#NAME,l_##NAME);

#define REG_RANA_TIMER(TYPE) \
	static RanaSpace::CFunctionItemInit _init_type##TYPE##_RegisterTimeEvent(TYPE,NULL,"RanaRegisterTimeEvent",ranaRegisterTimeEvent); \
	static RanaSpace::CFunctionItemInit _init_type##TYPE##_CreateTimer(TYPE,NULL,"RanaCreateTimer",ranaCreateTimer); \
	static RanaSpace::CFunctionItemInit _init_type##TYPE##_SetTimerPeriodTime(TYPE,NULL,"RanaSetTimerPeriodTime",ranaSetTimerPeriodTime); \
	static RanaSpace::CFunctionItemInit _init_type##TYPE##_SetTimerRepeat(TYPE,NULL,"RanaSetTimerRepeat",ranaSetTimerRepeat); \
	static RanaSpace::CFunctionItemInit _init_type##TYPE##_SetTimerOnOff(TYPE,NULL,"RanaSetTimerOnOff",ranaSetTimerOnOff); \
	static RanaSpace::CFunctionItemInit _init_type##TYPE##_SetResetTimer(TYPE,NULL,"RanaResetTimer",ranaResetTimer);

#define REG_RANA_EVENT(TYPE) \
	static RanaSpace::CFunctionItemInit _init_type##TYPE##_SetSpaceEventOnOff(TYPE,NULL,"RanaSetSpaceEventOnOff",ranaSetSpaceEventOnOff); \
	static RanaSpace::CFunctionItemInit _init_type##TYPE##_SetSetTypeEventOnOff(TYPE,NULL,"RanaSetTypeEventOnOff",ranaSetTypeEventOnOff); \
	static RanaSpace::CFunctionItemInit _init_type##TYPE##_SetTriggerEventOnOff(TYPE,NULL,"RanaSetTriggerEventOnOff",ranaSetTriggerEventOnOff); \
	static RanaSpace::CFunctionItemInit _init_type##TYPE##_SetEventOnOff(TYPE,NULL,"RanaSetEventOnOff",ranaSetEventOnOff);

#define REG_RANA_TRIGGER(TYPE) \
	static RanaSpace::CFunctionItemInit _init_type##TYPE##_SetTriggerOnOff(TYPE,NULL,"RanaSetTriggerOnOff",ranaSetTriggerOnOff);

#define REG_RANA_CONTEXT(TYPE) \
	REG_RANA_FUNC(TYPE,getContextBool) \
	REG_RANA_FUNC(TYPE,getContextString) \
	REG_RANA_FUNC(TYPE,getContextNumber) \
	REG_RANA_FUNC(TYPE,getContext)

#define REG_RANA_MYSQL(TYPE) \
	REG_RANA_FUNC(TYPE,mysql_query)

#endif //__RANA_SPACE_H__

