////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	autoudpate   version:  1.0   ? date: 09/28/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 ztgame Corporation- All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AUTOUPDATE_H
#define AUTOUPDATE_H
/****************************************************************************************************************************/
/* 
the main module class
*/
/****************************************************************************************************************************/
class CAutoUpdateModule : public CAppModule
{
public:
	// Overrides of CAppModule::Init and Term
	//init env and app object
	void Init(ATL::_ATL_OBJMAP_ENTRY* pObjMap, HINSTANCE hInstance, const GUID* pLibID = NULL);
	//finish
	void Term();
	//run the main message loop
	int Run(LPTSTR lpstrCmdLine = NULL, int nCmdShow = SW_SHOWDEFAULT);
	//return cur process path
	CString GetLocalPath();
protected:
	//check if the same instance existed
	void Check_Exist();
protected:
	CString	m_path;
};
#endif