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
#include "security.h"

#define NULL_FILE_NAME ".null"
#define SELINUX_MAGIC		0xf97cff8c

// static struct kobject *mymodule; 
// struct kobject *fs_kobj;
int err;
enum sel_inos {
	SEL_ROOT_INO = 2,
	SEL_LOAD,	/* load policy */
	SEL_ENFORCE,	/* get or set enforcing status */
	// SEL_CONTEXT,	/* validate context */
	// SEL_ACCESS,	/* compute access decision */
	// SEL_CREATE,	/* compute create labeling decision */
	// SEL_RELABEL,	/* compute relabeling decision */
	// SEL_USER,	/* compute reachable user contexts */
	SEL_POLICYVERS,	/* return policy version for this kernel */
	// SEL_COMMIT_BOOLS, /* commit new boolean values */
	// SEL_MLS,	/* return if MLS policy is enabled */
	// SEL_DISABLE,	/* disable SELinux until next reboot */
	// SEL_MEMBER,	/* compute polyinstantiation membership decision */
	// SEL_CHECKREQPROT, /* check requested protection, not kernel-applied one */
	// SEL_COMPAT_NET,	/* whether to use old compat network packet controls */
	// SEL_REJECT_UNKNOWN, /* export unknown reject handling to userspace */
	// SEL_DENY_UNKNOWN, /* export unknown deny handling to userspace */
	// SEL_STATUS,	/* export current status using mmap() */
	// SEL_POLICY,	/* allow userspace to read the in kernel policy */
	// SEL_VALIDATE_TRANS, /* compute validatetrans decision */
	// SEL_INO_NEXT,	/* The next inode number to use */
};

struct vfsmount *selinuxfs_mount;
struct path selinux_null;
struct qstr null_name = QSTR_INIT(NULL_FILE_NAME,
                    sizeof(NULL_FILE_NAME)-1);
struct selinux_fs_info {
	struct dentry *bool_dir;
	unsigned int bool_num;
	char **bool_pending_names;
	unsigned int *bool_pending_values;
	struct dentry *class_dir;
	unsigned long last_class_ino;
	bool policy_opened;
	struct dentry *policycap_dir;
	unsigned long last_ino;
	// struct selinux_state *state;
	struct super_block *sb;
};

static int selinux_fs_info_create(struct super_block *sb)
{
	struct selinux_fs_info *fsi;

	fsi = kzalloc(sizeof(*fsi), GFP_KERNEL);
	if (!fsi)
		return -ENOMEM;

	fsi->last_ino = SEL_POLICYVERS - 1;
	// fsi->state = 0;
	fsi->sb = sb;
	sb->s_fs_info = fsi;
	return 0;
}

#define TMPBUFLEN 20
#define POLICYDB_VERSION_MAX 33
#define XXXXX 1
int avc_has_perm(struct selinux_state *state, u32 ssid, u32 tsid, u16 tclass,
		 u32 requested, struct common_audit_data *auditdata)
{
	struct av_decision avd;
	int rc, rc2;

	rc = avc_has_perm_noaudit(state, ssid, tsid, tclass, requested, 0,
				  &avd);

	rc2 = avc_audit(state, ssid, tsid, tclass, requested, &avd, rc,
			auditdata, 0);
	if (rc2)
		return rc2;
	return rc;
}

static ssize_t sel_write_load(struct file *file, const char __user *buf,
			      size_t count, loff_t *ppos)

{
	struct selinux_fs_info *fsi = file_inode(file)->i_sb->s_fs_info;
	struct selinux_load_state load_state;
	ssize_t length;
	void *data = NULL;

	mutex_lock(&fsi->state->policy_mutex);

	length = avc_has_perm(&selinux_state,
			      current_sid(), SECINITSID_SECURITY,
			      SECCLASS_SECURITY, SECURITY__LOAD_POLICY, NULL);
	if (length)
		goto out;

	/* No partial writes. */
	length = -EINVAL;
	if (*ppos != 0)
		goto out;

	length = -ENOMEM;
	data = vmalloc(count);
	if (!data)
		goto out;

	length = -EFAULT;
	if (copy_from_user(data, buf, count) != 0)
		goto out;

	length = security_load_policy(fsi->state, data, count, &load_state);
	if (length) {
		pr_warn_ratelimited("SELinux: failed to load policy\n");
		goto out;
	}

	length = sel_make_policy_nodes(fsi, load_state.policy);
	if (length) {
		selinux_policy_cancel(fsi->state, &load_state);
		goto out;
	}

	selinux_policy_commit(fsi->state, &load_state);

	length = count;

	audit_log(audit_context(), GFP_KERNEL, AUDIT_MAC_POLICY_LOAD,
		"auid=%u ses=%u lsm=selinux res=1",
		from_kuid(&init_user_ns, audit_get_loginuid(current)),
		audit_get_sessionid(current));
out:
	mutex_unlock(&fsi->state->policy_mutex);
	vfree(data);
	return length;
}


static ssize_t sel_read_policyvers(struct file *filp, char __user *buf,
				   size_t count, loff_t *ppos)
{
	char tmpbuf[TMPBUFLEN];
	ssize_t length;

	length = scnprintf(tmpbuf, TMPBUFLEN, "%u", POLICYDB_VERSION_MAX);
	return simple_read_from_buffer(buf, count, ppos, tmpbuf, length);
}
static ssize_t sel_read_enforce(struct file *filp, char __user *buf,
				   size_t count, loff_t *ppos)
{
	char tmpbuf[TMPBUFLEN];
	ssize_t length;

	length = scnprintf(tmpbuf, TMPBUFLEN, "%u", XXXXX);
	return simple_read_from_buffer(buf, count, ppos, tmpbuf, length);
}

static const struct file_operations sel_load_ops = {
	.write		= sel_write_load,
	.llseek		= generic_file_llseek,
};

static const struct file_operations sel_enforce_ops = {
	.read		= sel_read_enforce,
	// .write		= sel_write_enforce,
	.llseek		= generic_file_llseek,
};

static const struct file_operations sel_policyvers_ops = {
	.read		= sel_read_policyvers,
	.llseek		= generic_file_llseek,
};



static int sel_fill_super(struct super_block *sb, struct fs_context *fc)
{
	struct selinux_fs_info *fsi;
	int ret;
	struct dentry *dentry;
	struct inode *inode;
	struct inode_security_struct *isec;

	static const struct tree_descr selinux_files[] = {
		[SEL_LOAD] = {"load", &sel_load_ops, S_IRUSR|S_IWUSR},
		[SEL_ENFORCE] = {"enforce", &sel_enforce_ops, S_IRUGO|S_IWUSR},
		// [SEL_CONTEXT] = {"context", &transaction_ops, S_IRUGO|S_IWUGO},
		// [SEL_ACCESS] = {"access", &transaction_ops, S_IRUGO|S_IWUGO},
		// [SEL_CREATE] = {"create", &transaction_ops, S_IRUGO|S_IWUGO},
		// [SEL_RELABEL] = {"relabel", &transaction_ops, S_IRUGO|S_IWUGO},
		// [SEL_USER] = {"user", &transaction_ops, S_IRUGO|S_IWUGO},
		[SEL_POLICYVERS] = {"policyvers", &sel_policyvers_ops, S_IRUGO},
		// [SEL_COMMIT_BOOLS] = {"commit_pending_bools", &sel_commit_bools_ops, S_IWUSR},
		// [SEL_MLS] = {"mls", &sel_mls_ops, S_IRUGO},
		// [SEL_DISABLE] = {"disable", &sel_disable_ops, S_IWUSR},
		// [SEL_MEMBER] = {"member", &transaction_ops, S_IRUGO|S_IWUGO},
		// [SEL_CHECKREQPROT] = {"checkreqprot", &sel_checkreqprot_ops, S_IRUGO|S_IWUSR},
		// [SEL_REJECT_UNKNOWN] = {"reject_unknown", &sel_handle_unknown_ops, S_IRUGO},
		// [SEL_DENY_UNKNOWN] = {"deny_unknown", &sel_handle_unknown_ops, S_IRUGO},
		// [SEL_STATUS] = {"status", &sel_handle_status_ops, S_IRUGO},
		// [SEL_POLICY] = {"policy", &sel_policy_ops, S_IRUGO},
		// [SEL_VALIDATE_TRANS] = {"validatetrans", &sel_transition_ops,
		// 			S_IWUGO},
		/* last one */ {""}
	};

	// ret = selinux_fs_info_create(sb);
	// if (ret)
	// 	goto err;

	//simple_fill_super主要是填充对应文件系统的super_block的，也会在这个函数创建aufs的根目录的dentry和inode
	ret = simple_fill_super(sb, SELINUX_MAGIC, selinux_files);
	if (ret)
		goto err;

	// fsi = sb->s_fs_info;
	// fsi->bool_dir = sel_make_dir(sb->s_root, BOOL_DIR_NAME, &fsi->last_ino);
	// if (IS_ERR(fsi->bool_dir)) {
	// 	ret = PTR_ERR(fsi->bool_dir);
	// 	fsi->bool_dir = NULL;
	// 	goto err;
	// }

	// ret = -ENOMEM;
	// dentry = d_alloc_name(sb->s_root, NULL_FILE_NAME);
	// if (!dentry)
	// 	goto err;

	// ret = -ENOMEM;
	// inode = sel_make_inode(sb, S_IFCHR | S_IRUGO | S_IWUGO);
	// if (!inode) {
	// 	dput(dentry);
	// 	goto err;
	// }

	// inode->i_ino = ++fsi->last_ino;
	// isec = selinux_inode(inode);
	// isec->sid = SECINITSID_DEVNULL;
	// isec->sclass = SECCLASS_CHR_FILE;
	// isec->initialized = LABEL_INITIALIZED;

	// init_special_inode(inode, S_IFCHR | S_IRUGO | S_IWUGO, MKDEV(MEM_MAJOR, 3));
	// d_add(dentry, inode);

	// dentry = sel_make_dir(sb->s_root, "avc", &fsi->last_ino);
	// if (IS_ERR(dentry)) {
	// 	ret = PTR_ERR(dentry);
	// 	goto err;
	// }

	// ret = sel_make_avc_files(dentry);
	// if (ret)
	// 	goto err;

	// dentry = sel_make_dir(sb->s_root, "ss", &fsi->last_ino);
	// if (IS_ERR(dentry)) {
	// 	ret = PTR_ERR(dentry);
	// 	goto err;
	// }

	// ret = sel_make_ss_files(dentry);
	// if (ret)
	// 	goto err;

	// dentry = sel_make_dir(sb->s_root, "initial_contexts", &fsi->last_ino);
	// if (IS_ERR(dentry)) {
	// 	ret = PTR_ERR(dentry);
	// 	goto err;
	// }

	// ret = sel_make_initcon_files(dentry);
	// if (ret)
	// 	goto err;

	// fsi->class_dir = sel_make_dir(sb->s_root, CLASS_DIR_NAME, &fsi->last_ino);
	// if (IS_ERR(fsi->class_dir)) {
	// 	ret = PTR_ERR(fsi->class_dir);
	// 	fsi->class_dir = NULL;
	// 	goto err;
	// }

	// fsi->policycap_dir = sel_make_dir(sb->s_root, POLICYCAP_DIR_NAME,
	// 				  &fsi->last_ino);
	// if (IS_ERR(fsi->policycap_dir)) {
	// 	ret = PTR_ERR(fsi->policycap_dir);
	// 	fsi->policycap_dir = NULL;
	// 	goto err;
	// }

	// ret = sel_make_policycap(fsi);
	// if (ret) {
	// 	pr_err("SELinux: failed to load policy capabilities\n");
	// 	goto err;
	// }

	return 0;
err:
	pr_err("SELinux: %s:  failed while creating inodes\n",
		__func__);

	// selinux_fs_info_free(sb);

	return ret;
}

static int sel_get_tree(struct fs_context *fc)
{
	return get_tree_single(fc, sel_fill_super);
}

static const struct fs_context_operations sel_context_ops = {
	.get_tree	= sel_get_tree,
};

static int sel_init_fs_context(struct fs_context *fc)
{
	fc->ops = &sel_context_ops;
	return 0;
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



    // static void sel_kill_sb(struct super_block *sb)
    // {
    //     selinux_fs_info_free(sb);
    //     kill_litter_super(sb);
    // }


    static struct file_system_type sel_fs_type = {
        .name           = "selinuxfs",
        .init_fs_context = sel_init_fs_context,
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