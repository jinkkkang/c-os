#include <linux/kernel.h> /*  kzalloc */
#include <linux/module.h> /* Needed by all modules */ 
#include <linux/printk.h> /* Needed for pr_info() */ 
#include <linux/fs.h> 
#include <linux/fs_context.h>
#include <linux/init.h> 
#include <linux/kobject.h> 
#include <linux/module.h> 
#include <linux/string.h> 
#include <linux/sysfs.h> 
#include <linux/mount.h>


#define NULL_FILE_NAME ".null"
#define SELINUX_MAGIC           0xf97cff8c

#define MAXLEN 8
#define MAX_FILES    32
#define MAX_BLOCKSIZE  512
// static struct kobject *mymodule; 
// struct kobject *fs_kobj;
int err;

struct file_blk {
    uint8_t busy;
    mode_t mode;
    uint8_t idx;

    union {
        uint8_t file_size;
        uint8_t dir_children;
    };
    char data[0];
};

struct file_blk block[MAX_FILES+1];
int curr_count = 0; 
struct vfsmount *selinuxfs_mount;
struct path selinux_null;

int tinyfs_fill_super(struct super_block *sb, void *data, int silent)
{
    struct inode *root_inode;
    int mode = S_IFDIR;

    root_inode = new_inode(sb);
    root_inode->i_ino = 1;
    inode_init_owner(root_inode, NULL, mode);
    root_inode->i_sb = sb;
//    root_inode->i_op = &tinyfs_inode_ops;
 //   root_inode->i_fop = &tinyfs_dir_operations;
    //root_inode->i_atime = root_inode->i_mtime = root_inode->i_ctime = CURRENT_TIME;

    block[1].mode = mode;
    block[1].dir_children = 0;
    block[1].idx = 1;
    block[1].busy = 1;
    root_inode->i_private = &block[1];

    sb->s_root = d_make_root(root_inode);
    curr_count ++;

    return 0;
}

static struct dentry *tinyfs_mount(struct file_system_type *fs_type, int flags, const char *dev_name, void *data)
{
    return mount_nodev(fs_type, flags, data, tinyfs_fill_super);
}


int init_hello(void)
{ 
    //第一步: 使用 kobject_create_and_add 在sys 下面创建一个fs object
    pr_info("Hello world 1.\n");
    // fs_kobj = kobject_create_and_add("fs", NULL);
        // if (!fs_kobj)
        //      printk(KERN_WARNING "%s: kobj create error\n", __func__);
    // fs_kobj->name = "fs"
    pr_info("Hello world 2.\n");
    err = sysfs_create_mount_point(fs_kobj, "selinux");
    if (err)
        return err;

    //static const struct fs_context_operations sel_context_ops = {
     //   .get_tree     = sel_get_tree,
    //};




    static struct file_system_type sel_fs_type = {
        .name  = "selinuxfs",
        .mount = tinyfs_mount 
        // .init_fs_context = sel_init_fs_context,
        // .kill_sb     = sel_kill_sb,
    };


    err = register_filesystem(&sel_fs_type);
    if (err) {
            sysfs_remove_mount_point(fs_kobj, "selinux");
            return err;
    }
    // kern_mount(&sel_fs_type);
    selinux_null.mnt = selinuxfs_mount = kern_mount(&sel_fs_type);
    if (IS_ERR(selinuxfs_mount)) {
         pr_err("selinuxfs:  could not mount!\n");
         err = PTR_ERR(selinuxfs_mount);
         selinuxfs_mount = NULL;
    }
    // selinux_null.dentry = d_hash_and_lookup(selinux_null.mnt->mnt_root,
    //                                      &null_name);
    // if (IS_ERR(selinux_null.dentry)) {
    //      pr_err("selinuxfs:  could not lookup null!\n");
    //      err = PTR_ERR(selinux_null.dentry);
    //      selinux_null.dentry = NULL;
    // }

    return err;


    /* A non 0 return means init_module failed; module can't be loaded. */ 
    return 0; 
} 
 
void cleanup_hello(void)
{ 
    pr_info("Goodbye world 1.\n");
    sysfs_remove_mount_point(fs_kobj, "selinux");
}


module_init(init_hello);
module_exit(cleanup_hello);
MODULE_LICENSE("GPL");