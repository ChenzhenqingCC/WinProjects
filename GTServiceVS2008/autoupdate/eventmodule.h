////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	eventmodule   version:  1.0   ? date: 09/29/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 ztgame Corporation- All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef EVENTMODULE_H
#define EVENTMODULE_H
//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* 
//	Event contains the info about the event
// this class is used for containing information,when an event happens
// in this case,the src_obj and tar_obj can be defined by derive class
//	by default,these are pointed to objects,in another case ,register/
//	unregister event handler,also use this class,so the type is the event
// which the handler will care about,and src_obj now is the handler's 
//	address
*/
/************************************************************************/
class Event
{
public:
	//special event type:remove_handler_event/broadcast_event
	enum{REMOVEALL=-2,BROADCAST};
	//event type define by derive class
	int			type;
	//point to source object which notify this event
	void*		src_obj;
	//point to the target object which can handle this event
	void*		tar_obj;
	//buffer size,if needed
	DWORD	dwData;
	Event(const int t=BROADCAST,void *ps=0,void *pt=0,
				const DWORD dw =0):type(t),src_obj(ps),
				tar_obj(pt),dwData(dw)
	{
	}
};
/************************************************************************/
/* 
//	EventHandler
*/
/************************************************************************/
class EventHandler
{
public:
	//handle the event,if return -1 will stop the chain to down
	virtual int Handler(const Event &pe)=0;
	virtual ~EventHandler(){}
};
/************************************************************************/
/* 
//Dispatcher event to listeners
*/
/************************************************************************/
class Dispatcher
{
public:
	//add listener in base,the result is undefined
	virtual DWORD Register(const Event &e)=0;
	//remove the listener from base by its id or addr
	virtual DWORD	UnRegister(const Event &e)=0;
	//raise an event
	virtual	int Notify(const Event &e)=0;
	//notify all the listeners
	virtual int Notify()=0;
	virtual ~Dispatcher(){}
};
#endif