#include "EventLoop.h"

EventLoop::EventLoop() 
	: eventHandling_(false) 
{
	poller_ = std::unique_ptr<Epoll>(new Epoll(this));
}


void EventLoop::updateChannel(Channel* ch) {
	poller_->updateChannel(ch);

}

void EventLoop::removeChannel(Channel* ch) {
	poller_->removeChannel(ch);
}

void EventLoop::loop() {
	for (;;) {
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
	}
}

