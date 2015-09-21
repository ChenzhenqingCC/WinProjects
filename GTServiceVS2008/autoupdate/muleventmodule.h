////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	eventmodule   version:  1.0   ? date: 09/30/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 ztgame Corporation- All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "eventmodule.h"
#include <map>
using namespace std;

/************************************************************************/
/* 
//dispatcher use multimap contains all l
*/
/************************************************************************/
class MultiDispatcher : public Dispatcher
{
public:
	//add listener in base,and return its id,always return 0
	virtual DWORD Register(const Event &e)
	{
		EventHandler *pel = (EventHandler*)e.src_obj;
		if(pel)
		{
			m_listeners.insert(std::make_pair(e.type,pel));
		}
		return 0;
	}
	//remove the listener from base by its addr
	virtual DWORD	UnRegister(const Event &e)
	{
		//remove all the events sent to this handler
		if (e.type == Event::REMOVEALL)
		{
			Iter iter = m_listeners.begin();
			while (iter != m_listeners.end())
			{
				if (iter->second == e.tar_obj)
				{
					m_listeners.erase(iter);
				}
				iter++;
			}
		}
		//remove the handler just on this event
		else
		{
			Iterate(e.type,boost::bind(&MultiDispatcher::EraseHandler,this,e,_1));
		}
		return 0;
	}
	//raise an event,check if got all listeners
	virtual	int Notify(const Event &e)
	{
		//notify the event's handlers first
		int res = Iterate(e.type,boost::bind(&MultiDispatcher::Notify_Impl,this,e,_1));
		if (res != -1)
		{
			//broadcast
			res = Iterate(Event::BROADCAST,boost::bind(&MultiDispatcher::Notify_Impl,this,e,_1));
		}
		return res;
	}
	//notify all the listeners
	virtual int Notify()
	{
		return 0;
	}
protected:
	typedef std::multimap<int,EventHandler*>		L_Table;
	typedef L_Table::const_iterator 							CIter;
	typedef	L_Table::iterator										Iter;
	typedef std::pair<Iter, Iter> 									Range;
protected:
	int Notify_Impl(const Event &e,Iter &iter)
	{
		EventHandler* pe = iter->second;
		if (pe)
		{
			return pe->Handler(e);
		}
		return -1;
	}

	int EraseHandler(const Event &e,Iter &iter)
	{
		if ((iter->second) == e.tar_obj)
		{
			m_listeners.erase(iter);
			return -1;
		}
		return 0;
	}

	template<class Fun>int Iterate(const int type,Fun iterfun)
	{
		Range range= m_listeners.equal_range(type);
		for (Iter iter = range.first; iter != range.second;iter++)
		{
			if(iterfun(iter) ==-1)
			{
				return -1;
			}
		}
		return 0;
	}
protected:
	std::multimap<int,EventHandler*>						m_listeners;
};