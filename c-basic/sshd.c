#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>  

int use_privsep = -1;
int privsep_chroot = 1;
int main() {  
	/* Store privilege separation user for later use if required. */
	privsep_chroot = use_privsep && (getuid() == 0 || geteuid() == 0);
    printf("privsep_chroot=%d\n", privsep_chroot);
}