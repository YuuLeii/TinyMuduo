#include <sys/eventfd.h>
#include <unistd.h>

#include "EventLoop.h"

int createEventfd() {
	int eventFd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (eventFd < 0) {
		// LOG_SYSERR
		abort();
	}
	return eventFd;
}

EventLoop::EventLoop() 
	: quit_(false), eventHandling_(false), 
	  eventfd_(createEventfd()), wakeupChannel_(new Channel(this, eventfd_)),
	  poller_(new Epoll(this)) 
{
	// poller_ = std::unique_ptr<Epoll>(new Epoll(this));

	wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
	wakeupChannel_->enableReading();
}


void EventLoop::updateChannel(Channel* ch) {
	poller_->updateChannel(ch);

}

void EventLoop::removeChannel(Channel* ch) {
	poller_->removeChannel(ch);
}

void EventLoop::loop() {
	while (!quit_) {
		// 清空活跃 Channel 列表
		activeChannels_.clear();

		poller_->poll(3, activeChannels_);

		eventHandling_ = true;
		for (Channel* channel : activeChannels_) {
			currentActiveChannel_ = channel;
			currentActiveChannel_->handleEvent();
		}
		currentActiveChannel_ = nullptr;
		eventHandling_ = false;

		doPendingFunctors();
	}
}

void EventLoop::wakeup() {
	uint64_t one = 1;
	ssize_t n = ::write(eventfd_, &one, sizeof(one));
	if (n != sizeof(one)) {
		// LOG_ERROR
	}
}

void EventLoop::handleRead() {
	uint64_t one = 1;
	ssize_t n = ::read(eventfd_, &one, sizeof(one));

	if (n != one) {
		// LOG_ERROR
	}
}

void EventLoop::queueInLoop(Functor cb) {
	pendingFunctors_.push_back(std::move(cb));
	if (!callingPendingFunctors_)
		wakeup();
}

void EventLoop::doPendingFunctors() {
	std::vector<Functor> functors;
	functors.swap(pendingFunctors_);
	callingPendingFunctors_ = true;
	for (const Functor& functor : functors) {
		functor();
	}
	callingPendingFunctors_ = false;
}
