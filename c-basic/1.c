#include <stdio.h>
#include <string.h>

#include <stdio.h>  
  
int main() {  
    // 假设我们有一个指向int的内存地址，这里我们使用一个示例地址  
    // 注意：在实际编程中，不要直接使用这样的硬编码地址  
    unsigned long exampleAddress = 0x12345678; // 示例地址，请替换为实际有效的地址  
  
    // 将这个地址转换为指向int的指针  
    int* ptrToInt = (int*)exampleAddress;  
  
    // 假设这个地址确实指向了一个int值，我们尝试读取它  
    // 注意：如果地址无效或未指向int，下面的行为将是未定义的  
    int value = *ptrToInt;  
  
    // 打印读取到的值  
    printf("The value at address 0x%lx is: %d\n", exampleAddress, value);  
  
    return 0;  
}