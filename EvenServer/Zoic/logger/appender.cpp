#include "appender.h"

using namespace Zoic;

Appender::Appender(unsigned char type):
m_type(type),
m_isActive(true)
{
	
}

Appender::~Appender()
{

}

void Appender::init()
{
	m_isActive = true;
}