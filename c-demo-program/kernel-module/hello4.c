#include <linux/module.h> /* Needed by all modules */ 
#include <linux/printk.h> /* Needed for pr_info() */ 

#include <linux/sched.h>
#include <linux/sched/signal.h>

void list_thread_test(void){
    int count = 0;
	struct task_struct *thread;
	struct task_struct *group_leader = current;
	for_each_thread(current, thread){
        count++;
		printk(KERN_INFO "Thread found: %s (PID: %d)\n", thread->comm, thread->pid);
	}
    printk("Total process %d\n", count);	
	return;
}
int __init init_hello(void) 
{ 
    list_thread_test();
    return 0; 
} 
 
void __exit cleanup_hello(void) 
{ 
    pr_info("Goodbye world 1.\n");
} 




module_init(init_hello);
module_exit(cleanup_hello);
MODULE_LICENSE("GPL");