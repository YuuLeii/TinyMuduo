#include <memory>
#include <map>

#include "EventLoop.h"
#include "Acceptor.h"
#include "TcpConnction.h"

using std::unique_ptr;

class TcpConnection;
class TcpServer {
    typedef std::function<void()> MessageCallback;
public:
    TcpServer();
    ~TcpServer() {}
    void start();
    void loop();
    void setMessageCallback(const MessageCallback &cb) { messageCallback_ = cb; }
    
    void handleNewConnection(int sockfd);
    void removeConnection(int sockfd);
private:
    unique_ptr<EventLoop> loop_;
    unique_ptr<Acceptor> acceptor_;
    MessageCallback messageCallback_;
    
    std::map<int, TcpConnection*> connections_;
};