struct file_operations {
	struct module *owner;
    指向拥有该结构的模块的指针，通常是 THIS_MODULE

	loff_t (*llseek) (struct file *, loff_t, int);
    修改文件的当前读写位置，返回新的位置，如果返回值小于0，则表示出错


	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
    从设备中读取数据，返回读取的字节数，如果返回值小于0，则表示出错。 非负值表示成功读取的字节数


	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
    向设备发送数据，返回发送的字节数，如果返回值小于0，则表示出错。 非负值表示成功发送的字节数


	ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);

	ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);


	int (*iopoll)(struct kiocb *kiocb, bool spin);

	int (*iterate) (struct file *, struct dir_context *);

	int (*iterate_shared) (struct file *, struct dir_context *);

	__poll_t (*poll) (struct file *, struct poll_table_struct *);
    poll 方法是poll, epoll 和 select 这三个系统调用的后端实现。

	long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);

    ioctl（Input/Output Control）是一个通用的系统调用，用于对设备驱动程序执行特定于设备的操作。它允许用户空间程序与设备驱动程序进行双向数据交换，执行如配置设备、获取设备状态、修改设备行为等操作。ioctl 调用的参数通常包括设备文件描述符、请求码（一个命令标识符）以及一个指向数据的指针（可选）。

    unlocked_ioctl
    unlocked_ioctl 是内核中用于处理 ioctl 请求的函数的一个变体，特别是在支持多线程（特别是 NPTL，即 Native POSIX Thread Library）的系统中。在 Linux 内核中，为了提高并发性，许多系统调用和内核函数都被设计为可重入的。unlocked_ioctl 函数就是这样一个例子，它允许在持有较少锁（或没有锁）的情况下执行 ioctl 请求，从而减少了多线程环境下的潜在死锁问题。

    当设备驱动程序使用 unlocked_ioctl 而不是标准的 ioctl 入口点时，它通常意味着该驱动程序被设计为在多线程环境中更高效、更安全地运行。

	long (*compat_ioctl) (struct file *, unsigned int, unsigned long);

    用于处理来自 32 位用户空间程序的 ioctl 请求，以确保 32 位应用程序能够在 64 位 Linux 系统上正确运行。


	int (*mmap) (struct file *, struct vm_area_struct *);
    用于请求将设备内存映射到进程地址空间。

	unsigned long mmap_supported_flags;
	int (*open) (struct inode *, struct file *);
    这是对设备文件执行的第一个操作，然后并不要求设备驱动一定要实现一个相应的方法，如果这个入口为NULL，设备的打开永远是成功的
    但系统不会通知驱动程序。

	int (*flush) (struct file *, fl_owner_t id);
    flush 操作的调用发生在进程关闭设备文件描述符副本的时，他应该执行并等待设备上尚未结束的操作。


	int (*release) (struct inode *, struct file *);
    当file 结构被释放的时，将调用这个操作。

	int (*fsync) (struct file *, loff_t, loff_t, int datasync);
    fsync 系统调用的后端实现，调用它用来刷新待处理的数据。

	int (*fasync) (int, struct file *, int);
	int (*lock) (struct file *, int, struct file_lock *);
    实现文件锁定。设备驱动不太实现这个。

	ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);



	unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
    该方法的目的是在进程的地址空间中找到一个合适的位置，以便将底层设备中的内存段映射到该位置。
    该任务通常是内存管理代码完成。 也可以允许驱动实现特定设备需要的任何对齐需求。


	int (*check_flags)(int);
	int (*flock) (struct file *, int, struct file_lock *);
	ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
	ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
	int (*setlease)(struct file *, long, struct file_lock **, void **);
	long (*fallocate)(struct file *file, int mode, loff_t offset,
			  loff_t len);
	void (*show_fdinfo)(struct seq_file *m, struct file *f);
#ifndef CONFIG_MMU
	unsigned (*mmap_capabilities)(struct file *);
#endif
	ssize_t (*copy_file_range)(struct file *, loff_t, struct file *,
			loff_t, size_t, unsigned int);
	loff_t (*remap_file_range)(struct file *file_in, loff_t pos_in,
				   struct file *file_out, loff_t pos_out,
				   loff_t len, unsigned int remap_flags);
	int (*fadvise)(struct file *, loff_t, loff_t, int);
	bool may_pollfree;
} __randomize_layout;