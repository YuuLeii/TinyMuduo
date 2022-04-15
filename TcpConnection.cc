#include "TcpConnction.h"
#include <unistd.h>
// #include <string.h>

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
	ssize_t n = ::read(channel_->fd(), readbuf_, sizeof(readbuf_));
	if (n > 0) {
		if (messageCallback_)
			messageCallback_();

	} else if (n == 0) {
		handleClose();
	} else {
		handleError();
	}
}

void TcpConnection::handleWrite() {
	// ssize_t n = ::write(channel_->fd(), readbuf_, strlen(readbuf_));
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