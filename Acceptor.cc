#include "Acceptor.h"
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <arpa/inet.h>

using std::cout;
using std::endl;

extern int MAX_LISTENFD;

struct sockaddr_in cliaddr;
socklen_t clilen = sizeof(struct sockaddr_in);


// 注意成员初值列和 Acceptor 类成员的顺序
Acceptor::Acceptor(EventLoop *loop, int listenfd): 
                    listenfd_(listenfd), loop_(loop),
					acceptChannel_(loop_, listenfd_)
                    
{

}

void Acceptor::listen() {
    fcntl(listenfd_, F_SETFL, O_NONBLOCK);
    // 端口重用
	int on = 1;
	setsockopt(listenfd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    struct sockaddr_in seraddr;
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	seraddr.sin_port = htons(9006);

	int ret;
	ret = bind(listenfd_, (struct sockaddr *)&seraddr, sizeof(seraddr));
	assert(ret != -1);

	ret = ::listen(listenfd_, MAX_LISTENFD);
	assert(ret != -1);

    acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
    acceptChannel_.enableReading();
}

void Acceptor::handleRead() {
    int connfd = accept(listenfd_, (sockaddr *)&cliaddr, (socklen_t *)&clilen);
	if (connfd > 0)
	{
		cout << "new connection from [" << inet_ntoa(cliaddr.sin_addr) << ":"
			 << ntohs(cliaddr.sin_port) << "]"
			 << " accept sock fd: " << connfd << endl;
	}
	else
	{
		cout << "accept error, connfd: " << connfd << ", errno: " << errno << endl;
	}
	fcntl(connfd, F_SETFL, O_NONBLOCK);

	newConnectionCallback_(connfd);
}