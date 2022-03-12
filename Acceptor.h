#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "Channel.h"
class Channel;
class EventLoop;

class Acceptor {
public:
    typedef std::function<void(int)> NewConnectionReadCallback;
    Acceptor() = delete;
    Acceptor(EventLoop *loop, int listenfd, NewConnectionReadCallback &cb);
    ~Acceptor() {}
    void listen();
    void handleRead();
    void setNewConnectionReadCallback();
private:
    int listenfd_;
    EventLoop *loop_;
    NewConnectionReadCallback newConnectionReadCallback_;
    Channel acceptChannel_;
};

#endif
