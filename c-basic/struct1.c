#include <stdio.h>
#include <string.h>


struct Books
{
   int   book_id;
   char  title[20];

};

void printstr(struct Books *book){
    printf("boot id: %d", book->book_id);
}

int main(){ 
    struct Books book1;
    //记住，这里不能这样写,  book1.title = "world" 字符串在c里面是数组，一旦定义好后，数组的名字就是数组的地址，
    //即数组首元素的地址，数组名本身是常量地址，意味着它的值是不能被改变的，一旦确定，就不能再指向其他地方。
    //参考： https://www.runoob.com/cprogramming/c-arrays.html
    //所以这里不能这样写： book1.title = "world"
    //而是要用strcpy 
    strcpy(book1.title, "helloworld");

    //或者用memcpy，复制内存过去
    char newname[] = "hahaha";
    memcpy(book1.title, &newname, strlen(newname));

    printf("book,%s\n",book1.title);

    book1.book_id = 12;

    printf("book title: %s\n", book1.title);
    printf("book id: %d\n", book1.book_id);
    printstr(&book1);
}
