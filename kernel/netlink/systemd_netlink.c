udevd


udev_monitor->sock = socket(PF_NETLINK, SOCK_RAW|SOCK_CLOEXEC|SOCK_NONBLOCK, NETLINK_KOBJECT_UEVENT);



内核

```
static int uevent_net_init(struct net *net)
{
	struct uevent_sock *ue_sk;
	struct netlink_kernel_cfg cfg = {
		.groups	= 1,
		.input = uevent_net_rcv,
		.flags	= NL_CFG_F_NONROOT_RECV
	};

	ue_sk = kzalloc(sizeof(*ue_sk), GFP_KERNEL);
	if (!ue_sk)
		return -ENOMEM;

	ue_sk->sk = netlink_kernel_create(net, NETLINK_KOBJECT_UEVENT, &cfg);
	if (!ue_sk->sk) {
		pr_err("kobject_uevent: unable to create netlink socket!\n");
		kfree(ue_sk);
		return -ENODEV;
	}

	net->uevent_sock = ue_sk;

	/* Restrict uevents to initial user namespace. */
	if (sock_net(ue_sk->sk)->user_ns == &init_user_ns) {
		mutex_lock(&uevent_sock_mutex);
		list_add_tail(&ue_sk->list, &uevent_sock_list);
		mutex_unlock(&uevent_sock_mutex);
	}

	return 0;
}
```