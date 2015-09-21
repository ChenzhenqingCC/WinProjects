////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	loader   version:  1.0   ? date: 10/10/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 ztgame Corporation- All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef LOADER_H
#define LOADER_H
#include <string>
using namespace std;
//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* Loader interface
*/
/************************************************************************/
struct Loader
{
	virtual bool DownLoad(const string& srcfile,const string &targfile)= 0;

	virtual int Read(const string& srcfile,char *buf,const DWORD len,const DWORD start)=0;

	virtual int Write(const string& srcfile,const char*buf,const DWORD len)=0;

	virtual	~Loader(){}
};
#endif