#include <linux/module.h> /* Needed by all modules */ 
#include <linux/printk.h> /* Needed for pr_info() */ 
#include <linux/notifier.h>


//定义原子通知链
static ATOMIC_NOTIFIER_HEAD(test_demo_notifier_list);

static int
test_notifier_call_func(struct notifier_block *this, unsigned long event, void *ptr)
{
    pr_info("recive a notify %d %s\n", event, (char *)ptr); //recive a notify 123 hello
	return NOTIFY_DONE;
}

static struct notifier_block test_block = {
	.notifier_call = test_notifier_call_func,
};


int __init init_hello(void)
{ 
    pr_info("Hello world notify test module\n"); 
    atomic_notifier_chain_register(&test_demo_notifier_list, &test_block);
    unsigned long event = 123;
    char *strtest = "hello";
    void *data = (void *)strtest;
    atomic_notifier_call_chain(&test_demo_notifier_list, event, data);
    return 0; 
} 
 
void __exit cleanup_hello(void) 
{ 
    pr_info("Goodbye world 1.\n");
} 




module_init(init_hello);
module_exit(cleanup_hello);
MODULE_LICENSE("GPL");