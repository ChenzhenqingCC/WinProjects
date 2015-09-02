#ifndef ZOIC_NO_SERVICE
#include "include/Signal.h"
#include "include/Log.h"
#include <signal.h>

using namespace Zoic;

static void  signalset(void (*func)(int));
static const char * signame(int sig);
 
Signal::Signal():
	m_sig(0)
{
	signalset(sighandler);
}

int Signal::run()
{
	if(m_sig)
	{
		logerror_f("[Signal::run]: %s [%d] received.", signame(m_sig), m_sig);
//		printf("%s [%d] received.\n",signame(m_sig),m_sig);
	}
	return m_sig;
}

void Signal::sighandler(int sig)
{
	Signal::getInstance()->m_sig=sig;
}

/* signal interrupt function set */
static void  signalset(void (*func)(int))
{
#ifdef SIGHUP
	signal(SIGHUP,		func);		/* 1 : hangup */
#endif
#ifdef SIGINT
	signal(SIGINT,		func);		/* 2 : interrupt (rubout) */
#endif
#ifdef SIGBUS
	signal(SIGBUS,		SIG_IGN);	/* 10 : bus error */
#endif
#ifdef SIGPIPE
	signal(SIGPIPE,		SIG_IGN);	/* 13 : write on a pipe with no one to read it */
#endif
#ifdef SIGALRM
	signal(SIGALRM,		func);		/* 14 : alarm clock */
#endif
#ifdef SIGTERM
	signal(SIGTERM,		func);		/* 15 : software termination signal from kill */
#endif
#ifdef SIGURG
	signal(SIGURG,		func);		/* 16 : urgent condition on IO channel */
#endif
#ifdef SIGCONT
	signal(SIGCONT,		func);		/* 19 : continue a stopped process */
#endif
#ifdef SIGTTIN
	signal(SIGTTIN,		func);		/* 21 : to readers pgrp upon background tty read */
#endif
#ifdef SIGTTOU
	signal(SIGTTOU,		func);		/* 22 : like TTIN if (tp->t_local&LTOSTOP) */
#endif
#ifdef SIGIO
	signal(SIGIO,		func);		/* 23 : input/output possible signal */
#endif
#ifdef SIGXCPU
	signal(SIGXCPU,		func);		/* 24 : exceeded cpu limit */
#endif
#ifdef SIGXFSZ
	signal(SIGXFSZ,		func);		/* 25 : exceeded file size limit */
#endif
#ifdef SIGVTALRM
	signal(SIGVTALRM,	func);		/* 26 : virtual time alarm */
#endif
#ifdef SIGPROF
	signal(SIGPROF,		func);		/* 27 : profiling time alarm */
#endif
#ifdef SIGWINCH
	signal(SIGWINCH,	SIG_IGN);	/* 28 : window size change */
#endif
#ifdef SIGINFO
	signal(SIGINFO,		func);		/* 29 : information request */
#endif
#ifdef SIGUSR1
	signal(SIGUSR1,		func);		/* 30 : user defined signal 1 */
#endif
#ifdef SIGUSR2
	signal(SIGUSR2,		func);		/* 31 : user defined signal 2 */
#endif
}

struct SingnalNameItem
{
	int	sig;
	const char * name;
};

static SingnalNameItem s_SingnalNameItemTable[]=
{
#ifdef SIGHUP
	{ SIGHUP,	 "SIGHUP"},
#endif
#ifdef SIGINT
	{ SIGINT,	 "SIGINT"},
#endif
#ifdef SIGQUIT
	{ SIGQUIT,	 "SIGQUIT"},
#endif
#ifdef SIGILL
	{ SIGILL,	 "SIGILL"},
#endif
#ifdef SIGTRAP
	{ SIGTRAP,	 "SIGTRAP"},
#endif
#ifdef SIGABRT
	{ SIGABRT,	 "SIGABRT"},
#endif
#ifdef SIGEMT
	{ SIGEMT,	 "SIGEMT"},
#endif
#ifdef SIGFPE
	{ SIGFPE,	 "SIGFPE"},
#endif
#ifdef SIGKILL
	{ SIGKILL,	 "SIGKILL"},
#endif
#ifdef SIGBUS
	{ SIGBUS,	 "SIGBUS"},
#endif
#ifdef SIGSEGV
	{ SIGSEGV,	 "SIGSEGV"},
#endif
#ifdef SIGSYS
	{ SIGSYS,	 "SIGSYS"},
#endif
#ifdef SIGPIPE
	{ SIGPIPE,	 "SIGPIPE"},
#endif
#ifdef SIGALRM
	{ SIGALRM,	 "SIGALRM"},
#endif
#ifdef SIGTERM
	{ SIGTERM,	 "SIGTERM"},
#endif
#ifdef SIGURG
	{ SIGURG,	 "SIGURG"},
#endif
#ifdef SIGSTOP
	{ SIGSTOP,	 "SIGSTOP"},
#endif
#ifdef SIGTSTP
	{ SIGTSTP,	 "SIGTSTP"},
#endif
#ifdef SIGCONT
	{ SIGCONT,	 "SIGCONT"},
#endif
#ifdef SIGCHLD
	{ SIGCHLD,	 "SIGCHLD"},
#endif
#ifdef SIGTTIN
	{ SIGTTIN,	 "SIGTTIN"},
#endif
#ifdef SIGTTOU
	{ SIGTTOU,	 "SIGTTOU"},
#endif
#ifdef SIGIO
	{ SIGIO,		 "SIGIO"},
#endif
#ifdef SIGXCPU
	{ SIGXCPU,	 "SIGXCPU"},
#endif
#ifdef SIGXFSZ
	{ SIGXFSZ,	 "SIGXFSZ"},
#endif
#ifdef SIGVTALRM
	{ SIGVTALRM,	 "SIGVTALRM"},
#endif
#ifdef SIGPROF
	{ SIGPROF,	 "SIGPROF"},
#endif
#ifdef SIGWINCH
	{ SIGWINCH,	 "SIGWINCH"},
#endif
#ifdef SIGINFO			
	{ SIGINFO,	 "SIGINFO"},
#endif
#ifdef SIGUSR1
	{ SIGUSR1,	 "SIGUSR1"},
#endif
#ifdef SIGUSR2
	{ SIGUSR2,	 "SIGUSR2"},
#endif
#ifdef SIGTHR
	{ SIGTHR,	 "SIGTHR"},
#endif
};

static const char * signame(int sig)
{
	for(int i=0;i< sizeof(s_SingnalNameItemTable)/sizeof(s_SingnalNameItemTable[0]);++i)
	{
		if(s_SingnalNameItemTable[i].sig==sig)
		{
			return s_SingnalNameItemTable[i].name;
		}
	}
	return "UNKNOWN";
}

#endif
