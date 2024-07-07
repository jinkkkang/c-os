#include <stdio.h>
 
//第一个枚举成员的默认值为整型的 0，后续枚举成员的值在前一个成员上加 1。我们在这个实例中把第一个枚举成员的值定义为 1，第二个就为 2，以此类推
//
// int main(){ 
//     enum seaon {spring,subb,fail=5,winter};
//     enum seaon jijie;
//     jijie = subb;
//     printf("%d\n",jijie);
// }



// 枚举类型转换
int main()
{
 
    enum day
    {
        saturday,
        sunday,
        monday,
        tuesday,
        wednesday,
        thursday,
        friday
    } workday;
 
    int a = 1;
    enum day weekend;
    weekend = ( enum day ) a;  //类型转换
    //weekend = a; //错误
    printf("weekend:%d",weekend);
    return 0;
}
