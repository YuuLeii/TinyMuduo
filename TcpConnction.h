#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <memory>
#include <functional>
#include "Channel.h"

class EventLoop;

class TcpConnection {
private:
	typedef std::function<void()> MessageCallback;
	typedef std::function<void(int)> CloseCallback;
	typedef char (Buffer)[1024];
public:
	TcpConnection(EventLoop *loop, int sockfd);
	~TcpConnection() {}

	void setMessageCallback(const MessageCallback& cb) { messageCallback_ = cb; }
	void setCloseCallback(CloseCallback cb) {  closeCallback_ = std::move(cb);  }

	// called when TcpServer accepts a new connection and should be called only once.
	void connectEstablished();
private:
	void handleRead();
	void handleWrite();
	void handleClose();
	void handleError();
	
	EventLoop *loop_;
	int sockfd_;
	std::unique_ptr<Channel> channel_;

	Buffer readbuf_;
	Buffer writebuf_;

	MessageCallback messageCallback_;
	CloseCallback closeCallback_;
};

#endif