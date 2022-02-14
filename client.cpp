#include "util.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define WRITE_BUFFER 1024

int main(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create error!");

    sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));

    while(true){
        char buf[WRITE_BUFFER];
        bzero(buf, sizeof(buf));
        scanf("%s", buf);
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
        if(write_bytes == -1){
            printf("client fd: %dwrite error!\n", sockfd);
        }
        bzero(buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if(read_bytes > 0){
            printf("message from server : %s\n", buf);
        } else if(read_bytes == 0){
            printf("server already disconnected!\n");
            break;
        } else if(read_bytes == -1){
            printf("socket read error!\n");
            break;
        }
    }
    close(sockfd);
    return 0;
}
