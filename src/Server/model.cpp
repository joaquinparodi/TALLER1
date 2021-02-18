#include "model.h"
#include "../Common/command.h"

void Model::send_gamestate() {
	std::unique_lock<std::mutex> lock(m);
	std::vector<Player*> players(map->players.begin(), map->players.end());
	GameState state = GameState(players, map->get_current_level(), remaining_time, map->get_camera()->x, map->get_camera()->y, map->status, map->sounds);
	for (auto connection : connections) {
		if (!connection.second.first->has_ended()){
			connection.second.first->send_gamestate(&state);
		} else {
			connection.second.second->disconnect();
		}
	}
	map->sounds.clear();
}

Model::Model(Config* config, Logger* main_logger) {
	logger = main_logger;
	map = new Map(config, main_logger);
	player_amount = config->get_max_players();
	remaining_time = config->get_time();
	start();
}

Model::~Model() {
	delete map;
	for (auto connection : connections) {
		connection.second.first->end();
		delete connection.second.first;
		delete connection.second.second;
	}
}

bool Model::add_connection(Connection* new_connection) {
	std::unique_lock<std::mutex> lock(m);
	if (connections.find(new_connection->user) != connections.end()) {
		delete connections[new_connection->user].first;
		connections[new_connection->user].first = new_connection;
		connections[new_connection->user].second->reconnect();
		return true;
	} else if (connections.size() < player_amount) {
		connections[new_connection->user] = 
			std::pair<Connection*, Player*>(new_connection, 
				new Player(logger, new_connection->user, connections.size() + 1));
		map->players.insert(connections[new_connection->user].second);
		if (connections.size() == player_amount)
			started = true;
		return true;
	}
	return false;
}

void Model::run() {
	//accepter.start();
	using namespace std::chrono;

	// Esto define el framerate
	using Framerate = duration<steady_clock::rep, std::ratio<1, 30>>;
	auto next = steady_clock::now() + Framerate{ 0 };
	int time_delta = 0;
	int time_counter = 0;
	while (!ended) {
		// Read Player Queues
		if (started) {
			read_input();
			map->update();
			send_gamestate();
		}

		time_delta = 1;
		next += Framerate{ 1 };
		if (started)
			time_counter++;
		// Codigo en caso de que se salteen frames
		while (next < steady_clock::now()) {
			time_delta++;
			if (started)
				time_counter++;
			next += Framerate{ 1 };
		}
		if (time_counter > 29) {
			time_counter = 0;
			if (!map->test_mode)
				remaining_time--;
			if (remaining_time < 0){
				map->status = GameStatus::END_GAME_LOSE;
			}
		}
		std::this_thread::sleep_until(next);
	}

	//accepter.join();
	// Guardar estado una vez mas 
}

void Model::read_input() {
	std::unique_lock<std::mutex> lock(m);
	bool flag_break = false;
	for (auto connection : connections) {
		if (connection.second.first->has_ended()) {
			continue;
		}
		Command command = connection.second.first->commands.pop_or_empty();
		switch (command) {
		case Command::left:
			connection.second.second->set_left(true);
			break;
		case Command::right:
			connection.second.second->set_right(true);
			break;
		case Command::jump:
			connection.second.second->jump();
			break;
		case Command::crouch:
			connection.second.second->bend(true);
			break;
		case Command::no_left:
			connection.second.second->set_left(false);
			break;
		case Command::no_right:
			connection.second.second->set_right(false);
			break;
		case Command::disconnect:
			connection.second.first->commands.clear();
			connection.second.first->end();
			break;
		case Command::no_crouch:
			connection.second.second->bend(false);
			break;
		case Command::test:
			map->test_mode = !map->test_mode;
			break;
		default:
			break;
		}
		if (flag_break)
			return;
	}
}