#include "EventLoop.h"
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include <functional>

using std::cout;
using std::endl;

extern int MAX_LINE;
extern int MAX_EVENTS;

using std::placeholders::_1;

EventLoop::EventLoop(int epfd, int listenfd) 
	: epollfd_(epfd), listenfd_(listenfd), eventHandling_(false) 
{
}


void EventLoop::updateChannel(Channel* ch) {
	struct epoll_event ev;
	ev.data.ptr = ch;
	ev.events = ch->events();
	int ret = epoll_ctl(epollfd_, EPOLL_CTL_ADD, ch->fd(), &ev);
	assert(ret != -1);
}
void EventLoop::loop() {
    int ret;
	struct epoll_event events[MAX_EVENTS];
    struct epoll_event ev;
    memset(&ev, 0, sizeof ev);

	for (;;) {
		int num = epoll_wait(epollfd_, events, MAX_EVENTS, -1);
		// 清空活跃 Channel 列表
		activeChannels_.clear();
		for (int i = 0; i < num; ++ i) {
			Channel *ch = static_cast<Channel*>(events[i].data.ptr);
			ch->set_revents(events[i].events);
			activeChannels_.push_back(ch);
		}
		eventHandling_ = true;
		for (Channel* channel : activeChannels_) {
			currentActiveChannel_ = channel;
			currentActiveChannel_->handleEvent();
		}
		currentActiveChannel_ = nullptr;
		eventHandling_ = false;
	}
}

