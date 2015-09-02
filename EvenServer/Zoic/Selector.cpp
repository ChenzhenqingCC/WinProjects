#include "include/Selector.h"
#include "include/Selectable.h"

using namespace Zoic;

#ifdef ZOIC_NO_SERVICE
	Zoic::Selector Zoic::theSelector;
#endif

Selector::Selector():
	m_socketArray(NULL),
	m_socketArraySize(0),
	m_activeSockets(NULL),
	m_activeSocketsSize(0)
{
}

Selector::~Selector()
{
	if(m_socketArray)
	{
		delete [] m_socketArray;
	}
	if(m_activeSockets)
	{
		delete [] m_activeSockets;
	}
}

//选择超时值
static timeval selectTimeOut = {0,0};

int Selector::run()
{
	if(m_sockets.empty())
	{
		return 0;
	}

	m_baseFdSet.copyTo(m_recvFdSet);
	m_baseFdSet.copyTo(m_sendFdSet);

	int num = select(m_baseFdSet.getFdCount(),m_recvFdSet,m_sendFdSet,NULL,&selectTimeOut);
	if( num <=0 )
	{
		return 0;
	}

	{//接收数据标志
		int checkCount;
		m_recvFdSet.getSockets(&checkCount,m_activeSockets);
		for(int index =0;index < checkCount;++index)
		{
			Selectable * socket = getSelectable(m_activeSockets[index]);
			socket->setCanRecvData(true);
		}
	}

	{//发送数据标志
		int checkCount;
		m_sendFdSet.getSockets(&checkCount,m_activeSockets);
		for(int index =0;index < checkCount;++index)
		{
			Selectable * socket = getSelectable(m_activeSockets[index]);
			socket->setCanSendData(true);
		}
	}
	return 0;
}

bool Selector::insert(Selectable * socket)
{
	if(m_sockets.insert(socket).second)
	{
		SOCKET s = socket->getSocket();
		m_baseFdSet.setSocket(s);

		if ( s < m_socketArraySize )
		{
			m_socketArray[s]=socket;
		}
		else
		{//增长m_socketArray
			DWORD newSize = s+1;
			Selectable ** newArray = new Selectable *[newSize];
			if(m_socketArray)
			{
				memcpy(newArray,m_socketArray,sizeof(Selectable *)*m_socketArraySize);
				delete [] m_socketArray;
			}
			memset(newArray+m_socketArraySize,0,sizeof(Selectable *)*(newSize-m_socketArraySize-1));
			newArray[s] = socket;
			m_socketArray = newArray;
			m_socketArraySize = newSize;
		}

		//增长m_activeSockets
		unsigned int size = static_cast<unsigned int>(m_sockets.size());
		if(size > m_activeSocketsSize)
		{
			SOCKET * activeSockets = new SOCKET [size];
			if(m_activeSockets)
			{
				memcpy(activeSockets,m_activeSockets,sizeof(SOCKET)*m_activeSocketsSize);
				delete [] m_activeSockets;
			}
			m_activeSockets = activeSockets;
			m_activeSocketsSize = size;
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool Selector::remove(Selectable * socket)
{
	if(m_sockets.erase(socket)!=0)
	{
		SOCKET s = socket->getSocket();
		m_baseFdSet.clearSocket(s);
		m_socketArray[s]=NULL;
		return true;
	}
	else
	{
		return false;
	}
}

bool Selector::CmpSocket::operator()(Selectable *a ,Selectable * b) const
{
	return a->getSocket() < b->getSocket();
}
