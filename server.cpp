#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "util.h"
#include "InetAddress.h"
#include "Epoll.h"
#include "Socket.h"

#define MAX_BUFFER 1024

void handleReadEvent(int fd);

int main(){
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    
    serv_sock->setnonblocking();
    serv_sock->bind(serv_addr);
    serv_sock->listen();    
    
    Epoll *ep = new Epoll();
    ep->addFd(serv_sock->getFd(), EPOLLIN | EPOLLET);
    
    while(true){
        std::vector<epoll_event> es;
        es = ep->poll(-1);
        int nfds = es.size();
        for(int i; i < nfds; ++i){
            if(serv_sock->getFd() == es[i].data.fd){
                InetAddress *clnt_addr = new InetAddress("127.0.0.1", 8888);
                Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr));
                printf("new client fd %d IP:%s", clnt_sock->getFd(), inet_ntoa(clnt_addr->addr.sin_addr));
                ep->addFd(clnt_sock->getFd(), EPOLLIN | EPOLLET);
            }else if(es[i].events & EPOLLIN){
                handleReadEvent(es[i].data.fd);
            }else{
                printf("something else happened!\n");
            }
        }
    }
    delete serv_sock;
    delete serv_addr;
    return 0;
}

void handleReadEvent(int fd){
    char buf[MAX_BUFFER];
    while(true){
        bzero(&buf, MAX_BUFFER);
        ssize_t read_bytes = read(fd, buf, sizeof(buf));
        if(read_bytes > 0){
            printf("message from clinet fd %d : %s\n", fd, buf);
            write(fd, buf, sizeof(buf));
        }else if(read_bytes == -1 && errno == EINTR){
            printf("continue reading\n");
            continue;
        }else if(read_bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)){
            printf("finish reading, errno:%d\n", errno);
            break;
        }else if(read_bytes == 0){
            printf("EOF, client fd %d disconnected\n", fd);
            close(fd);
            break;
        }
    }

}
