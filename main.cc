#include "TcpServer.h"

int main(int argc, char* argv[]) {
	TcpServer server;
	server.start();
	server.loop();
	return 0;
}