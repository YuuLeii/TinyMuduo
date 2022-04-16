#include <unistd.h>
#include <iostream>
using std::cout;
using std::endl;

#include "Buffer.h"

extern int MAX_LINE;
int Buffer::readFd(int sockfd) {
    char line[MAX_LINE];
    int readlen = ::read(sockfd, line, MAX_LINE);
    if (readlen > 0) {
        buffer_.append(line, readlen);
    }
    return readlen;
}

void Buffer::append(const char* data, size_t len) {
    buffer_ += data;
}

void Buffer::print() {
    cout << buffer_ << endl;
    buffer_.clear();
}

void Buffer::update(int have_sent) {
    buffer_ = buffer_.substr(have_sent);
}