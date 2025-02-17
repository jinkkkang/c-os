#include <linux/module.h> /* Needed by all modules */ 
#include <linux/printk.h> /* Needed for pr_info() */ 
#include <linux/kthread.h>             //kernel threads
#include <linux/delay.h>

static struct task_struct *etx_thread;

int thread_function(void *pv)
{
    int i=0;
    while(!kthread_should_stop()) {
        pr_info("test hello %d\n", i++);
        msleep(1000);
    }
    return 0;
}

int __init init_hello(void) 
{ 
    etx_thread = kthread_run(thread_function,NULL,"hello_kthread");
    if(etx_thread) {
        pr_info("Kthread Created Successfully...\n");
    } else {
        pr_err("Cannot create kthread\n");
    }
 
    /* A non 0 return means init_module failed; module can't be loaded. */ 
    return 0; 
} 
 
void __exit cleanup_hello(void) 
{ 
    kthread_stop(etx_thread);
    pr_info("Goodbye world 1.\n");
} 




module_init(init_hello);
module_exit(cleanup_hello);
MODULE_LICENSE("GPL");