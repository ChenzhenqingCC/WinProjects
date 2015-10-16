// NewTreeListCtrl.h : main header file for the NEWTREELISTCTRL application
//

#if !defined(AFX_NEWTREELISTCTRL_H__F112567C_0EFB_43C4_9A66_EE05D8C5D4D6__INCLUDED_)
#define AFX_NEWTREELISTCTRL_H__F112567C_0EFB_43C4_9A66_EE05D8C5D4D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CNewTreeListCtrlApp:
// See NewTreeListCtrl.cpp for the implementation of this class
//

class CNewTreeListCtrlApp : public CWinApp
{
public:
	CNewTreeListCtrlApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewTreeListCtrlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CNewTreeListCtrlApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWTREELISTCTRL_H__F112567C_0EFB_43C4_9A66_EE05D8C5D4D6__INCLUDED_)
