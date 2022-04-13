#include "EventLoop.h"
#include "Acceptor.h"
#include "TcpConnction.h"

#include <memory>

#include <iostream>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <map>

using namespace std;

class TcpConnection;

class TcpServer {
    typedef std::function<void()> MessageCallback;
    typedef std::function<void(int)> CloseCallback;
public:
    TcpServer();
    ~TcpServer() {}
    void start();
    void loop();
    void setMessageCallback(const MessageCallback &cb) { messageCallback_ = cb; }
    // void setConnectionCallback(const ConnectionCallback& cb) { connectionCallback_ = cb; }
    void handleNewConnection(int sockfd);
    void removeConnection(int sockfd);
private:
    int epollfd_;
    int listenfd_;
    unique_ptr<EventLoop> loop_;
    unique_ptr<Acceptor> acceptor_;
    MessageCallback messageCallback_;
    CloseCallback closeCallback_;
    
    map<int, TcpConnection*> connections_;
};