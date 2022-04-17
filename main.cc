#include <iostream>

#include "TcpServer.h"

void func(TcpConnection* conn, Buffer* buffer) {
	// buffer->print();
	
	conn->send("seding a message.");
}

// 当 TcpConnection outputBuffer 中的数据都发送出去之后，通知库的使用者
void onWriteComplete(TcpConnection* conn) {
	std::cout << "write complete." << std::endl;
}

int main(int argc, char* argv[]) {
	TcpServer server;

	server.setMessageCallback(std::bind(func, std::placeholders::_1, std::placeholders::_2));
	server.setWriteCompleteCallback(std::bind(onWriteComplete, std::placeholders::_1));

	server.start();
	server.loop();
	return 0;
}