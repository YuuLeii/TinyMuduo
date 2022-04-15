#ifndef EPOLL_H
#define EPOLL_H

#include <vector>
#include <sys/epoll.h>

class EventLoop;
class Channel;

const int MAX_EVENTS = 500;

class Epoll {
public:
    Epoll() = delete;
    Epoll(const Epoll& ) = delete;
    Epoll& operator=(const Epoll&) = delete;
    Epoll(EventLoop* loop_);
    ~Epoll() {};

    void poll(int timeout, std::vector<Channel*>& activeChannels);
    void updateChannel(Channel* ch);
    void removeChannel(Channel* ch);
private:
    void update(int operation, Channel* ch);

    int epollfd_;
    struct epoll_event events_[MAX_EVENTS];

    EventLoop* ownerLoop_;
};

#endif