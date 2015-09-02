#include "extendfileappender.h"


using namespace Zoic;

ExtendFileAppender::ExtendFileAppender()
{
	//½«LAT_FILE¸ÄÎªLAT_EXTENDFILE
	setType(LAT_EXTENDFILE);
}

ExtendFileAppender::~ExtendFileAppender()
{

}

void ExtendFileAppender::init()
{
	FileAppender::init();
}