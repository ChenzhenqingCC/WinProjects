#include "include/SplitString.h"
#include "include/MissingFunction.h"

static const int MAX_STRING_LENGTH=5000;
void Zoic::splitString(const char *src,std::vector<std::string> &dst,const char *separator)
{
	dst.clear();
	char buf[MAX_STRING_LENGTH+1];
	strlcpy(buf,src,sizeof(buf));
	char *token=strtok(buf,separator);
	while(token)
	{
		dst.push_back(token);
		token=strtok(NULL,separator);
	}
}
