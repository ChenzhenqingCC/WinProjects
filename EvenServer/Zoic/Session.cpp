#include <new>
#include "include/Session.h"
#include "include/Message.h"
#include "include/Clock.h"
#include "include/SysPing.h"
#include "include/Exception.h"
#include "include/SessionError.h"
#include "include/DataBuffer.h"
#include "include/Log.h"

#ifndef _MSC_VER
#include <sys/socket.h>
#include <netinet/tcp.h>
#endif

#ifndef HAVE_MINGW32
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#include <errno.h>
#include <fcntl.h>

#ifdef HAVE_MINGW32
#define ERRORNO_EWOULDBLOCK WSAEWOULDBLOCK
#define GET_ERRORNO (WSAGetLastError())
#else
#define ERRORNO_EWOULDBLOCK EAGAIN
#define GET_ERRORNO (errno)
#endif

#ifdef HAVE_MINGW32
typedef int socklen_t;
#endif

using namespace Zoic;

Session::Session(SessionUnpackMethod* unpack_method)
	:_m_send_buffer(NULL)
	,_m_recv_buffer(NULL)
	,m_send_buffer_rule(8*NETWORK_BUF_SIZE,false,10*1024*1024)
	,m_recv_buffer_rule(2*NETWORK_BUF_SIZE,false,10*1024*1024)
	,m_socket_rule(false)
	,m_unpackMethod( selectUnpackMethod(unpack_method) )
	,m_send_buffer(*new DataBuffer)
	,m_recv_buffer(*new DataBuffer)
	,m_syspingTimes(&g_sysPingTime)
	//以下变量在close时要重设	
	,m_socket(INVALID_SOCKET)
	,m_lastReceivedTick(0)
	,m_next_unpack_min_length(0)
	,m_error(0)
	,m_systemError(0)
	,m_syspingCount(0)
{
	ASSERT(!m_unpackMethod.getSession());
	m_unpackMethod.pSession = this;
}

Session::~Session()
{
	close();
	m_send_buffer.forceUnlock();
	m_recv_buffer.forceUnlock();
	clearBuffer(m_send_buffer_rule,_m_send_buffer,m_send_buffer);
	clearBuffer(m_recv_buffer_rule,_m_recv_buffer,m_recv_buffer);
	delete &m_send_buffer;
	delete &m_recv_buffer;
	delete &m_unpackMethod;
}

SessionUnpackMethod& Session::selectUnpackMethod(SessionUnpackMethod* unpack_method)
{
	if( unpack_method && unpack_method->pSession == NULL )
	{
		return *unpack_method;
	}
	return *new DefaultUnpackMethod;
}

bool Session::open(SOCKET sock,const sockaddr_in &addr)
{
	//该函数必须保证在单线程下运行,多线程必须加锁,且必须和close公用同一把锁
#ifdef HAVE_MINGW32
	unsigned long sw = 1;
	if(ioctlsocket(sock, FIONBIO, &sw))
	{
		return false;
	}
#else	
	if(fcntl(sock, F_SETFL, FNDELAY)==-1)
	{
		return false;
	}
#endif
	if( isSocketValid() )
	{
		return false;
	}
	m_socket=sock;
	m_ip=inet_ntoa(addr.sin_addr);
	m_port=ntohs(addr.sin_port);
	ASSERT(m_send_buffer.getDataSize()==0);
	ASSERT(m_recv_buffer.getDataSize()==0);

	if(m_socket_rule.m_close_no_linger)
	{
		linger m_sLinger;
		m_sLinger.l_onoff = 1;  // (在closesocket()调用,但是还有数据没发送完毕的时候容许逗留)
		m_sLinger.l_linger = 0; // (容许逗留的时间为秒)
		setsockopt(m_socket,SOL_SOCKET,SO_LINGER,(const char*)&m_sLinger,sizeof(linger));
	}
	sockaddr_in localAddr;
	socklen_t length = sizeof(localAddr);
	if( getsockname(m_socket,(sockaddr *)&localAddr,&length) == 0 )
	{
		m_local_ip =inet_ntoa(localAddr.sin_addr);
		m_local_port=ntohs(localAddr.sin_port);
	}
	else
	{
		m_local_ip = "";
		m_local_port = 0;
	}
	
	netInitialize();
	setCanRecvData(true);
	setCanSendData(true);
	joinSelector(m_socket);
	m_unpackMethod.open();

#ifndef _MSC_VER
	const int val = 1;
	if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, static_cast<const void*>(&val), sizeof(val)) < 0)
#else
	const char val = 1;
	if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, static_cast<const char*>(&val), sizeof(val)) < 0) 
#endif
	{
		logwarning_f("[%s]: setsockopt with TCP_NODELAY failed on %s:%d", __PRETTY_FUNCTION__, getIP(),getPort());
	}

	return true;
}

void Session::netInitialize()
{
	m_error=0;
	m_systemError = 0;
	tryInitBuffer();
	m_lastReceivedTick=g_Clock->getMilliSeconds();
}

int Session::tryRecvData()
{
	ZOIC_VMP_BEGIN
	if(!canRecvData())
	{
		return -1;
	}
	int len = recvData();
	//setCanRecvData(false);
	ZOIC_VMP_END
	return len;
}

bool Session::pingCheck()
{
	//检测对方是否当机
	if(m_syspingCount >= m_syspingTimes->getMaxPingCount())
	{//已超过最大探测时间
	}
	else if(g_Clock->getMilliSeconds()-m_lastReceivedTick >= m_syspingTimes->getNextPingTime(m_syspingCount) )
	{
		++m_syspingCount;
		if(m_syspingCount >= m_syspingTimes->getMaxPingCount())
		{//已超过最大探测时间
			setError(ERROR_PINGTIMEOUT);
		}
		else
		{//发送探测包
			Message msg;
			msg.ID(SYSPING_REQ);
			setCanSendData(true);
			sendMessage(msg);
		}
		return true;
	}
	return false;
}

int Session::trySendData()
{
	ZOIC_VMP_BEGIN
	if(!canSendData())
	{
		return -1;
	}
	int len = sendData();
	//setCanSendData(false);
	ZOIC_VMP_END
	return len;
}

bool Session::routine()
{
	ASSERT( isSocketValid() );
	tryRecvData();
	pingCheck();
	trySendData();
	unpack();//解包,如果不想直接在routine中解析协议的话，那就覆盖routine并在别的位置进行
	if(m_error)
	{//出错
		//结束前发送最后一次数据
		sendData();
		return false;
	}
	else
	{
		return true;
	}
}

void Session::close()
{
	//该函数必须保证在单线程下运行,多线程必须加锁,且必须和open公用同一把锁
	if( isSocketValid() )
	{
		quitSelector();
		int code = closesocket(m_socket);
		if( code != 0 )
		{
			logerror_f( "Session: close socket failed %d(%d)", m_socket,GET_ERRORNO );
		}
		m_socket=INVALID_SOCKET;
	}
	if(! m_send_buffer.cleanAllData() )
	{
		ASSERT(0);
		m_send_buffer.forceUnlock();
		m_send_buffer.cleanAllData();
	}
	if(! m_recv_buffer.cleanAllData() )
	{
		ASSERT(0);
		m_recv_buffer.forceUnlock();
		m_recv_buffer.cleanAllData();
	}
	m_next_unpack_min_length=0;
	m_unpackMethod.close();
	m_syspingCount=0;
	tryRevertBuffer();
}

void Session::clearBuffer(const BufferRule& rule,char*& raw,DataBuffer& buffer)
{
	if( !buffer.getBufferSize() )
	{
		ASSERT(!raw);
		return;
	}
	ASSERT(raw);
	if( ! buffer.cleanAllData() )
	{
		ASSERT(0);
	}
	delete [] raw;
	buffer.replace(NULL,0,&raw);
	raw = NULL;
}

void Session::tryRevertBuffer()
{
	if( m_send_buffer_rule.m_init_buff_size != m_send_buffer.getBufferSize() )
	{
		clearBuffer(m_send_buffer_rule,_m_send_buffer,m_send_buffer);
	}
	if( m_recv_buffer_rule.m_init_buff_size != m_recv_buffer.getBufferSize() )
	{
		clearBuffer(m_recv_buffer_rule,_m_recv_buffer,m_recv_buffer);
	}
}

bool Session::createBuffer(DWORD & size,char * & raw,char* & data)
{
	if(size < NETWORK_BUF_SIZE)
	{
		size = NETWORK_BUF_SIZE;
	}
	char * buf = new(std::nothrow) char [offsetof(Message::Fake,buffer)+size+1];
	if(!buf)
	{
		return false;
	}
	raw = buf;
	data=raw+offsetof(Message::Fake,buffer);
	data[size]=0;
	return true;
}

void Session::initBuffer(BufferRule& rule,char*& raw,DataBuffer& buffer)
{
	if(rule.m_init_buff_size < NETWORK_BUF_SIZE)
	{
		ASSERT(0);
		rule.m_init_buff_size = NETWORK_BUF_SIZE;
	}
	if(buffer.getBufferSize() < rule.m_init_buff_size)
	{
		clearBuffer(rule,raw,buffer);
		char* pData;
		createBuffer(rule.m_init_buff_size,raw,pData);
		buffer.replace(pData,rule.m_init_buff_size);
	}
}


void Session::tryInitBuffer()
{
	initBuffer(m_send_buffer_rule,_m_send_buffer,m_send_buffer);
	initBuffer(m_recv_buffer_rule,_m_recv_buffer,m_recv_buffer);
}

bool Session::parseMessage(Message &msg)
{
	return true;
}

int Session::sendData()
{
	ZOIC_VMP_BEGIN
	const DWORD send_size = m_send_buffer.getDataSize();
	if(send_size==0)
	{
		return 0;
	}
	char * pData = m_send_buffer.lockRead( send_size );
	if(!pData)
	{
		setError(ERROR_SENDLOCK);
		return 0;
	}
	int len=send(m_socket,(const char *)pData, send_size, 0 );
	m_send_buffer.unlockRead(len>0 ? len : 0);
	if(len>0)
	{
		//m_send_buffer.forceAdjustBuffer();
	}
	else
	{
		if(m_error==0)
		{
			if (len==0)
			{
				setError(ERROR_SENDFAILED);
			}
			else if( len<0 )
			{
				const int errorNo = GET_ERRORNO;
				if(errorNo != ERRORNO_EWOULDBLOCK)
				{
					m_systemError = errorNo;
					setError(ERROR_SENDFAILED2);
				}
			}			
		}
	}
	ZOIC_VMP_END
	return len;
}

inline bool Session::pushData(const BYTE *data,WORD size)
{
	ZOIC_VMP_BEGIN
	if(!_m_send_buffer)
	{
		return false;
	}
	if(m_send_buffer.isLocked() )
	{
		return false;
	}
	do
	{
		char* pTarget = m_send_buffer.lockWrite(size);
		if(pTarget)
		{
			memcpy(pTarget,data,size);
			m_send_buffer.unlockWrite(size);
			return true;
		}
	}
	while(increaseBuffer(m_send_buffer_rule,_m_send_buffer,m_send_buffer));

#ifdef _ENABLE_FULL_DAT
	FILE *fp=fopen("full.dat","wb+");
	if(fp!=NULL)
	{
		fwrite(m_send_buffer,m_send_size,1,fp);
		fclose(fp);
	}
#endif
	setError(ERROR_SENDBUFFER);
	ZOIC_VMP_END
	return false;
}

bool Session::sendMessage(const Message &msg)
{
	ZOIC_VMP_BEGIN
	if( m_error || !isSocketValid() )
	{
		return false;
	}
	bool res = pushData(msg.getData(),msg.getSize());
	ZOIC_VMP_END
	return res;
}

#ifdef HAVE_MINGW32
namespace
{
	class loadWinSock
	{
	public:
		loadWinSock()
		{
			WSADATA w;
			WORD wVersionRequested = MAKEWORD( 1, 1 );
      		if(WSAStartup(wVersionRequested, &w) != 0)
      		{
      			puts("Can't load WinSock DLL.");
      			exit(1);
      		}
		}
		~loadWinSock()
		{
			WSACleanup();
		}
	} load;
}
#endif

int Session::recvData()
{
	ZOIC_VMP_BEGIN
	if(m_recv_buffer.isLocked() )
	{
		setError(ERROR_RECVLOCK);
		return 0;
	}
	char* pTarget = NULL;
	int space = 0;
	if( m_recv_buffer_rule.m_init_buff_size * 2 <= m_recv_buffer.getBufferSize() )
	{//使用了dynamicBuff的场合，内存较大，尝试减少memmove
		space = m_recv_buffer.getBufferSize()/2;
		pTarget = m_recv_buffer.lockWrite(space);
	}
	if(!pTarget)
	{
		space = m_recv_buffer.getBufferSize() - m_recv_buffer.getDataSize();
		if(!space)
		{
			setError(ERROR_RECVBUFFER);
			return 0;
		}
		pTarget = m_recv_buffer.lockWrite(space);
		if(!pTarget)
		{
			ASSERT(0);
			return 0;
		}
	}
	ASSERT(space > 0);
	int len = ::recv(m_socket,(char *)pTarget,space,0);
	m_recv_buffer.unlockWrite(len>0 ? len : 0);
	if( len == space )
	{
		increaseBuffer(m_recv_buffer_rule,_m_recv_buffer,m_recv_buffer);
	}
	if(len>0)
	{
		m_syspingCount = 0;
		m_lastReceivedTick = g_Clock->getMilliSeconds();
	}
	else
	{
		if(m_error==0)
		{
			if (len==0)
			{
				setError(ERROR_REMOTECLOSED);
			}
			else if( len<0 ) 
			{
				const int errorNo = GET_ERRORNO;
				if(errorNo != ERRORNO_EWOULDBLOCK)
				{
					m_systemError = errorNo;
					setError(ERROR_RECVFAILED);
				}
			}			
		}
	}
	ZOIC_VMP_END
	return len;
}

bool Session::increaseBuffer(const BufferRule& rule,char*& raw,DataBuffer& buffer)
{
	if(buffer.isLocked())
	{
		return false;
	}
	if(!rule.m_dynamic)
	{
		return false;
	}
	//已达到最大值,不能理增长
	if(buffer.getBufferSize() >= rule.m_max_dynamic_buff_size)
	{
		return false;
	}
	DWORD buffer_size = buffer.getBufferSize()*2;
	if(buffer_size > rule.m_max_dynamic_buff_size)
	{
		buffer_size = rule.m_max_dynamic_buff_size;
	}
	char * temp_raw;
	char * data;
	if(!createBuffer(buffer_size,temp_raw,data))
	{
		return false;
	}
	char* discardData;

	//移动数据
	buffer.replace(data,buffer_size,&discardData);
	if(raw)
	{
		delete [] raw;
	}
	raw = temp_raw;
	return true;
}

void Session::unpack()
{
	if( !needUnpack() )
	{
		return;
	}
	char* msg_buf = m_recv_buffer.lockRead( m_recv_buffer.getDataSize() );
	if(!msg_buf)
	{
		return;
	}
	m_next_unpack_min_length = 0;
	DWORD index = m_unpackMethod.unpack( msg_buf,m_recv_buffer.getDataSize(),m_next_unpack_min_length );
	m_recv_buffer.unlockRead(index);
	//m_recv_buffer.forceAdjustBuffer();
}

bool Session::needUnpack() const
{
	return m_recv_buffer.getDataSize() >= m_next_unpack_min_length && m_recv_buffer.getDataSize();
}

bool Session::setSysPingTime(const SysPingTime* sysPingTime)
{
	if(!sysPingTime)
	{
		return false;
	}
	m_syspingTimes=sysPingTime;
	return true;
}


Session::DefaultUnpackModule::DefaultUnpackModule()
	:m_last_imcomplete_packet_len(0)
	,m_method(NULL)
{
}

void Session::DefaultUnpackModule::open(SessionUnpackMethod& method)
{
	m_method = &method;
	ASSERT(m_last_imcomplete_packet_len==0);
}

void Session::DefaultUnpackModule::close()
{
	m_last_imcomplete_packet_len = 0;
	m_method = NULL;
}

DWORD Session::DefaultUnpackModule::unpack(char* pLockedBuff,DWORD size,WORD& next_unpack_req_buff_size)
{
	DWORD index=0;	//已处理的数据包
	next_unpack_req_buff_size = sizeof(WORD);
	for(;;)
	{
		if(m_last_imcomplete_packet_len==0)
		{//获得包长度
			if(index + 2 > size)
			{
				return index;
			}
			m_last_imcomplete_packet_len = *(WORD *)pLockedBuff;
			if(m_last_imcomplete_packet_len > NETWORK_BUF_SIZE||m_last_imcomplete_packet_len < 2)
			{
				m_method->getSession()->setError(ERROR_UNPACK);
				return index;
			}
		}
		if( index + m_last_imcomplete_packet_len > size )
		{//不是完整的包
			next_unpack_req_buff_size = m_last_imcomplete_packet_len;
			return index;
		}
		Message * receivedMessage = (Message *)(pLockedBuff-offsetof(Message::Fake,buffer));
		receivedMessage->clear();
		if( !m_method->getSession()->parseMessage(*receivedMessage))
		{
			m_method->getSession()->setError(ERROR_PARSEMESSAGE);
			return index;
		}
		index+=m_last_imcomplete_packet_len;
		pLockedBuff+=m_last_imcomplete_packet_len;
		m_last_imcomplete_packet_len=0;	
		if( m_method->recommend_unpack_speed && index >= m_method->recommend_unpack_speed )
		{
			return index;
		}
	}
	return index;
}

Session::DefaultUnpackMethod::DefaultUnpackMethod()
{
}

void Session::DefaultUnpackMethod::open()
{
	m_module.open(*this);
}

void Session::DefaultUnpackMethod::close()
{
	m_module.close();
}

DWORD Session::DefaultUnpackMethod::unpack(char* pLockedBuff,DWORD size,WORD& next_unpack_req_buff_size)
{
	return m_module.unpack(pLockedBuff,size,next_unpack_req_buff_size);
}
