#include "connection.h"
#include <vector>
#include <mutex>
#include "validator.h"

Validator::Validator(Model* modelo, Config* config, Logger* main_logger) {
	model = modelo;
	logger = main_logger;
	for (ConfigUser* user : config->credentials) {
		users[user->username] = user->password;
	}
	start();
}

bool Validator::validate_connection(Connection* connection) {
	std::string log_data;
	log_data.append(connection->user);
	if (users[connection->user] == connection->password) {
		if (!model->add_connection(connection)) {
			connection->server_filled();
			logger->info(log_data.append(" tried to connect, but the server was full.").c_str());
		} else {
			connection->connected();
			logger->info(log_data.append(" connected succesfully.").c_str());
			return true;
		}
	} else {
		connection->wrong_user_password();
		logger->info(log_data.append(" tried to connect, but the user or password was wrong.").c_str());
	}
	return false;
}

Validator::~Validator() {
	std::unique_lock<std::mutex> lock(m);
	for (auto connection : connections) {
		delete connection;
	}
}

bool Validator::add_connection(Connection* new_connection) {
	std::unique_lock<std::mutex> lock(m);
	connections.push_back(new_connection);
	return true;
}

void Validator::run() {
	while (!ended) {
		std::list<Connection*>::iterator it = connections.begin();
		while (it != connections.end()) {
			if ((*it)->validated && !(*it)->logged_in && validate_connection(*it)){
				connections.erase(it++);
			} else {
				++it;
			}
		}
	}
}
