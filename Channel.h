#ifndef CHANNEL_H
#define CHANNEL_H
#include <assert.h>
#include <functional>
#include "EventLoop.h"

class EventLoop;

class Channel {
public:
	typedef std::function<void()> EventCallback;
	typedef std::function<void()> ReadEventCallback;
	Channel () = delete;
	Channel(const Channel&) = delete;
	Channel& operator=(const Channel&) = delete;

	Channel(EventLoop *loop, int fd);
	~Channel();

	void handleEvent();

	void setReadCallback(ReadEventCallback cb) {
		readCallback_ = std::move(cb);
	}
	void setWriteCallback(EventCallback cb) {
		writeCallback_ = std::move(cb);
	}
	void setCloseCallback(EventCallback cb) {
		closeCallback_ = std::move(cb);
	}
	void setErrorCallback(EventCallback cb) {
		errorCallback_ = std::move(cb);
	}

	void enableReading() { events_ |= kReadEvent; update(); }
	void disableReading() {	events_ &= ~kReadEvent; update(); }
	void enableWriting() { events_ |= kWriteEvent;  update(); }
	void disableWriting() { events_ &= ~kWriteEvent; update(); }

	void disableAll() { events_ = kNoneEvent; update(); }
	bool isWriting() const { return events_ & kWriteEvent; }
	bool isReading() const { return events_ & kReadEvent; }

	int fd() const {	return fd_;	}
	int events() const {	return events_;	}
	void set_revents(int revt) { revents_ = revt; }   // used by epoll

	void remove();
private:
	void update();

	static const int kNoneEvent;
	static const int kReadEvent;
	static const int kWriteEvent;

	EventLoop* loop_;
	const int fd_;
	int events_;
	int revents_;

	bool eventHandling_;

	ReadEventCallback readCallback_;
	EventCallback writeCallback_;
	EventCallback closeCallback_;
	EventCallback errorCallback_;
};
#endif