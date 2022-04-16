#include <iostream>

#include "TcpServer.h"

void func(TcpConnection* conn, Buffer* buffer) {
	// buffer->print();
	
	conn->send("seding a message.");
}

int main(int argc, char* argv[]) {
	TcpServer server;

	server.setMessageCallback(std::bind(func, std::placeholders::_1, std::placeholders::_2));

	server.start();
	server.loop();
	return 0;
}