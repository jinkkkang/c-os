# 内核中的锁

## 自旋锁 Spin Lock

结构体: spinlock_t

初始化:
spinlock_t lock;



spin_lock(lock) 
该宏用于获得自旋锁lock，如果能够立即获得锁，它就马上返回，否则，它将在那里自旋，直到该自旋锁的保持者释放。【即 会等】

spin_trylock(lock)
该宏尝试获得自旋锁lock，如果能立即获得锁，它获得锁并返回true，否则立即返回false，实际上不再“在原地打转”。【即 不会等】

自旋锁的使用
```
/* 定义一个自旋锁*/
spinlock_t lock;
spin_lock_init(&lock);
spin_lock (&lock) ; /* 获取自旋锁，保护临界区 */
. . ./* 临界区*/
spin_unlock (&lock) ; /* 解锁*/
```

## 互斥锁 

mutex_lock

## 信号量 Semaphore

