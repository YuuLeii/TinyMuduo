#include "Epoll.h"
#include "Channel.h"
#include <string.h>

Epoll::Epoll(EventLoop* loop): ownerLoop_(loop) 
{
    epollfd_ = epoll_create(1);
	assert(epollfd_ != -1);
}

void Epoll::poll(int timeout, std::vector<Channel*>& activeChannels) {
	int num = epoll_wait(epollfd_, events_, MAX_EVENTS, timeout);

	for (int i = 0; i < num; ++ i) {
		Channel *ch = static_cast<Channel*>(events_[i].data.ptr);
		ch->set_revents(events_[i].events);
		activeChannels.push_back(ch);
	}
}

void Epoll::updateChannel(Channel* ch) {
	update(EPOLL_CTL_ADD, ch);
}

void Epoll::removeChannel(Channel* ch) {
	update(EPOLL_CTL_DEL, ch);
}

void Epoll::update(int operation, Channel* channel) {
	struct epoll_event event;
	memset(&event, 0, sizeof event);
	event.events = channel->events();
	event.data.ptr = channel;

	int fd = channel->fd();
	if (::epoll_ctl(epollfd_, operation, fd, &event) < 0) {
		if (operation == EPOLL_CTL_DEL) {
			// LOG_SYSERR
		} else {
			// LOG_SYSFATAL
		}
	}
}