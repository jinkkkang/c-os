#include <linux/module.h> /* Needed by all modules */ 
#include <linux/printk.h> /* Needed for pr_info() */ 

#include <linux/sched.h>
#include <linux/sched/signal.h>

void listprocess_test(void){
	struct task_struct *tsk;
	int count = 0;
 
	printk("for_each_process\n");
	printk("PID\tCOMM\n");
	for_each_process(tsk){
		count++;
		printk("%d\t%s\n", tsk->pid, tsk->comm);
	}
	printk("Total process %d\n", count);
	
	return;
}

void workqueue_fn(struct work_struct *work); 

/* Work structure */
static struct work_struct workqueuename;

 

 
/*Workqueue Function*/
void workqueue_fn(struct work_struct *work)
{
    pr_info("Executing Workqueue Function dongtai\n");
}
 
 
// //Interrupt handler for IRQ 11. 
// static irqreturn_t irq_handler(int irq,void *dev_id) {
//         pr_info("Shared IRQ: Interrupt Occurred");
//         schedule_work(&workqueue);
        
//         return IRQ_HANDLED;
// }

void testwork(void) {
    schedule_work(&workqueuename);
}
int __init init_hello(void) 
{ 
    pr_info("Hello world 1.\n"); 
    /*Creating work by Dynamic Method */
    INIT_WORK(&workqueuename,workqueue_fn);
    // listprocess_test();
    testwork();
    /* A non 0 return means init_module failed; module can't be loaded. */ 
    return 0; 
} 
 
void __exit cleanup_hello(void) 
{ 
    pr_info("Goodbye world 1.\n");
} 




module_init(init_hello);
module_exit(cleanup_hello);
MODULE_LICENSE("GPL");