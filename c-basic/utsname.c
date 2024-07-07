#include <stdio.h>
#include <sys/utsname.h>


static void kern_release(void)
{
    struct utsname name;

    if (uname(&name) == -1) {
        printf("cannot get system version\n");
        return;
    }
    printf("release: %s machine: %s version:%s\n",name.release, name.machine, name.version);
   //  strncpy(kern_version, name.release, sizeof(name.release));
   //  strncpy(machine, name.machine, sizeof(name.machine));
}

int main(void){
   kern_release();
   return 0;
 
}