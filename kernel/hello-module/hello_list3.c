#include <linux/module.h> /* Needed by all modules */ 
#include <linux/printk.h> /* Needed for pr_info() */ 
#include <linux/list.h> 
#include <linux/slab.h>
// LIST_HEAD(my_list);

struct student
{
    unsigned age;
    char *name;
    struct  list_head list    
};

// 如果一个结构是在编译期间静态创建的，那么可以使用宏INIT_LIST_HEAD()来初始化。
// struct strudent xiaowang =  {
//     .age = 18,
//     .name = "xiaowang",
//     .list = LIST_HEAD_INIT(xiaowang.list),
// };


int __init init_hello(void) 
{ 
    // struct list_head dynamic_list;
    // INIT_LIST_HEAD(&dynamic_list);

    // LIST_HEAD(student_list);

    

    static struct student zhangsan = { .name = "zhangsan", .age = 18, .list = LIST_HEAD_INIT(zhangsan.list)};
    static struct student lisi = { .name = "lisi", .age = 19 };
    static struct student wangwu = { .name = "wangwu", .age = 20 };

    list_add_tail(&lisi.list, &zhangsan.list);
    list_add_tail(&wangwu.list, &zhangsan.list);


	// //遍历链表
	struct  student *get_node;
	struct list_head *pos;	//遍历小结构体
	list_for_each(pos, &zhangsan.list)
	{
		get_node = list_entry(pos, struct student, list); //这样只有从lisi开始打
		pr_info("name: %s, age:%d\n",get_node->name, get_node->age);
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