#include "extendfileappender.h"


using namespace Zoic;

ExtendFileAppender::ExtendFileAppender()
{
	//��LAT_FILE��ΪLAT_EXTENDFILE
	setType(LAT_EXTENDFILE);
}

ExtendFileAppender::~ExtendFileAppender()
{

}

void ExtendFileAppender::init()
{
	FileAppender::init();
}