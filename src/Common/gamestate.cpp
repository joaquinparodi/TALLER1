#include "gamestate.h"
#include "socket.h"


GameState::GameState(std::vector<Player*> current_players, Level* current_level, int t, int camera_X, int camera_Y, GameStatus st, std::vector<SoundType> send_sounds) {
	players = current_players;
	platforms = current_level->platforms;
	surprises = current_level->surprises;
	enemies = current_level->enemies;
	level_number = current_level->get_number();
	camera_x = camera_X;
	camera_y = camera_Y;
	time = t;
	status = st;
	sounds = send_sounds;
}

// decodea todo menos el largo del mensaje
GameState::GameState(char* network_string) {
	time = Socket::big_endian_to_int(&network_string[0]);
	level_number = Socket::big_endian_to_int(&network_string[4]);
	camera_x = Socket::big_endian_to_int(&network_string[8]);
	camera_y = Socket::big_endian_to_int(&network_string[12]);
	status = (GameStatus)Socket::big_endian_to_int(&network_string[16]);
	//players
	int cant_players = Socket::big_endian_to_int(&network_string[20]);
	int i = 24;
	int index;
	int largo_username;
	char* username;
	for (index = 0; index < cant_players; i += 32) {
		largo_username = Socket::big_endian_to_int(&network_string[i + 28]);
		username = (char*)malloc(largo_username);
		memcpy(username, &network_string[i + 32], largo_username);
		c_players.push_back(
			ClientPlayer(
				Socket::big_endian_to_int(&network_string[i]), //x
				Socket::big_endian_to_int(&network_string[i + 4]), //y
				Socket::big_endian_to_int(&network_string[i + 8]), //height
				Socket::big_endian_to_int(&network_string[i + 12]),//sprite
				Socket::big_endian_to_int(&network_string[i + 16]),//player_number
				Socket::big_endian_to_int(&network_string[i + 20]),//cantidad_vidas
				Socket::big_endian_to_int(&network_string[i + 24]),//puntos
				username
			)
		);
		free(username);
		index++;
		i += largo_username;
	}

	//enemies
	int cant_enemies = Socket::big_endian_to_int(&network_string[i]);
	i += 4;
	for (index = 0; index < cant_enemies; i += 16) {
		c_enemies.push_back(
			ClientEnemy(
				Socket::big_endian_to_int(&network_string[i]), //x
				Socket::big_endian_to_int(&network_string[i + 4]), //y
				Socket::big_endian_to_int(&network_string[i + 8]), //sprite
				Socket::big_endian_to_int(&network_string[i + 12])//type
			)
		);
		index++;
	}

	//platforms
	int cant_platforms = Socket::big_endian_to_int(&network_string[i]);
	i += 4;
	for (index = 0; index < cant_platforms; i += 16) {
		c_platforms.push_back(
			ClientPlatform(
				Socket::big_endian_to_int(&network_string[i]), //x
				Socket::big_endian_to_int(&network_string[i + 4]), //y
				Socket::big_endian_to_int(&network_string[i + 8]), //amount
				Socket::big_endian_to_int(&network_string[i + 12]) //type
			)
		);
		index++;
	}

	//surprises
	int cant_surprises = Socket::big_endian_to_int(&network_string[i]);
	i += 4;
	for (index = 0; index < cant_surprises; i += 12) {
		c_surprises.push_back(
			ClientSurprise(
				Socket::big_endian_to_int(&network_string[i]), //x
				Socket::big_endian_to_int(&network_string[i + 4]), //y
				Socket::big_endian_to_int(&network_string[i + 8]) //type
			)
		);
		index++;
	}

	//sonidos
	int cant_sounds = Socket::big_endian_to_int(&network_string[i]);
	i += 4;
	for (index = 0; index < cant_sounds; i += 4) {
		sounds.push_back((SoundType)Socket::big_endian_to_int(&network_string[i]));
		index++;
	}

}

char* GameState::encode() {
	int largo = 24;
	largo += 4;
	for (auto player : players) {
		largo += 32;
		largo += player->username.size() + 1;
	}

	largo += 4;
	for (auto kv : enemies)
		largo += 16;

	largo += 4;
	for (auto kv : platforms)
		largo += 16;

	largo += 4;
	for (auto kv : surprises)
		largo += 12;

	largo += 4;
	for (auto kv : sounds)
		largo += 4;

	char* buffer = (char*)malloc(largo);
	std::memset(buffer, '\0', largo);

	int i = 0;
	Socket::int_to_big_endian(largo, &buffer[i]);
	i += 4;

	Socket::int_to_big_endian(time, &buffer[i]);
	i += 4;

	Socket::int_to_big_endian(level_number, &buffer[i]);
	i += 4;

	Socket::int_to_big_endian(camera_x, &buffer[i]);
	i += 4;

	Socket::int_to_big_endian(camera_y, &buffer[i]);
	i += 4;

	Socket::int_to_big_endian((int)status, &buffer[i]);
	i += 4;

	//Players
	Socket::int_to_big_endian(players.size(), &buffer[i]);
	i += 4;
	for (auto player : players) {
		Socket::int_to_big_endian(player->get_x(), &buffer[i]);
		i += 4;
		Socket::int_to_big_endian(player->get_y(), &buffer[i]);
		i += 4;
		Socket::int_to_big_endian(player->height(), &buffer[i]);
		i += 4;
		Socket::int_to_big_endian(player->get_sprite_index(), &buffer[i]);
		i += 4;
		Socket::int_to_big_endian(player->get_player_number(), &buffer[i]);
		i += 4;
		Socket::int_to_big_endian(player->get_vidas_restantes(), &buffer[i]);
		i += 4;
		Socket::int_to_big_endian(player->get_puntos(), &buffer[i]);
		i += 4;
		Socket::int_to_big_endian(player->username.size() + 1, &buffer[i]);
		i += 4;
		memcpy(&buffer[i], player->username.c_str(), player->username.size());
		i += player->username.size() + 1;
	}

	//Enemies
	Socket::int_to_big_endian(enemies.size(), &buffer[i]);
	i += 4;
	for (auto enemy : enemies) {
		Socket::int_to_big_endian(enemy->get_x(), &buffer[i]);
		i += 4;
		Socket::int_to_big_endian(enemy->get_y(), &buffer[i]);
		i += 4;
		Socket::int_to_big_endian((int)enemy->get_sprite_index(), &buffer[i]);
		i += 4;
		Socket::int_to_big_endian(enemy->get_type(), &buffer[i]);
		i += 4;
	}

	//Platforms
	Socket::int_to_big_endian(platforms.size(), &buffer[i]);
	i += 4;
	for (auto platform : platforms) {
		Socket::int_to_big_endian(platform->get_x(), &buffer[i]);
		i += 4;
		Socket::int_to_big_endian(platform->get_y(), &buffer[i]);
		i += 4;
		Socket::int_to_big_endian(platform->get_amount(), &buffer[i]);
		i += 4;
		Socket::int_to_big_endian(platform->get_type(), &buffer[i]);
		i += 4;
	}

	//surprises
	Socket::int_to_big_endian(surprises.size(), &buffer[i]);
	i += 4;
	for (auto surprise : surprises) {
		Socket::int_to_big_endian(surprise->get_x(), &buffer[i]);
		i += 4;
		Socket::int_to_big_endian(surprise->get_y(), &buffer[i]);
		i += 4;
		Socket::int_to_big_endian(surprise->get_type(), &buffer[i]);
		i += 4;
	}

	//sounds
	Socket::int_to_big_endian(sounds.size(), &buffer[i]);
	i += 4;
	for (auto sound : sounds) {
		Socket::int_to_big_endian((int)sound, &buffer[i]);
		i += 4;
	}

	lenght = largo;
	return buffer;
}
