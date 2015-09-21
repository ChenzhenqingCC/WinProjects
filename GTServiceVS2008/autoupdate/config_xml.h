////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	config_xml   version:  1.0   ? date: 04/28/2009
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//	Copyright (C) 2009 - All Rights Reserved
//	Written By Jack Liu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef CONFIG_XML_H
#define CONFIG_XML_H
//////////////////////////////////////////////////////////////////////////
#include "lib/tinyxml.h"
#include <string>
using namespace std;
//#ifdef _DEBUG
//#pragma comment(lib,"tinyxmld.lib")
//#else
//#pragma comment(lib,"tinyxml.lib")
//#endif
//#pragma comment(lib,"lib/tinyxml_mt.lib")
/************************************************************************/
/* 
*/
/************************************************************************/
class Config
{
public:
	//////////////////////////////////////////////////////////////////////////
	Config(const string &pth):m_local_base(pth)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool Open(const string &pth)
	{
		m_path = pth;
		bool r =m_doc.LoadFile(pth.c_str());
		return r;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Close()
	{
		return m_doc.SaveFile();
	}
	//////////////////////////////////////////////////////////////////////////
	string GetUser()
	{
		string cur_ver;
		TiXmlNode* pver = m_doc.FirstChild("remote");
		if (pver)
		{
			TiXmlElement* pver_elem = pver->ToElement();
			cur_ver = pver_elem->Attribute("user");
		}
		return cur_ver;
	}
	//////////////////////////////////////////////////////////////////////////
	string GetPwd()
	{
		string cur_ver;
		TiXmlNode* pver = m_doc.FirstChild("remote");
		if (pver)
		{
			TiXmlElement* pver_elem = pver->ToElement();
			cur_ver = pver_elem->Attribute("pwd");
		}
		return cur_ver;
	}
	//////////////////////////////////////////////////////////////////////////
	string GetRemoteVerPath()
	{
		string cur_ver;
		TiXmlNode* pver = m_doc.FirstChild("remote");
		if (pver)
		{
			TiXmlElement* pver_elem = pver->ToElement();
			cur_ver = pver_elem->Attribute("version_url");
		}
		return cur_ver;
	}
	//////////////////////////////////////////////////////////////////////////
	string GetLocalVerPath()
	{
		string cur_ver;
		TiXmlNode* pver = m_doc.FirstChild("local");
		if (pver)
		{
			TiXmlElement* pver_elem = pver->ToElement();
			cur_ver = pver_elem->Attribute("version_url");
		}
		return cur_ver;
	}
	//////////////////////////////////////////////////////////////////////////
	string GetLocalUpdatePath()
	{
		string cur_ver=m_local_base;
		TiXmlNode* pver = m_doc.FirstChild("local");
		if (pver)
		{
			TiXmlElement* pver_elem = pver->ToElement();
			cur_ver += pver_elem->Attribute("update_path");

		}
		return cur_ver;
	}
	//////////////////////////////////////////////////////////////////////////
	string GetBaseUrl()
	{
		string cur_ver;
		TiXmlNode* pver = m_doc.FirstChild("remote");
		if (pver)
		{
			TiXmlElement* pver_elem = pver->ToElement();
			cur_ver = pver_elem->Attribute("base_url");
		}
		return cur_ver;
	}
	//////////////////////////////////////////////////////////////////////////
	string GetTargetPath()
	{
		string cur_ver=m_local_base;
		TiXmlNode* pver = m_doc.FirstChild("local");
		if (pver)
		{
			TiXmlElement* pver_elem = pver->ToElement();
			cur_ver += pver_elem->Attribute("targ_path");
		}
		return cur_ver;
	}
	//////////////////////////////////////////////////////////////////////////
	string GetUpdateePath()
	{
		string cur_ver;
		TiXmlNode* pver = m_doc.FirstChild("updatee");
		if (pver)
		{
			TiXmlElement* pver_elem = pver->ToElement();
			cur_ver = pver_elem->Attribute("path");
		}
		return cur_ver;
	}
	//////////////////////////////////////////////////////////////////////////
	string GetLocalPath()
	{
		return m_local_base;
	}
protected:
	string							m_path;
	string							m_local_base;
	TiXmlDocument		m_doc;
};
#endif