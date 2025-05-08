# 引用计数

kref [https://www.kernel.org/doc/Documentation/kref.txt]


https://www.cnblogs.com/liangliangge/p/11389898.html




```
static inline struct task_struct *get_task_struct(struct task_struct *t)
{
	refcount_inc(&t->usage);
	return t;
}
```

