#include "Socket.h"
#include "Socket.cpp"
#include "InetAddress.h"
#include "InetAddress.cpp"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUFFER 1024

int main(){
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    
    serv_sock->connect(serv_addr);
    
    while(true){
        char buf[MAX_BUFFER];
        bzero(&buf, sizeof(buf));
        
        scanf("%s", buf);
        ssize_t write_bytes = write(serv_sock->getFd(), buf, sizeof(buf));
        if(write_bytes == -1){
            printf("socket already disconnected!\n");
            break;
        }
        ssize_t read_bytes = read(serv_sock->getFd(), buf, sizeof(buf));
        if(read_bytes > 0){
            printf("message from server:%s\n", buf);
        }else if(read_bytes == 0){
            printf("socket already disconneted!\n");
            break;
        }else if(read_bytes == -1){
            printf("socket read error!\n");
            close(serv_sock->getFd());
            break;
        }
    }
    close(serv_sock->getFd());
    return 0;
}
