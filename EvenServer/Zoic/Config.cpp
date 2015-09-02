#include "include/Config.h"
#include "include/Exception.h"
#include "include/Log.h"

using namespace Zoic;

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

ConfigLoader::ConfigLoader()
	:m_loaded(false)
{
}

bool ConfigLoader::load(FILE *fp)
{
	VAR_MAP loadingMap;
	char buf[1024];
	size_t currentline = 0;
	while(fgets(buf,1024,fp))
	{
		++currentline;
		if( !readLine(loadingMap,buf) )
		{
			logerror_f("Failed of load config file of line(%d),Content: %s\n",currentline,buf);
			return false;
		}
	}
	std::swap(m_vars,loadingMap);
	m_loaded = true;
	return true;
}

bool ConfigLoader::load(const char *file)
{
	FILE *fp = fopen(file, "r");
	if(fp == NULL)
	{
		logerror_f("Cannot open config file: %s\n", file);
		return false;
	}
	bool result = load(fp);
	fclose(fp);
	if(!result)
	{
		logerror_f("Cannot parse config file %s\n", file);
		return false;
	}
	return true;
}

bool ConfigLoader::readLine(ConfigLoader::VAR_MAP& output,const char *line)
{
	char buf[1024];
	strncpy(buf,line,1024);

	char *key=trim(buf);
	if(*key=='#' || *key=='\0')
	{
		return true;
	}
	char *val=strstr(buf,"=");
	if(!val)
	{
		return false;
	}
	*(val++)='\0';
	char *t_key=trim(key);
	char *t_val=trim(val);
	if(t_key&&t_val) 
	{
		output[t_key]=t_val;
		return true;
	}
	return false;
}
