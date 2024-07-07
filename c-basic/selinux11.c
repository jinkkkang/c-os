#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <selinux/selinux.h>
// int setreuid(uid_t ruid, uid_t euid);

int main(int argc, char **argv){
   int ret, enforce=0;
   ret = selinux_mkload_policy(1);

   if (ret < 0) {
            fprintf(stderr, _("%s:  Can't load policy:  %s\n"),
                  argv[0], strerror(errno));
           return 2;
   }
   return 0;
}