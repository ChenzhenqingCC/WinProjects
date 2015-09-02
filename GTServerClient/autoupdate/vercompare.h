////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	vercompare   version:  1.0   ? date: 04/27/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 - All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef VERCOMPARE_H
#define	VERCOMPARE_H
//////////////////////////////////////////////////////////////////////////
#include "libxmlnode.h"
#include <atlmisc.h>
#include <list>
#include <vector>
using namespace std;
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
/************************************************************************/
/* 
checker behavoir
*/
/************************************************************************/
struct XML_Checker 
{
	XML_Checker(const string& s_ver_path,const string& l_ver_path):
m_server_ver(s_ver_path),m_local_ver(l_ver_path),m_need_restart(false)
{
	//check the file exist or not		
}
//////////////////////////////////////////////////////////////////////////
bool Checking()
{
	if (!PathFileExists(m_local_ver.c_str()))
	{
		return true;
	}
	//compare the two files,if got diff return true
	xmlnode xserver = xmlnode_file(const_cast<char*>(m_server_ver.c_str()));
	xmlnode xlocal = xmlnode_file(const_cast<char*>(m_local_ver.c_str()));

	char* rootName = xmlnode_get_name(xserver);
	char* localrootName = xmlnode_get_name(xlocal);
	if (!rootName || !localrootName)
	{
		return false;
	}
	//compare got bug,not use string <
	string pser_ver = xmlnode_get_attrib(xserver,"version");
	string ploc_ver = xmlnode_get_attrib(xlocal,"version");
	
	//split string into vector
	vector<string> pws;
	vector<int>ver_vct_ser;

	boost::split(pws,pser_ver,boost::is_any_of("."));
	for(int i=0;i<pws.size();i++)
	{
		ver_vct_ser.push_back(atol(pws[i].c_str()));
	}
	vector<int>ver_vct_loc;
	boost::split(pws,ploc_ver,boost::is_any_of("."));
	for(int i=0;i<pws.size();i++)
	{
		ver_vct_loc.push_back(atol(pws[i].c_str()));
	}
	if (ver_vct_ser.size() != 4 || ver_vct_loc.size() != 4)
	{
		return false;
	}
	bool isold = false;
	for (int i=0;i<4;i++)
	{
		if (ver_vct_ser[i] > ver_vct_loc[i])
		{
			isold = true;
			break;
		}
	}

	if (isold)
	{
		for (xmlnode item = xmlnode_get_firstchild(xserver); item != NULL; item = xmlnode_get_nextsibling(item))
		{
			if(item->type != NTYPE_TAG)
				continue;
			string fname = xmlnode_get_attrib(item,"name");
			m_updatefiles.push_back(fname);
		}
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
list<string> GetUpdateFiles()
{
	return m_updatefiles;
}
//////////////////////////////////////////////////////////////////////////
bool NeedReStart()
{
	return m_need_restart;
}

protected:
	string					m_server_ver;
	string					m_local_ver;
	bool					m_need_restart;
	list<string>		m_updatefiles;
};
#endif