#include <linux/init.h>   // 包含宏定义如 module_init 和 module_exit
#include <linux/module.h> // 包含核心模块函数和宏定义
#include <linux/sched.h>  // 包含 task_struct 的定义和其他调度相关的定义
#include <linux/pid.h>    // 提供操作 PID 的接口

MODULE_LICENSE("GPL"); // 声明模块许可证为 GPL，避免警告信息

static int __init get_task_struct_init(void) {
    struct pid *pid_struct;
    struct task_struct *task;

    // 假设我们要查找的 PID 号为 1 (通常是 init 或 systemd)
    pid_t target_pid = 1;
    
    // 根据 PID 获取 pid 结构
    pid_struct = find_get_pid(target_pid);
    if (!pid_struct) {
        printk(KERN_ERR "Failed to find PID %d\n", target_pid);
        return -EINVAL;
    }
    
    // 根据 pid 结构获取 task_struct
    task = pid_task(pid_struct, PIDTYPE_PID);
    if (!task) {
        printk(KERN_ERR "Failed to find task_struct for PID %d\n", target_pid);
        return -EINVAL;
    }

    // 打印进程名
    printk(KERN_INFO "Process name: %s [PID=%d]\n", task->comm, task->pid);

    return 0; // 成功返回0
}

static void __exit get_task_struct_exit(void) {
    printk(KERN_INFO "Module exiting.\n");
}

module_init(get_task_struct_init); // 模块初始化函数
module_exit(get_task_struct_exit); // 模块退出函数