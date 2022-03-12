#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <vector>
#include "Channel.h"

class Channel;
class EventLoop {
public:
	EventLoop() = delete;
	EventLoop(int epfd, int listenfd);

	void updateChannel(Channel*);
	void loop();
private:
	const int epollfd_;
	const int listenfd_;

	typedef std::vector<Channel*> ChannelList;

	bool eventHandling_;

	ChannelList activeChannels_;
	Channel* currentActiveChannel_;
};

#endif