#include <linux/module.h> /* Needed by all modules */ 
#include <linux/printk.h> /* Needed for pr_info() */ 
 

static unsigned int param_uint;
module_param(param_uint, uint, 0600);
MODULE_PARM_DESC(param_uint, "This is a uint parameter!"); 

static int param_array[5];
static int array_num;
//module_param_array(param_char_array, int, NULL, 0600);
module_param_array(param_array, int, &array_num, 0600);
MODULE_PARM_DESC(param_array, "This is a array parameter!");


static int param_int_cb;
int param_int_cb_store(const char *val, const struct kernel_param *kp)
{
        int value;
        int err;

        err = kstrtoint(val, 0, &value);
        if (err)
                return err;

        if (value > 0)
                pr_info("value:%d\n", value);

        return param_set_uint(val, kp);
}

int param_int_cb_show(char *buffer, const struct kernel_param *kp)
{
        int value = *((int *)kp->arg);

        if (value > 0)
                return sprintf(buffer, "value:%d > 0\n", value);
        else
                return sprintf(buffer, "value:%d <= 0\n", value);
}
static const struct kernel_param_ops param_int_cb_ops = {
        .set = param_int_cb_store,
        .get = param_int_cb_show,
};
module_param_cb(param_int_cb, &param_int_cb_ops, &param_int_cb, 0600);
MODULE_PARM_DESC(param_int_cb, "This is param_int_cb\n");



int __init init_hello(void) 
{ 
    pr_info("Hello world: param  param_uint val:  %d\n", param_uint); 
    pr_info("Hello world: param  param_int_cb val:  %d\n", param_int_cb); 
    pr_info("Hello world: param arr num :  %d\n", array_num);
    pr_info("Hello world: start print all val in param_array\n");
    size_t i;
    for (i = 0; i < array_num; i++)
    {
        pr_info("Hello world: param index: %d,val: %d\n", i, param_array[i]);
    }
    
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