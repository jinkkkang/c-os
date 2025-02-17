/*
 * Load test for cppc_cpufreq driver
 * Author: Rajasi Kulkarni
 *
 * Copyright (c) 2017 Red Hat, Inc. All rights reserved. This copyrighted material
 * is made available to anyone wishing to use, modify, copy, or
 * redistribute it subject to the terms and conditions of the GNU General
 * Public License v.2.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

void usage(char *name) {
	printf ("Usage: %s cpunum\n", name);
}

int main(int argc, char **argv) {
	int i, cpu, fd;
	struct timeb before, after;
	long long int start, finish, total;
	double freq;
	cpu_set_t cpuset;

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

	ftime(&before);
	start = before.time*1000 + before.millitm;

	for (i=0; i<0x8fffffff; i++) {
		sqrt(i);
		/* show periodic output */
		if(i!=0 && ((i == 0x8ffffffe) || (i % 0x2fffffff == 0))){
			ftime(&after);
			finish = after.time*1000 + after.millitm;
			total = finish - start;
			printf("running: %4.2f\n", 1.0*total/1000);
		}
	}

	printf("runTime: %4.2f\n", 1.0*total/1000);
	return 0;
}
