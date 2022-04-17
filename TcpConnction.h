#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <memory>
#include <functional>

#include "Channel.h"
#include "Buffer.h"

class EventLoop;

class TcpConnection {
private:
	typedef std::function<void(TcpConnection*, Buffer*)> MessageCallback;
	typedef std::function<void(TcpConnection*)> WriteCompleteCallback;
	typedef std::function<void(int)> CloseCallback;
public:
	TcpConnection() = delete;
	TcpConnection(const TcpConnection&) = delete;
	TcpConnection& operator=(const TcpConnection&) = delete;
	TcpConnection(EventLoop *loop, int sockfd);
	~TcpConnection() {}

	void setMessageCallback(const MessageCallback& cb) { messageCallback_ = cb; }
	void setCloseCallback(CloseCallback cb) {  closeCallback_ = std::move(cb);  }
	void setWriteCompleteCallback(WriteCompleteCallback cb) {	writeCompleteCallback_ = std::move(cb);	}

	// called when TcpServer accepts a new connection and should be called only once.
	void connectEstablished();
	// called when TcpServer removes me from its map and should be called only once.
	void connectDestory();
	void send(const string&);
private:
	void handleRead();
	void handleWrite();
	void handleClose();
	void handleError();
	
	EventLoop *loop_;
	int sockfd_;
	std::unique_ptr<Channel> channel_;

	Buffer inputBuffer_;
	Buffer outputBuffer_;

	MessageCallback messageCallback_;
	WriteCompleteCallback writeCompleteCallback_;
	CloseCallback closeCallback_;
};

#endif