#include "consoleappender.h"
#include "logcontent.h"

using namespace Zoic;


ConsoleAppender::ConsoleAppender():
Appender(LAT_CONSOLE)
{

}

ConsoleAppender::~ConsoleAppender()
{

}

void ConsoleAppender::init()
{
	Appender::init();
}

bool ConsoleAppender::reopen()
{
	return true;
}

bool ConsoleAppender::close()
{
	return true;
}

bool ConsoleAppender::doAppend(LogContent* pContent)
{
	pContent->formate();
	printf(pContent->getMessage());
	return true;
}