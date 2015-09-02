#ifndef __RANA_SPACEBANK_H__
#define __RANA_SPACEBANK_H__

#pragma warning (disable : 4786)

#include <set>
#include <map>
#include <list>
#include "RanaSpace.h"

class RanaSpaceBank
{
	friend class RanaSpace;
public:
	typedef std::map<RanaSpaceName,RanaSpace*> RANA_NAMEMAP;
	typedef std::map<RanaBYTE,RANA_NAMEMAP> RANA_TYPEMAP;//空间类型的管理
	typedef void (*LOGFUNC)(const char* pChar);

	static RanaSpaceBank* getInstance();
	static void release();

	// 创建一个RanaSpace
	RanaSpace* createSpace(RanaSpaceName spaceName, 
		const char* fileName, 
		RanaBYTE type = 0,
		RanaContext * context = NULL,
		bool forceCreate = false	//是否在文件不存在时强制生成
		);
	// 删除一个RanaSpace
	int delSpace(const RanaSpaceHandle& spaceHandle);
	// 重载一个RanaSpace
	int reloadSpace(const RanaSpaceHandle& spaceHandle, 
		const char* fileName,
		bool deleteFailed = false );
	// 获得一个RanaSpace
	RanaSpace* getLuaSpace(const RanaSpaceHandle& spaceHandle);
	void ActiveSpaceMessage( RanaBYTE eSpaceType, RanaMessage *msg, const char * spaceName = NULL);
//	RanaSpace* getLuaSpace(const RanaSpaceName& spaceName);

	void routine();


	static RanaSpaceName getCurrentSpaceName();

	void initMaxSpaces(RanaWORD maxSpaces);

	int getSpaceCount( RanaWORD type );
	
	const std::map<RanaSpaceName,RanaSpace*>& listSpaces( RanaWORD type );
	const std::list<RanaSpace*>& listAllSpaces() const;

	void setLogFunc( LOGFUNC logFunc );
	void setErrorFunc( LOGFUNC errorFunc );

	void printLog(const char* pChar);
	void printError(const char* pChar);

	RanaSpace* forceFindSpaceByIndex(unsigned short index);

	void forceDisableTrackLogTrigger(bool isDisable);
	bool isAllowTrackTrigger() const;
	RanaWORD getMaxSpaceLimit() const;
private:
	RanaSpaceBank();
	~RanaSpaceBank();

	static RanaSpaceBank *INSTANCE;

	RanaWORD m_maxSpaces;
	RanaSpace ** m_RanaSpaces;
	std::list<RanaSpace*> m_unusedSpaces;
	std::list<RanaSpace*> m_activeSpaces;
	RanaDWORD	m_currentSerial;
	RANA_TYPEMAP m_SpaceMap;
	static RanaSpaceName m_currentSpaceName;
	const RANA_NAMEMAP m_emptyList;
	LOGFUNC m_logFunc;
	LOGFUNC m_errorFunc;

	void setSpaceExit(const RanaSpaceHandle& spaceHandle, RanaSpace::EXCEPTION_CODE exitCode);
	std::list<RanaSpace*> m_exitSpaces;

	bool m_allowTrackTrigger;
};

#define ACTIVE_EVENT(eSpaceType, msg)\
	RanaSpaceBank::getInstance()->ActiveSpaceMessage(eSpaceType, msg);
#define CREATE_SPACE(spaceName, fileName, type)\
	RanaSpaceBank::getInstance()->createSpace(spaceName, fileName, type, NULL);

#endif //__RANA_SPACEMANAGER_H__
