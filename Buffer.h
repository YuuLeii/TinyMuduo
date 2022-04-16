#ifndef BUFFER_H
#define BUFFER_H

#include <string>
#include <new>

using std::string;

class Buffer {
public:
	string& str() { return buffer_;	}
	bool empty() { return buffer_.empty();	}
	void append(const char* data, size_t);
	int readFd(int sockfd);
	
	void print();
	void update(int have_sent);
private:
	string buffer_;
	// size_t readerIndex_;
	// size_t writerIndex_;
};

#endif // BUFFER_H