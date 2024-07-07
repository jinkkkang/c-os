#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// int setreuid(uid_t ruid, uid_t euid);

int main(void){
   int r1, r2;

   r2 = setregid(987, 987);
   printf("r2: %d\n", r2);

   r1 = setreuid(991, 991);
   printf("r1: %d\n", r1);

   int fd=open("/var/lib/chrony/drift", O_RDONLY);
 
   if(fd == -1){
      printf("error_code: %d\n", errno);
      printf("Error opening file: %s\n", strerror(errno)); //strerror from string.h
      return 1;
 
   }
   printf("fd: %d\n", fd);
   printf("successful to open the file\n");
 
   close(fd);
 
   return 0;
 
}