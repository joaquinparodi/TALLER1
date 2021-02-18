#pragma once

#include <mutex>
#include "../Common/socket.h"
#include "../Common/thread.h"
#include "../Common/gamestate.h"
#include "../Common/command.h"

class Connection : Thread {
private:
	//Server::ClientAccepter c_a;
	Socket* sock;
	bool ended = false;
	std::mutex m;
public:
	Connection(Socket* socket);
	~Connection();

	void end() { ended = true; }
	bool has_ended() { return ended; }
	bool validated = false;
	bool logged_in = false;
	std::string user;
	std::string password;
	void run();
	void server_filled();
	void connected();
	void wrong_user_password();
	void send_gamestate(GameState* game_state);
	CommandQueue commands;
};
