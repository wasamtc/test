#pragma once

class InetAddress;

class Socket{
    private:
        int fd;
    public:
        Socket();
        Socket(int _fd);
        ~Socket();
        void bind(InetAddress* serv_addr);
        void listen();
        void setnonblocking();

        int accept(InetAddress* clnt_addr);

        int getFd();
};