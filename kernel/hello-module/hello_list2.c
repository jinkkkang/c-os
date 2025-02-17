//list.c
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

/* header of list */
#include <linux/list.h>

/* private structure */
struct node {
    const char *name;
    struct list_head list;
};

/* Initialize a group node structure */
static struct node node0 = { .name = "BiscuitOS_node0", };
static struct node node1 = { .name = "BiscuitOS_node1", };
static struct node node2 = { .name = "BiscuitOS_node2", };
static struct node node3 = { .name = "BiscuitOS_node3", };
static struct node node4 = { .name = "BiscuitOS_node4", };
static struct node node5 = { .name = "BiscuitOS_node5", };
static struct node node6 = { .name = "BiscuitOS_node6", };

/* Declaration and implement a bindirect-list */
LIST_HEAD(BiscuitOS_list);

static __init int list_entry_init(void)
{
  struct node *np;

  /* add a new entry on special entry */
  list_add_tail(&node0.list, &BiscuitOS_list);
  list_add_tail(&node1.list, &BiscuitOS_list);
  list_add_tail(&node2.list, &BiscuitOS_list);

  printk("BiscuitOS_list:\n");
  /* Traverser all node on bindirect-list */
  list_for_each_entry(np, &BiscuitOS_list, list)
    printk("%s\n", np->name);

  /* get the struct for this entry */
  np = list_entry(BiscuitOS_list.next, struct node, list);
  printk("The entry struct node: %s\n", np->name);

  /* get the first element from a list */
  np = list_first_entry(&BiscuitOS_list, struct node, list);
  printk("The first entry struct node: %s\n", np->name);

  return -1;
}


static void __exit list_entry_exit(void)
{
  printk("Bye bye list_entry\n");
}

module_init(list_entry_init);
module_exit(list_entry_exit); 
MODULE_LICENSE("GPL");
