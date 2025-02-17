#include <linux/module.h> /* Needed by all modules */ 
#include <linux/printk.h> /* Needed for pr_info() */ 

#include <linux/sched.h>
#include <linux/pid.h>

void list_thread_test(void){
    int count = 0;
	struct task_struct *task;
    pid_t testpid = 1;
	task = find_get_task_by_vpid(testpid);
    if (task != NULL) {
        pr_info("pid %d comm: %s.\n", task->pid, task->comm);
    } else {
        pr_info("pid %d not found.\n", testpid);

    }
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