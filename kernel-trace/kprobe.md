
kprobe 和 kretprobe（也称为返回探测器）。kprobe 几乎可以插入内核中的任何指令。返回探测器在指定函数返回时触发。



```
struct kprobe {
	struct hlist_node hlist; //被用于kprobe全局hash，索引值为被探测点的地址。

	/* list of kprobes for multi-handler support 用于链接同一被探测点的不同探测kprobe */
	struct list_head list;

	/*count the number of times this probe was temporarily disarmed */
	unsigned long nmissed;

	/* location of the probe point  被探测点的地址。*/
	kprobe_opcode_t *addr;

	/* Allow user to indicate symbol name of the probe point  被探测函数的名称*/
	const char *symbol_name;

	/* Offset into the symbol 被探测点在函数内部的偏移，用于探测函数内核的指令，如果该值为0表示函数的入口 */
	unsigned int offset;

	/* Called before addr is executed. 被探测点指令执行之前调用的回调函数 */
	kprobe_pre_handler_t pre_handler;

	/* Called after addr is executed, unless...  被探测点指令执行之后调用的回调函数。*/
	kprobe_post_handler_t post_handler;

	/*
	 * ... called if executing addr causes a fault (eg. page fault).
	 * Return 1 if it handled fault, otherwise kernel will see it.
     在执行pre_handler、post_handler或单步执行被探测指令时出现内存异常则会调用该回调函数。
	 */
	kprobe_fault_handler_t fault_handler;

	/* Saved opcode (which has been replaced with breakpoint)  保存的被探测点原始指令。*/
	kprobe_opcode_t opcode;

	/* copy of the original instruction  被复制的被探测点的原始指令，用于单步执行，架构强相关。*/
	struct arch_specific_insn ainsn;

	/*
	 * Indicates various status flags.
	 * Protected by kprobe_mutex after this kprobe is registered.
     状态标记。
	 */
	u32 flags;
};

```


# x86_64 pt_regs
```
struct pt_regs {
/*
 * C ABI says these regs are callee-preserved. They aren't saved on kernel entry
 * unless syscall needs a complete, fully filled "struct pt_regs".
 */
	unsigned long r15;
	unsigned long r14;
	unsigned long r13;
	unsigned long r12;
	unsigned long bp;
	unsigned long bx;
/* These regs are callee-clobbered. Always saved on kernel entry. */
	unsigned long r11;
	unsigned long r10;
	unsigned long r9;  //函数调用的第6个参数
	unsigned long r8;  //函数调用的第5个参数
	unsigned long ax;
	unsigned long cx;  //函数调用的第4个参数
	unsigned long dx;  //函数调用的第3个参数
	unsigned long si;  //函数调用的第2个参数
	unsigned long di;  //函数调用的第1个参数
/*
 * On syscall entry, this is syscall#. On CPU exception, this is error code.
 * On hw interrupt, it's IRQ number:
 */
	unsigned long orig_ax;
/* Return frame for iretq */
	unsigned long ip;
	unsigned long cs;
	unsigned long flags;
	unsigned long sp; //栈顶指针，有了这个指针，可以打印堆栈信息，来获得信息
	unsigned long ss;
/* top of stack page */
};

```


```
这里举个使用的pt_regs的例子,通过获取第一个参数，读取相关变量值。

比如想获取函数调用中queue_ra_show 的q->backing_dev_info.ra_pages的值。

static ssize_t queue_ra_show(struct request_queue *q, char *page)
  {
      unsigned long ra_kb = q->backing_dev_info.ra_pages <<
                      (PAGE_CACHE_SHIFT - 10);                                        return queue_var_show(ra_kb, (page));
  }
我们可以再probe注册函数中这样写

static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    struct request_queue *q = regs->di;
          
    printk("ra_pages =%d\n", q->backing_dev_info.ra_pages);
  
    return 0;
}
```


# kretprobe

```
/*
 * Function-return probe -
 * Note:
 * User needs to provide a handler function, and initialize maxactive.
 * maxactive - The maximum number of instances of the probed function that
 * can be active concurrently.
 * nmissed - tracks the number of times the probed function's return was
 * ignored, due to maxactive being too low.
 *
 */
struct kretprobe {
	struct kprobe kp;

    //用户自定义回调函数，被探测函数返回后被调用，一般在这个函数中获取被探测函数的返回值。
	kretprobe_handler_t handler;  

    /*
    用户自定义回调函数，这是Kretprobes 提供了一个可选的用户指定的处理程序，它在函数入口上运行。 每当 kretprobe 放置在函数入口处的 kprobe 被命中时，都会调用用户定义的 entry_handler，如果有的话。 如果 entry_handler 返回 0（成功），则保证在函数返回时调用相应的返回处理程序。 如果 entry_handler 返回非零错误，则 Kprobes 将返回地址保持原样，并且 kretprobe 对该特定函数实例没有进一步的影响。
    */
	kretprobe_handler_t entry_handler;


	int maxactive; //被探测函数可以同时活动的最大实例数。来指定可以同时探测多少个指定函数的实例
	int nmissed; //跟踪被探测函数的返回被忽略的次数（maxactive设置的过低）。
	size_t data_size; //表示kretprobe私有数据的大小，在注册kretprobe时会根据该大小预留空间。
	struct hlist_head free_instances; // 表示空闲的kretprobe运行实例链表，它链接了本kretprobe的空闲实例struct kretprobe_instance结构体表示
	raw_spinlock_t lock;
};
```




# 获取函数返回值
regs_return_value(regs)


这个函数就是来获取探测点函数返回值。 这个函数与体系架构有关，从struct pt_regs结构体获取返回值：

对于x86体系架构：
RAX：Accumulator for operands and results data 。一般用于存放返回值。



```
static inline unsigned long regs_return_value(struct pt_regs *regs)
{
	return regs->ax;
}
```

对于ARM64架构：
regs[0]：X0，一般存放函数返回值。

```
static inline unsigned long regs_return_value(struct pt_regs *regs)
{
	unsigned long val = regs->regs[0];

	/*
	 * Audit currently uses regs_return_value() instead of
	 * syscall_get_return_value(). Apply the same sign-extension here until
	 * audit is updated to use syscall_get_return_value().
	 */
	if (compat_user_mode(regs))
		val = sign_extend64(val, 31);

	return val;
}

```


关于 current 
current 是一个全局变量，它指向当前正在运行的进程。

```
#define current get_current()
```