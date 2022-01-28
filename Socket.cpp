/*
 *Author: wasamtc
 *EditTime: 2022-01-27 10:47
 *LastEditors: wasamtc
 *LastEditTime: 2022-01-27 10:47
 *Description: 用Socket class把套接字所需的功能封装起来，从而在主程序中精简代码，减少重复使用，有构造，析构，绑定，监听，接收等功能
 */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include "Socket.h"
#include "InetAddress.h"
#include "util.h"

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

void Socket::setnonblocking(){
    fcntl(fd, F_SETFL, O_NONBLOCK);
}

void Socket::bind(InetAddress* addr){
    errif(::bind(fd, (sockaddr*)&addr->addr, addr->addr_len) == -1, "socket bind error!"); //这里bind前面加::主要是为了在全局里面找
}

void Socket::listen(){
    errif(::listen(fd, SOMAXCONN) == -1, "socket listen error!");
}

void Socket::connect(InetAddress *addr){
    errif(::connect(fd, (sockaddr*)&addr->addr, sizeof(addr->addr)) == -1, "socket connect error");
}

int Socket::accept(InetAddress* addr){
    int clnt_sockfd = ::accept(fd, (sockaddr*)&addr->addr, &addr->addr_len);
    errif(clnt_sockfd == -1, "socket accept error!");
    return clnt_sockfd;
}

int Socket::getFd(){
    return fd;
}

