#pragma once

#include "../Common/config.h"
#include "connection.h"
#include "model.h"
#include <list>
#include <mutex>
#include <unordered_map>
#include<condition_variable>

/*
* Clase Usable para multhithreading
*/

class Validator : public Thread {
private:
	bool ended = false;
	std::mutex m;
	std::condition_variable cond_var;

	Logger* logger;
	Model* model;
	std::list<Connection*> connections;
	std::unordered_map<std::string, std::string> users;

	bool validate_connection(Connection* connection);
public:
	Validator(Model* modelo, Config* config, Logger* main_logger);
	~Validator();
	void end() { ended = true; };
	bool add_connection(Connection* new_connection);
	void virtual run() override;
};

#pragma once
