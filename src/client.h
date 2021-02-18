#pragma once

#include "Common/socket.h"
#include "Common/logger.h"
#include "Client/renderer.h"

class Client {
private:
	//Server::ClientAccepter c_a;
	Logger* logger;
	Socket* socket;
	Renderer* renderer;
public:
	Client(char* port, char* hostname, Config* config, Logger* main_logger);
	~Client();
	void run();
};
