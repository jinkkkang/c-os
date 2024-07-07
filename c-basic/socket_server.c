#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
#include <arpa/inet.h>  
  
#define SERVER_PORT 8080  
#define BUFFER_SIZE 1024  
  
int main() {  
    int server_fd, client_fd;  
    struct sockaddr_in server_addr, client_addr;  
    socklen_t client_len = sizeof(client_addr);  
    char buffer[BUFFER_SIZE];  
  
    // 创建socket  
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {  
        perror("socket creation failed");  
        exit(EXIT_FAILURE);  
    }  
  
    // 设置服务器地址信息  
    memset(&server_addr, 0, sizeof(server_addr));  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_addr.s_addr = INADDR_ANY;  
    server_addr.sin_port = htons(SERVER_PORT);  
  
    // 绑定socket到服务器地址  
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {  
        perror("bind failed");  
        exit(EXIT_FAILURE);  
    }  
  
    // 开始监听连接  
    if (listen(server_fd, 5) < 0) {  
        perror("listen failed");  
        exit(EXIT_FAILURE);  
    }  
  
    printf("Server is listening on port %d...\n", SERVER_PORT);  
  
    // 接受客户端连接  
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len)) < 0) {  
        perror("accept failed");  
        exit(EXIT_FAILURE);  
    }  
  
    // 读取客户端发送的消息  
    memset(buffer, 0, BUFFER_SIZE);  
    if (read(client_fd, buffer, BUFFER_SIZE - 1) < 0) {  
        perror("read failed");  
        exit(EXIT_FAILURE);  
    }  
  
    printf("Received message from client: %s\n", buffer);  
  
    // 回复客户端消息  
    sprintf(buffer, "Hello from server! You sent: %s", buffer);  
    if (write(client_fd, buffer, strlen(buffer)) < 0) {  
        perror("write failed");  
        exit(EXIT_FAILURE);  
    }  
  
    // 关闭连接和socket  
    close(client_fd);  
    close(server_fd);  
  
    return 0;  
}