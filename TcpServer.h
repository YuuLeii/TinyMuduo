#include <iostream>
#include <sys/epoll.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

using namespace std;

class TcpServer {
private:
    int epollfd;
    int listenfd;
public:
    TcpServer();
    ~TcpServer() {}
    void start();
    void loop();
};