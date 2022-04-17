#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <vector>
#include <memory>
#include <atomic>

#include "Channel.h"
#include "Epoll.h"

class Channel;

class EventLoop {
	typedef std::function<void()> Functor;
public:
	EventLoop();
	EventLoop(const EventLoop&) = delete;
	EventLoop& operator=(const EventLoop&) = delete;

	void updateChannel(Channel*);
	void removeChannel(Channel*);
	void loop();

	void wakeup();
	void handleRead();
	void queueInLoop(Functor cb);
	void doPendingFunctors();
private:
	std::unique_ptr<Epoll> poller_;

	typedef std::vector<Channel*> ChannelList;

	std::atomic<bool> quit_;
	bool eventHandling_;
	bool callingPendingFunctors_;
	int eventfd_;
	ChannelList activeChannels_;
	Channel* currentActiveChannel_;

	std::unique_ptr<Channel> wakeupChannel_;

	// Need to be guarded.
	std::vector<Functor> pendingFunctors_;
};

#endif