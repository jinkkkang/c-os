#include <stdio.h>
#include <stdlib.h>
#include <sys/statfs.h>
#include <errno.h>
#include <unistd.h>
 
int main()
{
    struct statfs diskInfo1;

    int fd1 = open("/", O_RDONLY);
    if(fd1 == -1) {
        perror("open /");
        return -1;
    }

    fd1 = fstatfs(fd1, &diskInfo1);
    if(fd1 == -1) {
        perror("fstatfs fd1");
        close(fd1);
        return -1;
    }
    unsigned long long blocksize1 = diskInfo1.f_bsize; //每个block里包含的字节数
    unsigned long long totalsize1 = blocksize1 * diskInfo1.f_blocks; //总的字节数，f_blocks为block的数目
    unsigned long long freeDisk1 = diskInfo1.f_bfree * blocksize1; //剩余空间的大小
    unsigned long long availableDisk1 = diskInfo1.f_bavail * blocksize1; //可用空间大小
    printf("fstype: %#x \n",diskInfo1.f_type);
    printf("Total_size = %llu B = %llu KB = %llu MB = %llu GB\n",
           totalsize1, totalsize1>>10, totalsize1>>20, totalsize1>>30);
    printf("Disk_free1      = %llu MB \t= %llu GB\n", freeDisk1>>20, freeDisk1>>30);
    printf("Disk_available1 = %llu MB \t= %llu GB\n", availableDisk1>>20, availableDisk1>>30);
    return 0;
}
 
 