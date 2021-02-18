#include "receiver.h"

Receiver::Receiver(Socket* socket) {
	sock = socket;
}

Receiver::~Receiver() {
	delete state;
}

void Receiver::run() {
	int largo;
	int largo_read;
	char buffer_cant[4];
	char* buffer;
	while (!ended) {
		int result = sock->s_read(buffer_cant, 4);
		if (result != 4){
			ended = true;
			return;
		}
		largo = sock->big_endian_to_int(buffer_cant);
		if (largo < 0) {
			end_status = EndStatus::GAME_ENDED;
			ended = true;
			return;
		}
		buffer = (char*)malloc(largo);
		std::memset(buffer, '\0', largo);
		largo_read = sock->s_read(buffer, largo - 4);
		if ((largo - 4) != largo_read){
			ended = true;
			return;
		}
		update_state(buffer);
		free(buffer);
	}
}

void Receiver::end() {
	ended = true;
}

void Receiver::update_map(MapState* map) {
	std::unique_lock<std::mutex> lock(m);
	if (state)
		map->update(state);
}

GameStatus Receiver::get_game_state() {
	return state->status;
}

void Receiver::update_state(char* network_string) {
	std::unique_lock<std::mutex> lock(m);
	if (state != NULL)
		delete state;
	state = new GameState(network_string);
	for (SoundType sound : state->sounds) {
		sound_subsystem->play_sound(sound);
	}
}

