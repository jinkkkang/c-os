#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>


#define BUFFERSIZE 100
int32_t value = 0;
char proc_buffer[BUFFERSIZE] = "hello world 111";
// static unsigned long procfs_buffer_size = 0;
static ssize_t demo_write(struct file *file, const char __user *buf,
						size_t count, loff_t *ppos)
{
    copy_from_user(proc_buffer, buf, count);
    return count;
}

static ssize_t demo_read(struct file *file, char __user *buf,
						size_t count, loff_t *ppos)
{
	static int completed = 0;

	if (completed) {
			completed = 0;
			return 0;
	}
    completed = 1;
		
	if (copy_to_user(buf, proc_buffer, sizeof(proc_buffer))) {
		return -EFAULT;
	}
	return sizeof(proc_buffer);
}

static const struct proc_ops test_proc_ops = {
	.proc_read	= demo_read,
	.proc_write	= demo_write,
	.proc_lseek	= noop_llseek,
};

static int __init example_init(void)
{

	//struct proc_dir_entry *base = proc_mkdir("base", NULL);
	//if (proc_create("testdemo", 0, NULL, &test_proc_ops) == NULL) {
	// 	return -ENOMEM;
	// }
	if (proc_create("testdemo", 0, NULL, &test_proc_ops) == NULL) {
		return -ENOMEM;
	}
	return 0;
}

static void __exit example_exit(void)
{
	remove_proc_entry("testdemo", NULL);
}

module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("test author");
