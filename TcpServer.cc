#include "TcpServer.h"
#include <assert.h>

extern const int MAX_LINE = 100;
extern const int MAX_EVENTS = 500;
const int MAX_LISTENFD = 5; 

struct sockaddr_in cliaddr;
socklen_t clilen = sizeof(struct sockaddr_in);

void onRead(int sockfd) {
	char buf[MAX_LINE];
	memset(buf, 0, sizeof(buf));
	int readlen = read(sockfd, buf, MAX_LINE);
	if (readlen <= 0)
	{
		cout << "closed sock fd: " << sockfd << "from sockfd: " << sockfd << endl;
		close(sockfd);
	} else {
		int writelen = write(sockfd, buf, readlen);
		cout << "write " << writelen << " bytes to sockfd: " << sockfd << endl;
	}
}


// 如果用全局函数，则需要传两个参数进函数里。
void onConnection(int listenfd, EventLoop* loop)
{
	int connfd = accept(listenfd, (sockaddr *)&cliaddr, (socklen_t *)&clilen);
	if (connfd > 0)
	{
		cout << "new connection from [ " << inet_ntoa(cliaddr.sin_addr) << ":"
			 << ntohs(cliaddr.sin_port) << "]"
			 << " accept sock fd: " << connfd << endl;
	}
	else
	{
		cout << "accept error, connfd: " << connfd << ", errno: " << errno << endl;
	}
	fcntl(connfd, F_SETFL, O_NONBLOCK);

	Channel *newChannel = new Channel(loop, connfd);
	newChannel->setReadCallBack(std::bind(onRead, connfd));
	newChannel->enableReading();
}

TcpServer::TcpServer() {
    epollfd_ = epoll_create(1);
	assert(epollfd_ != -1);
    
    listenfd_ = socket(AF_INET, SOCK_STREAM, 0);
    assert(listenfd_ != -1);

	loop_ = std::move(unique_ptr<EventLoop>(new EventLoop(epollfd_, listenfd_)));
}

void TcpServer::start() {
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

	ret = listen(listenfd_, MAX_LISTENFD);
	assert(ret != -1);
	
	Channel* listenChannel = new Channel(loop_.get(), listenfd_);
	listenChannel->enableReading();

	// 也可用 lambda 函数
	// auto onConnection = [&]() -> void {
	// 	int connfd = accept(listenfd_, (sockaddr *)&cliaddr, (socklen_t *)&clilen);
	// 	if (connfd > 0)
	// 	{
	// 		cout << "new connection from [ " << inet_ntoa(cliaddr.sin_addr) << ":"
	// 			 << ntohs(cliaddr.sin_port) << "]"
	// 			 << " accept sock fd: " << connfd << endl;
	// 	}
	// 	else
	// 	{
	// 		cout << "accept error, connfd: " << connfd << ", errno: " << errno << endl;
	// 	}
	// 	fcntl(connfd, F_SETFL, O_NONBLOCK);

	// 	Channel *newChannel = new Channel(loop_.get(), connfd);
	// 	newChannel->setReadCallBack(std::bind(onRead, connfd));
	// 	newChannel->enableReading();
	// };

	listenChannel->setReadCallBack(bind(onConnection, listenfd_, loop_.get()));
}

void TcpServer::loop() {
	loop_->loop();	   
}
