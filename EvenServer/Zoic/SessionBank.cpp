#include "include/SessionBank.h"
#include "include/SessionS.h"
#include "include/Message.h"
#include "include/ProtocolVersion.h"
#include "include/Log.h"

#include <fcntl.h>
#ifndef HAVE_MINGW32
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#include <typeinfo>

#ifdef HAVE_MINGW32
typedef int socklen_t;
#endif

using namespace Zoic;

SessionBank::SessionBank( MESSAGE_FUNC defaultFunc ):
	CommandFactory( defaultFunc ),
	m_listenSocket(INVALID_SOCKET),
	m_nextSerial(0),
	m_sessions(NULL),
	m_maxSessions(0),
	m_allowRun(true)
{
}
SessionBank::~SessionBank()
{
}

void SessionBank::setListenAddress(const char *ip,WORD port)
{
	m_listenIP=ip;
	m_listenPort=port;
}

bool SessionBank::setMaxSessions(HandleType maxSessions)
{
	if( m_activeSessions.size() )
	{
		return false;
	}
	clearSessions();
	m_maxSessions=maxSessions;
	m_sessions=new SessionS * [m_maxSessions];
	//创建SessionS对象
	for(HandleType i=0;i<m_maxSessions;++i)
	{
		SessionS * pSession=createSession();
		pSession->m_handle=i;
		pSession->m_commandFactory=this;
		m_unusedSessions.push_back(pSession);
		m_sessions[i]=NULL;
	}
	return true;
}

int SessionBank::start()
{
	SessionS * pDummySession=createSession();
	registerCommands(typeid(*pDummySession) );
	delete pDummySession;

	m_listenSocket = socket(PF_INET, SOCK_STREAM, 0);
	if(m_listenSocket==INVALID_SOCKET)
	{
		return 1;
	}
#ifdef HAVE_MINGW32
	unsigned long sw = 1;
	if(ioctlsocket(m_listenSocket, FIONBIO, &sw))
	{
		return 2;
	}
#else
	if(fcntl(m_listenSocket, F_SETFL, FNDELAY)==-1)
	{
		return 2;
	}
#endif
	int opt = 1;
	int len = sizeof(opt);
	if(setsockopt(m_listenSocket,SOL_SOCKET,SO_REUSEADDR,(const char *)&opt,len)!=0)
	{
		return 3;
	}

	sockaddr_in sa;
	memset(&sa,0,sizeof(sa));
	sa.sin_addr.s_addr = inet_addr(m_listenIP.c_str());
	sa.sin_port = htons(m_listenPort);
	sa.sin_family = PF_INET;
	if(bind(m_listenSocket, (sockaddr *)&sa, sizeof(sa)) != 0){
		char errorPre[256];
		sprintf(errorPre,"bind error(%s:%d)",inet_ntoa(sa.sin_addr),ntohs(sa.sin_port));
		{
			const bool isPut = g_Log->isDoPut();
			g_Log->undoPut();
			loginfo_f("%s",errorPre);
			if(isPut)g_Log->doPut();
		}
		perror(errorPre);
		return 5;
	}
	if(listen(m_listenSocket, 10)!=0)
	{
		perror("listen error");
		return 6;
	}
	joinSelector(m_listenSocket);
	return 0;
}

int	SessionBank::run()
{
	if(!m_allowRun)
	{
		return 0;
	}

	//if(canRecvData())
	{
		recvData();
		//setCanRecvData(false);
	}

	//驱动活动连接
	for(LIST_SESSIONS::iterator it=m_activeSessions.begin();it!=m_activeSessions.end();)
	{
		SessionS *pSession=*it++;
		if(!pSession->routine())
		{
			closeSession(pSession);
		}
	}
	return 0;
}

int SessionBank::stop()
{
	clearSessions();
	if(m_listenSocket!=INVALID_SOCKET)
	{
		quitSelector();
		closesocket(m_listenSocket);
		m_listenSocket = INVALID_SOCKET;
	}
	return 0;
}

void SessionBank::clearSessions()
{
	for(LIST_SESSIONS::iterator it=m_activeSessions.begin();it!=m_activeSessions.end();)
	{
		SessionS *pSession=*it++;
		closeSession(pSession);
	}
	for(LIST_SESSIONS::iterator it=m_unusedSessions.begin();it!=m_unusedSessions.end();++it)
	{
		delete *it;
	}
	m_unusedSessions.clear();
	if(m_sessions)
	{
		delete [] m_sessions;
		m_sessions = NULL;
	}
	m_maxSessions = 0;
}

SessionS * SessionBank::findSession(const HandleType & handle,const SerialType &serial)
{
	if(handle < m_maxSessions)
	{
		Zoic::SessionS* pSession = m_sessions[handle];
		if(pSession&&pSession->m_serial==serial)
		{
			return pSession;
		}
	}
	return NULL;
}

void SessionBank::closeSession(SessionS *pSession)
{
	pSession->close();
	m_activeSessions.erase(pSession->m_itList);
	m_sessions[pSession->m_handle]=NULL;
	onSessionClosed(pSession);
}

void SessionBank::broadcast(const Zoic::Message &msg)
{
	for(LIST_SESSIONS::iterator it=m_activeSessions.begin();it!=m_activeSessions.end();++it)
	{
		SessionS *pSession=*it;
		if(pSession->isReady())
		{
			pSession->sendMessage(msg);
		}
	}
}

void SessionBank::recvData()
{
	//接受新连接
	sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	SOCKET sock = accept(m_listenSocket, (sockaddr *)&addr, &addr_len);
	if( sock != INVALID_SOCKET )
	{
		bool opened=false;
		if(m_unusedSessions.size())
		{
			SessionS *pSession=m_unusedSessions.front();
			pSession->m_serial=m_nextSerial;
			if(opened=pSession->open(sock,addr))
			{
				m_nextSerial++;
				m_unusedSessions.pop_front();
				onSessionOpened(pSession);
#ifdef _DEBUG
				//发送协议版本
				Message msg;
				msg.ID(PROTOCOL_VERSION_NTF) << g_ProtocolVersion->getVersion();
				pSession->sendMessage(msg);
#endif
			}
		}
		if(!opened)
		{
			closesocket(sock);
		}
	}
}

void SessionBank::onSessionOpened(SessionS *pSession)
{
	addSessionToActiveList(pSession);
}

void SessionBank::onSessionClosed(SessionS *pSession)
{
	addSessionToUnusedList(pSession);
}

bool SessionBank::addSessionToActiveList(SessionS * pSession)
{
	ASSERT(!m_sessions[pSession->m_handle]);
	m_activeSessions.push_back(pSession);
	pSession->m_itList=--m_activeSessions.end();
	m_sessions[pSession->m_handle]=pSession;
	return true;
}

bool SessionBank::addSessionToUnusedList(SessionS * pSession)
{
	m_unusedSessions.push_back(pSession);
	return true;
}
