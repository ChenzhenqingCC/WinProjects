
// PngBkTemplate.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPngBkTemplateApp: 
// �йش����ʵ�֣������ PngBkTemplate.cpp
//

class CPngBkTemplateApp : public CWinApp
{
public:
	CPngBkTemplateApp();

	ULONG_PTR m_pGdiToken;
// ��д
public:
	virtual BOOL InitInstance();
	virtual BOOL ExitInstance();
// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPngBkTemplateApp theApp;