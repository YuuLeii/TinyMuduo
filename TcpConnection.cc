#include "TcpConnction.h"
#include <unistd.h>

extern int MAX_LINE;

class EventLoop;
class Channel;

TcpConnection::TcpConnection(EventLoop *loop, int sockfd)
				: loop_(loop), sockfd_(sockfd),
					channel_(new Channel(loop_, sockfd)) 
{

	channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this));
	channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
	channel_->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
	channel_->setErrorCallback(std::bind(&TcpConnection::handleError, this));

}

void TcpConnection::connectEstablished() {
	channel_->enableReading();
}

void TcpConnection::connectDestory() {
	channel_->remove();
}

void TcpConnection::handleRead() {
	char line[MAX_LINE];
	ssize_t readlength = inputBuffer_.readFd(channel_->fd());
	if (readlength  > 0) {
		if (messageCallback_)
			messageCallback_(this, &inputBuffer_);

		channel_->enableWriting();
	} else if (readlength == 0) {
		handleClose();
	} else {
		handleError();
	}
}

void TcpConnection::handleWrite() {
	if (channel_->isWriting()) {
		ssize_t n = ::write(channel_->fd(), outputBuffer_.str().c_str(), outputBuffer_.str().length());
		if (n > 0) {
			outputBuffer_.retrieve(n);
			if (outputBuffer_.empty()) {
				channel_->disableWriting();

				if (writeCompleteCallback_)
					loop_->queueInLoop(std::bind(writeCompleteCallback_, this));

			}
		}
	}
}

// 库的使用者只需要调用send把要发给客户端连接的数据存到outputBuffer里，该库负责把数据发送出去
void TcpConnection::send(const string& message) {
	size_t n = 0;
	// if outputBuffer is empty, write to socket immediately.
	if (outputBuffer_.empty()) {
		n = write(sockfd_, message.c_str(), message.length());
		if (n < 0) 
			;
			// LOG("write error");
		else if (n == static_cast<int>(message.length())) {
			if (writeCompleteCallback_)
				loop_->queueInLoop(std::bind(writeCompleteCallback_, this));
		}
	}

	if (n < static_cast<int>(message.length())) {
		outputBuffer_.str() += message.substr(n);
		if (!channel_->isWriting())
			channel_->enableWriting();
	}
}

void TcpConnection::handleClose() {
	// channel_->disableAll();
	close(sockfd_);
	if (closeCallback_) 
		closeCallback_(sockfd_);
}

void TcpConnection::handleError() {
	// close(sockfd_);
}
