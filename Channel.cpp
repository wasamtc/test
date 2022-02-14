#include "Channel.h"
#include "Epoll.h"

Channel::Channel(int _fd, Epoll *_ep) : events(0), revents(0), inEpoll(false){
    fd = _fd;
    ep = _ep;
}

Channel::~Channel(){

}

void Channel::enableReading(){
    events = EPOLLIN | EPOLLET;
    ep->updateChannel(this);
}

int Channel::getFd(){
    return fd;
}

uint32_t Channel::getEvents(){
    return events;
}

uint32_t Channel::getRevents(){
    return revents;
}

bool Channel::getInEpoll(){
    return inEpoll;
}

void Channel::setInEpoll(){
    inEpoll = true;
}

void Channel::setRevents(uint32_t _ev){
    revents = _ev;
}