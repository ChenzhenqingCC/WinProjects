#ifndef __EXTENDFILEAPPENDER_H
#define __EXTENDFILEAPPENDER_H

#include "fileappender.h"

namespace Zoic
{
	class ExtendFileAppender:
		public FileAppender
	{
	public:
		ExtendFileAppender();
		virtual ~ExtendFileAppender();
		virtual void init();
	};

}


#endif