#ifndef __BUFFERAPPENDER_H
#define __BUFFERAPPENDER_H

#include "appender.h"
#include <vector>

namespace Zoic
{
	class BufferAppender:
		public Appender
	{
	public:
		BufferAppender();
		virtual ~BufferAppender();

		const std::string& getFileName() const;
		void setFileName(const std::string& fname,const std::string& absolutePath);

		virtual void init();

		virtual bool reopen();
		virtual bool close();
		virtual bool doAppend(LogContent* pContent);

	private:
		std::string m_fname;
		std::string m_absolutePath;
	};

	inline const std::string& BufferAppender::getFileName() const
	{
		return m_fname;
	}

	inline void BufferAppender::setFileName(const std::string& fname,const std::string& absolutePath)
	{
		m_fname = fname;
		m_absolutePath = absolutePath;
	}
}

#endif