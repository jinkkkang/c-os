## 一个简单的内核模块

module_init driver initialization entry point
module_exit driver exit entry point



## 常用到宏
__init 是一个宏，用来标记某个函数或数据仅在内核初始化期间使用，使用这个标记可以节省资源，因为这样标记的代码和数据在内核自动完成后会被释放。就是用在那些一旦初始化完成任务就不再需要的代码段。


 __exit，这是标记模块在卸载的时候执行。


 ## 在插入模块时添加参数
 
* module_param(name, type, perm)  *  是最常规的传参方式，支持对普通数据类型的参数的读写。

name ：表示模块参数名 （模块中定义和sysfs中显示的都是这个名字）
type：表示数据类型，如uint表示unsigned int
perm：sysfs文件中参数文件的访问权限 （一般8进制表示）

```
static unsigned int param_uint;
module_param(param_uint, uint, 0600);
MODULE_PARM_DESC(param_uint, "This is a uint parameter!"); 
```

传入参数的几个方法：
* insmod hello.ko param_uint=999
* grub cmdline中加入 module_param_test.param_uint=999, 参考nvme timeout参数: nvme_core.admin_timeout=4294967295
* 通过 /sys/module/ 接口 读取或者写入.
```
[root@jinkang-sersi-hz kernel-dev]# ls -al /sys/module/hello/parameters/param_uint 
-rw-------. 1 root root 4096 Jul 13 23:45 /sys/module/hello/parameters/param_uint
[root@jinkang-sersi-hz kernel-dev]# cat  /sys/module/hello/parameters/param_uint 
999
[root@jinkang-sersi-hz kernel-dev]# echo 77 > /sys/module/hello/parameters/param_uint 
[root@jinkang-sersi-hz kernel-dev]# cat  /sys/module/hello/parameters/param_uint 
77
```

* module_param_array(name, type, nump, perm)  传入数组
name：表示数组名 
type：数组元素类型 
nump：一个整型变量，用于存储数组中元素的数量，可选（不关心可以写为NULL） 
perm：sysfs文件中参数文件的权限 （一般8进制表示）

```
/* array: echo "1,2,3,4,4" > param_array */
static int param_array[5];
static int array_num;
module_param_array(param_array, int, &array_num, 0600);
MODULE_PARM_DESC(param_bool, "This is a array parameter!");
```

插入模块注入参数
```
insmod hello.ko  param_array=1,4,6
[877426.171910] Hello world: start print all val in param_array 39
[877426.171910] Hello world: param index: 0,val: 1
[877426.171911] Hello world: param index: 1,val: 4
[877426.171912] Hello world: param index: 2,val: 6
```

插入模块注入比较多的参数时，会有异常报错:
```
insmod hello.ko  param_array=1,4,6,4,4,4,4,4
[877444.686592] param_array: can only take 5 arguments
[877444.686809] hello: `1' invalid for parameter `param_array'
```

## 参数的回调函数

在读取参数/设置参数时要操作的函数
module_param_cb(name, ops, arg, perm)
name ：表示模块参数名 （模块中定义和sysfs中显示的都是这个名字）
ops：参数的 set&get 操作集 
arg：用于操作集的参数 perm：sysfs文件中参数文件的权限 （一般8进制表示）




