#include "TcpServer.h"
#include <assert.h>

#include <iostream>

extern const int MAX_LINE = 100;
extern const int MAX_EVENTS = 500;
extern const int MAX_LISTENFD = 5; 

void TcpServer::handleNewConnection(int sockfd) {
	TcpConnection *newTcpConn = new TcpConnection(loop_.get(), sockfd);

	// set Callback
	// newTcpConn->set**Callback();
	newTcpConn->setMessageCallback(messageCallback_);
	newTcpConn->setCloseCallback(std::bind(&TcpServer::removeConnection, this, placeholders::_1));

	connections_[sockfd] = newTcpConn;
	// std::cout << "now have " << (int)connections_.size() << " connections." << std::endl;
	newTcpConn->connectEstablished();
}

void TcpServer::removeConnection(int sockfd) {
	connections_.erase(sockfd);
	std::cout << "now have " << (int)connections_.size() << " connections." << std::endl;
}

TcpServer::TcpServer() {
    epollfd_ = epoll_create(1);
	assert(epollfd_ != -1);
    
    listenfd_ = socket(AF_INET, SOCK_STREAM, 0);
    assert(listenfd_ != -1);

	loop_ = unique_ptr<EventLoop>(new EventLoop(epollfd_, listenfd_));
}

void TcpServer::start() {
	assert(loop_ != nullptr);
	assert(listenfd_ != -1);

	acceptor_ = unique_ptr<Acceptor>(new Acceptor(loop_.get(), listenfd_));
	acceptor_->setNewConnectionCallback(std::bind(&TcpServer::handleNewConnection, this, placeholders::_1));
	acceptor_->listen();
}

void TcpServer::loop() {
	loop_->loop();	   
}
