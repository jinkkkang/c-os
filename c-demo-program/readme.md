这是一个c语言项目, 目的是学习c的基础语法，把一些系统编程串起来，只是学习用的。


这个项目试图模仿 openEuler项目的 sysmonitor, 作系统监控。


准备实现的项目有:

1 信号监控: 使用 kprobe 监控信号，把信号信息打印出来; done
2  进程监控，有僵尸进程的话 把进程信息打出来
 : kernel module中 遍历所有的 task_list
  学习下task_struct
  进程的top内存情况统计等等.
3  系统内存监控. 类似 echo m > sys trigger  那样


process 

client 端

kernel module


rsync -r /Users/jinkangkang/learnc/c-demo-program/kernel-module root@47.99.152.7:/wenaoc/