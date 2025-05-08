#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/device.h>
#include <linux/dcache.h>
#include <net/sock.h>
#include <asm/siginfo.h>
#include <linux/sched.h>
#include <linux/netlink.h>


#define NETLINK_TEST     30
#define USER_PORT        100
struct sock *nlsk = NULL;
extern struct net init_net;

typedef struct send_sig_info_data {
	int sig;
	struct kernel_siginfo *info;
	struct task_struct *p;
} send_sig_info_data_t;

//TODO: add time recored
struct sig_info_data_test {
	int sig;
	char send_comm_path[PATH_MAX];
	int  send_pid;

	// char send_parent_comm_path[PATH_MAX];
	int send_parent_pid;

	char to_comm_path[PATH_MAX];
	int  to_pid;
};

/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp = {
	.symbol_name	= "do_send_sig_info",
};


int send_sig_info_to_user(struct sig_info_data_test *info)
{
    struct sk_buff *nl_skb;
    struct nlmsghdr *nlh;

    int ret;

    /* 创建sk_buff 空间 */
	int len;
	// len = NLMSG_SPACE(1024);
	len = NLMSG_SPACE(sizeof(struct sig_info_data_test));
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
		//消息的长度
	int msg_size = sizeof(struct sig_info_data_test);
	// 获取消息数据部分的指针
    struct sig_info_data_test *msg_data = (struct sig_info_data_test *) nlmsg_data(nlh);

    // 将自定义结构体的数据复制到消息数据部分
    memcpy(msg_data, info, msg_size);
	// nlmsg_data(nlh) = info;
	/* 拷贝数据发送 */
	// sprintf(nlmsg_data(nlh), "signal cached: from=  common: %s  pid: %d ppid: %d  to=: pid: %d common: %s  sig:%d\n",
	// info->send_comm, info->send_pid, info->send_parent_pid, info->to_pid , info->to_comm,  info->sig);
    // // memcpy(nlmsg_data(nlh), info, len);
    ret = netlink_unicast(nlsk, nl_skb, USER_PORT, MSG_DONTWAIT);
	printk("send sig info to user finished %d\n", ret);
    return ret;
}


static int get_full_path_by_task_struct(struct task_struct *task, char *ptest){
	char *cp;
	// memset(buf, 0, sizeof(buf));
	if (task->mm) {
		if (task->mm->exe_file) {
			// buf = kmalloc(PATH_MAX, GFP_ATOMIC);
			char buf[PATH_MAX];
			cp = d_path(&(task->mm->exe_file->f_path), buf, PATH_MAX);
			if (IS_ERR(cp))
				goto out;
			printk("wenao:ptest11: %s\n", cp);
			printk("wenao:ptest11 strlen ptest: %zu\n", strlen(ptest)); 
			printk("wenao:ptest11 strlen cp: %zu\n", strlen(cp)); 
			strncpy(ptest, cp, strlen(cp));
			printk("wenao:ptest22: %s\n", ptest);
			//remember free
			// kfree(buf);
			return 0;
		}
	}
	out:
		printk("get full path by task struct failed\n");
	return -1;
}


static int handler_pre(struct kprobe *p, struct pt_regs *regs) 
{
	send_sig_info_data_t data;
	data.sig = regs->di;
	data.info = (struct kernel_siginfo *)((unsigned long *)regs->si);
	data.p = (struct task_struct *)((unsigned long *)regs->dx);


	struct  sig_info_data_test test = {
		.sig = data.sig,
		.send_pid = current->pid,
		.to_pid = data.p->pid,
		.send_parent_pid = current->parent->pid,
		// .send_comm_path = (char *)kmalloc(PATH_MAX, GFP_ATOMIC),
		// .to_comm_path = (char *)kmalloc(PATH_MAX, GFP_ATOMIC),
	};
	// memset(test.to_comm_path, 0, PATH_MAX);
	// memset(test.send_comm_path, 0, PATH_MAX);
	if (get_full_path_by_task_struct(current, test.send_comm_path) != 0) {
		printk("get full path by task struct failed\n");
		strncpy(test.send_comm_path, "wenaofailedpath", strlen("wenaofailedpath"));
	} else {
		printk("get full path by task struct success\n");
	}
	if (get_full_path_by_task_struct(data.p, test.to_comm_path) != 0) {
		printk("get full path by task struct failed\n");
		strncpy(test.send_comm_path, "wenaofailedpath", strlen("wenaofailedpath"));
	} else {
		printk("get full path by task struct success\n");
	}

	// test.send_pid = current->pid;
	// test.send_comm_path = (char *)kmalloc(PATH_MAX, GFP_ATOMIC);
	// test.send_comm_path = get_full_path_by_task_struct(current);
	// test.send_parent_pid = current->parent->pid;
	// test.sig = data.sig;
	// test.to_pid = data.p->pid;
	// test.to_comm_path = (char *)kmalloc(PATH_MAX, GFP_ATOMIC);
	// test.to_comm_path = get_full_path_by_task_struct(data.p);
	pr_info("signal cached: from=  common: %s  pid: %d ppid: %d  to=: pid: %d common: %s  sig:%d\n",
	test.send_comm_path, test.send_pid, test.send_parent_pid, test.to_pid , test.to_comm_path,  test.sig);
	send_sig_info_to_user(&test);

	return 0;
}

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