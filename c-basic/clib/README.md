
# 动态库和静态库

## 动态链接

库名后缀: .so

gcc -fPIC -shared  mylib1.c mylib2.c -o libmylibx.so


gcc main.c -L. libmylibx.so -o main

./main 
./main: error while loading shared libraries: libmylib1.so: cannot open shared object file: No such file or directory

需要将库文件复制至/lib64 目录才能使用

[root@test]# cp libmylibx.so  /lib64/
[root@test]# ./main 
main call mylib1 function sum: 7
main call mylib2 function sub: 3


```
ldd  main
linux-vdso.so.1 (0x00007ffde51a8000)
libmylibx.so => /lib64/libmylibx.so (0x00007f7b049f5000)
libc.so.6 => /lib64/libc.so.6 (0x00007f7b04819000)
/lib64/ld-linux-x86-64.so.2 (0x00007f7b04a02000)
```

## 静态链接：
库名后缀: .a
在链接阶段，会将汇编生成的目标文件(.o)与引用的库一起链接打包到可执行文件中。因此对应的链接方式称为静态链接。


gcc -c mylib1.c -o mylib1.o
gcc -c mylib2.c -o mylib2.o

使用ar命令将多个目标文件打包
ar -crv lib+(库名)+.a  .o文件
ar  -crv libmyliball.a  mylib1.o mylib2.o 



方法1
gcc main.c libmyliball.a  -L. -o main
方法2 gcc 会在静态库名前加上前缀 lib，然后追加扩展名.a 得到的静态库文件名来查找静态库文件。
gcc -o main main.c -L . -lmyliball 

```
ldd ./main
linux-vdso.so.1 (0x00007ffd6399e000)
libc.so.6 => /lib64/libc.so.6 (0x00007f82adb91000)
/lib64/ld-linux-x86-64.so.2 (0x00007f82add75000)
```