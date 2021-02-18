#pragma once

#include <vector>
#include <string>
#include "../Server/player.h"
#include "../Server/level.h"
#include "../Client/clientplayer.h"
#include "../Client/clientlevel.h"
#include "../Client/clientsurprise.h"
#include "../Client/clientplatform.h"
#include "../Client/sound.h"

enum struct GameStatus : int {
	PLAYING,
	SCORE,
	END_GAME_SCORE,
	END_GAME_LOSE
};

class GameState {
private:
	// Server Data
	std::vector<Player*> players;
	std::vector<Platform*> platforms;
	std::vector<Surprise*> surprises;
	std::vector<Enemy*> enemies;

public:
	// Client data
	int level_number;
	std::vector<ClientPlayer> c_players;
	std::vector<ClientEnemy> c_enemies;
	std::vector<ClientSurprise> c_surprises;
	std::vector<ClientPlatform> c_platforms;
	std::vector<SoundType> sounds;

	int camera_x;
	int camera_y;
	
	// server data
	int lenght;
	int time;
	GameStatus status = GameStatus::PLAYING;

	GameState(std::vector<Player*> players, Level* current_level, int t, int camera_X, int camera_Y, GameStatus st, std::vector<SoundType> send_sounds);
	GameState(char* network_string);
	char* encode();
};

/*
Protocolo de estado de juego Todo big endian

	largo del state
	Numero del nivel int32
	Camera
	 - x int32
	 - y int32
	Players
	  - Cantidad
		- X int32
		- Y int32
		- Sprite_index int32
	Enemigos
	  - Cantidad
		- X int32
		- Y int32
		- Type
	Plataformas
	  - Cantidad
		- X int32
		- Y int32
		- Amount int 32
		- Type int32
	Monedas
	  - Cantidad
		- X int32
		- Y int32

	Ejemplo: https://cdn.discordapp.com/attachments/766067323886764036/782316366790787102/560ebbe7dd0895325c8b458e.png
	[1][20][60][3][2][60][70][2][65][70][2][4]

*/

