char *pathname, *p;
mm = current->mm;
if (mm) {
    down_read(&mm->mmap_sem);
    if (mm->exe_file) {
                pathname = kmalloc(PATH_MAX, GFP_ATOMIC);
                if (pathname) {
                    p = d_path(&mm->exe_file->f_path, pathname, PATH_MAX);
                    printk("pathname:%s\n", p);
                }
            }
    up_read(&mm->mmap_sem);
}


current->nameidata->filename->name should also work.

