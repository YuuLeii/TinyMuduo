#include <iostream>

#include "TcpServer.h"

void func() {
	// std::cout << "what happened." << std::endl;
}

int main(int argc, char* argv[]) {
	TcpServer server;

	server.setMessageCallback(func);

	server.start();
	server.loop();
	return 0;
}