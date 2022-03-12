#include "EventLoop.h"
#include "Acceptor.h"
#include <memory>

#include <iostream>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

using namespace std;

class TcpServer {
public:

    typedef std::function<void(int)> MessageCallback;
    TcpServer();
    ~TcpServer() {}
    void start();
    void loop();
    void setMessageCallback(const MessageCallback &cb) { messageCallback_ = cb; }

private:
    int epollfd_;
    int listenfd_;
    unique_ptr<EventLoop> loop_;
    unique_ptr<Acceptor> acceptor_;
    MessageCallback messageCallback_;
};