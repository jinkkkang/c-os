#define _GNU_SOURCE
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/timeb.h>
#include <sched.h>
#include <errno.h>

void usage(char *name) {
	printf ("Usage: %s cpunum\n", name);
}

int main(int argc, char **argv) {
	int cpu;
	cpu_set_t cpuset;
	unsigned int junk=0;

	if (argc != 2) {
		usage(argv[0]);
		return 1;
	}

	errno = 0;
 	cpu = strtol(argv[1], (char **) NULL, 10);

	if (errno) {
		usage(argv[0]);
		return 1;
	}


	CPU_ZERO(&cpuset);
	CPU_SET(cpu, &cpuset);

	if (sched_setaffinity(0, sizeof(cpu_set_t), &cpuset)) {
		perror("Failed to set cpu affinity");
		return 1;
	}

	while(1)
	{
		sqrt(junk++);
	}

	return 0;
}
