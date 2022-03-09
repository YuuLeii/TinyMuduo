#include <iostream>
#include <sys/epoll.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

using namespace std;

const int MAX_LINE = 100;
const int MAX_EVENTS = 500;
const int MAX_LISTENFD = 5;

int main(int argc, char* argv[]) {
	int epollfd = epoll_create(1);
	assert(epollfd != -1);
	

	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in seraddr;
	fcntl(listenfd, F_SETFL, O_NONBLOCK);
	int on = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	seraddr.sin_port = htons(9006);

	int ret;
	ret = bind(listenfd, (struct sockaddr *)&seraddr, sizeof(seraddr));
	assert(ret != -1);

	ret = listen(listenfd, MAX_LISTENFD);
	assert(ret != -1);
	

	struct epoll_event ev;
	ev.data.fd = listenfd;
	ev.events = EPOLLIN;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);

	struct sockaddr_in cliaddr;
	socklen_t clilen = sizeof(struct sockaddr_in);

	struct epoll_event events[MAX_EVENTS];
	for (;;) {
		int num = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		for (int i = 0; i < num; ++ i) {
			if (events[i].data.fd == listenfd) {
				int connfd = accept(listenfd, (sockaddr *)&cliaddr, (socklen_t*)&clilen);
				if (connfd > 0) {
					cout << "new connection from [ " << inet_ntoa(cliaddr.sin_addr) << ":"
						 << ntohs(cliaddr.sin_port) << "]" << " accept sock fd: " << connfd << endl;
					fcntl(connfd, F_SETFL, O_NONBLOCK);
					ev.data.fd = connfd;
					ev.events = EPOLLIN;
					ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev);
					assert(ret != -1);
				} else {
					cout << "accept error, connfd: " << connfd << ", errno: " << errno << endl;
				}
			} else if (events[i].events & EPOLLIN) {
				int sockfd = events[i].data.fd;
				char buf[MAX_LINE];
				memset(buf, 0, sizeof(buf));
				int readlen = read(sockfd, buf, MAX_LINE);
				if (readlen <= 0) {
					cout << "closed sock fd: " << sockfd << endl;
					close(sockfd);
				} else {
					int writelen = write(sockfd, buf, readlen);
					cout << "write " << writelen << " bytes." << endl;
				}
			}
		}
	}
	return 0;
}