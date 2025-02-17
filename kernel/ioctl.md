# ioctl 


https://mp.weixin.qq.com/s/lbhCd6gri4hQKDT6ezsW7A


ioctl() 是应用程序与设备驱动程序交互的最常用方法

命令编号定义
命令编号或请求编号是传递给 ioctl 系统调用的第二个参数。虽然它可以是任何 32 位数字，用于唯一标识特定驱动程序的操作，但定义它们有许多约定



## 用户态
"linux/ioctl.h"头文件中提供了创建ioctl命令的宏：
#define "ioctl name" __IOX("magic number","command number","argument type")

IOX可以是
"IO": ioctl命令不带参数
"IOW": 携带写入参数
"IOR": 携带读取参数
"IOWR": 同时携带读写参数

```
#define _IOC(dir,type,nr,size) \
	(((dir)  << _IOC_DIRSHIFT) | \  _IOC_DIRSHIFT 30
	 ((type) << _IOC_TYPESHIFT) | \  _IOC_TYPESHIFT 8
	 ((nr)   << _IOC_NRSHIFT) | \  _IOC_NRSHIFT 0
	 ((size) << _IOC_SIZESHIFT))  _IOC_SIZESHIFT 16
```




## 内核态
include/uapi/asm-generic/ioctl.h





struct file_operations {
	struct module *owner;
    ......
    #64为应用程序调用的
	long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
    #32为应用程序调用的, 一般都不实现
	long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
    ......
}


ioctl  number 列表
https://docs.kernel.org/userspace-api/ioctl/ioctl-number.html



copy_to_user
功能：用于将内核空间的数据拷贝到用户空间。
函数原型：unsigned long copy_to_user(void __user *to, const void *from, unsigned long n);

*to是用户空间的指针，
*from是内核空间指针，
n表示从内核空间向用户空间拷贝数据的字节数

如果数据拷贝成功，则返回零；否则，返回没有拷贝成功的数据字节数


copy_from_user
功能：用于将用户空间的数据传送到内核空间。
函数原型：unsigned long copy_from_user(void * to, const void __user * from, unsigned long n)

第一个参数to是内核空间的数据目标地址指针，
第二个参数from是用户空间的数据源地址指针，
第三个参数n是数据的长度。

如果数据拷贝成功，则返回零；否则，返回没有拷贝成功的数据字节数。
此函数将from指针指向的用户空间地址开始的连续n个字节的数据发送到to指针指向的内核空间地址




copy_to_user 是怎么工作的？

check_copy_size  检查要拷贝的对象的长度

access_ok 检查要访问的用户空间中指针的有效性, 是不是位于用户空间，而不是内核中。
