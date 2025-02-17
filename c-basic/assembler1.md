## 常见的寄存器

rbp 寄存器 
栈帧基址指针（Base Pointer）, 指向当前函数的“最低地址”,

rbp可以通过固定的偏移量来访问栈中的数据，


rsp 寄存器 
栈指针（Stack Pointer）, 指向当前函数的栈顶指针，指向栈中当前函数的 “最高地址”。
rsp 寄存器则会随着栈的变化而变化，反映了栈的动态变化


pc 指针用来保存下一条指令地址，指示接下来要执行那一条指令。



16 个寄存器 

%rax 作为函数返回值使用。
%rdi (第1个)，%rsi (第2个)，%rdx (第3个)，%rcx (第4个)，%r8 (第5个)，%r9 (第6个) 用作函数参数
%rbx，%rbp，%r12，%r13，%14，%15 用作数据存储，遵循被调用者使用规则(Callee Save)，简单说就是随便用，调用子函数之前要备份它，以防他被修改
%r10，%r11 用作数据存储，遵循调用者使用规则(Caller Save)，简单说就是使用之前要先保存原值



# 函数的调用
```
#include <stdio.h>

int testhello(){
    int a = 3;
    a += 2;
    return a;
}
int main() {
    int res = testhello();
    printf("res: %d", res);
}
```

```
Disassembly of section .text:

0000000000000000 <testhello>:
   0:   55                      push   %rbp   
   
   #将基指针寄存器（rbp）的值压入栈中. 这是函数开始时的常见做法，用于保存旧的基指针，以便在函数结束时恢复它。

   1:   48 89 e5                mov    %rsp,%rbp 
   #将栈指针寄存器（rsp）的值移动到基指针寄存器（rbp）。这样，rbp就指向了当前栈帧的开始。

   4:   c7 45 fc 03 00 00 00    movl   $0x3,-0x4(%rbp)
   将立即数0x3移动到rbp寄存器指向的地址减去4的位置。这实际上是在局部变量的位置存储3。

   b:   83 45 fc 02             addl   $0x2,-0x4(%rbp)
   将rbp寄存器指向的地址减去4的位置的值加上0x2。这是对上一步中存储的3进行加法操作，结果是5。

   f:   8b 45 fc                mov    -0x4(%rbp),%eax
   将rbp寄存器指向的地址减去4的位置的值移动到eax寄存器。eax寄存器通常用于存储函数的返回值。

  12:   5d                      pop    %rbp
  将栈顶的值（即旧的rbp值）弹出到rbp寄存器，恢复基指针。
  13:   c3                      retq  
```



# 函数的传参
```
#include <stdio.h>

int testhello(int a){
    int ret = a + 3;
    return ret;
}
int main() {
    int res = testhello(10);
    return res;
}
```

```
objdump -S test.o  

test.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <testhello>:
   0:   55                      push   %rbp
   1:   48 89 e5                mov    %rsp,%rbp
   4:   89 7d ec                mov    %edi,-0x14(%rbp)
   7:   8b 45 ec                mov    -0x14(%rbp),%eax
   a:   83 c0 03                add    $0x3,%eax
   d:   89 45 fc                mov    %eax,-0x4(%rbp)
  10:   8b 45 fc                mov    -0x4(%rbp),%eax
  13:   5d                      pop    %rbp
  14:   c3                      retq   

0000000000000015 <main>:
  15:   55                      push   %rbp
  16:   48 89 e5                mov    %rsp,%rbp
  19:   48 83 ec 10             sub    $0x10,%rsp
  1d:   bf 0a 00 00 00          mov    $0xa,%edi
  22:   e8 00 00 00 00          callq  27 <main+0x12>
  27:   89 45 fc                mov    %eax,-0x4(%rbp)
  2a:   8b 45 fc                mov    -0x4(%rbp),%eax
  2d:   c9                      leaveq 
  2e:   c3                      retq   
```

## Leaveq:

leaveq用于函数的退出和栈帧的清理。这个指令通常在函数的结尾处执行，以完成函数的退出操作，包括释放局部变量所占用的栈空间和返回到调用函数的位置。leaveq 的作用如下：
`恢复栈帧基址指针 %rbp`：

leaveq 指令会将栈帧基址指针 %rbp 的值恢复为之前保存在栈上的值。这个值通常是在函数开始时通过 push %rbp 指令保存的。
恢复 %rbp 的值可以将当前函数的栈帧的基址重新设置为调用函数的栈帧，从而回到了调用函数的上下文。
`释放栈上的局部变量`：

当函数执行时，它会在栈上分配空间来存储局部变量和其他临时数据。leaveq 指令会通过增加栈指针 %rsp 的值，释放这些局部变量所占用的栈空间。
通过释放局部变量所占用的栈空间，函数可以安全地退出而不会导致内存泄漏或栈不一致的问题。

`返回到调用函数`：

leaveq 指令通常与 ret 指令一起使用，以完成函数的返回操作。ret 指令将从栈中弹出返回地址，并将程序计数器（PC）设置为返回地址，从而将控制权返回到调用函数的位置。
这使得函数能够正常退出并返回到调用函数的上下文中。