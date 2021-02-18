#pragma once

#include <vector>
#include <string>
#include <utility>
#include <mutex>
#include <unordered_map>
#include "../Common/thread.h"
#include "connection.h"
#include "map.h"

class Model : public Thread {
private:
	bool ended = false;
	bool started = false;

	int player_amount = 4;

	int remaining_time = 60;
	Map* map;
	Logger* logger;
	std::unordered_map<std::string, std::pair<Connection*, Player*>> connections;
	std::mutex m;

	// Internal contol functions
	void send_gamestate();
	void read_input();
public:
	Model(Config* config, Logger* main_logger);
	~Model();

	int get_remaining_time() { return remaining_time; }
	bool add_connection(Connection* new_connection);
	void virtual run() override;
	void end() { ended = true; }
};