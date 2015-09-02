////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	workitem   version:  1.0   ? date: 1/15/2013
//  -------------------------------------------------------------
// 
//
//
//  -------------------------------------------------------------
//	Copyright (C) 2013 dasjack- All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef WORKITEM_H
#define WORKITEM_H
#include <tchar.h>
#include <atlstr.h>
//forward declear
class CGtserviceModule;
/**
*
*/
class CWorkItem{
public:
	//
	//enum {ERROR=-1,INITED=0,RUNNING=1,DONE=2};
public:

	CWorkItem(LPCTSTR name,const DWORD por):m_name(name),m_poir(por),m_state(0),m_papp(0)
	{
	}
	/**
	*
	*/
	virtual void Run(const DWORD interval,const DWORD timed)=0;
	/**
	*
	*/
	int GetState()
	{
		return m_state;
	}
	/**
	*
	*/
	void SetApp(CGtserviceModule* papp)
	{
		m_papp = papp;
	}
protected:
	CString				m_name;
	DWORD				m_poir;
	int					m_state;
	CGtserviceModule*	m_papp;
};

#endif