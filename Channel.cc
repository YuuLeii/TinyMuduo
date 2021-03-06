#include "Channel.h"
#include <poll.h>

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop *loop, int fd)
	: loop_(loop), fd_(fd), events_(0), revents_(0),
	  eventHandling_(false)
{
}

Channel::~Channel() {
	assert(!eventHandling_);
}
void Channel::update() {
	loop_->updateChannel(this);
}

void Channel::remove() {
	loop_->removeChannel(this);
}

void Channel::handleEvent() {
	eventHandling_ = true;
	if ((revents_ & POLLHUP) && !(revents_ & POLLIN)) {
		if (closeCallback_) closeCallback_();
	}

	if (revents_ & (POLLERR | POLLNVAL)) {
		if (errorCallback_) errorCallback_();
	}
	if (revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
		if (readCallback_) readCallback_();
	}
	if (revents_ & POLLOUT) {
		if (writeCallback_) writeCallback_();
	}
	eventHandling_ = false;
}