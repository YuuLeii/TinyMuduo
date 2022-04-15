#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <vector>
#include <memory>

#include "Channel.h"
#include "Epoll.h"

class Channel;

class EventLoop {
public:
	EventLoop();
	EventLoop(const EventLoop&) = delete;
	EventLoop& operator=(const EventLoop&) = delete;

	void updateChannel(Channel*);
	void removeChannel(Channel*);
	void loop();
private:
	std::unique_ptr<Epoll> poller_;
	typedef std::vector<Channel*> ChannelList;

	bool eventHandling_;

	ChannelList activeChannels_;
	Channel* currentActiveChannel_;
};

#endif