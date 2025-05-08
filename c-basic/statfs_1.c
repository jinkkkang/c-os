#include <stdio.h>
#include <stdlib.h>
#include <sys/statfs.h>
#include <errno.h>
#include <unistd.h>
 
//   struct statfs
//   {
//     __fsword_t f_type; /* 文件系统类型 *
//     __fsword_t f_bsize; /* 经过优化的传输块大小，单位B*/
// #ifndef __USE_FILE_OFFSET64
//     __fsblkcnt_t f_blocks; /* 文件系统数据块总数 *
//     __fsblkcnt_t f_bfree; /* 可用块数 */
//     __fsblkcnt_t f_bavail; /* 非超级用户可获取的块数 *
//     __fsfilcnt_t f_files; /* 文件结点总数 */
//     __fsfilcnt_t f_ffree; /* 可用文件结点数 */
// #else
//     __fsblkcnt64_t f_blocks;
//     __fsblkcnt64_t f_bfree;
//     __fsblkcnt64_t f_bavail;
//     __fsfilcnt64_t f_files;
//     __fsfilcnt64_t f_ffree;
// #endif
//     __fsid_t f_fsid; /* 文件系统标识 */
//     __fsword_t f_namelen; /* 文件名的最大长度 */
//     __fsword_t f_frsize;
//     __fsword_t f_flags;
//     __fsword_t f_spare[4];
//   };
 
int main()
{
    struct statfs diskInfo1;
    statfs("/", &diskInfo1);
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
 
 