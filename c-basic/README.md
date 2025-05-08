
## C的一些基础语法

[字符/字符串的基本介绍](./char1.c)  
[字符串的连接，拷贝等](./char2.c)  
[条件判断](./condition1.c)  
[数组](./arr1.c)  
[枚举](./enum1.c)  
[共用体union](./union1.c)  
[结构体](./struct1.c)   
[文件读写fopen](./file1.c) 
[文件读写open]
[函数/回调函数](./function_callback1.c)  
[错误处理/errno](./err1.c)  
[指针](./pointer1.c)   
[命令行](./args1.c)   
[命令行/参数解析getopt](./args2.c)   
[命令行/参数解析getopt_long](./args3.c)    
[进程中的内存]

[宏](./macros1.c)  
[预定义宏](./macros2.c)   

## 系统编程
[文件/查找某路径文件](./glob1.c)  
[文件/判断文件是否存在的几个方法](./file_checkexist.c)  

[内存/内存分配](./mem_malloc1.c)  
[内存/mmap映射](./mem_file_mmap1.c)  
[内存/sysinfo获取(sys/sysinfo.h)](./mem_sysinfo1.c)  

[信号](./signal1.c)  
[信号的处理*推荐](./signal2.c)  

[系统命令/system](./sys_exec1.c)  
[系统命令/popen,pclose](./sys_exec2.c)  
[获取系统&内核版本等](./utsname.c)  
[环境变量/env1.c](./env1.c)  
[获取系统cpu数量(sys/sysinfo.h)](./sysinfo1.c)


[获取已挂载的磁盘的文件系统信息_by_挂载路径](./statfs_1.c)
[获取已挂载的磁盘的文件系统信息_by_fd](./statfs_2.c)  


/usr/include/sys/ 下的头文件学习

* sys/sysinfo.h 头文件 可以获取的信息
1 
```
struct sysinfo {
        long uptime;             /* 启动到现在经过的时间 */
        unsigned long loads[3];  /* I/O 在1, 5, 和 15 分钟内执行的平均时间 */
        unsigned long totalram;  /* 总的可用的内存大小  */
        unsigned long freeram;   /* 还未被使用的内存大小  */
        unsigned long sharedram; /* 共享的存储器的大小 */
        unsigned long bufferram; /* 缓冲区大小 */
        unsigned long totalswap; /* 交换区大小 */
        unsigned long freeswap;  /* 还可用的交换区大小 */
        unsigned short procs;    /* 当前进程数目 */
        unsigned long totalhigh; /* 高内存大小 */
        unsigned long freehigh;  /* 可用的高内存大小 */
        unsigned int mem_unit;   /* 内存单元大小 */
        char _f[20-2*sizeof(long)-sizeof(int)]; /* 填充到64个字节 */
};
```
2  获取cpu数量，物理页等

```
/* Return number of configured processors.  */
extern int get_nprocs_conf (void) __THROW;

/* Return number of available processors.  */
extern int get_nprocs (void) __THROW;


/* Return number of physical pages of memory in the system.  */
extern long int get_phys_pages (void) __THROW;

/* Return number of available physical pages of memory in the system.  */
extern long int get_avphys_pages (void) __THROW;

```

* sys/utsname.h  获取内核八本

```
/* Structure describing the system and machine.  */
struct utsname
  {
    /* Name of the implementation of the operating system.  */
    char sysname[_UTSNAME_SYSNAME_LENGTH];

    /* Name of this node on the network.  */
    char nodename[_UTSNAME_NODENAME_LENGTH];

    /* Current release level of this implementation.  */
    char release[_UTSNAME_RELEASE_LENGTH];
    /* Current version level of this release.  */
    char version[_UTSNAME_VERSION_LENGTH];

    /* Name of the hardware type the system is running on.  */
    char machine[_UTSNAME_MACHINE_LENGTH];

#if _UTSNAME_DOMAIN_LENGTH - 0
    /* Name of the domain of this node on the network.  */
# ifdef __USE_GNU
    char domainname[_UTSNAME_DOMAIN_LENGTH];
# else
    char __domainname[_UTSNAME_DOMAIN_LENGTH];
# endif
#endif
  };

#ifdef __USE_MISC
/* Note that SVID assumes all members have the same size.  */
# define SYS_NMLN  _UTSNAME_LENGTH
#endif
```

  

## 进程/线程  
[资料](https://blog.csdn.net/m0_74985965/article/details/128706134)  
[获取当前进程pid](./pid1.c)  
[创建线程](./fork1.c)  
[守护进程](./fork_daemon1.c)  
[多线程](./thread1.c)  
[僵尸进程](./zombie_ps1.c)  


## nvme

[iochtl](./nvme.c)  


## 数据结构
[链表](./datastructure/link2.c)