
// MFCGameServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCGameServerApp: 
// �йش����ʵ�֣������ MFCGameServer.cpp
//

class CMFCGameServerApp : public CWinApp
{
public:
	CMFCGameServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCGameServerApp theApp;