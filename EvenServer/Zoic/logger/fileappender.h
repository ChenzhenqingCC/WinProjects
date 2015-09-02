#ifndef __FILEAPPENDER_H
#define __FILEAPPENDER_H

#include "appender.h"

#ifdef _MSC_VER
#else
#include <stdio.h>
#endif

namespace Zoic
{
	class FileAppender:
		public Appender
	{
	public:
		FileAppender();
		virtual ~FileAppender();

		const std::string& getFileName() const;
		const std::string& getPrefix() const;
		const std::string& getFileMode() const;

		void setFileName(const std::string& fname,const std::string& absolutePath);
		void setPrefix(const std::string& prefix);
		static FILE* openFile(const std::string& filename,const std::string& absolutePath,const std::string& filemode);
		static void addFileNamePrefix(const std::string& orgName,const std::string& prefix,std::string& path);
		virtual void init();
	protected:
		virtual bool reopen();
		virtual bool close();
		virtual bool doAppend(LogContent* pContent);

		FILE* getFile() const;
	protected:
		std::string m_filename;
		std::string m_filenamePrefix;
		std::string m_filemode;
		std::string m_absolutePath;
		FILE* m_fp;
		bool m_isWriteBack;
	};

	inline const std::string& FileAppender::getFileName() const
	{
		return m_filename;
	}

	inline const std::string& FileAppender::getFileMode() const
	{
		return m_filemode;
	}

	inline const std::string& FileAppender::getPrefix() const
	{
		return m_filenamePrefix;
	}

	inline void FileAppender::setFileName(const std::string& fname,const std::string& absolutePath)
	{
		m_filename = fname;
		m_absolutePath = absolutePath;
	}

	inline void FileAppender::setPrefix(const std::string& prefix)
	{
		m_filenamePrefix = prefix;
	}

	inline FILE* FileAppender::getFile() const
	{
		return m_fp;
	}
}

#endif