#include <linux/module.h> /* Needed by all modules */ 
#include <linux/printk.h> /* Needed for pr_info() */ 
#include <linux/spinlock.h>



char * testvalue = "hello";
struct testdemo {
    char *data;
    spinlock_t lock;
};

int __init init_hello(void) 
{ 
    struct  testdemo test;
    spin_lock_init(&test.lock);
    spin_lock(&test.lock);
    test.data = testvalue;
    spin_unlock(&test.lock);
    pr_info("Hello world %s\n",test.data); 
     return 0; 
} 
 
void __exit cleanup_hello(void) 
{ 
    pr_info("Goodbye world 1.\n");
} 




module_init(init_hello);
module_exit(cleanup_hello);
MODULE_LICENSE("GPL");