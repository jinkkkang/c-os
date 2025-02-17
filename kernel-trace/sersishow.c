#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/device.h>


/*

[711030.237554] Planted kprobe at 00000000622f7f9c
[711030.246815] Planted return probe at serial_show: 00000000622f7f9c
[711035.739083] <serial_show> pre_handler: p->addr = 0x00000000622f7f9c, device name:vda
[711035.739086] CPU: 3 PID: 149415 Comm: cat Kdump: loaded Tainted: G           OE     5.10.134-16.1.al8.x86_64 #1
[711035.739087] Hardware name: Alibaba Cloud Alibaba Cloud ECS, BIOS 2221b89 04/01/2014
[711035.739087] Call Trace:
[711035.739096]  ? serial_show+0x5/0x40
[711035.739099]  dump_stack+0x57/0x6e
[711035.739103]  handler_pre+0x25/0x2c [kprobe_example]
[711035.739106]  ? aggr_pre_handler+0x40/0x80
[711035.739107]  ? serial_show+0x1/0x40
[711035.739110]  ? kprobe_ftrace_handler+0x82/0xf0
[711035.739114]  ? dev_attr_show+0x16/0x40
[711035.739115]  ? ftrace_ops_assist_func+0x83/0x110
[711035.739119]  ? 0xffffffffc07b60c8
[711035.739120]  ? virtblk_get_id.isra.0+0xc0/0xc0
[711035.739121]  ? serial_show+0x1/0x40
[711035.739122]  ? serial_show+0x5/0x40
[711035.739124]  ? elfcorehdr_read+0x40/0x40
[711035.739126]  ? sysfs_kf_seq_show+0x98/0xf0
[711035.739128]  ? seq_read_iter+0x11f/0x4b0
[711035.739132]  ? new_sync_read+0x102/0x180
[711035.739134]  ? vfs_read+0xf8/0x180
[711035.739135]  ? ksys_read+0x4f/0xd0
[711035.739137]  ? do_syscall_64+0x30/0x40
[711035.739139]  ? entry_SYSCALL_64_after_hwframe+0x61/0xc6
[711035.739141] <serial_show> post_handler: p->addr = 0x00000000622f7f9c, flags = 0x286
[711035.739189] serial_show returned 20 size serial and took 711047893483887 ns to execute

*/

#define MAX_SYMBOL_LEN	64
static char symbol[MAX_SYMBOL_LEN] = "serial_show";
module_param_string(symbol, symbol, sizeof(symbol), 0644);

/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp = {
	.symbol_name	= symbol,
};

/* kprobe pre_handler: called just before the probed instruction is executed */
static int __kprobes handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    struct device *dev = regs->di;
	pr_info("<%s> pre_handler: p->addr = 0x%p, device name:%s\n",
		p->symbol_name, p->addr, dev->kobj.name);
	/* A dump_stack() here will give a stack backtrace */
    dump_stack();
	return 0;
}

/* kprobe post_handler: called after the probed instruction is executed */
static void __kprobes handler_post(struct kprobe *p, struct pt_regs *regs,
				unsigned long flags)
{
	pr_info("<%s> post_handler: p->addr = 0x%p, flags = 0x%lx\n",
		p->symbol_name, p->addr, regs->flags);
}

/*
 * fault_handler: this is called if an exception is generated for any
 * instruction within the pre- or post-handler, or when Kprobes
 * single-steps the probed instruction.
 */
static int handler_fault(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
	pr_info("fault_handler: p->addr = 0x%p, trap #%dn", p->addr, trapnr);
	/* Return 0 because we don't handle the fault. */
	return 0;
}
/* NOKPROBE_SYMBOL() is also available */
NOKPROBE_SYMBOL(handler_fault);

/* per-instance private data */
struct my_data {
	ktime_t entry_stamp;
};

/* Here we use the entry_hanlder to timestamp function entry */
static int entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	struct my_data *data;

	if (!current->mm)
		return 1;	/* Skip kernel threads */

	data = (struct my_data *)ri->data;
	data->entry_stamp = ktime_get();
	return 0;
}
NOKPROBE_SYMBOL(entry_handler);

/*
 * Return-probe handler: Log the return value and duration. Duration may turn
 * out to be zero consistently, depending upon the granularity of time
 * accounting on the platform.
 */
static int ret_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	unsigned long retval = regs_return_value(regs);
	struct my_data *data = (struct my_data *)ri->data;
	s64 delta;
	ktime_t now;

	now = ktime_get();
	delta = ktime_to_ns(ktime_sub(now, data->entry_stamp));
	pr_info("%s returned %lu size serial and took %lld ns to execute\n",
			symbol, retval, (long long)delta);
	return 0;
}
NOKPROBE_SYMBOL(ret_handler);

static struct kretprobe my_kretprobe = {
	.handler		= ret_handler,
	.data_size		= sizeof(struct my_data),
	/* Probe up to 20 instances concurrently. */
	.maxactive		= 20,
};



static int __init kprobe_init(void)
{
	int ret;
	kp.pre_handler = handler_pre;
	kp.post_handler = handler_post;
	kp.fault_handler = handler_fault;

	ret = register_kprobe(&kp);
	if (ret < 0) {
		pr_err("register_kprobe failed, returned %d\n", ret);
		return ret;
	}
    pr_info("Planted kprobe at %p\n", kp.addr);

    my_kretprobe.kp.symbol_name = symbol;
	ret = register_kretprobe(&my_kretprobe);
	if (ret < 0) {
		pr_err("register_kretprobe failed, returned %d\n", ret);
		return ret;
	}
	pr_info("Planted return probe at %s: %p\n",
			my_kretprobe.kp.symbol_name, my_kretprobe.kp.addr);
	return 0;
	
	return 0;
}

static void __exit kprobe_exit(void)
{
	unregister_kprobe(&kp);
	pr_info("kprobe at %p unregistered\n", kp.addr);
    unregister_kretprobe(&my_kretprobe);
	pr_info("kretprobe at %p unregistered\n", my_kretprobe.kp.addr);
}


module_init(kprobe_init)
module_exit(kprobe_exit)
MODULE_LICENSE("GPL");





