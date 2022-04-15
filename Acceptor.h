#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "Channel.h"
class Channel;
class EventLoop;

class Acceptor {
    typedef std::function<void(int)> NewConnectionCallback;
public:
    Acceptor() = delete;
    Acceptor(EventLoop *loop);
    Acceptor(const Acceptor&) = delete;
    ~Acceptor() {}
    void listen();
    void handleRead();
    void setNewConnectionCallback(NewConnectionCallback cb) { newConnectionCallback_ = std::move(cb); }
private:
    int listenfd_;
    EventLoop *loop_;
    Channel acceptChannel_;
    NewConnectionCallback newConnectionCallback_;
};

#endif
