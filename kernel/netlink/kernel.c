/****************************************
* Author: zhangwj
* Date: 2017-01-19
* Filename: netlink_test.c
* Descript: netlink of kernel
* Kernel: 3.10.0-327.22.2.el7.x86_64
* Warning:
******************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <net/sock.h>
#include <linux/netlink.h>

#define NETLINK_TEST     30
#define MSG_LEN            125
#define USER_PORT        100

MODULE_LICENSE("GPL");
MODULE_AUTHOR("test");
MODULE_DESCRIPTION("netlink example");

struct sock *nlsk = NULL;
extern struct net init_net;

int send_usrmsg(char *pbuf, uint16_t len)
{
    struct sk_buff *nl_skb;
    struct nlmsghdr *nlh;

    int ret;

    /* 创建sk_buff 空间 */
    nl_skb = nlmsg_new(len, GFP_ATOMIC);
    if(!nl_skb)
    {
        printk("netlink alloc failure\n");
        return -1;
    }

        /**
     * nlmsg_put - Add a new netlink message to an skb
     * @skb: socket buffer to store message in
     * @portid: netlink PORTID of requesting application
     * @seq: sequence number of message
     * @type: message type
     * @payload: length of message payload
     * @flags: message flags
     *
     * Returns NULL if the tailroom of the skb is insufficient to store
     * the message header and payload.
     */
    // static inline struct nlmsghdr *nlmsg_put(struct sk_buff *skb, u32 portid, u32 seq,
    // 					 int type, int payload, int flags)
    // {
    // 	if (unlikely(skb_tailroom(skb) < nlmsg_total_size(payload)))
    // 		return NULL;

    // 	return __nlmsg_put(skb, portid, seq, type, payload, flags);
    // }

        /* 设置netlink消息头部 */
    //可以在这里设置消息的 类型，消息的序列号 等.
    nlh = nlmsg_put(nl_skb, 0, 0, NETLINK_TEST, len, 0);
    if(nlh == NULL)
    {
        printk("nlmsg_put failaure \n");
        nlmsg_free(nl_skb);
        return -1;
    }

    /* 拷贝数据发送 */
    memcpy(nlmsg_data(nlh), pbuf, len);
    ret = netlink_unicast(nlsk, nl_skb, USER_PORT, MSG_DONTWAIT);

    return ret;
}

static void netlink_rcv_msg(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = NULL;
    char *umsg = NULL;
    char *kmsg = "hello users!!!";

    if(skb->len >= nlmsg_total_size(0))
    {
        nlh = nlmsg_hdr(skb);
        umsg = NLMSG_DATA(nlh);
        if(umsg)
        {
            printk("kernel recv from user: %s\n", umsg);
            send_usrmsg(kmsg, strlen(kmsg));
        }
    }
}

struct netlink_kernel_cfg cfg = {
        .input  = netlink_rcv_msg, /* set recv callback */
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
    printk("test_netlink_init\n");

    return 0;
}

void test_netlink_exit(void)
{
    if (nlsk){
        netlink_kernel_release(nlsk); /* release ..*/
        nlsk = NULL;
    }
    printk("test_netlink_exit!\n");
}

module_init(test_netlink_init);
module_exit(test_netlink_exit);