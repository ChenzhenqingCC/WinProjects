#ifndef __RANAINPUTARGUMENTEXCEPTION_H
#define __RANAINPUTARGUMENTEXCEPTION_H
#include <exception>
#include <string>
#include "RanaHeader.h"

class RanaInputArgumentException:
	public std::exception
{
public:
	RanaInputArgumentException(RanaState L,const char * str);
	const char *what() const throw();
	~RanaInputArgumentException() throw();
	//追加前缀
	void appendPrefix(const char *name,bool array=false);
	static std::string create_tail_call_info(RanaState L);
private:
	//禁止拷贝操作
	RanaInputArgumentException(const RanaInputArgumentException &);
	RanaInputArgumentException & operator=(const RanaInputArgumentException &);

	std::string m_str;
	int	m_level;
	bool m_array;	//上一次是否是数组元素
};

#endif
