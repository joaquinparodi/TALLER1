#include "server.h"

Server::Server(char* port, Config* config, Logger* main_logger) {
	logger = main_logger;
	model = new Model(config, main_logger);
	validator = new Validator(model, config, main_logger);
	accepter = new Accepter(port, validator);
}

Server::~Server() {
}

void Server::run() {
	char c = '0';
	while (c != 'q') {
		c = getchar();
	}
	accepter->end();
	validator->end();
	model->end();
	accepter->join();
	validator->join();
	model->join();
	delete accepter;
	delete validator;
	delete model;
}
