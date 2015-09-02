#include "include/SessionC.h"
#include "include/Clock.h"

#ifndef HAVE_MINGW32
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#include <errno.h>
#include <fcntl.h>
#include <typeinfo>

using namespace Zoic;

SessionC::SessionC( MESSAGE_FUNC defaultFunc ):
	CommandFactory( defaultFunc ),
	m_localIP("0.0.0.0"),
	m_localPort(0),
	m_connectSocket(INVALID_SOCKET),
	m_opened(false)
{
	m_send_buffer_rule.m_init_buff_size*=10;
	m_recv_buffer_rule.m_init_buff_size*=10;
	m_send_buffer_rule.m_dynamic=true;
}

SessionC::~SessionC()
{
	if(m_connectSocket!=INVALID_SOCKET)
	{
		closesocket(m_connectSocket);
	}
}

void SessionC::setConnectAddress(const char *ip,WORD port)
{
	m_connectIP=ip;
	m_connectPort=port;
}


void SessionC::initForConnect()
{
	registerCommands(typeid(*this));
}

int SessionC::start()
{
	initForConnect();
	g_Clock->setTime();
	int code=connect(inet_addr(m_connectIP.c_str()),m_connectPort);
	if(code)
	{
		return code;
	}

	for(;;)
	{
		g_Clock->setTime();
		code=isConnectSuccess();
		if(code==0)
		{
			return 0;
		}
		else if(code < 0)
		{
			return 5;
		}
		WAIT_A_WHILE
    }    
    return 6;
}

int	SessionC::run()
{
	if(routine())
	{
		return 0;
	}
	else
	{
		close();
		return 1;
	}
}

int	SessionC::stop()
{
	if(m_opened)
	{
		close();
	}
	if(	m_connectSocket!=INVALID_SOCKET )
	{
		closesocket(m_connectSocket);
		m_connectSocket=INVALID_SOCKET;
	}
	return 0;
}

void SessionC::close()
{
	Session::close();
	m_opened=false;
}

bool SessionC::parseMessage(Message &msg)
{
	return CommandFactory::parseMessage(this,msg);
}

int SessionC::connect(DWORD ip,WORD port,BYTE timeout)
{
	SOCKET sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock==INVALID_SOCKET)
	{
		return 1;
	}
#ifdef HAVE_MINGW32
	unsigned long sw = 1;
	if(ioctlsocket(sock, FIONBIO, &sw))
	{
		closesocket(sock);
		return 2;
	}
#else
	if(fcntl(sock, F_SETFL, FNDELAY)==-1)
	{
		closesocket(sock);
		return 2;
	}
#endif

	sockaddr_in	sa;
	memset(&sa,0,sizeof(sa));
	sa.sin_addr.s_addr = inet_addr(m_localIP.c_str());
	sa.sin_port = htons(m_localPort);
	sa.sin_family = PF_INET;
	if(bind(sock, (sockaddr *)&sa, sizeof(sa)) != 0)
	{
		closesocket(sock);
		char errorPre[256];
		sprintf(errorPre,"bind error(%s:%d)",inet_ntoa(sa.sin_addr),ntohs(sa.sin_port));
		perror(errorPre);
		return 3;
	}
	memset(&m_saServer,0,sizeof(m_saServer));
	m_saServer.sin_addr.s_addr = ip;
	m_saServer.sin_port = htons(port);
	m_saServer.sin_family = PF_INET;
	
	::connect(sock, (sockaddr *)&m_saServer, sizeof(m_saServer));
	m_connectSocket=sock;
	m_connectTimeOut=g_Clock->getMilliSeconds()+timeout*1000;
	return 0;
}

int SessionC::isConnectSuccess(void)
{	
	::connect(m_connectSocket, (sockaddr *)&m_saServer, sizeof(m_saServer));
	int code=getConnectStatus();
	if(code==0)
	{
		m_opened=open(m_connectSocket,m_saServer);
		if(m_opened)
		{
			m_connectSocket=INVALID_SOCKET;
		}
		else
		{
			code=-1;
		}
	}
	if(code==-1)
	{
		closesocket(m_connectSocket);
		m_connectSocket=INVALID_SOCKET;
	}
	return code;
}

#ifdef HAVE_MINGW32
int SessionC::getConnectStatus()
{
	switch(WSAGetLastError())
	{
		case WSAEISCONN:
			return 0;
		case WSAEINPROGRESS:
		case WSAEINVAL:
		case WSAEALREADY:
			return g_Clock->getMilliSeconds()<m_connectTimeOut?1:-1;
			break;
		default:
			return -1;
	}
}
#else
int SessionC::getConnectStatus()
{
	switch(errno)
	{
		case EISCONN:
			return 0;
		case EINPROGRESS:
		case EINTR:
		case EALREADY:
			return g_Clock->getMilliSeconds()<m_connectTimeOut?1:-1;
			break;
		default:
			return -1;
	}
}
#endif
