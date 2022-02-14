/* Author: wasamtc
 * EditTime: 2022-02-13
 * LastEditTime: 2022-02-14 10:45
 * LastEditors: wasamtc
 */

#include "Channel.h"
#include "Epoll.h"
#include "InetAddress.h"
#include "Socket.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MAXEVENT 1024
#define READ_BUFFER 1024

void handleReadEvent(int fd);

int main(){
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->bind(serv_addr);
    serv_sock->listen();

    Epoll *ep = new Epoll();
    serv_sock->setnonblocking();

    Channel *servChannel = new Channel(serv_sock->getFd(), ep);
    servChannel->enableReading();

    while(true){
        std::vector<Channel*>activeChannels = ep->poll();
        int nfds = activeChannels.size();
        for(int i = 0; i < nfds; ++i){
            if(serv_sock->getFd() == activeChannels[i]->getFd()){
                InetAddress *clnt_addr = new InetAddress();
                Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr));
                printf("new client fd:%d, ip:%s, port:%d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
                clnt_sock->setnonblocking();
                Channel *clntChannel = new Channel(clnt_sock->getFd(), ep);
                clntChannel->enableReading();
            } else if(activeChannels[i]->getRevents() & EPOLLIN){
                handleReadEvent(activeChannels[i]->getFd());
            } else{
                printf("something else happened\n");
            }
        }
    }
    delete serv_sock;
    delete serv_addr;

}

void handleReadEvent(int fd){
    char buf[READ_BUFFER];
    int i = 1;
    while(true){
        bzero(buf, READ_BUFFER); // 这里和源程序有一点不同，源程序是&buf,但是我觉得buf已经是指针类型的了。
        ssize_t read_bytes = read(fd, buf, READ_BUFFER);
        if(read_bytes > 0){
            printf("the message from client %d : %s\n", fd, buf);
            write(fd, buf, sizeof(buf));
        } else if(read_bytes == -1 && errno == EINTR){
            printf("continue reading");
            continue;
        } else if(read_bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)){
            printf("finish reading once. errno: %d\n", errno);
            break;
        } else if(read_bytes == 0){
            printf("EOF, client fd %d disconnected\n", fd);
            close(fd);
            break;
        }
    }
}