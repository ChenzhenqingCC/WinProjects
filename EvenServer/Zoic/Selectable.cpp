#include "include/Selectable.h"
#include "include/Selector.h"

using namespace Zoic;

Selectable::Selectable():
	m_socket(INVALID_SOCKET),
	m_canRecvData(true),
	m_canSendData(true)
{
}

Selectable::~Selectable()
{
	if(m_socket!=INVALID_SOCKET)
	{
		quitSelector();
	}
}

bool Selectable::joinSelector(SOCKET socket)
{
	//修复进程退出时的BUG
	if(g_Selector==NULL)
	{
		return false;
	}

	ASSERT(m_socket==INVALID_SOCKET);
	m_socket=socket;
	if(g_Selector->insert(this))
	{
		return true;
	}
	else
	{
		m_socket=INVALID_SOCKET;
		return false;
	}
}

bool Selectable::quitSelector()
{
	//修复进程退出时的BUG
	if(g_Selector==NULL)
	{
		return false;
	}

	ASSERT(m_socket!=INVALID_SOCKET);
	if(g_Selector->remove(this))
	{
		m_socket=INVALID_SOCKET;
		return true;
	}
	else
	{
		return false;
	}
}

