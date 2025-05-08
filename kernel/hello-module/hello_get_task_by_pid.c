#include <linux/module.h> /* Needed by all modules */ 
#include <linux/printk.h> /* Needed for pr_info() */ 
#include <linux/sched.h>
int __init init_hello(void) 
{ 
 
    struct  task_struct *mytask;
    pid_t testpid = 161567;
    mytask = find_get_task_by_vpid(testpid);
    if (mytask) {
        pr_info("pid %d comm %s\n", mytask->pid, mytask->comm);
    }
    pr_info("Hello world 1.\n"); 
     return 0; 
} 
 
void __exit cleanup_hello(void) 
{ 
    pr_info("Goodbye world 1.\n");
}


module_init(init_hello);
module_exit(cleanup_hello);
MODULE_LICENSE("GPL");