#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>



/*特别注意，当映射文件大小为0时，不能创建映射区。所以：用于映射的文件必须要有实际大小！！
 mmap使用时常常会出现总线错误，通常是由于共享文件存储空间大小引起的。
 */

int main(int argc, const char *argv[])
{
   
        int fd;
        char *addr;
        int count = 0;


        fd = open("/home/hello.log", O_RDWR|O_CREAT );
        if (fd < 0) {
            printf("error_code: %d\n", errno);
            printf("Error opening file: %s\n", strerror(errno));
            return 1;
        }
        int pagesize =  sysconf(_SC_PAGESIZE);
        int size = pagesize * 3;
        addr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
        if (addr == MAP_FAILED) {
            printf("error_code: %d\n", errno);
            printf("mmap filed: %s\n", strerror(errno));
            return 1;
        }

        printf("PID: %d, ADDR: %p\n", getpid(), addr);
        while(1) {
   
                printf("%d: write ...\n", count++);
                sprintf(addr, "hello world %d!\n", count);
                sleep(3);
                fsync(fd);
                sleep(5);

        }
}