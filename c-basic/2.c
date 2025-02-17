#include <stdio.h>
#include <string.h>


struct Books
{
   int   book_id;
   char  title[10];
   char  name[10];
};

int main(){ 
    struct Books book1;

    //记住，这里不能这样写, 字符串在c里面是数组，一旦定义好后，数组的名字就是数组的地址，
    //即数组首元素的地址，数组名本身是常量地址，意味着它的值是不能被改变的，一旦确定，就不能再指向其他地方。
    //参考： https://www.runoob.com/cprogramming/c-arrays.html
    //所以这里不能这样写： book1.title = "world"
    //而是要用strcpy 或者strmem
    strcpy(book1.title, "helltitle");
    char newname = "hahaha";
    // memccpy(book1.name, &newname,'a',strlen(newname));

    printf("book,%s\n",book1.title);
    printf("name,%s\n",book1.name);

}
