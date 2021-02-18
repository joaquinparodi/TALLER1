#pragma once

#include "../Common/thread.h"
#include "../Common/socket.h"
#include "mapstate.h"
#include "sound.h"
#include <mutex>

enum struct EndStatus : int {
	CRASH,
	GAME_ENDED
};

class Receiver : public Thread {
public:
	// El parametro type indica que tipo de productor es
	Receiver(Socket* socket);
	~Receiver();
	void virtual run() override;

	void end();
	bool has_ended() { return ended; }
	void update_map(MapState* map);
	GameState* state = NULL;
	EndStatus end_status = EndStatus::CRASH;
	GameStatus get_game_state();

	// Sonido
	Sound* sound_subsystem = NULL;
private:
	Socket* sock;
	bool ended = false;
	std::mutex m;
	void update_state(char* network_string);
};
