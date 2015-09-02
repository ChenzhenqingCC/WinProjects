#include "logstatusstrategy.h"
#include "loglevel.h"

using namespace Zoic;

LogStatusStrategy::LogStatusStrategy(unsigned char type):
m_logType(type),
m_logLevel(ALL_LOG_LEVEL)
{

}

LogStatusStrategy::~LogStatusStrategy()
{

}