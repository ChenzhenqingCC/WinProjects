#ifndef __CONSOLEAPPENDER_H
#define __CONSOLEAPPENDER_H

#include "appender.h"

namespace Zoic
{
	class LogContent;
	class ConsoleAppender:
		public Appender
	{
	public:
		ConsoleAppender();
		virtual ~ConsoleAppender();

		virtual void init();

		virtual bool reopen();
		virtual bool close();
		virtual bool doAppend(LogContent* pContent);
	};
}

#endif