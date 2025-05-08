#include <limits.h>
#include <sys/socket.h>
#include <linux/netlink.h>

struct sig_info_data_test {
	int sig;
	char send_comm_path[PATH_MAX];
	int  send_pid;

	// char *send_parent_comm_path;
	int send_parent_pid;

	char to_comm_path[PATH_MAX];
	int  to_pid;
};



typedef struct _user_msg_info
{
    struct nlmsghdr hdr;
    struct sig_info_data_test msg;
} user_msg_info;

int signal_monitor_init(void);