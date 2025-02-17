bpftrace -l '*nvme*'   列出名字带 nvme 的 probe
bpftrace -e  执行单行程序


暴力
BPFTRACE_MAX_PROBES=1000 bpftrace -e  'kprobe:acpiphp_hotplug_notify {printf("kkk\n"); }'


几种事件来源

# tracepoint 
内核静态插桩点
内核的静态trace 通过vl 能看到函数的参数 



/sys/kernel/debug/tracing/events


```
bpftrace -lv tracepoint:syscalls:sys_enter_openat
tracepoint:syscalls:sys_enter_openat
    int __syscall_nr
    int dfd
    const char * filename
    int flags
    umode_t mode

参数可以通过内置变量 args 来获取，如 args.filename
```



ustd  用户静态定义插桩

kprobe  内核动态函数插桩
kretprobe . 内核动态函数返回值插桩

uprobe 用户态 动态函数插桩
uretprobe  用户态动态函数插桩
software



过滤器

/pid == 123/

过滤条件表达式也支持布尔运算，如("&&", "||")。

变量

 内置变量
args  ：包含所有tracepoint参数的结构
str()  ：用来把字符串指针转换成字符串。
arg0    表示探针的第一个参数，其含义由探针类型决定。对于kprobe类型探针，它表示函数的第一个参数。其它参数使用arg1,...,argN访问。

总的如下：
uid:用户id。
tid：线程id
pid：进程id。
cpu：cpu id。
cgroup：cgroup id.
probe：当前的trace点。
comm：进程名字。
nsecs：纳秒级别的时间戳。
kstack：内核栈描述
curtask：当前进程的task_struct地址。
args:获取该kprobe或者tracepoint的参数列表
arg0:获取该kprobe的第一个变量，tracepoint不可用
arg1:获取该kprobe的第二个变量，tracepoint不可用
arg2:获取该kprobe的第三个变量，tracepoint不可用
retval: kretprobe中获取函数返回值
args->ret: kretprobe中获取函数返回值


 临时变量
$  开头

@  符号代表的 是一个特殊的类型变量 map ， 可以以不同的方式来存储和描述数据。你可以在@后添加可选的变量名(如@num)，用来增加可读性或者区分不同的map。


如何传递变量

用一个map来保存变量，map 是全局变量
 probe1 { @a = $name; }
 probe2 { $name = @a; }

示例

bpftrace -e 'tracepoint:syscalls:sys_enter_openat { printf("%s %s\n", comm, str(args->filename)); }'

计数
bpftrace -e 'tracepoint:raw_syscalls:sys_enter { @[comm] = count(); }' 
bpftrace -e 'tracepoint:raw_syscalls:sys_enter { @[comm, pid] = count(); }' 
bpftrace -e 'tracepoint:raw_syscalls:sys_enter { @kk[comm, pid] = count(); }'

bpftrace -e 'tracepoint:raw_syscalls:sys_enter { @ = count(); }'   // 输出总的，所有的 