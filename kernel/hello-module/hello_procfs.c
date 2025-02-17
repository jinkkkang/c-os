#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>


int32_t value = 0;
static ssize_t demo_write(struct file *file, const char __user *buf,
						size_t count, loff_t *ppos)
{
    copy_from_user(&value, (int32_t *)buf, sizeof(value));
    return 0;
}

static ssize_t demo_read(struct file *file, char __user *buf,
						size_t count, loff_t *ppos)
{
	copy_to_user((int32_t *)buf, &value, sizeof(value));
	return sizeof(value);
}

static const struct proc_ops test_proc_ops = {
	.proc_read	= demo_read,
	.proc_write	= demo_write,
	.proc_lseek	= noop_llseek,
};

static int __init example_init(void)
{

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
