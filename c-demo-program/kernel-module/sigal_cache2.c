#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/device.h>
#include <net/sock.h>
#include <asm/siginfo.h>

typedef struct send_sig_info_data {
	int sig;
	struct kernel_siginfo *info;
	struct task_struct *p;
} send_sig_info_data_t;

struct sig_info_data_test {
	int sig;
	char *send_comm;
	int  send_pid;
	int send_parent_pid;
	char *to_comm;
	int  to_pid;
};

/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp = {
	.symbol_name	= "do_send_sig_info",
};

static int handler_pre(struct kprobe *p, struct pt_regs *regs) 
{
	send_sig_info_data_t data;
	data.sig = regs->di;
	data.info = (struct kernel_siginfo *)((unsigned long *)regs->si);
	data.p = (struct task_struct *)((unsigned long *)regs->dx);


	// struct task_struct *ptask = NULL;

	int send_pid = current->pid;
	int send_parent_pid = current->parent->pid;
	char *from_comm = current->parent->comm;

	struct  sig_info_data_test test;
	test.send_comm = from_comm;
	test.send_pid = send_pid;
	test.send_parent_pid = send_parent_pid;
	test.sig = data.sig;
	test.to_pid = data.p->pid;
	test.to_comm = data.p->comm;
	pr_info("signal cached: from=  common: %s  pid: %d ppid: %d  to=: pid: %d common: %s  sig:%d\n",
	test.send_comm, test.send_pid, test.send_parent_pid, test.to_pid , test.to_comm,  test.sig);
	return 0;
}

static int __init kprobe_init(void)
{
	int ret;
	kp.pre_handler = handler_pre;

	ret = register_kprobe(&kp);
	if (ret < 0) {
		pr_err("register_kprobe failed, returned %d\n", ret);
		return ret;
	}
	return 0;
}

static void __exit kprobe_exit(void)
{
	unregister_kprobe(&kp);
	pr_info("kprobe at %p unregistered\n", kp.addr);
	if (nlsk){
        netlink_kernel_release(nlsk); /* release ..*/
        nlsk = NULL;
    }
}


module_init(kprobe_init)
module_exit(kprobe_exit)
MODULE_LICENSE("GPL");