#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/device.h>
#include <linux/netlink.h>
#include <net/sock.h>
#include <asm/siginfo.h>


#define NETLINK_TEST     30
#define USER_PORT        100

/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp = {
	.symbol_name	= "do_send_sig_info",
};

struct sock *nlsk = NULL;
extern struct net init_net;

/* kprobe pre_handler: called just before the probed instruction is executed */
// static int __kprobes handler_pre(struct kprobe *p, struct pt_regs *regs)
// {
//     struct device *dev = regs->di;
// 	pr_info("<%s> pre_handler: p->addr = 0x%p, device name:%s\n",
// 		p->symbol_name, p->addr, dev->kobj.name);
// 	/* A dump_stack() here will give a stack backtrace */
//     dump_stack();
// 	return 0;
// }

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

int send_sig_info_to_user(struct sig_info_data_test *info)
{
    struct sk_buff *nl_skb;
    struct nlmsghdr *nlh;

    int ret;

    /* 创建sk_buff 空间 */
	int len;
	len = NLMSG_SPACE(1024);
    nl_skb = nlmsg_new(len, GFP_ATOMIC);
    if(!nl_skb)
    {
        printk("netlink alloc failure\n");
        return -1;
    }

    /* 设置netlink消息头部 */
    nlh = nlmsg_put(nl_skb, 0, 0, NETLINK_TEST, len, 0);
    if(nlh == NULL)
    {
        printk("nlmsg_put failaure \n");
        nlmsg_free(nl_skb);
        return -1;
    }
	/* 拷贝数据发送 */
	sprintf(nlmsg_data(nlh), "signal cached: from=  common: %s  pid: %d ppid: %d  to=: pid: %d common: %s  sig:%d\n",
	info->send_comm, info->send_pid, info->send_parent_pid, info->to_pid , info->to_comm,  info->sig);
    // memcpy(nlmsg_data(nlh), info, len);
    ret = netlink_unicast(nlsk, nl_skb, USER_PORT, MSG_DONTWAIT);
    return ret;
}



// //from wen xin yiyan
// static void send_nlmsg(char *msg)
// {
//     struct sk_buff *skb;
//     struct nlmsghdr *nlh;
//     int len = strlen(msg) + 1; // 消息长度加1以包含终止符
 
//     skb = nlmsg_new(NLMSG_SPACE(len), GFP_KERNEL);
//     if (!skb) {
//         printk(KERN_ERR "Failed to allocate skb\n");
//         return;
//     }
 
//     nlh = nlmsg_put(skb, 0, msg_seq++, NLMSG_DONE, len, 0);
//     if (!nlh) {
//         printk(KERN_ERR "Failed to put nlmsg header\n");
//         nlmsg_free(skb);
//         return;
//     }
 
//     strcpy(nlmsg_data(nlh), msg);
 
//     nlmsg_multicast(netlink_sk, skb, 0, 0, GFP_KERNEL);
// }

static int handler_pre(struct kprobe *p, struct pt_regs *regs) 
{
// #if (defined(CONFIG_ARM64) || defined(CONFIG_LOONGARCH))
// 	send_sig_info_data_t data;
// 	data.sig = regs->regs[0];
// 	data.info = (struct kernel_siginfo *)((unsigned long *)regs->regs[1]);
// 	data.p = (struct task_struct *)((unsigned long *)regs->regs[2]);
// 	do_store_sig_info(&data);
// #endif
	// unsigned long cx;  //函数调用的第4个参数
	// unsigned long dx;  //函数调用的第3个参数
	// unsigned long si;  //函数调用的第2个参数
	// unsigned long di;  //函数调用的第1个参数
	// int do_send_sig_info(int sig, struct kernel_siginfo *info, struct task_struct *p,
	// 		enum pid_type type)
// #ifdef CONFIG_X86_64
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
	send_sig_info_to_user(&test);
	// do_store_sig_info(&data);
// #endif
	return 0;
}




/* per-instance private data */
struct my_data {
	ktime_t entry_stamp;
};


struct netlink_kernel_cfg cfg = {
        .input  = NULL,
};
int test_netlink_init(void)
{
    /* create netlink socket */
    nlsk = (struct sock *)netlink_kernel_create(&init_net, NETLINK_TEST, &cfg);
    if(nlsk == NULL)
    {
        printk("netlink_kernel_create error !\n");
        return -1;
    }
    pr_info("test_netlink_init finshed\n");
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
	test_netlink_init();
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