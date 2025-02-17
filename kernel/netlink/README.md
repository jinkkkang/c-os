netlink是一种用于内核态和用户态进程之间进行数据传输的特殊的IPC机制。它的特点是：
（1） 用户态采用socket风格的API
（2）除了预定义的协议类型之外，支持自定义协议类型
（3）异步通讯
（4）支持消息组播
（5）全双工（特别是支持内核主动发起会话）



Netlink 消息的基本结构
Netlink 消息由头部和负载组成。头部包含消息的元数据，如消息类型和长度，而负载包含实际的数据。

struct nlmsghdr {
    __u32 nlmsg_len;   // 消息长度
    __u16 nlmsg_type;  // 消息类型
    __u16 nlmsg_flags; // 标志
    __u32 nlmsg_seq;   // 序列号
    __u32 nlmsg_pid;   // 发送者的进程 ID
};




```
static inline struct sock *
netlink_kernel_create(struct net *net, int unit, struct netlink_kernel_cfg *cfg)
{
	return __netlink_kernel_create(net, unit, THIS_MODULE, cfg);
}

```


/* optional Netlink kernel configuration parameters */
struct netlink_kernel_cfg {
	unsigned int	groups; //指定最多的多播组
	unsigned int	flags;

    //input 回调函数用于处理收到的用户空间的消息
	void		(*input)(struct sk_buff *skb);
	struct mutex	*cb_mutex;
	int		(*bind)(struct net *net, int group);
	void		(*unbind)(struct net *net, int group);
	bool		(*compare)(struct net *net, struct sock *sk);
};


# 常用的宏和函数

include/uapi/linux/netlink.h

#define NLMSG_ALIGNTO	4U
#define NLMSG_ALIGN(len) ( ((len)+NLMSG_ALIGNTO-1) & ~(NLMSG_ALIGNTO-1) )
#define NLMSG_HDRLEN	 ((int) NLMSG_ALIGN(sizeof(struct nlmsghdr)))


#define NLMSG_LENGTH(len) ((len) + NLMSG_HDRLEN)
根据消息数据部分长度，返回消息长度，即头长度+len，但是这个长度不是对齐的（消息头和整个消息都是4字节对其的；

len：消息数据部分长度

#define NLMSG_SPACE(len) NLMSG_ALIGN(NLMSG_LENGTH(len))
根据消息数据部分长度，返回消息实际占用空间，也就是在NLMSG_LENGTH(len)基础上进行4字节对齐。

len：消息数据部分长度


#define NLMSG_DATA(nlh)  ((void*)(((char*)nlh) + NLMSG_LENGTH(0)))
根据消息内存地址，返回数据部分地址.

nlh：消息头地址,消息头地址和消息地址是一样。



#define NLMSG_NEXT(nlh,len)	 ((len) -= NLMSG_ALIGN((nlh)->nlmsg_len), \
				  (struct nlmsghdr*)(((char*)(nlh)) + NLMSG_ALIGN((nlh)->nlmsg_len)))

根据当前消息地址，返回下一个消息地址

nlh: 当前消息地址

len：剩余消息部分长度,比如当前recvfrom收到100字节数据，包含4个长度为25字节的消息，存储在buf中：


#define NLMSG_OK(nlh,len) ((len) >= (int)sizeof(struct nlmsghdr) && \
			   (nlh)->nlmsg_len >= sizeof(struct nlmsghdr) && \
			   (nlh)->nlmsg_len <= (len))

检查nlh地址是否是一条完整的消息

nlh: 通过recvfrom收到数据的缓冲区地址

len: 收到数据的长度


#define NLMSG_PAYLOAD(nlh,len) ((nlh)->nlmsg_len - NLMSG_SPACE((len)))
消息数据部分剩余长度

nlh： 消息的首地址

len: 数据部分已经使用的长度


include/net/netlink.h

* Message Construction:
 *   nlmsg_new()			create a new netlink message
 *   nlmsg_put()			add a netlink message to an skb
 *   nlmsg_put_answer()			callback based nlmsg_put()
 *   nlmsg_end()			finalize netlink message
 *   nlmsg_get_pos()			return current position in message
 *   nlmsg_trim()			trim part of message
 *   nlmsg_cancel()			cancel message construction
 *   nlmsg_free()			free a netlink message
 *
 * Message Sending:
 *   nlmsg_multicast()			multicast message to several groups
 *   nlmsg_unicast()			unicast a message to a single socket
 *   nlmsg_notify()			send notification message
 *
 * Message Length Calculations:
 *   nlmsg_msg_size(payload)		length of message w/o padding
 *   nlmsg_total_size(payload)		length of message w/ padding
 *   nlmsg_padlen(payload)		length of padding at tail
 *
 * Message Payload Access:
 *   nlmsg_data(nlh)			head of message payload
 *   nlmsg_len(nlh)			length of message payload
 *   nlmsg_attrdata(nlh, hdrlen)	head of attributes data
 *   nlmsg_attrlen(nlh, hdrlen)		length of attributes data
 *
 * Message Parsing:
 *   nlmsg_ok(nlh, remaining)		does nlh fit into remaining bytes?
 *   nlmsg_next(nlh, remaining)		get next netlink message
 *   nlmsg_parse()			parse attributes of a message
 *   nlmsg_find_attr()			find an attribute in a message
 *   nlmsg_for_each_msg()		loop over all messages
 *   nlmsg_validate()			validate netlink message incl. attrs
 *   nlmsg_for_each_attr()		loop over all attributes