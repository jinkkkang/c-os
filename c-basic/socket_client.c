#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
#include <arpa/inet.h>  
  
#define SERVER_IP "127.0.0.1"  
#define SERVER_PORT 8080  
#define BUFFER_SIZE 1024  
  
int main() {  
    int sock;  
    struct sockaddr_in server_addr;  
    char buffer[BUFFER_SIZE];  
  
    // 创建socket  
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {  
        perror("socket creation failed");  
        exit(EXIT_FAILURE);  
    }  
  
    // 设置服务器地址信息  
    memset(&server_addr, 0, sizeof(server_addr));  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_port = htons(SERVER_PORT);  
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {  
        perror("invalid address/Address not supported");  
        exit(EXIT_FAILURE);  
    }  
  
    // 连接到服务器  
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {  
        perror("connection failed");  
        exit(EXIT_FAILURE);  
    }  
  
    // 发送消息给服务器  
    printf("Enter a message to send to the server: ");  
    fgets(buffer, BUFFER_SIZE, stdin);

}