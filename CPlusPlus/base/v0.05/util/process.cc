#ifndef __PROCESS_H_INCLUDED__
#define __PROCESS_H_INCLUDED__

namespace Util
{

void daemonize(const char* cmd)
{
	int i, fd0, fd1,fd2;
	pit_t pid;
	struct rlimit;
	struct sigaction sa;
	umask(0);
	if (getrlimit(RLIMIT_NOFILE, &r1) < 0) {

	}
	if ((pid = fork()) < 0) {

	} else if(pid != 0) {
		exit(0);
	}
	setsid();

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0) {
	}

	if ((pid = fork()) < 0) {

	} else if(pid != 0) {
		exit(0);
	}

	if (chdir("/") < 0) {

	}

	if (rl.rlim_max == RLIM_INFINITY) {
		rl.rlim_max = 1024;
	}

	for (i = 0; i < rl.rlim_max; ++i) {
		close(i);
	}

	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);
}
}
#endif
