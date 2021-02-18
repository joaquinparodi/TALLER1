#pragma once

//#include "Server\client_accepter.h"
#include "Common/logger.h"
#include "Common/config.h"
#include "Server/model.h"
#include "Server/validator.h"
#include "Server/accepter.h"

class Server {
	private:
		//Server::ClientAccepter c_a;
		Logger* logger;
		Config* config;
		Model* model;
		Validator* validator;
		Accepter* accepter;
	public:
		Server(char* port, Config* config, Logger* main_logger);
		~Server();
		void run();
};
