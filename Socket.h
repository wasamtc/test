#pragma once // 这个保证每个头文件只编译一次

class InetAddress;
class Socket
{
    private:
        int fd;
    public:
        Socket();
        Socket(int);
        ~Socket();
        
        void bind(InetAddress*);
        void listen();
        void setnonblocking();
        void connect(InetAddress*);
        
        int accept(InetAddress*);
        
        int getFd();
};
