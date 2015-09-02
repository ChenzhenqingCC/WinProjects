#pragma warning(disable:4996)

#include "RanaSpaceBank.h"
#include "RanaTimerBank.h"
#include "RanaContext.h"

RanaSpaceBank* RanaSpaceBank::INSTANCE = NULL;
//RanaSpaceName RanaSpaceBank::m_currentSpaceName;

RanaSpaceBank::RanaSpaceBank()
	:m_RanaSpaces(NULL)
	,m_logFunc(NULL)
	,m_errorFunc(NULL)
	,m_currentSerial(0)
#ifdef _DEBUG
	,m_allowTrackTrigger(true)
#else
	,m_allowTrackTrigger(false)
#endif
{
}

RanaSpaceBank::~RanaSpaceBank()
{
	initMaxSpaces(0);
	if (m_RanaSpaces)
	{
		delete [] m_RanaSpaces;
	}
}

void RanaSpaceBank::initMaxSpaces(RanaWORD maxSpaces)
{
	if(m_activeSpaces.size())
	{
		for(std::list<RanaSpace*>::iterator it = m_activeSpaces.begin();it!=m_activeSpaces.end();it = m_activeSpaces.begin())
		{
			RanaSpace* pRanaSpace = *it;
			m_activeSpaces.erase(it);
			const RanaSpaceHandle& spaceHandle = pRanaSpace->getHandle();
			delSpace(spaceHandle);
		}
		RANA_ASSERT(m_activeSpaces.empty());
	}
	if(m_unusedSpaces.size())
	{
		for(std::list<RanaSpace*>::iterator it = m_unusedSpaces.begin();it!=m_unusedSpaces.end();it = m_unusedSpaces.begin())
		{
			RanaSpace* pRanaSpace = *it;
			m_unusedSpaces.erase(it);
			delete pRanaSpace;
		}
		RANA_ASSERT(m_unusedSpaces.empty());
	}
	m_SpaceMap.clear();
	if(m_RanaSpaces){
		delete [] m_RanaSpaces;
		m_RanaSpaces = NULL;
	}
	m_maxSpaces = maxSpaces;
	m_RanaSpaces = new RanaSpace*[m_maxSpaces];
	for(RanaWORD i=0;i<m_maxSpaces;i++)
	{
		RanaSpace* pRanaSpace = new RanaSpace;
		pRanaSpace->m_spaceHandle.handle = i;
		m_unusedSpaces.push_back(pRanaSpace);
		m_RanaSpaces[i] = NULL;
	}
}

RanaWORD RanaSpaceBank::getMaxSpaceLimit() const
{
	return m_maxSpaces;
}

RanaSpaceBank* RanaSpaceBank::getInstance()
{
	if (!INSTANCE)
	{
		INSTANCE = new RanaSpaceBank;
	}
	return INSTANCE;
}

void RanaSpaceBank::release()
{
	if (INSTANCE)
	{
		delete INSTANCE;
		INSTANCE = NULL;
	}
}

RanaSpace* RanaSpaceBank::forceFindSpaceByIndex(unsigned short index)
{
	if(index>=m_maxSpaces)
	{
		return NULL;
	}
	return m_RanaSpaces[index];
}

// 获得一个RanaSpace
RanaSpace* RanaSpaceBank::getLuaSpace(const RanaSpaceHandle& spaceHandle)
{
	if( !forceFindSpaceByIndex(spaceHandle.handle) )
	{
		return NULL;
	}
	if( m_RanaSpaces[spaceHandle.handle]->m_spaceHandle.serial != spaceHandle.serial )
	{
		return NULL;
	}
	return m_RanaSpaces[spaceHandle.handle];
}

//向空间发消息
void RanaSpaceBank::ActiveSpaceMessage( RanaBYTE eSpaceType, RanaMessage *msg, const char * spaceNameChar)
{
	RANA_TYPEMAP::iterator itmap = m_SpaceMap.find(eSpaceType);
	if(itmap != m_SpaceMap.end())
	{
		RANA_NAMEMAP* pNameMap = &(itmap->second);
		if(spaceNameChar)
		{
			RanaSpaceName spaceName = spaceNameChar;
			RANA_NAMEMAP::iterator itname = pNameMap->find(spaceName);
			if(itname != pNameMap->end())
			{
				itname->second->onMessage( msg );
			}
			else
			{
				debug_ranaLog("RanaSpaceBank::ActiveSpaceMessage : can not find space [%s]", spaceName);
			}
		}
		else
		{
			for (RANA_NAMEMAP::iterator itname = pNameMap->begin();itname != pNameMap->end(); ++itname)
			{
				itname->second->onMessage( msg );
			}
		}
	}
	else
	{
		debug_ranaLog("RanaSpaceBank::ActiveSpaceMessage : can not find space which type is [%d]", eSpaceType);
	}
}

// 创建一个RanaSpace
RanaSpace* RanaSpaceBank::createSpace(RanaSpaceName spaceName, const char* fileName, RanaBYTE type, RanaContext * context,bool forceCreate)
{
	if(fileName == NULL)
	{
		debug_ranaLog("RanaSpaceBank::createSpace : filname error");
		return NULL;
	}
	if( !m_unusedSpaces.size() )
	{
		debug_ranaLog("RanaSpaceBank::createSpace : spaces full");
		return NULL;
	}
	RANA_TYPEMAP::iterator itmap = m_SpaceMap.find(type);
	if(itmap != m_SpaceMap.end())
	{
		RANA_NAMEMAP* pNameMap = &(itmap->second);
		RANA_NAMEMAP::iterator itname = pNameMap->find(spaceName);
		if(itname != pNameMap->end())
		{
			//@todo: 名字已存在，错误处理
			char namestr[sizeof(RanaSpaceName)+1];
			strncpy(namestr, (char *)spaceName.name, sizeof(RanaSpaceName));
			namestr[sizeof(RanaSpaceName)] =  0;
			debug_ranaLog("RanaSpaceBank::createSpace : luaspace[%s] exist", namestr);
			return NULL;
		}
	}
	RanaSpace *pRanaSpace=m_unusedSpaces.front();
#ifdef _DEBUG
	std::vector<RanaTrigger*> triggers;
	pRanaSpace->getTriggerBank()->listTriggers(triggers);
	RANA_ASSERT( triggers.empty() );
#endif
	pRanaSpace->m_spaceHandle.serial = m_currentSerial++;
	const RanaSpaceHandle& spaceHandle = pRanaSpace->getHandle();
	//RanaSpaceBank::m_currentSpaceName = spaceName;
	m_unusedSpaces.pop_front();
	m_activeSpaces.push_back(pRanaSpace);
	m_SpaceMap[type][spaceName] = pRanaSpace;
	m_RanaSpaces[spaceHandle.handle] = pRanaSpace;
	int retCode = pRanaSpace->initialEmptySpace(spaceName,type,context);
	if( retCode != RanaSpace::RESULT_OK )
	{
		if( delSpace(spaceHandle) != RANA_OK )
		{
			RANA_ASSERT(0);
		}
		return NULL;
	}
	retCode = pRanaSpace->initialFromFileForEmptySpace(fileName);
	if (retCode !=RanaSpace::RESULT_OK)
	{
		if(!forceCreate)
		{
			if( delSpace(spaceHandle) != RANA_OK )
			{
				RANA_ASSERT(0);
			}
			return NULL;
		}
		pRanaSpace->clear();
		pRanaSpace->initialEmptySpace(spaceName,type,context);
		if(fileName)
		{
			pRanaSpace->m_FileName = fileName;
			ranaLog("Warning: init Space [%s] Failed For Load File \"%s\"(code %d)!Force Load Empty.",static_cast<const char*>(spaceName),fileName,retCode );
		}
	}
	ranaLog("init Space [%s] of Handle[%d] OK!!!",static_cast<const char*>(spaceName),spaceHandle.handle );
	return pRanaSpace;
}

// 删除一个RanaSpace
int RanaSpaceBank::delSpace(const RanaSpaceHandle& spaceHandle)
{
	RanaSpace* pRanaSpace = getLuaSpace(spaceHandle);
	if(!pRanaSpace)
	{
		if( m_RanaSpaces[spaceHandle.handle] )
		{
			debug_ranaLog("RanaSpaceBank::delSpace : handle[%d] serial error!", spaceHandle.handle);
		}
		else
		{
			debug_ranaLog("RanaSpaceBank::delSpace : handle[%d] not exist", spaceHandle.handle);
		}
		return -1;
	}
	RANA_ASSERT(pRanaSpace->getHandle().handle==spaceHandle.handle);
	if(pRanaSpace->m_errorCode!=RanaSpace::EC_SUCCESS)
	{
		pRanaSpace->m_errorCode = RanaSpace::EC_SUCCESS;
		m_exitSpaces.erase(pRanaSpace->m_itErrorList);
	}
	RanaSpaceName spaceName = pRanaSpace->getSpaceName();
	RanaSpace::SpaceType type = pRanaSpace->getSpaceType();

	//关闭lua空间
	pRanaSpace->clear();
	pRanaSpace->setContext(NULL);
	pRanaSpace->setHook(NULL);
	pRanaSpace->clearTriggerLogSetting();

	ranaLog("close Space of Handle[%d].",spaceHandle.handle );
	m_activeSpaces.remove(pRanaSpace);
	m_unusedSpaces.push_back(pRanaSpace);
	m_RanaSpaces[spaceHandle.handle] = NULL;
	RANA_TYPEMAP::iterator itmap = m_SpaceMap.find(type);
	if(itmap!=m_SpaceMap.end())
	{
		RANA_NAMEMAP* pNameMap = &(itmap->second);
		size_t count = pNameMap->erase(spaceName);
		if(!count)
		{
			debug_ranaError("RanaSpaceError:can not find deleting space of name \"%s\".",static_cast<const char*>(spaceName) );
			RANA_ASSERT(0);
		}
//		RANA_NAMEMAP::iterator itname = pNameMap->find(spaceName);
//		if(itname != pNameMap->end())
//		{
//			pNameMap->erase(itname);
//		}
	}
	return RANA_OK;
}

// 重载一个RanaSpace
int RanaSpaceBank::reloadSpace(const RanaSpaceHandle& spaceHandle, const char* fileName, bool deleteFailed)
{
	RanaSpace *pRanaSpace = getLuaSpace(spaceHandle);
	if(!pRanaSpace)
	{
		debug_ranaLog("RanaSpaceBank::reloadSpace : luaspace handle[%d] not exist", spaceHandle.handle);
		return -1;
	}
	RanaSpaceName spaceName = pRanaSpace->getSpaceName();
	RanaSpace::SpaceType type = pRanaSpace->getSpaceType();
//	void (*pf)(RanaSpace::CFunctionRegister &) = pRanaSpace->m_pf;
	RanaContext * context = pRanaSpace->getContext();

	pRanaSpace->clear();

	int retCode = pRanaSpace->initialEmptySpace(spaceName,type,context);
	if( retCode != RanaSpace::RESULT_OK )
	{
		ranaError("Reload Space [%s] Failed For Init File! Close Space ! File: \"%s\"(code %d)",static_cast<const char*>(spaceName),fileName,retCode );
		if( delSpace(spaceHandle) != RANA_OK )
		{
			RANA_ASSERT(0);
		}
		return -1;
	}
	retCode = pRanaSpace->initialFromFileForEmptySpace(fileName);
	if (retCode !=RanaSpace::RESULT_OK)
	{
		if(deleteFailed)
		{
			if( delSpace(spaceHandle) != RANA_OK )
			{
				RANA_ASSERT(0);
			}
			ranaError("Reload Space [%s] Failed For Load File \"%s\"(code %d)! Close Space",static_cast<const char*>(spaceName),fileName,retCode );
			return -1;
		}
		pRanaSpace->clear();
		pRanaSpace->initialEmptySpace(spaceName,type,context);
		pRanaSpace->m_FileName = fileName;
		ranaLog("Warning: Reload Space [%s] Failed For Load File \"%s\"(code %d)!Force Load Empty.",static_cast<const char*>(spaceName),fileName,retCode );
		return -1;
	}
	return 0;
}


//RanaSpaceName RanaSpaceBank::getCurrentSpaceName()
//{
//	return m_currentSpaceName;
//}

int RanaSpaceBank::getSpaceCount(RanaWORD type)
{
	RANA_TYPEMAP::iterator itmap = m_SpaceMap.find((RanaBYTE)type);
	if( itmap != m_SpaceMap.end() )
	{
		return (int)itmap->second.size();
	}
	return 0;
}

const std::map<RanaSpaceName,RanaSpace*>& RanaSpaceBank::listSpaces( RanaWORD type )
{
	RANA_TYPEMAP::iterator itmap = m_SpaceMap.find((RanaBYTE)type);
	if(itmap != m_SpaceMap.end() )
	{
		return itmap->second;
	}
	return m_emptyList;
/*	std::vector<RanaSpace*> retList;
	RANA_TYPEMAP::iterator itmap = m_SpaceMap.find(type);
	if( itmap != m_SpaceMap.end() )
	{
		RANA_NAMEMAP* pNameMap = &(itmap->second);
		RANA_NAMEMAP::iterator itName;
		for(itName = pNameMap->begin();itName!=pNameMap->end();itName++)
		{
			retList.push_back(itName->second);
		}
	}
	return retList;
	*/
}

const std::list<RanaSpace*>& RanaSpaceBank::listAllSpaces() const
{
	return m_activeSpaces;
}

void RanaSpaceBank::routine()
{
	if(m_exitSpaces.size())
	{
		std::list<RanaSpace*>::iterator it = m_exitSpaces.begin();
		for(;it!=m_exitSpaces.end();it=m_exitSpaces.begin())
		{
			RanaSpace* pRanaSpace = *it;
			const RanaSpaceHandle& spaceHandle = pRanaSpace->getHandle();
			RanaSpace::EXCEPTION_CODE errorCode = pRanaSpace->m_errorCode;
			pRanaSpace->m_errorCode = RanaSpace::EC_SUCCESS;
			m_exitSpaces.erase(it);
			if( pRanaSpace->getContext()->onRanaException(errorCode) == RanaSpace::EM_DELSPACE )
			{
				delSpace(spaceHandle);
			}
		}
	}
#ifdef _DEBUG
	std::list<RanaSpace*>::const_iterator itSpace = m_activeSpaces.begin();
	for(; itSpace != m_activeSpaces.end(); ++itSpace )
	{
		RanaSpace* pSpace = *itSpace;
		const int currentStack = lua_gettop(pSpace->m_state);
		for( int i = currentStack;i>0;--i)
		{
			ranaError( "RanaSpaceBank:lua stack leak:%d(%s)", i,lua_typename(pSpace->m_state,lua_type(pSpace->m_state,i)) );
		}
		RANA_ASSERT( currentStack == 0 );
	}
#endif
}

void RanaSpaceBank::setSpaceExit(const RanaSpaceHandle& spaceHandle, RanaSpace::EXCEPTION_CODE exitCode)
{
	RanaSpace* pRanaSpace = getLuaSpace(spaceHandle);
	if(pRanaSpace)
	{
		if(exitCode!=RanaSpace::EC_SUCCESS)
		{
			if(pRanaSpace->m_errorCode == RanaSpace::EC_SUCCESS && pRanaSpace->m_context)
			{
				pRanaSpace->m_errorCode = exitCode;
				pRanaSpace->m_itErrorList = m_exitSpaces.insert(m_exitSpaces.end(),pRanaSpace);
			}
		}
		else
		{
			if(pRanaSpace->m_errorCode != RanaSpace::EC_SUCCESS)
			{
				pRanaSpace->m_errorCode = exitCode;
				m_exitSpaces.erase(pRanaSpace->m_itErrorList);
			}
		}
	}
}

void RanaSpaceBank::setLogFunc( LOGFUNC logFunc )
{
	m_logFunc = logFunc;
}

void RanaSpaceBank::setErrorFunc( LOGFUNC errorFunc )
{
	m_errorFunc = errorFunc;
}

void RanaSpaceBank::printLog(const char* pChar)
{
	if(!m_logFunc)
	{
		printf("%s\n",pChar);
		return;
	}
	(*m_logFunc)(pChar);
}

void RanaSpaceBank::printError(const char* pChar)
{
	if(m_errorFunc)
	{
		(*m_errorFunc)(pChar);
		return;
	}
	if(m_logFunc)
	{
		(*m_logFunc)(pChar);
	}
	perror(pChar);
}

void RanaSpaceBank::forceDisableTrackLogTrigger(bool isDisable)
{
	m_allowTrackTrigger = isDisable ? false : true;
}

bool RanaSpaceBank::isAllowTrackTrigger() const
{
	return m_allowTrackTrigger;
}
