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
	//׷��ǰ׺
	void appendPrefix(const char *name,bool array=false);
	static std::string create_tail_call_info(RanaState L);
private:
	//��ֹ��������
	RanaInputArgumentException(const RanaInputArgumentException &);
	RanaInputArgumentException & operator=(const RanaInputArgumentException &);

	std::string m_str;
	int	m_level;
	bool m_array;	//��һ���Ƿ�������Ԫ��
};

#endif
