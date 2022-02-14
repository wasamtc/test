#include "Socket.h"
#include <sys/socket.h>
#include "InetAddress.h"
#include "util.h"
#include <unistd.h>
#include <fcntl.h>

Socket::Socket() : fd(-1){
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "socket create error!");
}

Socket::Socket(int _fd) : fd(_fd){
    errif(fd == -1, "socket create error!");
}

Socket::~Socket(){
    if(fd != -1){
        close(fd);
        fd = -1;
    }

}

void Socket::bind(InetAddress* serv_addr){
    errif(::bind(fd, (sockaddr*)&serv_addr->addr, serv_addr->addr_len) == -1, "socket bind error!");
}

void Socket::listen(){
    errif(::listen(fd, SOMAXCONN) == -1, "socket listen error!");
}

void Socket::setnonblocking(){
    fcntl(fd, F_SETFL, O_NONBLOCK);
}

int Socket::accept(InetAddress* clnt_addr){
    int clnt_fd = ::accept(fd, (sockaddr*)&clnt_addr, &clnt_addr->addr_len);
    errif(clnt_fd == -1, "socket accept error!");
    return clnt_fd;
}

int Socket::getFd(){
    return fd;
}