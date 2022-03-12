#include "TcpServer.h"
#include <assert.h>

extern const int MAX_LINE = 100;
extern const int MAX_EVENTS = 500;
extern const int MAX_LISTENFD = 5; 


void onRead(int sockfd) {
	char buf[MAX_LINE];
	memset(buf, 0, sizeof(buf));
	int readlen = read(sockfd, buf, MAX_LINE);
	if (readlen <= 0)
	{
		cout << "closed sock fd: " << sockfd << "from sockfd: " << sockfd << endl;
		close(sockfd);
	} else {
		int writelen = write(sockfd, buf, readlen);
		cout << "write " << writelen << " bytes to sockfd: " << sockfd << endl;
	}
}


TcpServer::TcpServer() {
    epollfd_ = epoll_create(1);
	assert(epollfd_ != -1);
    
    listenfd_ = socket(AF_INET, SOCK_STREAM, 0);
    assert(listenfd_ != -1);

	loop_ = unique_ptr<EventLoop>(new EventLoop(epollfd_, listenfd_));
	messageCallback_ = std::function<void(int)>(onRead);
}

void TcpServer::start() {
	assert(loop_ != nullptr);
	assert(listenfd_ != -1);
	acceptor_ = unique_ptr<Acceptor>(new Acceptor(loop_.get(), listenfd_, messageCallback_ ));
	acceptor_->listen();
}

void TcpServer::loop() {
	loop_->loop();	   
}
