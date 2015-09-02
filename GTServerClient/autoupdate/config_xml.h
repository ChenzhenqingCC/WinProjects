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
//#include "lib/tinyxml.h"
#include "lib/rapidxml/rapidxml.hpp"
#include "lib/rapidxml/rapidxml_utils.hpp"
#include "lib/rapidxml/rapidxml_print.hpp"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
using namespace rapidxml;
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
		m_file = new file<>(pth.c_str());
		if (m_file)
			m_doc.parse<0>(m_file->data());
		//bool r =m_doc.LoadFile(pth.c_str());
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Close()
	{
		std::string text;
		rapidxml::print(std::back_inserter(text), m_doc, 0);

		std::cout << text << std::endl;

		std::ofstream out(m_path.c_str());
		out << m_doc;
		return true;
		//return m_doc.
		//return m_doc.SaveFile();
	}

	string GetNodeValue(const char* node, const char * attr)
	{
		string cur_ver = "";
		xml_node<>* pver = m_doc.first_node(node);
		if (pver)
		{
			//TiXmlElement* pver_elem = pver->ToElement();
			xml_attribute<> *xatt = pver->first_attribute(attr);
			if (xatt)
				cur_ver = xatt->value();
		}
		return cur_ver;
	}
	//////////////////////////////////////////////////////////////////////////
	string GetUser()
	{
		return GetNodeValue("remote", "user");
		//string cur_ver = "";
		//xml_node<>* pver = m_doc.first_node("remote");
		//if (pver)
		//{
		//	//TiXmlElement* pver_elem = pver->ToElement();
		//	xml_attribute<> *xatt = pver->first_attribute("user");
		//	if (xatt)
		//		cur_ver = xatt->value();
		//}
		//return cur_ver;
	}
	//////////////////////////////////////////////////////////////////////////
	string GetPwd()
	{
		return GetNodeValue("remote", "pwd");
		//string cur_ver = "";
		////TiXmlNode* pver = m_doc.FirstChild("remote");
		//xml_node<>* pver = m_doc.first_node("remote");
		//if (pver)
		//{
		//	//TiXmlElement* pver_elem = pver->ToElement();
		//	//cur_ver = pver_elem->Attribute("pwd");
		//	xml_attribute<> *xatt = pver->first_attribute("pwd");
		//	if (xatt)
		//		cur_ver = xatt->value();
		//}
		//return cur_ver;
	}
	//////////////////////////////////////////////////////////////////////////
	string GetRemoteVerPath()
	{
		return GetNodeValue("remote", "version_url");
		//string cur_ver = "";
		////TiXmlNode* pver = m_doc.FirstChild("remote");
		//xml_node<>* pver = m_doc.first_node("remote");
		//if (pver)
		//{
		//	//TiXmlElement* pver_elem = pver->ToElement();
		//	//cur_ver = pver_elem->Attribute("version_url");
		//	xml_attribute<> *xatt = pver->first_attribute("version_url");
		//	if (xatt)
		//		cur_ver = xatt->value();
		//}
		//return cur_ver;
	}
	//////////////////////////////////////////////////////////////////////////
	string GetLocalVerPath()
	{
		return GetNodeValue("local", "version_url");
		/*string cur_ver = "";
		TiXmlNode* pver = m_doc.FirstChild("local");
		if (pver)
		{
		TiXmlElement* pver_elem = pver->ToElement();
		cur_ver = pver_elem->Attribute("version_url");
		}
		return cur_ver;*/
	}
	//////////////////////////////////////////////////////////////////////////
	string GetLocalUpdatePath()
	{
		return GetNodeValue("local", "update_path");
		/*string cur_ver=m_local_base;
		TiXmlNode* pver = m_doc.FirstChild("local");
		if (pver)
		{
		TiXmlElement* pver_elem = pver->ToElement();
		cur_ver += pver_elem->Attribute("update_path");

		}
		return cur_ver;*/
	}
	//////////////////////////////////////////////////////////////////////////
	string GetBaseUrl()
	{
		return GetNodeValue("remote", "base_url");
		/*string cur_ver;
		TiXmlNode* pver = m_doc.FirstChild("remote");
		if (pver)
		{
		TiXmlElement* pver_elem = pver->ToElement();
		cur_ver = pver_elem->Attribute("base_url");
		}
		return cur_ver;*/
	}
	//////////////////////////////////////////////////////////////////////////
	string GetTargetPath()
	{
		return GetNodeValue("local", "targ_path");
		/*string cur_ver=m_local_base;
		TiXmlNode* pver = m_doc.FirstChild("local");
		if (pver)
		{
		TiXmlElement* pver_elem = pver->ToElement();
		cur_ver += pver_elem->Attribute("targ_path");
		}
		return cur_ver;*/
	}
	//////////////////////////////////////////////////////////////////////////
	string GetUpdateePath()
	{
		return GetNodeValue("updatee", "path");
		/*string cur_ver;
		TiXmlNode* pver = m_doc.FirstChild("updatee");
		if (pver)
		{
		TiXmlElement* pver_elem = pver->ToElement();
		cur_ver = pver_elem->Attribute("path");
		}
		return cur_ver;*/
	}
	//////////////////////////////////////////////////////////////////////////
	string GetLocalPath()
	{
		return m_local_base;
	}
protected:
	string							m_path;
	string							m_local_base;
	xml_document<>		m_doc;
	file<>*				m_file;
};
#endif