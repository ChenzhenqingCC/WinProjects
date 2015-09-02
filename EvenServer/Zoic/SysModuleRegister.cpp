#include "include/SysModuleRegister.h"
#include "include/Signal.h"
#ifndef ZOIC_NO_SERVICE

using namespace Zoic;

void Zoic::registerSysModules(void)
{
	MODULE_IMPL(Signal)
}

#endif
