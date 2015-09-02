#include "include/RanaScriptManager.h"

#ifdef ZOIC_NO_SERVICE
RanaScriptManager theRanaScriptManager;
#endif

static char *trim(char *src)
{
	char *ret;
	if(!src) return NULL;
	ret=src;
	while(*ret==' '||*ret=='\r'||*ret=='\n'||*ret=='\t') ret++;
	char *tmp=ret+strlen(ret)-1;

	while((*tmp==' '||*tmp=='\r'||*tmp=='\n'||*tmp=='\t')&&tmp>ret) tmp--;
	*(tmp+1)=0;
	return ret;
}

RanaScriptManager::RanaScriptManager() : m_changed(false),m_configPath("")
{

}

RanaScriptManager::~RanaScriptManager()
{
	PATH_SET::iterator itPath = m_filePaths.begin();
	for(;itPath != m_filePaths.end();++itPath)
	{
		delete *itPath;
	}
}

int RanaScriptManager::start()
{
	load(m_configPath);
	return 0;
}

int RanaScriptManager::stop()
{
	save(m_configPath);
	return 0;
}

void RanaScriptManager::setConfigPath(const char* path)
{
	if(m_configPath!=path)
	{
		m_configPath = path;
		m_changed = true;
	}
}

void RanaScriptManager::load(const char *file)
{
	if(!file || !strcmp(file,""))
	{
		return;
	}
	FILE *fp = fopen(file, "r");
	if(fp == NULL)
	{
		dprintf("Cannot open file \"%s\" for read.\n", file);
		return;
	}
	
	char buf[1024];
	buf[sizeof(buf)-1]='\0';
	while(fgets(buf,sizeof(buf)-1,fp))
	{
		if(*buf!='#')
		{
			loadScriptItem(buf);
		}
	}
	m_changed = (m_configPath != file);
	fclose(fp);
}

void RanaScriptManager::save(const char *file)
{
	if(!file || !strcmp(file,""))
	{
		return;
	}
	bool isSameFile = (m_configPath==file);
	if( !m_changed && isSameFile)
	{
		return;
	}
	FILE *fp = fopen(file, "w");
	if(fp == NULL)
	{
		dprintf("Cannot open file %s for write. The Rana Script Manager can't save the ManagerInfo!\n", file);
		return;
	}
	PATH_SET::iterator it = m_filePaths.begin();
	for(;it!= m_filePaths.end(); ++ it)
	{
		RanaScriptType* pScriptType = *it;
		const char* const& pPath = static_cast<const char* const&>(pScriptType->path);
		const char* const& pName = static_cast<const char* const&>(pScriptType->scriptName);
		fprintf( fp, "%s = %s : %u\n",pPath, pName,pScriptType->spaceType );
	}
	if(isSameFile)
	{
		m_changed = false;
	}
	fclose(fp);
}

bool RanaScriptManager::setScriptInfo( const char* path, const char* name, BYTE type)
{
	if( strstr(path,"=") || strstr(name,":") )
	{
		return false;
	}
	RanaScriptType* pScriptType = find(path);
	if( pScriptType )
	{
		if(pScriptType->spaceType != type)
		{
			return false;
		}
		pScriptType->scriptName = name;
	}
	else
	{
		RanaScriptType* pRanaScriptType = new RanaScriptType;
		pRanaScriptType->spaceType = type;
		pRanaScriptType->scriptName = name;
		pRanaScriptType->path = path;
		m_filePaths.insert( pRanaScriptType );
		m_fileTypes[pRanaScriptType->spaceType].insert(pRanaScriptType);
	}
	m_changed = true;
	save(m_configPath);
	return true;
}

bool RanaScriptManager::removeScriptInfo( const char* path )
{
	RanaScriptType* pScriptType = find(path);
	if(!pScriptType)
	{
		return false;
	}
	m_filePaths.erase(pScriptType);
	if( ! m_fileTypes[pScriptType->spaceType].erase(pScriptType) )
	{
		ASSERT(0);
	}
	delete pScriptType;
	m_changed = true;
	return true;
}

const RanaScriptManager::SCRIPT_SET * RanaScriptManager::getPaths(BYTE type)
{
	TYPE_MAP::iterator it = m_fileTypes.find(type);
	if(it == m_fileTypes.end())
	{
		return NULL;
	}
	return &(it->second);
}

void RanaScriptManager::loadScriptItem(char *buf)
{
	char *path=buf;
	char *name = NULL;
	char *type = NULL;
	name=strstr(buf,"=");
	if(!name)
	{
		return;
	}
	*name++=0;
	type=strstr(name,":");
	if(!type)
	{
		return;
	}
	*type++=0;
	char *t_path=trim(path);
	char *t_name=trim(name);
	char *t_type=trim(type);
	if(!t_path||!t_name||!t_type) 
	{
		return;
	}
	RanaScriptType* pScriptType = find(path);
	if(pScriptType)
	{
		return;
	}
	pScriptType = new RanaScriptType;
	pScriptType->spaceType = atoi(t_type);
	pScriptType->scriptName = t_name;
	pScriptType->path = t_path;
	if( !m_filePaths.insert(pScriptType).second )
	{
		ASSERT(0);
		delete pScriptType;
		return;
	}
	m_fileTypes[pScriptType->spaceType].insert(pScriptType);
}

RanaScriptManager::RanaScriptType* RanaScriptManager::find(const char* path)
{
	RanaScriptType searchType;
	searchType.path = path;
	PATH_SET::iterator itPath = m_filePaths.find(&searchType);
	if(itPath == m_filePaths.end() )
	{
		return NULL;
	}
	return *itPath;
}

const RanaScriptManager::RanaScriptType* RanaScriptManager::findItem(const char* path)
{
	return find(path);
}

