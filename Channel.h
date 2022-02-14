#pragma once
#include <sys/epoll.h>

class Epoll;
class Channel{
    private:
        Epoll *ep;
        int fd;
        uint32_t events;
        uint32_t revents;
        bool inEpoll;
    public:
        Channel(int _fd, Epoll *_ep);
        ~Channel();

        void enableReading();

        void setRevents(uint32_t);
        uint32_t getRevents();
        uint32_t getEvents();
        bool getInEpoll();
        void setInEpoll();
        int getFd();
};