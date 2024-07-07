#include <stdio.h>

/*共用体是一种特殊的数据类型，允许您在相同的内存位置存储不同的数据类型。您可以定义一个带有多成员的共用体，
但是任何时候只能有一个成员带有值。共用体提供了一种使用相同的内存位置的有效方式。
*/
int main(void) {
    union info
    {
        /* data */
        int age;
        char* name;

    };

    union  info result;
    result.age = 8;
    result.name = "zhangsan";
    printf("age: %d\n",result.age);
    printf("name: %s\n",result.name);

//同一时间只使用一个变量
// output
// age: 215801742
// name: zhangsan
    
}