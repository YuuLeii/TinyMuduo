#include "TcpServer.h"
#include <functional>

extern const int MAX_LINE = 100;
extern const int MAX_LISTENFD = 5; 

void TcpServer::handleNewConnection(int sockfd) {
	TcpConnection *newTcpConn = new TcpConnection(loop_.get(), sockfd);

	// set Callback
	// newTcpConn->set**Callback();
	newTcpConn->setMessageCallback(messageCallback_);
	newTcpConn->setCloseCallback(std::bind(&TcpServer::removeConnection, this, std::placeholders::_1));

	connections_[sockfd] = newTcpConn;

	newTcpConn->connectEstablished();
}

void TcpServer::removeConnection(int sockfd) {
	TcpConnection *tConn = connections_[sockfd];
	tConn->connectDestory();
	connections_.erase(sockfd);
}

TcpServer::TcpServer() {
}

void TcpServer::start() {
	loop_ = unique_ptr<EventLoop>(new EventLoop());
	
	acceptor_ = unique_ptr<Acceptor>(new Acceptor(loop_.get()));
	acceptor_->setNewConnectionCallback(std::bind(&TcpServer::handleNewConnection, this, std::placeholders::_1));
	acceptor_->listen();
}

void TcpServer::loop() {
	loop_->loop();	   
}
