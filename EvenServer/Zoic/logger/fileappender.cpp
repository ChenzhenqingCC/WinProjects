#include "fileappender.h"
#include "logcontent.h"
#include "Zoic/include/Types.h"
#include <sys/stat.h>

#ifdef _MSC_VER
#include <direct.h>
#else
#include <string.h>
#endif

static const std::string DEFAULT_LOG_PATH = "log/";

using namespace Zoic;

FileAppender::FileAppender():
Appender(LAT_FILE),
m_filename(""),
m_filenamePrefix(""),
m_filemode(""),
m_absolutePath(""),
m_fp(NULL),
m_isWriteBack(false)
{
	
}

FileAppender::~FileAppender()
{
	
}

FILE* FileAppender::openFile(const std::string& filename,const std::string& absolutePath,const std::string& filemode)
{
	if (filename == "" || filemode == "")
	{
		return NULL;
	}
	
	if (absolutePath != "")
	{
		int ret = 0;
#ifdef _MSC_VER
		ret = _chdir(absolutePath.c_str());
#else
		ret = chdir(absolutePath.c_str());
#endif
		if (ret < 0)
		{
			return NULL;
		}
	}

	FILE* fp = fopen(filename.c_str(),filemode.c_str());
	if (!fp)
	{
		char fname[640];
		strncpy(fname,filename.c_str(),640);
		for (int i=1;fname[i];++i)
		{
			if(fname[i]=='/')
			{
				fname[i]='\0';
#ifdef _MSC_VER
				_mkdir(fname);
#else
				mkdir(fname,0777);
#endif
				fname[i]='/';
			}
		}
		fp = fopen(fname,filemode.c_str());
		if (!fp)
		{
			return NULL;
		}
	}
	return fp;
}

void FileAppender::init()
{
	Appender::init();
	m_filename = "";
	m_filemode = "";
	m_filenamePrefix = "";
	m_fp = NULL;
	m_isWriteBack = false;
}

bool FileAppender::reopen()
{
	std::string filename = "";
	if (m_filenamePrefix.empty())
	{
		filename = m_filename;
	}
	else
	{
		addFileNamePrefix(m_filename,m_filenamePrefix,filename);
	}
	filename = DEFAULT_LOG_PATH + filename;
	m_fp = openFile(filename,m_absolutePath,m_filemode);
	return (m_fp)?(true):(false);
}

bool FileAppender::close()
{
	if (m_fp)
	{
		fclose(m_fp);
		m_fp = NULL;
	}
	return true;
}

bool FileAppender::doAppend(LogContent* pContent)
{
	(pContent->m_type == LCT_BINARY)?(m_filemode = "ab+"):(m_filemode = "a");
	if (!reopen())
	{
		//ASSERT(0);
		return false;
	}
	pContent->formate();
	fwrite(pContent->getMessage(),1,pContent->getLength(),getFile());
	close();
	return true;
}

void FileAppender::addFileNamePrefix(const std::string& orgName,const std::string& prefix,std::string& path)
{
	if (orgName.find('/') == -1)
	{
		path = prefix + orgName;
		return;
	}
	int len = orgName.length();
	for (int i = len-1;i>=0;--i)
	{
		if (orgName[i] == '/')
		{
			path = orgName.substr(0,i+1) + prefix + orgName.substr(i+1,len);
			return;
		}
	}
}

